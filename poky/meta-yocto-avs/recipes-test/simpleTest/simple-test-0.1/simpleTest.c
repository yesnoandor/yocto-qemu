#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/fcntl.h>

int main(int argc,char *argv[])
{
	int fd;
	char buf[]="This is a example for character devices driver by William!";//写入simple_device设备的内容

	char buf_read[4096]; //simple_device设备的内容读入到该buf中

	if((fd=open("/dev/simple_device",O_RDWR))==-1) //打开simple_device设备
	  printf("open simple_device WRONG \n");
	else
	  printf("open simple_device SUCCESS!\n");
	  
	printf("buf: %s\n",buf);

	write(fd,buf,sizeof(buf)); //  把buf中的内容写入simple_device设备

	 
	lseek(fd,0,SEEK_SET); // 把文件指针重新定位到文件开始的位置

	memset(buf_read,0x00,sizeof(buf_read));
	read(fd,buf_read,sizeof(buf)); //把simple_device设备中的内容读入到buf_read中
 
	printf("buf_read: %s\n",buf_read);
	return 0;
}
