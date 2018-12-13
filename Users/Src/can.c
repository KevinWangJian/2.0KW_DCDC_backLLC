
#include <stdlib.h>
#include "can.h"
#include "timtick.h"

/*
 * @brief  
 * @param  
 * @retval 
 */
void canPeripheralInit_LL(void)
{
    /* Filter Parameters */
    CAN_FilterNumber_TypeDef CAN_FilterNumber;
    FunctionalState CAN_FilterActivation;
    CAN_FilterMode_TypeDef CAN_FilterMode;
    CAN_FilterScale_TypeDef CAN_FilterScale;
    uint8_t CAN_FilterID1;
    uint8_t CAN_FilterID2;
    uint8_t CAN_FilterID3;
    uint8_t CAN_FilterID4;
    uint8_t CAN_FilterIDMask1;
    uint8_t CAN_FilterIDMask2;
    uint8_t CAN_FilterIDMask3;
    uint8_t CAN_FilterIDMask4; 
    
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_CAN, ENABLE);
  
    CAN_DeInit();
    
#if (CAN_BAUDRATE == 125)
    CAN_Init((CAN_MasterCtrl_TypeDef)(CAN_MasterCtrl_AutoBusOffManagement | CAN_MasterCtrl_AutoWakeUpMode | CAN_MasterCtrl_NoAutoReTx | CAN_MasterCtrl_TxFifoPriority),
             CAN_Mode_Normal,
             CAN_SynJumpWidth_1TimeQuantum,
             CAN_BitSeg1_8TimeQuantum,
             CAN_BitSeg2_7TimeQuantum,
             8);
#elif (CAN_BAUDRATE == 250)
    CAN_Init((CAN_MasterCtrl_TypeDef)(CAN_MasterCtrl_AutoBusOffManagement | CAN_MasterCtrl_AutoWakeUpMode | CAN_MasterCtrl_NoAutoReTx | CAN_MasterCtrl_TxFifoPriority),
             CAN_Mode_Normal,
             CAN_SynJumpWidth_1TimeQuantum,
             CAN_BitSeg1_4TimeQuantum,
             CAN_BitSeg2_3TimeQuantum,
             8);   
#elif (CAN_BAUDRATE == 500)
    CAN_Init((CAN_MasterCtrl_TypeDef)(CAN_MasterCtrl_AutoBusOffManagement | CAN_MasterCtrl_AutoWakeUpMode | CAN_MasterCtrl_NoAutoReTx | CAN_MasterCtrl_TxFifoPriority),
             CAN_Mode_Normal,
             CAN_SynJumpWidth_1TimeQuantum,
             CAN_BitSeg1_4TimeQuantum,
             CAN_BitSeg2_3TimeQuantum,
             4);    
#elif (CAN_BAUDRATE == 1000)
    CAN_Init((CAN_MasterCtrl_TypeDef)(CAN_MasterCtrl_AutoBusOffManagement | CAN_MasterCtrl_AutoWakeUpMode | CAN_MasterCtrl_NoAutoReTx | CAN_MasterCtrl_TxFifoPriority),
             CAN_Mode_Normal,
             CAN_SynJumpWidth_1TimeQuantum,
             CAN_BitSeg1_11TimeQuantum,
             CAN_BitSeg2_4TimeQuantum,
             1);    
#endif
    /* CAN filter init */
    CAN_FilterNumber        = CAN_FilterNumber_0;
    CAN_FilterActivation    = ENABLE;
    CAN_FilterMode          = CAN_FilterMode_IdMask;
    CAN_FilterScale         = CAN_FilterScale_32Bit;
    CAN_FilterID1           = 0;  
    CAN_FilterID2           = 0;
    CAN_FilterID3           = 0;
    CAN_FilterID4           = 0;
    CAN_FilterIDMask1       = 0;
    CAN_FilterIDMask2       = 0;
    CAN_FilterIDMask3       = 0;
    CAN_FilterIDMask4       = 0;  
    
    CAN_FilterInit(CAN_FilterNumber,
                   CAN_FilterActivation,
                   CAN_FilterMode,
                   CAN_FilterScale,
                   CAN_FilterID1,
                   CAN_FilterID2,
                   CAN_FilterID3,
                   CAN_FilterID4,
                   CAN_FilterIDMask1,
                   CAN_FilterIDMask2,
                   CAN_FilterIDMask3,
                   CAN_FilterIDMask4);
    
#if (CANRX_INT_ENABLE == 1)
    CAN_ITConfig(CAN_IT_FMP, ENABLE);     /* Enable Fifo message pending interrupt,Message reception is done by CAN_RX ISR*/
