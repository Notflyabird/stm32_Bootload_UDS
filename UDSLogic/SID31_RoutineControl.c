#include "SID31_RoutineControl.h"
#include "service_cfg.h"
#include "uds_service.h"
#include "flash_erase.h"
#include "SID34_36_37_UpDownloadData.h"
/*
 * 31 01 ff 00 擦除内存
 * 31 01 02 02 检查传输数据完整性
 * 31 01 02 03 更新条件查询
 * 31 01 ff 01 软硬件一致性检查
 * 31 01 df e0 从节点模式选择
 * 31 01 02 04 数字签名
*/ 


typedef enum __UDS_ROUTINE_CTRL_TYPE__
{
	UDS_ROUTINE_CTRL_NONE = 0,
	UDS_ROUTINE_CTRL_START = 0x01,
	UDS_ROUTINE_CTRL_STOP = 0x02,
	UDS_ROUTINE_CTRL_REQUEST_RESULT = 0x03
}uds_routine_ctrl_type;


/******************************************************************************
* 函数名称: bool_t service_31_check_len(const uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 检查 31 服务数据长度是否合法
* 输入参数: uint16_t msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: TRUE: 合法; FALSE: 非法
* 其它说明: 无
******************************************************************************/
bool_t service_31_check_len(const uint8_t* msg_buf, uint16_t msg_dlc)
{
	bool_t ret = FALSE;
	
	(void)msg_buf;
	if(msg_dlc > 4)
		ret = TRUE;

	return ret;
}


/******************************************************************************
* 函数名称: void service_31_RoutineControl(const uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 31 服务 - 例程控制
* 输入参数: uint8_t*    msg_buf         --数据首地址
    　　　　uint8_t     msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void service_31_RoutineControl(const uint8_t* msg_buf, uint16_t msg_dlc)
{
    uint8_t subfunction = UDS_GET_SUB_FUNCTION(msg_buf[1]);
    uint8_t rsp_buf[8];
    uint16_t rid = ((uint16_t)msg_buf[2] << 8) | msg_buf[3];
    bool_t find_rid = FALSE;

    rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_31);
    rsp_buf[1] = msg_buf[1];
    rsp_buf[2] = msg_buf[2];
    rsp_buf[3] = msg_buf[3];

    switch (subfunction)
    {
        case UDS_ROUTINE_CTRL_START:
        {
            if (rid == 0xFF00u)
            {
                if (msg_dlc < 12u)
                {
                    uds_negative_rsp(SID_31, NRC_INVALID_MESSAGE_LENGTH_OR_FORMAT);
                    break;
                }

                uint32_t startAddr = ((uint32_t)msg_buf[4] << 24) |
                                     ((uint32_t)msg_buf[5] << 16) |
                                     ((uint32_t)msg_buf[6] << 8)  |
                                     ((uint32_t)msg_buf[7] << 0);

                uint32_t length = ((uint32_t)msg_buf[8] << 24) |
                                  ((uint32_t)msg_buf[9] << 16) |
                                  ((uint32_t)msg_buf[10] << 8) |
                                  ((uint32_t)msg_buf[11] << 0);

                if (length == 0u)
                {
                    uds_negative_rsp(SID_31, NRC_REQUEST_OUT_OF_RANGE);
                    break;
                }

                if (flash_erase_region(startAddr, length) == HAL_OK)
                {
                    uds_positive_rsp(rsp_buf, 4);
                }
                else
                {
                    uds_negative_rsp(SID_31, NRC_GENERAL_PROGRAMMING_FAILURE);
                }
            }
			if (rid == 0x0202u)
			{
                if (msg_dlc < 2u)
                {
                    uds_negative_rsp(SID_31, NRC_INVALID_MESSAGE_LENGTH_OR_FORMAT);
                    break;
                }
				if(CRCValue == ((uint16_t)msg_buf[4] << 8)+((uint16_t)msg_buf[5] << 0))
				{
					rsp_buf[4]=0x10;
					uds_positive_rsp(rsp_buf, 5);
				}
				else
				{
					rsp_buf[4]=0x01; //crc check failed
					uds_positive_rsp(rsp_buf, 5);
				}
			}
            else if (find_rid == TRUE)
            {
                if (1)
                {
                    uds_negative_rsp(SID_31, NRC_REQUEST_SEQUENCE_ERROR);
                }
                else
                {
                    uds_positive_rsp(rsp_buf, 4);
                }
            }
            else
            {
                uds_negative_rsp(SID_31, NRC_REQUEST_OUT_OF_RANGE);
            }
            break;
        }
        case UDS_ROUTINE_CTRL_STOP:
            if (find_rid == TRUE)
            {
                if (1)
                {
                    uds_negative_rsp(SID_31, NRC_REQUEST_SEQUENCE_ERROR);
                }
                else
                {
                    uds_positive_rsp(rsp_buf, 4);
                }
            }
            else
            {
                uds_negative_rsp(SID_31, NRC_REQUEST_OUT_OF_RANGE);
            }
            break;
        case UDS_ROUTINE_CTRL_REQUEST_RESULT:
            if (find_rid == TRUE)
            {
                if (1)
                {
                    uds_negative_rsp(SID_31, NRC_REQUEST_SEQUENCE_ERROR);
                }
                else
                {
                }
            }
            else
            {
                uds_negative_rsp(SID_31, NRC_REQUEST_OUT_OF_RANGE);
            }
            break;
        default:
            uds_negative_rsp(SID_31, NRC_SUBFUNCTION_NOT_SUPPORTED);
            break;
    }
}


/****************EOF****************/
