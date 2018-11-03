#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/fcntl.h>

int main(int argc,char *argv[])
{
	int fd;
	char buf[]="This is a example for character devices driver by William!";//д��simple_device�豸������

	char buf_read[4096]; //simple_device�豸�����ݶ��뵽��buf��

	if((fd=open("/dev/simple_device",O_RDWR))==-1) //��simple_device�豸
	  printf("open simple_device WRONG \n");
	else
	  printf("open simple_device SUCCESS!\n");
	  
	printf("buf: %s\n",buf);

	write(fd,buf,sizeof(buf)); //  ��buf�е�����д��simple_device�豸

	 
	lseek(fd,0,SEEK_SET); // ���ļ�ָ�����¶�λ���ļ���ʼ��λ��

	memset(buf_read,0x00,sizeof(buf_read));
	read(fd,buf_read,sizeof(buf)); //��simple_device�豸�е����ݶ��뵽buf_read��
 
	printf("buf_read: %s\n",buf_read);
	return 0;
}
