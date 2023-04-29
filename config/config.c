#include <config.h>
#include <string.h>
#include <stdio.h>

static ItemCfg g_tItemCfgs[ITEMCFG_MAX_NUM];
static int g_iItemCfgCount = 0;

/*解析配置文件，构造g_tItemCfgs数组和g_iItemCfgCount。涉及文件操作*/
int ParseConfigFile()
{
	char buf[100];
	char* p = buf;

	/*1. 打开文件*/
	FILE* fp = fopen_any_on_path(CFG_FILE, "r");
	if(!fp)
	{
		printf("Can't open file: %s", CFG_FILE);
		return -1;
	}

	while(1)
	{
		/*2.1 读每一行*/
		buf[99] = '\0';
		
		/*2.2 吃掉每行前的空格和制表符*/
		p = buf;
		while(*p == ' ' || *p == '\t') ++p;

		/*2.3 忽略注释*/
		if(*p == '#') continue;

		/*2.4 处理*/
		g_tItemCfgs[g_iItemCfgCount].command[0] = '\0';  // 每一行里可能没有command，所以要赋初始值
		g_tItemCfgs[g_iItemCfgCount].index = g_iItemCfgCount;
		sscanf(p, "%s %d %s", g_tItemCfgs[g_iItemCfgCount].name, &g_tItemCfgs[g_iItemCfgCount].bCanBeTouched, g_tItemCfgs[g_iItemCfgCount].command);
		++g_iItemCfgCount;
	}
	return 0;
}

/*返回配置项的数量*/
int GetItemCfgCount()
{
	return g_iItemCfgCount;
}

/*根据索引返回某个配置项*/
PItemCfg GetItemCfgByIndex(int index)
{
	if(index < g_iItemCfgCount) return &g_tItemCfgs[index];
	else return NULL;
}

/*根据名称返回某个配置项*/
PItemCfg GetItemCfgByName(char* name)
{
	for(int i = 0; i < g_iItemCfgCount; ++i)
		if(strcmp(name, g_tItemCfgs[i].name) == 0) return &g_tItemCfgs[i];
	return NULL;
}
