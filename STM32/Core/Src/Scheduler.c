/*
 * Scheduler.c
 *
 *  Created on: Jan 10, 2023
 *      Author: 84915
 */
#include "Scheduler.h"
#include "uart_printf.h"
//static const uint32_t SYS_CLK_FREQ = 16000;

static uint32_t INTERRUPT_PERIOD = 0;
static SCH_ERROR Error_code = SCH_OKAY;

static uint32_t report_period = SCH_REPORT_FREQ;
static uint32_t print_task_period = SCH_PRINT_TASK_FREQ;

void SCH_Init(TIM_HandleTypeDef *htim) {
	if(htim == NULL) {
		Error_code = SCH_INIT_FAIL;
		return;
	}

	INTERRUPT_PERIOD = (htim->Instance->PSC+1)*(htim->Instance->ARR+1) / SCH_SYSCLK_FREQ;
	TL_init();
}

uint32_t SCH_Add_Task(void(*pFunc)(), unsigned int DELAY, unsigned int PERIOD) {
	if(TL_size() == SCH_MAX_TASKS) {
		Error_code = SCH_TOO_MANY_TASK;
		return 0;
	}
	if(pFunc == NULL) {
		Error_code = SCH_ADD_FAIL;
		return 0;
	}

	TL_insert(pFunc, DELAY, PERIOD);
	return (uint32_t)pFunc;
}

void SCH_Delete_Task(uint32_t TaskID) {
	STask* ret = TL_removeID(TaskID);
	if(ret == NULL) {
		Error_code = SCH_DELETE_FAIL;
	}
}

void SCH_Dispatch_Tasks(void) {
	STask* task =  TL_getFront();
	if(task && task->RunMe) {
		(*(task->pTask))();
		(task->RunMe)--;
		if(task->RunMe == 0) {
			struct Node* delNode = TL_removeFront();
			if((delNode->data).Period) {
				(delNode->data).Delay = (delNode->data).Period;
				TL_insertNode(delNode);
			}
			SCH_print();
		}
	}

	//SCH_Go_To_Sleep(); // break program when uart transmits
}

void SCH_Update(void) {

	STask* task = TL_getFront();
	if(task) {
		if(task->Delay == 0) {
			task->RunMe++;
		}
		else {
			task->Delay -= INTERRUPT_PERIOD;
		}
	}

	SCH_Report_Status();
	SCH_print();
}

void SCH_Report_Status() {
	report_period--;
	if(report_period > 0) return;

	switch(Error_code) {
	case SCH_OKAY:
		printf("Scheduler status: OKAY\r\n\r\n");
		break;
	case SCH_TOO_MANY_TASK:
		printf("Scheduler status: Too many tasks\r\n\r\n");
		break;
	case SCH_DELETE_FAIL:
		printf("Scheduler status: fail to delete task\r\n\r\n");
		break;
	case SCH_ADD_FAIL:
		printf("Scheduler status: fail to add task\r\n\r\n");
		break;
	case SCH_INIT_FAIL:
		printf("Scheduler status: fail to start scheduler\r\n\r\n");
		break;
	default:
		break;
	}

	report_period = SCH_REPORT_FREQ;
}

void SCH_Go_To_Sleep(void) {
	HAL_SuspendTick();
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void SCH_print() {
	print_task_period--;
	if(print_task_period > 0) return;

	TL_point_start();
	STask* task = TL_get();
	uint32_t accumDelay = 0;
	while(task) {
		accumDelay += task->Delay;
		printf("Task id %lu delay remain %lu ms\r\n", task->TaskID, accumDelay);
		task = TL_get();
	}
	printf("\r\n");

	print_task_period = SCH_PRINT_TASK_FREQ;
}
