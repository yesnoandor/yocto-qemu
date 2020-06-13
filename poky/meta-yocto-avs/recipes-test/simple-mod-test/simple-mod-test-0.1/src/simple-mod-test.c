#include <fcntl.h>
#include <stdio.h>


int main() 
{
	int fd;
	int val = 0x55aaff00;
	fd = open("/dev/simple_device", O_RDWR);
	if(fd < 0){
		printf("can't open!\n");
	}
	write(fd, &val, 4);

	val = 0;
	read(fd,&val,4);
	printf("val = 0x%x\r\n",val);

	return 0; 
}
