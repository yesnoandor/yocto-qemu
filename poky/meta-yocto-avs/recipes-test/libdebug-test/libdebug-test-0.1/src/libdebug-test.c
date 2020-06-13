#include  "debug.h"

int main() 
{
	DbgFuncEntry();

	DbgPrintf("hello, test libdebug.so!\r\n");

	DbgGood("hello, Good!\r\n");
	DbgWarn("hello, Warning!!\r\n");
	DbgError("hello, Error!!!\r\n");
	
	DbgFuncExit();

	return 0; 
}
