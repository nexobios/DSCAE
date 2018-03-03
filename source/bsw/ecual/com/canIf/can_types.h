#ifndef _CAN_TYPES_
#define _CAN_TYPES_

#include <stdint.h>
#include <stdlib.h>
#include "board.h"
#include "mcan.h"

/*----------------------------------------------------------------------------
 *        Data Types
 *----------------------------------------------------------------------------*/

typedef enum
{
    CAN_TX = 0,
    CAN_RX = 1,
} MCan_DirType;

typedef struct
{
    uint32_t CanId;
    MCan_IdType CanIdType;
    MCan_DlcType CanDlc;
    uint8_t* CanSdu;
} CanIf_PduType;

typedef struct
{
    uint8_t CanMsgIdNumber;
    MCan_DirType MCanDir;
    CanIf_PduType CanPdu;
} CanIf_MessageConfigType;

typedef struct
{
    uint8_t CanNumberOfMsgs;
    CanIf_MessageConfigType *CanIfMessageConfig;
} CanIf_MsgObjType;

#endif /* #ifndef _CAN_TYPES_ */