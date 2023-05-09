#ifndef __PAGE_MANAGER_H__
#define __PAGE_MANAGER_H__

#include <common.h>

typedef struct PageAction
{
	char* name;
	void (*Run)(void* pParams);  // 参数千奇百怪
	struct PageAction* ptNext;
}PageAction, * PPageAction;

void RegisterPage(PPageAction ptPageAction);
void PageSystemRegister(void);
PPageAction Page(char* name);

#endif