#endif
}

/*
 * @brief  
 * @param  
 * @retval 
 */
int canSendMessage_LL(CAN_MessageTypeDef* pSmsg)
{
    uint32_t id;
    CAN_Id_TypeDef idType;
    CAN_RTR_TypeDef rtrType;
    uint8_t dLc;
    uint8_t sData[8], i;
    CAN_TxStatus_TypeDef canTxStatus;
    
    if (pSmsg == NULL)return (-1);
    
    if ((pSmsg->frameType == DataFrameWithStandardId) || \
        (pSmsg->frameType == RemoteFrameWithStandardId))
    {
        if (pSmsg->frameId > 0x7FF)return (-1);
        
        idType = CAN_Id_Standard;
        id = pSmsg->frameId;
        
        if (pSmsg->frameType == DataFrameWithStandardId)
        {
            rtrType = CAN_RTR_Data;
            
            if (pSmsg->dLc > 8)return (-1);
            dLc = pSmsg->dLc;
            
            for (i = 0; i < dLc; i++)
            {
                sData[i] = pSmsg->data[i];
            }
        }
        else
        {
            rtrType = CAN_RTR_Remote;
            dLc = 0;
        }
    }
    else if ((pSmsg->frameType == DataFrameWithExtendedId) || \
			 (pSmsg->frameType == RemoteFrameWithExtendedId))
    {
        if (pSmsg->frameId > 0x1FFFFFFF)return (-1);
        
        idType = CAN_Id_Extended;
        id = pSmsg->frameId;
        
        if (pSmsg->frameType == DataFrameWithExtendedId)
        {
            rtrType = CAN_RTR_Data;
 
            if (pSmsg->dLc > 8)return (-1);
            dLc = pSmsg->dLc;
            
            for (i = 0; i < dLc; i++)
            {
                sData[i] = pSmsg->data[i];
            }
        }
        else
        {
            rtrType = CAN_RTR_Remote;
            dLc = 0;            
        }
    }
    else
    {
        return (-1);
    }
    
    canTxStatus = CAN_Transmit(id, idType, rtrType, dLc, sData);
    
    if (canTxStatus != CAN_TxStatus_NoMailBox)
    {
#if 1
       uint32_t tickStart = HAL_GetTick();              /* Get tick */
        
       while (CAN_TransmitStatus((CAN_TransmitMailBox_TypeDef)canTxStatus) != CAN_TxStatus_Ok)
       {
          if ((HAL_GetTick() - tickStart) > 100)        /* Transmit timeout while in 100ms. */
          {
              CAN_CancelTransmit((CAN_TransmitMailBox_TypeDef)canTxStatus);
              
              return (-1);
          }
       }
#endif
    }
    else
    {
        return (-1);
    }

    return (0);
}

/*
 * @brief  
 * @param  
 * @retval 
 */
int canReceiveMessage_LL(CAN_MessageTypeDef* pRmsg)
{
    uint8_t i;
    
    if (pRmsg == NULL)return (-1);
    
    CAN_Receive();
    
    pRmsg->frameId = CAN_GetReceivedId();
    
    if (CAN_GetReceivedIDE() == CAN_Id_Standard)
    {
        if (pRmsg->frameId > 0x7FF)return (-1);
        
        if (CAN_GetReceivedRTR() == CAN_RTR_Data)
        {
            pRmsg->frameType = DataFrameWithStandardId;
            pRmsg->dLc = CAN_GetReceivedDLC();
            
            if (pRmsg->dLc > 8)return (-1);
        }
        else if (CAN_GetReceivedRTR() == CAN_RTR_Remote)
        {
            pRmsg->frameType = RemoteFrameWithStandardId;
            pRmsg->dLc = 0;
        }
        else
        {
            return (-1);
        }
    }
    else if (CAN_GetReceivedIDE() == CAN_Id_Extended)
    {
        if (pRmsg->frameId > 0x1FFFFFFF)return (-1);
        
        if (CAN_GetReceivedRTR() == CAN_RTR_Data)
        {
            pRmsg->frameType = DataFrameWithExtendedId;
            pRmsg->dLc = CAN_GetReceivedDLC();
            
            if (pRmsg->dLc > 8)return (-1);
        }
        else if (CAN_GetReceivedRTR() == CAN_RTR_Remote)
        {
            pRmsg->frameType = RemoteFrameWithExtendedId;
            pRmsg->dLc = 0;
        }
    }
    else
    {
        return (-1);
    }
    
    for (i = 0; i < pRmsg->dLc; i++)
    {
        pRmsg->data[i] = CAN_GetReceivedData(i);
    }
    
    return (0);
}
