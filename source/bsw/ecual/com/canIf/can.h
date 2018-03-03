#ifndef _CAN_
#define _CAN_

#include <stdint.h>
#include <stdlib.h>
#include "board.h"
#include "can_types.h"

#define MSGID   0
#define BUFFID  1

extern uint8_t u8BuffInfo[2][4];

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/* Init the CAN Interface */
void CanIf_Init (uint8_t CanChannelId, CanIf_MsgObjType CanIfMsgConfig);

/* Sends CAN Frame */
void CanIf_Transmit (uint8_t CanChannelId, uint8_t MsgId);

#endif /* #ifndef _CAN_ */