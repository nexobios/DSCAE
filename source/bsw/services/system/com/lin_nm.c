/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "lin_nm.h"

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

uint8_t au8ResponseArray[MESSAGE_BUFFER][PAYLOAD_SIZE] = {
    {0x00, 0xA1, 0x2A, 0x30, 0xF4, 0xF5, 0x65, 0xA7},
    {0x01, 0x41, 0x2F, 0x73, 0x44, 0xD5, 0x67, 0xF7},
    {0x03, 0x67, 0x25, 0x03, 0x54, 0x50, 0x86, 0xC7},
    {0xF0, 0x71, 0x26, 0x53, 0x54, 0x05, 0x96, 0x87},
    {0x00, },
};

LinPduType_t aMessageList[MESSAGE_BUFFER] = {
    {0x10, LIN_ENHANCED_CS, LIN_MASTER_RESPONSE, PAYLOAD_SIZE, (uint8_t*) &au8ResponseArray[0] },
    {0x11, LIN_ENHANCED_CS, LIN_MASTER_RESPONSE, PAYLOAD_SIZE, (uint8_t*) &au8ResponseArray[1] },
    {0x12, LIN_ENHANCED_CS, LIN_MASTER_RESPONSE, PAYLOAD_SIZE, (uint8_t*) &au8ResponseArray[2] },
    {0x13, LIN_ENHANCED_CS, LIN_MASTER_RESPONSE, PAYLOAD_SIZE, (uint8_t*) &au8ResponseArray[3] },
    {0x14, LIN_ENHANCED_CS, LIN_SLAVE_RESPONSE , PAYLOAD_SIZE, (uint8_t*) &au8ResponseArray[4] }
};

uint8_t u8DataCounter = 0;

LinChannelType_t sChannelType =  {UART_CH, BAUD_RATE};

LinConfigType_t sConfig = {
    (LinChannelType_t*) &sChannelType,
    1
};

void Lin_NM_10ms (void)
{
    Lin_SendFrame((LinChannelType_t *)&sConfig.LinChannel, &aMessageList[u8DataCounter]);
    u8DataCounter++;

    if(u8DataCounter >= 5)
        u8DataCounter = 0;
}
