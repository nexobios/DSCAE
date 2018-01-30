/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "lin.h"

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

LinStateType eLinStates;

Lin_Frame_t *LinFrame;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

/**
 *  \brief Configure LEDs
 *
 *  Configures LEDs \#1 and \#2 (cleared by default).
 */

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/* This function shall initialize the UART driver with the given UART channel and speed */
void Lin_Init (uint16_t LinBaudrate){
    /* Local Variables */
    const Pin pPins[] = UART_PINS;
    
    /* LIN Frame Init */
    LinFrame                    = (Lin_Frame_t*) malloc(sizeof(Lin_Frame_t));
    LinFrame->eCurrentState     = IDLE;
    LinFrame->u8Break           = 0x00;
    LinFrame->u8Sync            = 0x55;
    LinFrame->u8PID             = 0x00;
    LinFrame->psPORT            = UART_DEVICE;

    /* PIN Configuration */
	PIO_Configure( pPins, PIO_LISTSIZE( pPins ) );

    /* Enable Peripheral Clock */
    PMC_EnablePeripheral(UART_ID);

    /* UART Init */
    UART_Configure(LinFrame->psPORT, (uint32_t) LIN_MODE, (uint32_t) LinBaudrate, (uint32_t) MASTER_CLK_LIN);

    /* NVIC Enable */
    NVIC_ClearPendingIRQ(UART2_IRQn);
    NVIC_EnableIRQ(UART2_IRQn);
}

/* Sends on LIN Frame if possible */
void Lin_SendFrame (uint8_t LinPid){
    /* Validate the current state */
    if (LinFrame->eCurrentState == IDLE){
        LinFrame->u8PID = LinPid;
        UART_SendByte(LinFrame->psPORT, LinFrame->u8Break);
        LinFrame->eCurrentState = SEND_BREAK;
        UART_EnableIt(LinFrame->psPORT, (uint32_t) MODE_TX_INT);
    }
    else
    {
        /* Do Nothing - Send is Busy */
    }
}

/* LIN ISR */
void Lin_Isr(void){
    UART_DisableIt(LinFrame->psPORT, (uint32_t) MODE_TX_INT);
    switch (LinFrame->eCurrentState)
    {
        case SEND_BREAK : 
            UART_SendByte(LinFrame->psPORT, LinFrame->u8Sync);
            LinFrame->eCurrentState = SEND_SYNC;
            UART_EnableIt(LinFrame->psPORT, (uint32_t) MODE_TX_INT);
            break;
        case SEND_SYNC : 
            UART_SendByte(LinFrame->psPORT, LinFrame->u8PID);
            LinFrame->eCurrentState = SEND_PID;
            UART_EnableIt(LinFrame->psPORT, (uint32_t) MODE_TX_INT);
            break;
        case SEND_PID : 
            LinFrame->eCurrentState = IDLE;
            break;
    default:
            /* This shouldn't happen */
        break;
    }
}