#ifndef _SID34_36_37_UP_DOWN_LOAD_DATA_H_
#define _SID34_36_37_UP_DOWN_LOAD_DATA_H_


#include <stdint.h>
#include "uds_type.h"

bool_t service_34_check_len(const uint8_t* msg_buf, uint16_t msg_dlc);
bool_t service_36_check_len(const uint8_t* msg_buf, uint16_t msg_dlc);
bool_t service_37_check_len(const uint8_t* msg_buf, uint16_t msg_dlc);
void service_34_RequestDownload(const uint8_t* msg_buf, uint16_t msg_dlc);
void service_36_TransferData(const uint8_t* msg_buf, uint16_t msg_dlc);
void service_37_RequestTransferExit(const uint8_t* msg_buf, uint16_t msg_dlc);
#endif

/****************EOF****************/