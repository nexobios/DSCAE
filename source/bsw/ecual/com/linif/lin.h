#ifndef _LIN_
#define _LIN_

#include <stdint.h>
#include <stdlib.h>
#include "board.h"
#include "uart.h"

#define MODE_TX_INT     UART_IDR_TXRDY
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
extern void Lin_Init (uint16_t LinBaudrate);

/* Sends on LIN Frame if possible */
extern void Lin_SendFrame (uint8_t LinPid);

/* LIN ISR */
extern void Lin_Isr(void);

/* SM States */
typedef enum
   {
     IDLE         = 0,
     SEND_BREAK,
     SEND_SYNC,
     SEND_PID,
     SEND_RESPONSE
   } LinStateType;

typedef struct Lin_Frame {
  LinStateType eCurrentState;
  uint8_t u8Break;
  uint8_t u8Sync;
  uint8_t u8PID;
  Uart *  psPORT;
} Lin_Frame_t;

#endif /* #ifndef _LIN_ */