/******************************************************************************
* 文件名称: msg_buf.c
* 内容摘要: 数据收发循环缓冲区
* 创建者の: 孔佳伟
* 个人主页: https://gitee.com/thin-wind/jump
* 修改记录: 
******************************************************************************/

#include "msg_buf.h"
#include "ring.h"
#include <string.h>

#define RING_BUF_SIZE   8

typedef struct RingQueue_S 
{
    uint32_t            head;					// 循环队列缓冲器中数据头指针
	uint32_t            tail;					// 循环队列缓冲器中数据尾指针
    uint32_t            len;                    // 循环队列缓冲器数据长度
    Msg_S               buf[RING_BUF_SIZE];     // 循环队列缓冲器的缓冲区
} RingQueue_S;

static RingQueue_S      rxQueue = {0};          // 数据接收循环缓冲器
static RingQueue_S      txQueue = {0};          // 数据发送循环缓冲器


/******************************************************************************
* 函数名称: int PutToRxBuf(const Msg_S *msg)
* 功能说明: 向数据接收循环缓冲区中放入一帧数据
* 输入参数: const Msg_S *msg		--数据节点
* 输出参数: 无
* 函数返回: 0:成功; -1:失败
* 其它说明: 无
******************************************************************************/
int PutToRxBuf(const Msg_S *msg)
{
    Msg_S msgTmp = {0};

    // 检查参数合法性
    if(NULL == msg)
        return -1;

    // 若循环缓冲区已满，则直接返回
    if(RING_BUF_SIZE == RING_NUMS(&rxQueue))
        return -1;

    // 往数据接收循环缓冲区中放入一帧数据
    msgTmp.id = msg->id;
    memcpy(msgTmp.data, msg->data, FRAME_SIZE);
    RING_PUT(msgTmp, &rxQueue);
    
    return 0;
}

/******************************************************************************
* 函数名称: int GetFromRxBuf(Msg_S *msg)
* 功能说明: 从数据接收循环缓冲区中读取一帧数据
* 输入参数: 无
* 输出参数: const Msg_S *msg		--数据节点
* 函数返回: 0:成功; -1:失败
* 其它说明: 无
******************************************************************************/
int GetFromRxBuf(Msg_S *msg)
{
    static Msg_S msgTmp = {0};

    // 检查参数合法性
    if(NULL == msg)
        return -1;

    // 若循环缓冲区为空，则直接返回
    if(0 == RING_NUMS(&rxQueue))
        return -1;

    // 从数据接收循环缓冲区中读取一帧数据
    RING_GET(&rxQueue, msgTmp);
    msg->id = msgTmp.id;
    memcpy(msg->data, msgTmp.data, FRAME_SIZE);

    return 0;
}

/******************************************************************************
* 函数名称: int PutToTxBuf(const Msg_S *msg)
* 功能说明: 向数据发送循环缓冲区中放入一帧数据
* 输入参数: const Msg_S *msg		--数据节点
* 输出参数: 无
* 函数返回: 0:成功; -1:失败
* 其它说明: 无
******************************************************************************/
int PutToTxBuf(const Msg_S *msg)
{
    Msg_S msgTmp = {0};

    // 检查参数合法性
    if(NULL == msg)
        return -1;

    // 若循环缓冲区已满，则直接返回
    if(RING_BUF_SIZE == RING_NUMS(&txQueue))
        return -1;

    // 往数据接收循环缓冲区中放入一帧数据
    msgTmp.id = msg->id;
    memcpy(msgTmp.data, msg->data, FRAME_SIZE);
    RING_PUT(msgTmp, &txQueue);
    
    return 0;
}

/******************************************************************************
* 函数名称: int GetFromTxBuf(Msg_S *msg)
* 功能说明: 从数据发送循环缓冲区中读取一帧数据
* 输入参数: 无
* 输出参数: const Msg_S *msg		--数据节点
* 函数返回: 0:成功; -1:失败
* 其它说明: 无
******************************************************************************/
int GetFromTxBuf(Msg_S *msg)
{
    static Msg_S msgTmp = {0};

    // 检查参数合法性
    if(NULL == msg)
        return -1;

    // 若循环缓冲区为空，则直接返回
    if(0 == RING_NUMS(&txQueue))
        return -1;

    // 从数据发送循环缓冲区中读取一帧数据
    RING_GET(&txQueue, msgTmp);
    msg->id = msgTmp.id;
    memcpy(msg->data, msgTmp.data, FRAME_SIZE);

    return 0;
}



