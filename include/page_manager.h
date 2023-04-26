#ifndef __PAGE_MANAGER_H__
#define __PAGE_MANAGER_H__

typedef struct PageAction
{
	char* name;
	void (*Run)(void* pParams);  // 参数千奇百怪
	struct PageAction* ptNext;
}PageAction, * PPageAction;

#endif