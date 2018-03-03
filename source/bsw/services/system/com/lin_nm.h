#ifndef _LIN_NM_
#define _LIN_NM_

#include <stdint.h>
#include "lin.h"

#define UART_CH                 UART2
#define BAUD_RATE               9600
#define MESSAGE_BUFFER          5
#define PAYLOAD_SIZE            8

/*----------------------------------------------------------------------------
 *        Variables
 *----------------------------------------------------------------------------*/

extern uint8_t au8ResponseArray[MESSAGE_BUFFER][PAYLOAD_SIZE];
extern LinPduType_t aMessageList[MESSAGE_BUFFER];
extern LinChannelType_t sChannelType;
extern LinConfigType_t sConfig;

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/* Init the LIN Interface */
extern void Lin_NM_10ms (void);

#endif /* #ifndef _LIN_NM_ */