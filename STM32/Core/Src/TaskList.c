/*
 * TaskList.c
 *
 *  Created on: Jan 10, 2023
 *      Author: 84915
 */

#include "TaskList.h"

static uint32_t taskCount = 0;

static struct Node* head = NULL;
static struct Node* curr = NULL;

void TL_init(void) {
	TA_init();
	head = TA_malloc(); // create a virtual node

	taskCount = 0;
	curr = NULL;
}

void TL_insert(void(*pFunc)(), unsigned int DELAY, unsigned int PERIOD) {
	struct Node* preTemp = head;
	struct Node* temp = head->next;

	uint32_t accumDelay = 0;
	while(temp && (accumDelay + (temp->data).Delay) <= DELAY) {
		accumDelay += (temp->data).Delay;

		preTemp = temp;
		temp = temp->next;
	}

	struct Node* newNode = TA_malloc();
	preTemp->next = newNode;
	newNode->next = temp;


	(newNode->data).Period = PERIOD;
	(newNode->data).RunMe = 0;
	(newNode->data).TaskID = (uint32_t)pFunc;
	(newNode->data).pTask = pFunc;
	(newNode->data).Delay = DELAY - accumDelay;

	if(temp) {
		(temp->data).Delay -= (newNode->data).Delay;
	}

	taskCount++;
}

void TL_insertNode(struct Node* node) {
	if(node == NULL) return;

	struct Node* preTemp = head;
	struct Node* temp = head->next;

	uint32_t accumDelay = 0;
	while(temp && (accumDelay + (temp->data).Delay) <= (node->data).Delay) {
		accumDelay += (temp->data).Delay;

		preTemp = temp;
		temp = temp->next;
	}

	preTemp->next = node;
	node->next = temp;

	(node->data).Delay -= accumDelay;
	if(temp) {
		(temp->data).Delay -= (node->data).Delay;
	}

	taskCount++;
}

STask* TL_removeID(uint32_t TaskID) {
	struct Node* preTemp = head;
	struct Node* temp = head->next;

	while(temp && (temp->data).TaskID != TaskID) {
		preTemp = temp;
		temp = temp->next;
	}

	if(temp == NULL) return NULL;

	preTemp->next = temp->next;
	taskCount--;

	TA_free(temp);
	return NULL;
}
struct Node* TL_removeFront(void) { // just remove node from current list in order to add it in again -> no need to free()
	if(taskCount == 0) return NULL;

	struct Node* delNode = head->next;
	head->next = delNode->next;

	taskCount--;
	return delNode;
}

STask* TL_getFront() {
	if(head)
		return &(head->next->data);
	return NULL;
}


void TL_point_start() {
	curr = head->next;
}
STask* TL_get() {
	if(curr) {
		STask* task = &(curr->data);
		curr = curr->next;
		return task;
	}
	return NULL;
}
uint32_t TL_size() {
	return taskCount;
}
