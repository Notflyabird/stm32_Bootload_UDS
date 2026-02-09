#ifndef _UDS_TP_H_
#define _UDS_TP_H_


#include <stdint.h>

typedef enum _N_TATYPE_T_
{
    N_TATYPE_NONE = 0,                  // none
    N_TATYPE_PHYSICAL,                  // 物理寻址
    N_TATYPE_FUNCTIONAL                 // 功能寻址
}n_tatype_t;

typedef enum _N_RESULT_
{
    N_OK = 0,
    N_TIMEOUT_Bs,                       // TIMER_N_BS 定时器超时
    N_TIMEOUT_Cr,                       // TIMER_N_CR 定时器超时
    N_WRONG_SN,                         // 接收到的连续帧帧序号错误
    N_INVALID_FS,                       // 接收到的流控帧中流状态非法
    N_UNEXP_PDU,                        // 不是期待的帧类型，比如在接收连续帧中莫名收到首帧
    N_BUFFER_OVFLW,                     // 接收到的流控帧中流状态为溢出
}n_result_t;


// 上层向 TP 层注册的一些接口函数，当 TP 层对数据做完处理后再通过这些接口函数将数据交由上层继续处理
typedef void (*ffindication_func) (n_result_t n_result);
typedef void (*indication_func) (uint8_t* msg_buf, uint16_t msg_dlc, n_result_t n_result);
typedef void (*confirm_func) (n_result_t n_result);

typedef struct _NETWORK_USER_DATA_T_
{
    ffindication_func   ffindication;
    indication_func     indication; 
    confirm_func        confirm;
}nt_usdata_t;

// 0:物理寻址; 1:功能寻址
extern uint8_t g_tatype;



/******************************************************************************
* 函数名称: void network_task(void)
* 功能说明: TP 层任务处理
* 输入参数: 无
* 输出参数: 无
* 函数返回: 无
* 其它说明: 该函数需要被 1ms 周期调用
******************************************************************************/
void network_task(void);


/******************************************************************************
* 函数名称: uds_tp_recv_frame(uint8_t func_addr, uint8_t* buf, uint8_t len)
* 功能说明: 接收到一帧报文并处理
* 输入参数: uint8_t     func_addr       --0:物理寻址; 1:功能寻址
    　　　　uint8_t*    frame_buf       --接收报文帧数据首地址
    　　　　uint8_t     frame_dlc       --接收报文帧数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: frame_dlc 长度必须等于 FRAME_SIZE，否则判断为无效帧
******************************************************************************/
void uds_tp_recv_frame(uint8_t func_addr, uint8_t* frame_buf, uint8_t frame_dlc);


/******************************************************************************
* 函数名称: int network_send_udsmsg(uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 发送数据
* 输入参数: uint8_t*    msg_buf         --发送数据首地址
    　　　　uint8_t     msg_dlc         --发送数据长度
* 输出参数: 无
* 函数返回: 0: OK; -1: ERR
* 其它说明: TP 层向上层提供的数据发送接口
******************************************************************************/
int network_send_udsmsg(uint8_t* msg_buf, uint16_t msg_dlc);


/******************************************************************************
* 函数名称: int network_reg(nt_usdata_t* usdata)
* 功能说明: 上层向 TP 层注册一些接口函数，当 TP 层对数据做完处理后再通过这些接口函数将数据交由上层继续处理
* 输入参数: nt_usdata_t* usdata      --上层接口函数
* 输出参数: 无
* 函数返回: 0: OK; -1: ERR
* 其它说明: 指示服务（Indication）：用于向更上层或应用层传递状态信息及接收到的数据
    　　　　确认服务（Confirm）：用于向更上层或应用层传递状态信息
    　　　　请求服务（Request）：用于上层向网络层传递控制报文信息及要发送的数据
******************************************************************************/
int network_reg(nt_usdata_t* usdata);

#endif
/****************EOF****************/
