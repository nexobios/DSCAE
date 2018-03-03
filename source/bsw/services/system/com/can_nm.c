/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "can_nm.h"

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

CanIf_MessageConfigType CanTxMsg[] =
{
   { 0, CAN_TX, { 0x1A0, CAN_STD_ID, CAN_DLC_3, (uint8_t *)0} },
   { 1, CAN_TX, { 0x123456A0, CAN_EXT_ID, CAN_DLC_6, (uint8_t *)0} },
   { 2, CAN_TX, { 0x1B0, CAN_STD_ID, CAN_DLC_7, (uint8_t *)0} },
   { 3, CAN_TX, { 0x123456B0, CAN_EXT_ID, CAN_DLC_8, (uint8_t *)0} }
};

CanIf_MsgObjType CanMsgObj =
{
    sizeof(CanTxMsg)/sizeof(CanIf_MessageConfigType),
    CanTxMsg
};

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/ 

void vCANInit(){
    CanIf_Init ( (uint8_t)CAN_CHANNEL, CanMsgObj );
}

void vCANSendMsg(){
    static uint8_t u8MsgID = 0;
    
    CanIf_Transmit ((uint8_t)CAN_CHANNEL, u8MsgID);
    
    if(u8MsgID >= CanMsgObj.CanNumberOfMsgs){
        u8MsgID = 0;
    }
    else
    {
        u8MsgID++;
    }
}