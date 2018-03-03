/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "lin.h"
#include "uart.h"

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

LinStateType_t eLinStates;

Lin_Frame_t *LinFrame;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/* Old Init Function */
/* This function shall initialize the UART driver with the given UART channel and speed */
// void Lin_Init (uint16_t LinBaudrate){
//     /* Local Variables */
//     const Pin pPins[] = UART_PINS;
    
//     /* LIN Frame Init */
//     LinFrame                    = (Lin_Frame_t*) malloc(sizeof(Lin_Frame_t));
//     LinFrame->eCurrentState     = IDLE;
//     LinFrame->u8Break           = 0x00;
//     LinFrame->u8Sync            = 0x55;
//     LinFrame->u8PID             = 0x00;
//     LinFrame->psPORT            = UART_DEVICE;

//     /* PIN Configuration */
// 	PIO_Configure( pPins, PIO_LISTSIZE( pPins ) );

//     /* Enable Peripheral Clock */
//     PMC_EnablePeripheral(UART_ID);

//     /* UART Init */
//     UART_Configure(LinFrame->psPORT, (uint32_t) LIN_MODE, (uint32_t) LinBaudrate, (uint32_t) MASTER_CLK_LIN);

//     /* NVIC Enable */
//     NVIC_ClearPendingIRQ(UART2_IRQn);
//     NVIC_EnableIRQ(UART2_IRQn);
// }

/* This function shall initialize the UART driver with the given UART channel and speed */
void Lin_Init (const LinConfigType_t* psConfig){
    /* LIN Frame Init */
    LinFrame                    = (Lin_Frame_t*) malloc(sizeof(Lin_Frame_t));
    /* Value Init */
    LinFrame->eCurrentState     = IDLE;
    LinFrame->eResponseStatus   = NONE;
    LinFrame->u8Break           = 0x00;
    LinFrame->u8Sync            = 0x55;
    LinFrame->pPDU              = NULL;
    LinFrame->psPORT            = psConfig->LinChannel->LinChannelId;
    
    vLinUARTInit(psConfig->LinChannel);
}

// /* Sends on LIN Frame if possible */
// void Lin_SendFrame (uint8_t LinPid){
//     /* Validate the current state */
//     if (LinFrame->eCurrentState == IDLE){
//         LinFrame->u8PID = LinPid;
//         UART_SendByte(LinFrame->psPORT, LinFrame->u8Break);
//         LinFrame->eCurrentState = SEND_BREAK;
//         UART_EnableIt(LinFrame->psPORT, (uint32_t) MODE_TX_INT);
//     }
//     else
//     {
//         /* Do Nothing - Send is Busy */
//     }
// }

/* Sends on LIN Frame if possible */
/* This function starts sending the current frame */
Std_ReturnType_t Lin_SendFrame (LinChannelType_t* psChannel, LinPduType_t* psPduInfoPtr){
    /* Set the PDU to send */
    LinFrame->pPDU              = psPduInfoPtr;
    LinFrame->psPORT            = psChannel->LinChannelId;

    Std_ReturnType_t ret_val = E_NOK;

    /* Validate the current state */
    if (LinFrame->eCurrentState == IDLE){
        UART_SendByte(LinFrame->psPORT, LinFrame->u8Break);
        LinFrame->eCurrentState = SEND_BREAK;
        UART_EnableIt(LinFrame->psPORT, (uint32_t) MODE_TX_INT);
        /* Transmission Started */
        ret_val = E_OK;
    }
    else
    {
        /* Do Nothing - Send is Busy */
    }
    return ret_val;
}

/* LIN ISR */
void Lin_Isr(void){
    static uint8_t u8Idx = 0;
    /* Disable Interruptions */
    UART_DisableIt(LinFrame->psPORT, (uint32_t) MODE_RX_INT);
    UART_DisableIt(LinFrame->psPORT, (uint32_t) MODE_TX_INT);
    switch (LinFrame->eCurrentState)
    {
        case SEND_BREAK : 
            UART_SendByte(LinFrame->psPORT, LinFrame->u8Sync);
            LinFrame->eCurrentState = SEND_SYNC;
            UART_EnableIt(LinFrame->psPORT, (uint32_t) MODE_TX_INT);
            break;
        case SEND_SYNC : 
            UART_SendByte(LinFrame->psPORT, LinFrame->pPDU->Pid);
            LinFrame->eCurrentState = SEND_PID;
            UART_EnableIt(LinFrame->psPORT, (uint32_t) MODE_TX_INT);
            break;
        case SEND_PID :
            /* Time to decide if we are going to expect a SLAVE RESPONSE or MASTER RESPONSE */ 
            if(LinFrame->pPDU->Drc == LIN_MASTER_RESPONSE){
                /* Master sends the first byte of the answer */
                if(u8Idx <= LinFrame->pPDU->Dl){
                    UART_SendByte(LinFrame->psPORT, LinFrame->pPDU->SduPtr[u8Idx]);
                    u8Idx++;
                }
                LinFrame->eCurrentState = SEND_RESPONSE;
                UART_EnableIt(LinFrame->psPORT, (uint32_t) MODE_TX_INT);
            }
            else{
                /* Buffer creation + 1 to Store the Checksum */
                LinFrame->pPDU->SduPtr = (uint8_t *) malloc( (sizeof(uint8_t)*LinFrame->pPDU->Dl) + 1 );
                /* Change of State */
                LinFrame->eCurrentState = RECEIVE_RESPONSE;
                LinFrame->eResponseStatus   = BUSY;
                /* SLAVE RESPONSE - We shall Enable the RX interrupt */
                UART_EnableIt(LinFrame->psPORT, (uint32_t) MODE_RX_INT);
            }
            break;
        case SEND_RESPONSE :
            UART_DisableIt(LinFrame->psPORT, (uint32_t) MODE_TX_INT);
            /* MASTER RESPONSE */ 
            /* Master sends the answer */
            if(u8Idx <= LinFrame->pPDU->Dl){
                /* We send Data length + 1 because of the Checksum */
                UART_SendByte(LinFrame->psPORT, LinFrame->pPDU->SduPtr[u8Idx]);
                u8Idx++;
            }
            if(u8Idx > LinFrame->pPDU->Dl){
                /* Transmission has been completed, checksum has been sent */
                LinFrame->eCurrentState = IDLE;
                u8Idx = 0;
            }
            else
                UART_EnableIt(LinFrame->psPORT, (uint32_t) MODE_TX_INT);
            break;
        case RECEIVE_RESPONSE :
            UART_DisableIt(LinFrame->psPORT, (uint32_t) MODE_RX_INT);
            /* SLAVE RESPONSE */
            if(u8Idx <= LinFrame->pPDU->Dl){
                LinFrame->pPDU->SduPtr[u8Idx] = UART_GetByte(LinFrame->psPORT);
                u8Idx++;
            }
            if(u8Idx > LinFrame->pPDU->Dl){
                /* Transmission has been completed, checksum has been received */
                LinFrame->eCurrentState = IDLE;
                LinFrame->eResponseStatus   = RECEIVED;
                u8Idx = 0;
            }
            else
                UART_EnableIt(LinFrame->psPORT, (uint32_t) MODE_RX_INT);
    default:
            /* This shouldn't happen */
        break;
    }
}

