#ifndef	__UDS_SERVICE_H_
#define	__UDS_SERVICE_H_


#include <stdint.h>
#include "uds_type.h"


typedef enum __UDS_NRC_ENUM__
{
	NRC_GENERAL_REJECT								= 0x10, // 该服务响应不是协议里已支持的
	NRC_SERVICE_NOT_SUPPORTED						= 0x11,	// ECU 压根就没做这个服务
	NRC_SUBFUNCTION_NOT_SUPPORTED 					= 0x12,	// ECU 不支持当前请求的子功能
	NRC_INVALID_MESSAGE_LENGTH_OR_FORMAT			= 0x13, // 请求报文的长度或者格式不正确
	NRC_CONDITIONS_NOT_CORRECT						= 0x22, // 先决条件不满足
	NRC_REQUEST_SEQUENCE_ERROR						= 0x24, // 请求报文的顺序不正确
	NRC_REQUEST_OUT_OF_RANGE						= 0x31, // 参数超出范围/数据 ID 不支持
    NRC_SECURITY_ACCESS_DENIED						= 0x33, // 不满足安全策略，请先解锁
    NRC_INVALID_KEY									= 0x35, // 密钥不匹配
    NRC_EXCEEDED_NUMBER_OF_ATTEMPTS					= 0x36, // 尝试解锁次数已达上限
    NRC_REQUIRED_TIME_DELAY_NOT_EXPIRED				= 0x37, // 安全访问失败，超时时间未到
	NRC_UPLOAD_DOWNLOAD_NOT_ACCEPTED				= 0x70, // 不允许上传/下载
	NRC_TRANSFER_DATA_SUSPENDED						= 0x71, // 数据传输终止
    NRC_GENERAL_PROGRAMMING_FAILURE					= 0x72, // 擦除或烧写内存时错误
	NRC_WRONG_BLOCK_SEQUENCE_COUNTER				= 0x73, // 块序列计数错误
    NRC_SERVICE_BUSY								= 0x78, // 已正确接收请求消息，但会晚些回复
	NRC_SUBFUNCTION_NOT_SUPPORTED_IN_ACTIVE_SESSION = 0x7E, // 当前会话下，该子功能不支持
	NRC_SERVICE_NOT_SUPPORTED_IN_ACTIVE_SESSION		= 0x7F, // 当前会话下，该服务不支持
	NRC_VOLTAGE_TOO_HIGH							= 0x92, // 电压过高
	NRC_VOLTAGE_TOO_LOW								= 0x93, // 电压过低
}uds_nrc_em;


// 第二个字节中的最高位 bit7 表示肯定响应抑制位
// 当其置 1 时，则表示不需要回复肯定响应，只进行服务处理即可
#define UDS_GET_SUB_FUNCTION_SUPPRESS_POSRSP(byte)    ((byte >> 7u)&0x01u)

// 获取子功能号 - 第二个字节中的低 7 位表示子功能号，范围：0 ~ 0x7F
#define UDS_GET_SUB_FUNCTION(byte)     (byte & 0x7fu)

// 肯定响应，服务 ID 需 +0x40
#define POSITIVE_RSP 			0x40
#define USD_GET_POSITIVE_RSP(server_id)         (POSITIVE_RSP + server_id)

// 否定响应
#define NEGATIVE_RSP 			0x7F

// 安全访问超时时间，单位：ms，如果安全访问种子匹配次数达到两次时，开启该定时器，在 TIMEOUT_FSA 时间内如果收到请求种子服务，则需要回复 NRC 37
#define TIMEOUT_FSA          	10000

// S3server 定时器超时时间，在非默认会话模式下，如果在 TIMEOUT_S3server 时间内没有收到任何消息的话，将自动回到默认会话，单位：ms
#define TIMEOUT_S3server     5000


typedef enum __UDS_TIMER_T__
{
	UDS_TIMER_FSA = 0,			// FSA 定时器，如果安全访问种子匹配次数达到两次时，开启该定时器，在 TIMEOUT_FSA 时间内如果收到请求种子服务，则需要回复 NRC 37
	UDS_TIMER_S3server,			// S3server 定时器，在非默认会话模式下，如果在 TIMEOUT_S3server 时间内没有收到任何消息的话，将自动回到默认会话
	UDS_TIMER_CNT				// 应用层定时器总个数
}uds_timer_t;


typedef enum __UDS_SESSION_T_
{
	UDS_SESSION_STD = 1,		// 默认会话
	UDS_SESSION_PROG,			// 编程会话
	UDS_SESSION_EXT				// 扩展会话
}uds_session_t;


typedef enum __UDS_SA_LV__
{
	UDS_SA_NON = 0,				// 安全访问等级 - 无
	UDS_SA_LV1,					// 安全访问等级 - 1 级
	UDS_SA_LV2,					// 安全访问等级 - 2 级
}uds_sa_lv;


typedef struct __UDS_SERVICE_T__
{
    uint8_t uds_sid;									// 服务 ID
	void (* uds_service)  (const uint8_t *, uint16_t);	// 服务处理函数
	bool_t (* check_len)  (const uint8_t *, uint16_t);	// 检查数据长度是否合法
    bool_t std_spt;   									// 是否支持默认会话
	bool_t prog_spt;  									// 是否支持编程会话
    bool_t ext_spt;   									// 是否支持扩展会话
    bool_t fun_spt;   									// 是否支持功能寻址
	bool_t ssp_spt;  									// 是否支持肯定响应抑制
    uds_sa_lv uds_sa; 									// 安全访问等级
}uds_service_t;


/******************************************************************************
* 函数名称: void uds_timer_start(uds_timer_t num)
* 功能说明: 启动应用层定时器
* 输入参数: uds_timer_t num              --定时器
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void uds_timer_start (uds_timer_t num);


/******************************************************************************
* 函数名称: void uds_negative_rsp(uint8_t sid, uds_nrc_em rsp_nrc)
* 功能说明: 否定响应
* 输入参数: uint8_t sid              	--服务 ID
    　　　　uds_nrc_em rsp_nrc          --否定响应具体原因
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void uds_negative_rsp(uint8_t sid, uds_nrc_em rsp_nrc);


/******************************************************************************
* 函数名称: void uds_positive_rsp(uint8_t* data, uint16_t len)
* 功能说明: 否定响应
* 输入参数: uint8_t* data             	--正响应回复数据首地址
    　　　　uint16_t len          		--正响应回复数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void uds_positive_rsp(uint8_t* data, uint16_t len);


/******************************************************************************
* 函数名称: int uds_timer_chk(uds_timer_t num)
* 功能说明: 检查定时器状态
* 输入参数: uds_timer_t num              --定时器
* 输出参数: 无
* 函数返回: 0: 定时器已停止运行;  1: 定时器正在计时运行
* 其它说明: 无
******************************************************************************/
int uds_timer_chk(uds_timer_t num);


/******************************************************************************
* 函数名称: void service_task(void)
* 功能说明: 应用层任务处理
* 输入参数: 无
* 输出参数: 无
* 函数返回: 无
* 其它说明: 该函数需要被 1ms 周期调用
******************************************************************************/
void service_task(void);


/******************************************************************************
* 函数名称: int service_init(void)
* 功能说明: 初始化
* 输入参数: 无
* 输出参数: 无
* 函数返回: 0: OK; -1: ERR
* 其它说明: 向 TP 层注册一些接口函数
******************************************************************************/
int service_init(void);

#endif
