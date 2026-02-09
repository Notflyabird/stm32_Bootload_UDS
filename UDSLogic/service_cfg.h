#ifndef _SERVICE_CFG_H_
#define _SERVICE_CFG_H_

#include <stdint.h>
#define SID_NUM       16 

#define SID_10        (0x10) /* SessionControl */
#define SID_11        (0x11) /* ECUReset */
#define SID_14        (0x14) /* ClearDTC */
#define SID_18        (0x18) /* KWPReadDTC */
#define SID_19        (0x19) /* ReadDTC */
#define SID_22        (0x22) /* ReadID */
#define SID_27        (0x27) /* SecurityAccess */
#define SID_2E        (0x2E) /* WriteID */
#define SID_2F        (0x2F) /* InputOutputControlID */
#define SID_28        (0x28) /* CommunicationControl */
#define SID_31        (0x31) /* RoutineControl */
#define SID_3E        (0x3E) /* TesterPresent */
#define SID_85        (0x85) /* ControlDTCSetting */
#define SID_34        (0x34) /*  */
#define SID_36        (0x36) /*  */
#define SID_37        (0x37) /*  */

#define DTC_NUM       2


typedef struct DTCSnapshot_struct
{
    uint8_t Odometer;  //总里程
    uint8_t voltage;    //电源电压
    uint8_t speed;      //车速
}DTCSnapshotFormat_t;

typedef struct DTC_struct
{
    uint32_t  DTCCode;
    uint8_t   DTCStatus;
    DTCSnapshotFormat_t *Snapshot_p;  //快照存储
    uint8_t SnapshotCunt; //快照记录数量
}DTC_Format_t;

extern DTC_Format_t DTC_Table[DTC_NUM];
extern DTCSnapshotFormat_t DTCSnapshot_record_0x960100[3];
extern DTCSnapshotFormat_t DTCSnapshot_record_0x960200[3];

#endif
/****************EOF****************/
