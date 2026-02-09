#include "uds_port.h"

#include "uds_service.h"
#include "uds_tp.h"

/******************************************************************************
* 函数名称: void uds_recv_frame(uint32_t id, uint8_t* frame_buf, uint8_t frame_dlc)
* 功能说明: 接收到一帧报文
* 输入参数: uint32_t    id              --消息帧 ID
    　　　　uint8_t*    frame_buf       --接收报文帧数据首地址
    　　　　uint8_t     frame_dlc       --接收报文帧数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: frame_dlc 长度必须等于 FRAME_SIZE，否则会被判断为无效帧
******************************************************************************/
void uds_recv_frame(uint32_t id, uint8_t* frame_buf, uint8_t frame_dlc)
{
    if(REQUEST_ID == id)
        uds_tp_recv_frame(0, frame_buf, frame_dlc);
    else if(FUNCTION_ID == id)
        uds_tp_recv_frame(1, frame_buf, frame_dlc);
    else
        ; // do nothing
}


/******************************************************************************
* 函数名称: void uds_send_frame(uint32_t id, uint8_t* frame_buf, uint8_t frame_dlc)
* 功能说明: 发送一帧报文
* 输入参数: uint8_t     response_id     --应答 ID
    　　　　uint8_t*    frame_buf       --发送报文帧数据首地址
    　　　　uint8_t     frame_dlc       --发送报文帧数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: frame_dlc 长度应当等于 FRAME_SIZE
******************************************************************************/
__attribute__((weak)) void uds_send_frame(uint32_t response_id, uint8_t* frame_buf, uint8_t frame_dlc)
{
    (void)(response_id);
    (void)(frame_buf);
    (void)(frame_dlc);
}


/******************************************************************************
* 函数名称: void uds_init(void)
* 功能说明: UDS 初始化
* 输入参数: 无
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void uds_init(void)
{
    service_init();
}


/******************************************************************************
* 函数名称: void uds_1ms_task(void)
* 功能说明: UDS 周期任务
* 输入参数: 无
* 输出参数: 无
* 函数返回: 无
* 其它说明: 该函数需要被 1ms 周期调用
******************************************************************************/
void uds_1ms_task(void)
{
    network_task();
    service_task();
}
