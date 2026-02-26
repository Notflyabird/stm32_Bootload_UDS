#include "SID34_36_37_UpDownloadData.h"
#include "service_cfg.h"
#include "uds_service.h"
#include "SID10_SessionControl.h"
#include "stm32f1xx_hal.h" /* for HAL_FLASH_Program */
#include "flash_program.h" /* for flash_write_data() */

#define DOWNLOAD    1
#define UPLOAD      2
#define maxNumberOfBlockLength 498
uint8_t UpDownLoadReq = 0;
uint8_t encryptingMethod =0;
uint8_t compressionMethod =0;
uint32_t memoryAddress =0;
uint32_t memorySize =0;
uint8_t blockSequenceCounter =0;
uint32_t writtenBytes = 0; /* Track bytes written during download */
/******************************************************************************
* 函数名称: bool_t service_34_check_len(const uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 检查 34 服务数据长度是否合法
* 输入参数: uint16_t msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: TRUE: 合法; FALSE: 非法
* 其它说明: 无
******************************************************************************/
bool_t service_34_check_len(const uint8_t* msg_buf, uint16_t msg_dlc)
{
	bool_t ret = FALSE;
	
	(void)msg_buf;
	if(msg_dlc >= 5)
		ret = TRUE;

	return ret;
}

/******************************************************************************
* 函数名称: bool_t service_36_check_len(const uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 检查 36 服务数据长度是否合法
* 输入参数: uint16_t msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: TRUE: 合法; FALSE: 非法
* 其它说明: 无
******************************************************************************/
bool_t service_36_check_len(const uint8_t* msg_buf, uint16_t msg_dlc)
{
	bool_t ret = FALSE;
	
	(void)msg_buf;
    if(UpDownLoadReq == DOWNLOAD)
    {
        if(msg_dlc >= 3)
		ret = TRUE;
    }
    else
    {
	if(msg_dlc >= 2)
		ret = TRUE;
    }
	return ret;
}

/******************************************************************************
* 函数名称: bool_t service_37_check_len(const uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 检查 37 服务数据长度是否合法
* 输入参数: uint16_t msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: TRUE: 合法; FALSE: 非法
* 其它说明: 无
******************************************************************************/
bool_t service_37_check_len(const uint8_t* msg_buf, uint16_t msg_dlc)
{
	bool_t ret = FALSE;
	
	(void)msg_buf;
	if(msg_dlc >= 1)
		ret = TRUE;

	return ret;
}


/******************************************************************************
* 函数名称: void service_34_RequestDownload(const uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 34 服务 - 请求下载
* 输入参数: uint8_t*    msg_buf         --数据首地址
    　　　　uint8_t     msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void service_34_RequestDownload(const uint8_t* msg_buf, uint16_t msg_dlc)
{
    uint8_t rsp_buf[6];
    uint8_t mslen, malen;
    uint8_t i;

    rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_34);
	rsp_buf[1] = 0x20;
    rsp_buf[2] = maxNumberOfBlockLength >>8;
    rsp_buf[3] = maxNumberOfBlockLength & 0xFF;

    compressionMethod = msg_buf[1] >>4;
    encryptingMethod  = msg_buf[1] &0x0f;
    mslen= msg_buf[2] >>4;
    malen= msg_buf[2] &0x0f;

    UpDownLoadReq = DOWNLOAD;
    memoryAddress = 0;
    memorySize = 0;
    
    /* Parse memory address (big-endian, network byte order) */
    for(i = 0; i < malen; i++)
    {
        memoryAddress |= ((uint32_t)msg_buf[3 + i] << ((malen - 1 - i) * 8));
    }
    
    /* Parse memory size (big-endian, network byte order) */
    for(i = 0; i < mslen; i++)
    {
        memorySize |= ((uint32_t)msg_buf[3 + malen + i] << ((mslen - 1 - i) * 8));
    }

    if((mslen > 4) || (malen > 4))
    {
        /* Restrict memorySize and memoryAddress parameter length to less than 4 bytes */
        uds_negative_rsp(SID_34, NRC_REQUEST_OUT_OF_RANGE);
    }
    else if((memoryAddress < FLASH_BASE) || ((memoryAddress + memorySize) > (FLASH_BASE + 512 * 1024)))
    {
        /* Validate address range (STM32F103 has 512KB flash) */
        uds_negative_rsp(SID_34, NRC_REQUEST_OUT_OF_RANGE);
    }
    else
    {
        writtenBytes = 0;
        blockSequenceCounter = 0;
        uds_positive_rsp(rsp_buf, 4);
    }
			
}

void ProgramDataToFlash(const uint8_t* msg_buf, uint16_t lenth)
{
    /*需要考虑加密压缩算法*/

}

/******************************************************************************
* 函数名称: void service_36_TransferData(const uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 36 服务 - 数据传输
* 输入参数: uint8_t*    msg_buf         --数据首地址
    　　　　uint8_t     msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void service_36_TransferData(const uint8_t* msg_buf, uint16_t msg_dlc)
{
    static uint8_t LastblockSequenceCounter = 0;
    uint8_t rsp_buf[6];
    uint16_t dataLen;
    uint32_t writeAddr;
    uint32_t remainingBytes;
    HAL_StatusTypeDef status;

    rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_36);
    rsp_buf[1] = msg_buf[1];

    LastblockSequenceCounter = blockSequenceCounter;
    blockSequenceCounter = msg_buf[1]; 

    if((UpDownLoadReq != DOWNLOAD) && (UpDownLoadReq != UPLOAD))
    {
        uds_negative_rsp(SID_36, NRC_REQUEST_SEQUENCE_ERROR);
    }
    else if((blockSequenceCounter - LastblockSequenceCounter) > 1)
    {
        uds_negative_rsp(SID_36, NRC_WRONG_BLOCK_SEQUENCE_COUNTER);
    }
    else
    {
        if(UpDownLoadReq == DOWNLOAD)
        {
            if(LastblockSequenceCounter == blockSequenceCounter)
            {
                /* Duplicate block sequence, no write */
            }
            else
            {
                dataLen = msg_dlc - 2;
                writeAddr = memoryAddress + writtenBytes;
                remainingBytes = memorySize - writtenBytes;

                /* Check if this block would exceed total download size */
                if(dataLen > remainingBytes)
                {
                    uds_negative_rsp(SID_36, NRC_REQUEST_OUT_OF_RANGE);
                    return;
                }

                /* Write data to Flash */
                status = flash_write_data(writeAddr, &msg_buf[2], dataLen);
                
                if(status == HAL_OK)
                {
                    writtenBytes += dataLen;
                }
                else
                {
                    uds_negative_rsp(SID_36, NRC_GENERAL_PROGRAMMING_FAILURE);
                    return;
                }
            }
            uds_positive_rsp(rsp_buf, 2);
        }
    }
    	
}

/******************************************************************************
* 函数名称: void service_37_RequestTransferExit(const uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 37 服务 - 请求结束传输
* 输入参数: uint8_t*    msg_buf         --数据首地址
    　　　　uint8_t     msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void service_37_RequestTransferExit(const uint8_t* msg_buf, uint16_t msg_dlc)
{
    uint8_t rsp_buf[6];

    rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_37);
    blockSequenceCounter = msg_buf[1];

    if((UpDownLoadReq != DOWNLOAD) && (UpDownLoadReq != UPLOAD))
    {
        uds_negative_rsp(SID_37, NRC_REQUEST_SEQUENCE_ERROR);
    }
    else
    {
        UpDownLoadReq = 0;
        uds_positive_rsp(rsp_buf, 1);
    }
}
/****************EOF****************/