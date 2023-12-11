

#ifndef INC_TASKLIST_H_
#define INC_TASKLIST_H_

#include "main.h"
#include <stdlib.h>

#include"TaskAllocate.h"

void TL_init(void);
void TL_insert(void(*pFunc)(), unsigned int DELAY, unsigned int PERIOD);
void TL_insertNode(struct Node* node);
STask* TL_removeID(uint32_t TaskID);
struct Node* TL_removeFront(void);
STask* TL_getFront();
void TL_point_start();
STask* TL_get();
uint32_t TL_size();

#endif /* INC_TASKLIST_H_ */
