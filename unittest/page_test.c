#include <page_manager.h>

int main(int argc, char** argv)
{
	PagesRegister();
	Page("main")->Run(NULL);
	return 0;
}
