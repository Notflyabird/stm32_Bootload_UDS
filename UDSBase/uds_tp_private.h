#ifndef _UDS_TP_PRIVATE_H_
#define _UDS_TP_PRIVATE_H_



typedef enum __NT_TIMER_T__
{
	TIMER_N_CR = 0,                 // N_CR 定时器，接收方收到连续帧间隔时间不能大于 TIMEOUT_N_CR，单位: ms
	TIMER_N_BS,                     // N_BS 定时器，发送方发送完成首帧后到接收到流控帧之间的时间不能大于 TIMEOUT_N_BS，单位: ms
    TIMER_STmin,                    // STmin 定时器，发送连续帧时，间隔时间最小为 g_rfc_stmin,单位: ms
	TIMER_CNT                       // 定时器总个数
}nt_timer_t;

typedef enum __NETWORK_LAYER_STATUS_
{
    NWL_IDLE = 0,                   // 空闲状态
    NWL_XMIT,                       // 发送状态
    NWL_RECV,                       // 接收状态
    NWL_CNT                         // 状态数量
}network_layer_st;


typedef enum __NETWORK_PCI_TYPE_
{
    PCI_SF = 0,                     // 单帧
    PCI_FF,                         // 首帧
    PCI_CF,                         // 连续帧
    PCI_FC                          // 流控帧
}network_pci_type_t;


typedef enum __NETWORK_FLOW_STATUS__
{
	FS_CTS = 0,                    // 允许继续发送
	FS_WT,                         // 等待
	FS_OVFLW,                      // 溢出
	FS_RESERVED                    // 非法
}network_flow_status_t;

// // 填充值，如果发送的有效数据不满一帧，则用该值填充
// #define PADDING_VAL                 (0x55)

// 设置帧类型为单帧 
#define NT_SET_PCI_TYPE_SF(low)     (0x00 | (low & 0x0f))

// 设置帧类型为首帧
#define NT_SET_PCI_TYPE_FF(low)     (0x10 | (low & 0x0f))

// 设置帧类型为连续帧
#define NT_SET_PCI_TYPE_CF(low)     (0x20 | (low & 0x0f))

// 设置帧类型为流控帧
#define NT_SET_PCI_TYPE_FC(low)     (0x30 | (low & 0x0f))

// 获取帧类型
#define NT_GET_PCI_TYPE(n_pci)      (n_pci >> 4)

// 获取单帧长度
#define NT_GET_SF_DL(n_pci)         (0x0f & n_pci)

// 获取连续帧帧序号
#define NT_GET_CF_SN(n_pci)         (0x0f & n_pci)

// 获取流状态
#define NT_GET_FC_FS(n_pci)         (0x0f & n_pci)

// 允许发送连续帧的个数，若为 0，则表示发送方可以一直无限制发送连续帧，直到发送完成所有的连续帧
// 若不为 0，则表示当发送方发送的连续帧个数为 NT_XMIT_FC_BS 后需等待接收方回复一帧流控帧，发送方根据流控帧决定接下来的发送情况
#define NT_XMIT_FC_BS               (0)

// 通知发送方发送连续帧的帧间隔最小时间，单位: ms
#define NT_XMIT_FC_STMIN            (0x0A)

// 接收方收到连续帧间隔时间不能大于 TIMEOUT_N_CR，单位: ms
#define TIMEOUT_N_CR                (1000)

// 发送方发送完成首帧后到接收到流控帧之间的时间不能大于 TIMEOUT_N_BS，单位: ms
#define TIMEOUT_N_BS                (1000)

#endif
/****************EOF****************/
