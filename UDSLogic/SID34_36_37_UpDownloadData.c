#include "SID34_36_37_UpDownloadData.h"
#include "service_cfg.h"
#include "uds_service.h"
#include "SID10_SessionControl.h"

#define DOWNLOAD    1
#define UPLOAD      2
#define maxNumberOfBlockLength 512
uint8_t UpDownLoadReq = 0;
uint8_t encryptingMethod =0;
uint8_t compressionMethod =0;
uint32_t memoryAddress =0;
uint32_t memorySize =0;
uint8_t blockSequenceCounter =0;
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
	uint8_t mslen,malen;

    rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_34);
	rsp_buf[1] = 0x40;
	rsp_buf[2] = maxNumberOfBlockLength >>24;
    rsp_buf[3] = maxNumberOfBlockLength >>16;
    rsp_buf[4] = maxNumberOfBlockLength >>8;
    rsp_buf[5] = maxNumberOfBlockLength;

    compressionMethod = msg_buf[1] >>4;
    encryptingMethod  = msg_buf[1] &0x0f;
    mslen= msg_buf[2] >>4;
    malen= msg_buf[2] &0x0f;

    UpDownLoadReq = DOWNLOAD;
    memoryAddress =0;
    memorySize =0;
    for(uint8_t i=0; i<malen; i++)
    {
         memoryAddress |= ((uint32_t)msg_buf[3+i] <<(i*8));
    }
    for(uint8_t i=0; i<mslen; i++)
    {
         memorySize |= ((uint32_t)msg_buf[3+malen+i] <<(i*8));
    }

    if((mslen >4 )|| (malen >4 ))// 限制 memorySize parameter memoryAddress parameter 长度小于4字节
    {
        uds_negative_rsp(SID_34, NRC_REQUEST_OUT_OF_RANGE);
    }
    else
	{
        uds_positive_rsp(rsp_buf, 6);
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
    static uint8_t  LastblockSequenceCounter =0;
    uint8_t rsp_buf[6];

    rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_36);
	rsp_buf[1] = msg_buf[1];

    LastblockSequenceCounter = blockSequenceCounter;
    blockSequenceCounter = msg_buf[1]; 

    if((UpDownLoadReq != DOWNLOAD) &&(UpDownLoadReq != UPLOAD))
    {
        uds_negative_rsp(SID_36, NRC_REQUEST_SEQUENCE_ERROR);
    }
    else if((blockSequenceCounter - LastblockSequenceCounter) >1)
    {
        uds_negative_rsp(SID_36, NRC_WRONG_BLOCK_SEQUENCE_COUNTER);
    }
    else
    {
        if(UpDownLoadReq == DOWNLOAD)
        {
            if(LastblockSequenceCounter == blockSequenceCounter)//不刷写，但回复正响应
            {

            }
            else
            {
                ProgramDataToFlash(msg_buf[2],msg_dlc-2);//刷写数据 ，msg_dlc长度减去SID 与 blockSequenceCounter
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

    if((UpDownLoadReq != DOWNLOAD) &&(UpDownLoadReq != UPLOAD))
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