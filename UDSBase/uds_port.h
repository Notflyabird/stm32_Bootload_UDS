#ifndef _UDS_PORT_H_
#define _UDS_PORT_H_

#include <stdint.h>

#define FRAME_SIZE      8               // 帧长度

#define REQUEST_ID      0x123           // 请求 ID
#define FUNCTION_ID     0x7DF           // 功能 ID
#define RESPONSE_ID     0x122           // 应答 ID

#define UDS_RX_MAX      1024            // 接收缓冲区长度 --UDS TP 层协议规定最大支持 4095 字节，但是可以根据实际需要改动，以免资源浪费
#define UDS_TX_MAX      128             // 发送缓冲区长度 --UDS TP 层协议规定最大支持 4095 字节，但是可以根据实际需要改动，以免资源浪费

#define PADDING_VAL     0xAA            // 填充值，如果发送的有效数据不满一帧，则用该值填充


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
void uds_recv_frame(uint32_t id, uint8_t* frame_buf, uint8_t frame_dlc);


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
__attribute__((weak)) void uds_send_frame(uint32_t response_id, uint8_t* frame_buf, uint8_t frame_dlc);


/******************************************************************************
* 函数名称: void uds_init(void)
* 功能说明: UDS 初始化
* 输入参数: 无
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void uds_init(void);


/******************************************************************************
* 函数名称: void uds_1ms_task(void)
* 功能说明: UDS 周期任务
* 输入参数: 无
* 输出参数: 无
* 函数返回: 无
* 其它说明: 该函数需要被 1ms 周期调用
******************************************************************************/
void uds_1ms_task(void);

#endif

