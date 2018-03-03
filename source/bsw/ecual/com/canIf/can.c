/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "can.h"

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/
uint8_t u8BuffInfo[2][4] = {{0, 0, 0, 0},{0, 1, 2, 3}};


/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

 /* Init the CAN Interface */
void CanIf_Init (uint8_t CanChannelId, CanIf_MsgObjType CanIfMsgConfig){

    uint8_t u8Idx = 0;
    const MCan_ConfigType* pMCAN_Cfg = (CanChannelId == 0) ? &mcan0Config : &mcan1Config;
    MCAN_Init( pMCAN_Cfg );

    /* Validate if the Msg is CAN_TX */
    for(u8Idx = 0; u8Idx < CanIfMsgConfig.CanNumberOfMsgs; u8Idx++){
        if(CanIfMsgConfig.CanIfMessageConfig[u8Idx].MCanDir == CAN_TX){
            /* SDU Buffer Configuration */
            u8BuffInfo[MSGID][u8Idx] = CanIfMsgConfig.CanIfMessageConfig[u8Idx].CanMsgIdNumber;
            CanIfMsgConfig.CanIfMessageConfig[u8Idx].CanPdu.CanSdu = MCAN_ConfigTxDedBuffer( pMCAN_Cfg, u8BuffInfo[BUFFID][u8Idx], CanIfMsgConfig.CanIfMessageConfig[u8Idx].CanPdu.CanId, CanIfMsgConfig.CanIfMessageConfig[u8Idx].CanPdu.CanIdType, CanIfMsgConfig.CanIfMessageConfig[u8Idx].CanPdu.CanDlc );
        }
    }
    MCAN_InitTxQueue( pMCAN_Cfg );
    MCAN_Enable( pMCAN_Cfg );
}

/* Sends CAN Frame */
void CanIf_Transmit (uint8_t CanChannelId, uint8_t MsgId){
    
    if(CanChannelId == 0)
        MCAN_SendTxDedBuffer( &mcan0Config, u8BuffInfo[BUFFID][MsgId] );
    else
        MCAN_SendTxDedBuffer( &mcan1Config, u8BuffInfo[BUFFID][MsgId] );
}