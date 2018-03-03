#ifndef _LIN_
#define _LIN_

#include <stdint.h>
#include <stdlib.h>
#include "board.h"
#include "lin_types.h"
#include "lin_cfg.h"


#define MODE_TX_INT     UART_IDR_TXRDY
#define MODE_RX_INT     UART_IDR_RXRDY
#define UART_DEVICE     UART2
#define UART_PINS       PINS_UART2
#define UART_ID         ID_UART2
#define MASTER_CLK_LIN  BOARD_MCK
#define LIN_MODE        (UART_MR_CHMODE_NORMAL | UART_MR_PAR_NO | UART_MR_BRSRCCK_PERIPH_CLK)


#define TX_ENABLED   1u
#define TX_DISABLED  0u



/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/* Init the LIN Interface */
extern void Lin_Init (const LinConfigType_t* psConfig);

/* Sends on LIN Frame if possible */
extern Std_ReturnType_t Lin_SendFrame (LinChannelType_t* psChannel, LinPduType_t* psPduInfoPtr);

/* LIN ISR */
extern void Lin_Isr (void);

/* Get the status of the LIN SDU reception */
extern Std_ReturnType_t Lin_GetSlaveResponse ( uint8_t Channel, uint8_t** LinSduPtr );

/* Additional Functions */
extern void vLinUARTInit(const LinChannelType_t* psUartConfig);

#endif /* #ifndef _LIN_ */