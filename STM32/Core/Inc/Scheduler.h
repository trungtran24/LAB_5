

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"
#include <stdlib.h>
#include "TaskList.h"
#include "SCH_Config.h"

typedef enum{
	SCH_OKAY,
	SCH_TOO_MANY_TASK,
	SCH_DELETE_FAIL,
	SCH_ADD_FAIL,
	SCH_INIT_FAIL
} SCH_ERROR;

void SCH_Init(TIM_HandleTypeDef *htim);
uint32_t SCH_Add_Task(void(*pFunc)(), unsigned int DELAY, unsigned int PERIOD);
void SCH_Delete_Task(uint32_t TaskID);
void SCH_Dispatch_Tasks(void);
void SCH_Update(void);
void SCH_Go_To_Sleep(void);
void SCH_print();
void SCH_Report_Status();

#endif /* INC_SCHEDULER_H_ */
