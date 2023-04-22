#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <input_manager.h>

int main(int argc, char **argv)
{
	InputEvent tEvent;
	int ret;

	InputRegister();
	InputDeviceInit();

	while(1)
	{
		ret = GetInputEvent(&tEvent);
		if(ret)
		{
			printf("GetInputEvent Err!\n");
			return -1;
		}
		else
		{
			if(tEvent.iType == INPUT_TYPE_TOUCH)
			{
				printf("Type       : %d\n", tEvent.iType);
				printf("iX         : %d\n", tEvent.iX);
				printf("iY         : %d\n", tEvent.iY);
				printf("iPressure  : %d\n", tEvent.iPressure);
			}
			else if(tEvent.iType == INPUT_TYPE_NET)
			{
				printf("Type       : %d\n", tEvent.iType);
				printf("Str        : %s\n", tEvent.str);
			}
		}
	}
	
	return 0;	
}

