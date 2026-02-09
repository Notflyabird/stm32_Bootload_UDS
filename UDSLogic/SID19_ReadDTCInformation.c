#include "SID19_ReadDTCInformation.h"
#include "service_cfg.h"
#include "uds_service.h"


#define REPORT_DTC_NUMBER_BY_STATUS_MASK  (0x01)
#define REPORT_DTC_BY_STATUS_MASK         (0x02)
#define REPORT_DTC_SNAPSHOT_IDENTIFICATION (0x03)
#define REPORT_DTC_SNOPSHOT_BY_DTC_NUMBER (0x04)
#define REPORT_DTC_EXTENDED_DATA_RECORD_BY_DTC_NUMBER (0x06)
#define REPORT_SUPPORTED_DTC              (0x0a)

DTC_Format_t * checkDtcCodeAvailable(const uint32_t dtccode)
{
	for(uint8_t i=0;i<DTC_NUM;i++)
	{
		if(DTC_Table[i].DTCCode == dtccode)
		{
			return  &DTC_Table[i];
		}

	}
	return NULL;
}

/******************************************************************************
* 函数名称: bool_t service_19_check_len(const uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 检查 19 服务数据长度是否合法
* 输入参数: uint16_t msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: TRUE: 合法; FALSE: 非法
* 其它说明: 无
******************************************************************************/
bool_t service_19_check_len(const uint8_t* msg_buf, uint16_t msg_dlc)
{
	bool_t ret = FALSE;
	

	return ret;
}


/******************************************************************************
* 函数名称: void service_19_ReadDTCInformation(const uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 19 服务 - 读取故障码信息
* 输入参数: uint8_t*    msg_buf         --数据首地址
    　　　　uint8_t     msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void service_19_ReadDTCInformation(const uint8_t* msg_buf, uint16_t msg_dlc)
{
	uint8_t subfunction;
    uint8_t rsp_buf[64];
	uint16_t dtc_count =0;

	subfunction = UDS_GET_SUB_FUNCTION (msg_buf[1]);

    switch (subfunction)
	{
		case REPORT_DTC_NUMBER_BY_STATUS_MASK:
		{
			for(uint8_t i=0;i<DTC_NUM;i++)
			{
				if(DTC_Table[i].DTCStatus == msg_buf[2])
				{
					dtc_count ++;
				}

			}
			rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_19);
			rsp_buf[1] = subfunction;
			rsp_buf[2] = 0x7F;  //支持的状态位置1
			rsp_buf[3] = 1; // 14229
			rsp_buf[4] = (uint8_t)dtc_count;
			rsp_buf[5] = (dtc_count >> 8) & 0xFF;
			uds_positive_rsp (rsp_buf, 6);
		    break;
		}
		case REPORT_DTC_BY_STATUS_MASK:
		{

			rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_19);
			rsp_buf[1] = subfunction;
			rsp_buf[2] = 0x7F;  //支持的状态位置1
			for(uint8_t i=0;i<DTC_NUM;i++)
			{
				if(DTC_Table[i].DTCStatus == msg_buf[2])
				{
					rsp_buf[3+dtc_count*4] = DTC_Table[i].DTCCode >>16;
					rsp_buf[4+dtc_count*4] = DTC_Table[i].DTCCode >>8;
					rsp_buf[5+dtc_count*4] = DTC_Table[i].DTCCode ;
					rsp_buf[6+dtc_count*4] = DTC_Table[i].DTCStatus;
					dtc_count ++;
				}

			}
			uds_positive_rsp (rsp_buf, 3+dtc_count*4);
			break;
		}
		case REPORT_DTC_SNAPSHOT_IDENTIFICATION:
		{
			uint32_t DTCMaskRecord =0;
			uint32_t DTCSnapshotRecordNumber =0;
			DTC_Format_t * Curr_Dtc;
			uint8_t length =0;
			rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_19);
			rsp_buf[1] = subfunction;
			length =2;
			for(uint8_t i=0; i<DTC_NUM; i++)
			{
				for(uint8_t j=0;j<DTC_Table[i].SnapshotCunt;j++)
				{
					rsp_buf[length++] = DTC_Table[i].DTCCode >>16;
					rsp_buf[length++] = DTC_Table[i].DTCCode >>8;
					rsp_buf[length++] = DTC_Table[i].DTCCode;
					rsp_buf[length++] = j +1;  //DTCSnapshotRecordNumbe
				}
			}
			uds_positive_rsp (rsp_buf, length);
			break;
		}
		case REPORT_DTC_SNOPSHOT_BY_DTC_NUMBER:
		{
			uint32_t DTCMaskRecord =0;
			uint32_t DTCSnapshotRecordNumber =0;
			DTC_Format_t * Curr_Dtc;
			rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_19);
			rsp_buf[1] = subfunction;
			rsp_buf[2] = msg_buf[2];  
			rsp_buf[3] = msg_buf[3];
			rsp_buf[4] = msg_buf[4];


			DTCMaskRecord |= (uint32_t)msg_buf[2] <<16;
			DTCMaskRecord |= (uint32_t)msg_buf[3] <<8;
			DTCMaskRecord |= (uint32_t)msg_buf[4] ;
			DTCSnapshotRecordNumber = msg_buf[5] ;
			
			Curr_Dtc = checkDtcCodeAvailable(DTCMaskRecord);
			if((Curr_Dtc == NULL)  ||( Curr_Dtc->SnapshotCunt <DTCSnapshotRecordNumber))
			{
				uds_negative_rsp(SID_19, NRC_REQUEST_OUT_OF_RANGE);
				break;
			}
		 	rsp_buf[5] = Curr_Dtc->DTCStatus;
			rsp_buf[6] = DTCSnapshotRecordNumber; //快照编号
			rsp_buf[7] = 1;//DID数量
			rsp_buf[8] = 0x11; //DID MSB
			rsp_buf[9] = 0x11; //DID LSB
			rsp_buf[10] = (Curr_Dtc->Snapshot_p+DTCSnapshotRecordNumber-1)->Odometer;
			rsp_buf[11] = (Curr_Dtc->Snapshot_p+DTCSnapshotRecordNumber-1)->speed;
			rsp_buf[12] = (Curr_Dtc->Snapshot_p+DTCSnapshotRecordNumber-1)->voltage;


			uds_positive_rsp (rsp_buf, 13);
			break;
		}
		default:
	        uds_negative_rsp(SID_19, NRC_SUBFUNCTION_NOT_SUPPORTED);
		    break;
	}
}


/****************EOF****************/