/* 
Service     : Lin_GetSlaveResponse
Sync/Async  : Synchronous
Parameters  :
    Channel     - LIN Channel to be checked
    LinSduPtr   - Pointer to pointer to a buffer mapped from the LIN module where the SDU is stored
Return values:
    E_OK        - SDU has been received and can be retrieved from the buffer
    E_NOK       - SDU reception not complete yet
 */
Std_ReturnType_t Lin_GetSlaveResponse ( uint8_t Channel, uint8_t** LinSduPtr ){
    Std_ReturnType_t ret_val = E_NOK;
    if(LinFrame->pPDU->Drc == LIN_SLAVE_RESPONSE && LinFrame->eCurrentState == IDLE){
        /* This Frame has a Slave Response and it is in IDLE */
        if(LinFrame->eResponseStatus == RECEIVED){
            *LinSduPtr = LinFrame->pPDU->SduPtr;
            ret_val = E_OK;
        }
    }
    return ret_val;
}

/* Additional Methods */

void vLinUARTInit(const LinChannelType_t* psUartConfig){

    /* Select the correct PINs for the Channel */
    
    const Pin pPins_0[] = PINS_UART0;
    const Pin pPins_1[] = PINS_UART1;
    const Pin pPins_2[] = PINS_UART2;
    const Pin pPins_3[] = PINS_UART3;
    const Pin pPins_4[] = PINS_UART4;
    
    /* PIN Configuration */
    if( psUartConfig->LinChannelId == UART0 ){
        /* PIN Configuration */
	    PIO_Configure( pPins_0, PIO_LISTSIZE( pPins_0 ) );
        /* Enable Peripheral Clock */
        PMC_EnablePeripheral(ID_UART0);
    }
    if( psUartConfig->LinChannelId == UART1 ){
        /* PIN Configuration */
	    PIO_Configure( pPins_1, PIO_LISTSIZE( pPins_1 ) );
        /* Enable Peripheral Clock */
        PMC_EnablePeripheral(ID_UART1);
    }
    if( psUartConfig->LinChannelId == UART2 ){
        /* PIN Configuration */
	    PIO_Configure( pPins_2, PIO_LISTSIZE( pPins_2 ) );
        /* Enable Peripheral Clock */
        PMC_EnablePeripheral(ID_UART2);
    }
    if( psUartConfig->LinChannelId == UART3 ){
        /* PIN Configuration */
	    PIO_Configure( pPins_3, PIO_LISTSIZE( pPins_3 ) );
        /* Enable Peripheral Clock */
        PMC_EnablePeripheral(ID_UART3);
    }
    if( psUartConfig->LinChannelId == UART4 ){
        /* PIN Configuration */
	    PIO_Configure( pPins_4, PIO_LISTSIZE( pPins_4 ) );
        /* Enable Peripheral Clock */
        PMC_EnablePeripheral(ID_UART4);
    }

    /* UART Init */
    UART_Configure(psUartConfig->LinChannelId, (uint32_t) LIN_MODE, (uint32_t) psUartConfig->LinChannelBaudrate, (uint32_t) MASTER_CLK_LIN);

    if( psUartConfig->LinChannelId == UART0 ){
        /* NVIC Enable */
        NVIC_ClearPendingIRQ(UART0_IRQn);
        NVIC_EnableIRQ(UART0_IRQn);
    }
    if( psUartConfig->LinChannelId == UART1 ){
        /* NVIC Enable */
        NVIC_ClearPendingIRQ(UART1_IRQn);
        NVIC_EnableIRQ(UART1_IRQn);
    }
    if( psUartConfig->LinChannelId == UART2 ){
        /* NVIC Enable */
        NVIC_ClearPendingIRQ(UART2_IRQn);
        NVIC_EnableIRQ(UART2_IRQn);
    }
    if( psUartConfig->LinChannelId == UART3 ){
        /* NVIC Enable */
        NVIC_ClearPendingIRQ(UART3_IRQn);
        NVIC_EnableIRQ(UART3_IRQn);
    }
    if( psUartConfig->LinChannelId == UART4 ){
        /* NVIC Enable */
        NVIC_ClearPendingIRQ(UART4_IRQn);
        NVIC_EnableIRQ(UART4_IRQn);
    }
}