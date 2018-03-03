#ifndef _CAN_NM_
#define _CAN_NM_

#include <stdint.h>
#include "can.h"

#define CAN_CHANNEL 0

/*----------------------------------------------------------------------------
 *        Variables
 *----------------------------------------------------------------------------*/

extern CanIf_MessageConfigType CanTxMsg[];
extern CanIf_MsgObjType CanMsgObj;

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/ 

void vCANInit();
void vCANSendMsg();

#endif /* #ifndef _CAN_NM_ */