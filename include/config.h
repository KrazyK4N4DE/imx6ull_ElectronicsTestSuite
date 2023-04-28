#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <common.h>

#define ITEMCFG_MAX_NUM 30
#define CFG_FILE "/etc/test_gui/gui.conf"  // 预定义默认配置文件

typedef struct ItemCfg
{
	int index;
	char name[100];  // 按钮名称
	int bCanBeTouched;  // 表示该按钮是否能被按压
	char command[100];  // 按钮测试时可运行的命令
}ItemCfg, * PItemCfg;

int ParseConfigFile(char* FileName);  // 解析配置文件
int GetItemCfgCount();        // 返回配置项的数量
PItemCfg GetItemCfgByIndex(int index);  // 根据索引返回某个配置项
PItemCfg GetItemCfgByName(char* name);  // 根据名称返回某个配置项

#endif
