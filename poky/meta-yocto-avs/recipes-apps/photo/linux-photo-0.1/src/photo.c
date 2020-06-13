/*
* 
* ---------------
* VIDIOC_QUERYCAP		// 获取设备支持的操作 
* VIDIOC_G_FMT			// 获取设置支持的视频格式 
* VIDIOC_S_FMT			// 设置捕获视频的格式 
* VIDIOC_REQBUFS			// 向驱动提出申请内存的请求 
* VIDIOC_QUERYBUF		// 向驱动查询申请到的内存 
* VIDIOC_QBUF			// 将空闲的内存加入可捕获视频的队列 
* VIDIOC_DQBUF			// 将已经捕获好视频的内存拉出已捕获视频的队列 
* VIDIOC_STREAMON		// 打开视频流 
* VIDIOC_STREAMOFF		// 关闭视频流 
* VIDIOC_QUERYCTRL		// 查询驱动是否支持该命令 
* VIDIOC_G_CTRL			// 获取当前命令值 
* VIDIOC_S_CTRL			// 设置新的命令值 
* VIDIOC_G_TUNER			// 获取调谐器信息 
* VIDIOC_S_TUNER			// 设置调谐器信息 
* VIDIOC_G_FREQUENCY		// 获取调谐器频率 
*
*--------------
* struct v4l2_fmtdesc
* {
*	u32 index;				// 要查询的格式序号，应用程序设置
*	enum v4l2_buf_type type;	// 帧类型，应用程序设置
*	u32 flags;					// 是否为压缩格式
*	u8 description[32];			// 格式名称
*	u32 pixelformat;			// 格式
*	u32 reserved[4];			// 保留
* };
*
* struct v4l2_pix_format
* {
*	__u32 width;				// Image width in pixels.
*	__u32 height;				// Image Height in pixels.
*	__u32 pixelformat;			// Image格式，最常见的有：V4L2_PIX_FMT_YYUV
*	enum v4l2_field field;		// 是否逐行扫描，是否隔行扫描. Sam通常采用V4L2_FIELD_NONE，逐行放置数据 (注1)
*	__u32 bytesperline;			// 每行的byte数
*	__u32 sizeimage;			// 总共的byte数，bytesperline * height
*	enum v4l2_colorspace colorspace;  //This information supplements the pixelformat and must be set by the driver
*	__u32 priv;
* };
*
* struct v4l2_captureparm
* {
*	__u32 capability;		//是否可以被timeperframe控制帧数。可以则：V4L2_CAP_TIMEPERFRAME
*	__u32 capturemode;	//是否为高清模式V4L2_MODE_HIGHQUALITY or 0
*	struct v4l2_fract timeperframe;	//帧数
*	__u32 extendedmode;	//定制的。如果不支持，设置为0
*	__u32 readbuffers;
*	__u32 reserved[4];
* };
*
* struct v4l2_fract {
* 	__u32 numerator;		// 分子, 例：1
*	__u32 denominator;		//分母 , 例：30
* };
*
* --------------
* V4L2_CAP_VIDEO_CAPTURE 0x00000001     The device supports the Video    Capture interface.
* V4L2_CAP_VIDEO_OUTPUT   0x00000002     The device supports the Video    Output interface.
* V4L2_CAP_VIDEO_OVERLAY 0x00000004     The device supports the Video    Overlay interface.
* V4L2_CAP_VBI_CAPTURE     0x00000010 The device supports the Raw  VBI Capture interface, providing Teletext and Closed Caption   data.
* V4L2_CAP_VBI_OUTPUT     0x00000020      The device supports the Raw  VBI Output interface.
* V4L2_CAP_SLICED_VBI_CAPTURE  0x00000040 The device supports the Sliced VBI Capture interface.
* V4L2_CAP_SLICED_VBI_OUTPUT   0x00000080 The device supports the Sliced VBI Output interface.
* V4L2_CAP_RDS_CAPTURE    0x00000100          [to be defined]
*
* #define V4L2_CAP_TUNER 0x00010000  
* #define V4L2_CAP_AUDIO 0x00020000  
* #define V4L2_CAP_RADIO 0x00040000  
*
* #define V4L2_CAP_READWRITE 0x01000000  
* #define V4L2_CAP_ASYNCIO 0x02000000  
* #define V4L2_CAP_STREAMING 0x04000000
* 
* --------------
* enum
* { 
*	DT_UNKNOWN = 0,		// 类型未知
*	DT_FIFO = 1,			// 命名管道
* 	DT_CHR = 2,			// 字符设备
*	DT_DIR = 4,			// 目录
*	DT_BLK = 6,			// 块设备
* 	DT_REG = 8,			// 常规文件
*	DT_LNK = 10,			// 符号链接		
*	DT_SOCK = 12,			// 套接字
*	DT_WHT = 14			//
* }; 
*
* 
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>		// open
#include <unistd.h> 	// close
#include <dirent.h>		// opendir

#include <errno.h>

#include <getopt.h>		// getopt_long()
#include <regex.h>


#include <sys/mman.h>

#include <sys/ioctl.h>	// ioctl
#include <linux/videodev2.h>

#define		DEFAULT_VIDEO_DEVICE	"/dev/video1"
#define		DEFAULT_YUV_FILE 		"photo.yuv"
#define		DEFAULT_VIDEO_CARD_NAME	"ICT Camera"

static void usage(FILE * fp, int argc, char ** argv) {
	fprintf(fp, 
			"Usage: %s [options]\r\n"
			"Options:\r\n"
			"-d | --device name   Video device name [/dev/videoX]\r\n"
			"-h | --help          Print this message\r\n"
			"", argv[0]);
}

static const char short_options[] = "d:h";

static const struct option long_options[] = { 
	{ "device", required_argument,NULL, 'd' }, 
	{ "help", no_argument, NULL, 'h' }, 
	{ 0, 0, 0, 0 } };




int reg_match(char *buff, char *pattern, char *outdata)
{
	regex_t reg;
	regmatch_t pm[1];
	int  status = 0;

	// 编译正则表达式
	status = regcomp(&reg, pattern, REG_EXTENDED|REG_NEWLINE);  // 扩展正则表达式和识别换行符
	if (status != 0)
	{
		return -1;
	}
	
	status = regexec(&reg, buff, 1, pm, 0);
	if (status == REG_NOMATCH)
	{
		//printf("no match!\n");
		status = -1;
	}
	else if (status != 0)
	{
		return -2;
	}
	else if (status == 0)
	{
	        int i, j;
	        for (i = pm[0].rm_so, j = 0; i < pm[0].rm_eo; i++, j++) {
	            outdata[j] = buff[i];
	        }
	        outdata[i] = '\0';

		printf("dev name match!\n");
	}

	regfree(&reg);

	return status;
}


static int open_device(const char * dev_name)
{
	int fd = -1;

	printf("%s::+++++++++\r\n",__FUNCTION__);

	fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);

	if (-1 == fd)
	{
		printf("can't open the video device(%s)\r\n",dev_name);
		return fd;
	}

	printf("%s::----------\r\n",__FUNCTION__);
	
	return fd;
}

static bool match_device(int fd)
{
	struct v4l2_capability cap;

	printf("%s::+++++++++\r\n",__FUNCTION__);

	ioctl(fd,VIDIOC_QUERYCAP,&cap);
	
	printf("Driver Name:%s\r\n",cap.driver);
	printf("Card Name:%s\r\n",cap.card);
	printf("Bus Info:%s\r\n",cap.bus_info);
	printf("Version: %u.%u.%u\n",(cap.version >> 16) & 0xFF,(cap.version >> 8) & 0xFF,cap.version & 0xFF);
	printf("capabilities:0x%x\r\n",cap.capabilities);
	printf("device_caps:0x%x\r\n",cap.device_caps);

	if(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)
		printf("v4l2 dev support capture \r\n");

	if(cap.capabilities & V4L2_CAP_VIDEO_OUTPUT)
		printf("v4l2 dev support output \r\n");

	if(cap.capabilities & V4L2_CAP_VIDEO_OVERLAY)
		printf("v4l2 dev support overlay \r\n");

	if(cap.capabilities & V4L2_CAP_STREAMING)
		printf("v4l2 dev support streaming \r\n");

	if(cap.capabilities & V4L2_CAP_READWRITE)
		printf("v4l2 dev support read write \r\n");

	if(strncmp(cap.card,DEFAULT_VIDEO_CARD_NAME,sizeof(DEFAULT_VIDEO_CARD_NAME) - 1) != 0)
	{
		printf("can't match the video device card name(%s Vs %s)\r\n",cap.card,DEFAULT_VIDEO_CARD_NAME);
		return false;
	}
	
	printf("%s::----------\r\n",__FUNCTION__);
	
	return true;
}

bool get_video_device(char * dev_name)
{
	bool ret = false;
	DIR *dir;
	struct dirent *ptr;
	const char * base_path = "/dev";
	int fd = -1;

	printf("%s::+++++++++\r\n",__FUNCTION__);
	
	if ((dir = opendir(base_path)) == NULL)
	{
		printf("Open dir(%s) error...\r\n",base_path);
		exit(1);
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)	///current dir OR parrent dir
			continue;
		else if(ptr->d_type == DT_REG)
		{
			//printf("this is a regular file,d_name:%s/%s\n",base_path,ptr->d_name);
		}
		else if(ptr->d_type == DT_DIR)		// --> dir
		{
			//printf("this is a dir,d_name:%s/%s\n",base_path,ptr->d_name);
		}
		else if(ptr->d_type == DT_LNK)		// --> link file
		{
			//printf("this is a link,d_name:%s/%s\n",base_path,ptr->d_name);
		}
		else if(ptr->d_type == DT_CHR)		// --> file
		{
			//printf("this is a char device,d_name:%s/%s\n",base_path,ptr->d_name);

			char result_name[20] = {0};
			static char video_name[256] = {0};
			if (reg_match(ptr->d_name,"video[0-9]",result_name) == 0)
			{
				printf("this is a video device,d_name:%s/%s\n",base_path,ptr->d_name);
				printf("result_name:%s/%s\n",base_path,result_name);

				sprintf(video_name,"%s/%s",base_path,result_name);

				printf("video_name:%s\r\n",video_name);
				fd = open_device(video_name);
				if (fd != -1)
				{
					if(match_device(fd))
					{
						strcpy(dev_name,video_name);
						ret = true;
						close(fd);
						break;
					}
					close(fd);
				}
			}
		}
	}

	closedir(dir);

	printf("dev_name:%s\r\n",dev_name);

	printf("%s::----------\r\n",__FUNCTION__);
	
	return ret;
}


int main (int argc,char ** argv)
{
	int fd;
	FILE *file_fd;
	struct v4l2_capability cap;
	struct v4l2_fmtdesc fmtdesc;
	struct v4l2_format fmt;
	struct v4l2_requestbuffers req;
	struct v4l2_streamparm stream_parm;
	struct v4l2_control ctrl;

	const char * dev_name = NULL;

	printf("photo::++++++++++\r\n");

	
	for (;;) {
		int index;
		int c;

		c = getopt_long(argc, argv, short_options, long_options, &index);
		
		if (-1 == c)
			break;

		switch (c) {
		case 0: /* getopt_long() flag */
			break;

		case 'd':
			dev_name = optarg;
			break;

		case 'h':
			usage(stdout, argc, argv);
			exit(EXIT_SUCCESS);

		default:
			usage(stderr, argc, argv);
			exit(EXIT_FAILURE);
		}
	}

	if(dev_name == NULL)
	{
		char video_name[256];
		if(get_video_device(video_name))
		{
			dev_name = video_name;
		}
		else
		{
			dev_name = DEFAULT_VIDEO_DEVICE;
		}
	}
	
	
	printf("video dev = %s\r\n",dev_name);
	
	// Open the video device
	fd = open (dev_name, O_RDWR, 0);

	// Query device capabilities
	ioctl(fd,VIDIOC_QUERYCAP,&cap);
				
	printf("Driver Name:%s\r\n",cap.driver);
	printf("Card Name:%s\r\n",cap.card);
	printf("Bus Info:%s\r\n",cap.bus_info);
	printf("Version: %u.%u.%u\n",(cap.version >> 16) & 0xFF,(cap.version >> 8) & 0xFF,cap.version & 0xFF);
	printf("capabilities:0x%x\r\n",cap.capabilities);
	printf("device_caps:0x%x\r\n",cap.device_caps);
	
	if(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)
		printf("v4l2 dev support capture \r\n");

	if(cap.capabilities & V4L2_CAP_VIDEO_OUTPUT)
		printf("v4l2 dev support output \r\n");

	if(cap.capabilities & V4L2_CAP_VIDEO_OVERLAY)
		printf("v4l2 dev support overlay \r\n");

	if(cap.capabilities & V4L2_CAP_STREAMING)
		printf("v4l2 dev support streaming \r\n");

	if(cap.capabilities & V4L2_CAP_READWRITE)
		printf("v4l2 dev support read write \r\n");
  	
	// Enumerate all of image formats (  枚举支持的像素格式)
	fmtdesc.index = 0; 
  	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  	
  	while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc) != -1){
  		printf("fmtdesc.index is %d\n",fmtdesc.index);
  		printf("fmtdesc.description is %s\n",fmtdesc.description);
  		printf("fmtdesc.flags is 0x%x\n",fmtdesc.flags);
  		printf("fmtdesc.pixelformat is %c%c%c%c\n",		// "YUYV"
				(fmtdesc.pixelformat ) & 0xFF,
  				(fmtdesc.pixelformat >> 8) & 0xFF,
  				(fmtdesc.pixelformat >> 16) & 0xFF,
  				(fmtdesc.pixelformat >> 24) & 0xFF
  				);
  		fmtdesc.index ++;
	}

	// Get the data format
	//CLEAR (fmtdesc);
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ioctl(fd,VIDIOC_G_FMT,&fmt);
	
	printf("fmt.fmt.pix.width is %d\r\n",fmt.fmt.pix.width);
	printf("fmt.fmt.pix.height is %d\r\n",fmt.fmt.pix.height);
	printf("fmt.fmt.pix.field is %d\r\n",fmt.fmt.pix.field);
	printf("fmt.fmt.pix.colorspace is %d\r\n",fmt.fmt.pix.colorspace);
	printf("fmt.fmt.pix.bytesperline is %d\r\n",fmt.fmt.pix.bytesperline);
	printf("fmt.fmt.pix.sizeimage is 0x%x\r\n",fmt.fmt.pix.sizeimage);
	printf("fmtdesc.pixelformat is %c%c%c%c\n",		// "YUYV"
				(fmtdesc.pixelformat ) & 0xFF,
  				(fmtdesc.pixelformat >> 8) & 0xFF,
  				(fmtdesc.pixelformat >> 16) & 0xFF,
  				(fmtdesc.pixelformat >> 24) & 0xFF
  				);
	
#if 0
	// Set the data format
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = 1920;
	fmt.fmt.pix.height = 1080;
	ioctl(fd, VIDIOC_S_FMT, &fmt);

	printf("fmt.fmt.pix.width is %d\r\n",fmt.fmt.pix.width);
	printf("fmt.fmt.pix.height is %d\r\n",fmt.fmt.pix.height);
	printf("fmt.fmt.pix.field is %d\r\n",fmt.fmt.pix.field);
	printf("fmt.fmt.pix.colorspace is %d\r\n",fmt.fmt.pix.colorspace);
	printf("fmt.fmt.pix.bytesperline is %d\r\n",fmt.fmt.pix.bytesperline);
	printf("fmt.fmt.pix.sizeimage is 0x%x\r\n",fmt.fmt.pix.sizeimage);
	printf("fmtdesc.pixelformat is %c%c%c%c\n",		// "YUYV"
				(fmtdesc.pixelformat ) & 0xFF,
  				(fmtdesc.pixelformat >> 8) & 0xFF,
  				(fmtdesc.pixelformat >> 16) & 0xFF,
  				(fmtdesc.pixelformat >> 24) & 0xFF
  				);
#endif

	// Get the stream params
	memset(&stream_parm, 0, sizeof(struct v4l2_streamparm));
	stream_parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	ioctl(fd, VIDIOC_G_PARM, &stream_parm);
	printf("stream_parm.capture.capability is 0x%x\r\n",stream_parm.parm.capture.capability);
	printf("stream_parm.capture.capturemode is 0x%x\r\n",stream_parm.parm.capture.capturemode);
	printf("stream_parm.capture.extendedmode is 0x%x\r\n",stream_parm.parm.capture.extendedmode);
	printf("stream_parm.capture.timeperframe.numerator is %d\r\n",stream_parm.parm.capture.timeperframe.numerator);
	printf("stream_parm.capture.timeperframe.denominator is %d\r\n",stream_parm.parm.capture.timeperframe.denominator);

	// Get ctrl setting
	ctrl.id = V4L2_CID_EXPOSURE_AUTO;		// 曝光模式
	ioctl(fd, VIDIOC_G_CTRL, &ctrl);
	printf("v4l2_exposure_auto_type is 0x%x\r\n",ctrl.value);

	ctrl.id = V4L2_CID_EXPOSURE_ABSOLUTE;	// 曝光度
	ioctl(fd, VIDIOC_G_CTRL, &ctrl);
	printf("v4l2_exposure_absolute is 0x%x\r\n",ctrl.value);

	ctrl.id = V4L2_CID_GAIN;				// 增益
	ioctl(fd, VIDIOC_G_CTRL, &ctrl);
	printf("v4l2_gain is 0x%x\r\n",ctrl.value);

	// ----------------------------------------------------------------
	// Initiate Memory Mapping or User Pointer I/O
	req.count = 8;		// 4-frame buffer
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	ioctl(fd,VIDIOC_REQBUFS,&req);
	
	struct buffer{
		void *start;
		unsigned int length;
	}*buffers;
	buffers = (struct buffer*)calloc (req.count, sizeof(*buffers));
	unsigned int n_buffers = 0;
  
	for(n_buffers = 0; n_buffers < req.count; ++n_buffers)
	{	
		// Query the status of a buffer
		struct v4l2_buffer buf;
		
		memset(&buf,0,sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;

		if(ioctl(fd,VIDIOC_QUERYBUF,&buf) == -1)
		{
			printf("VIDIOC_QUERYBUF failed!\r\n");
			close(fd);
			exit(-1);
		}

		printf("buf.length = 0x%x\r\n",buf.length);
		printf("buf.flags = 0x%x\r\n",buf.flags);
		printf("buf.m.offset = 0x%x\r\n",buf.m.offset);

		// mmap video into buffers
		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start = mmap(NULL,buf.length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,buf.m.offset);
		if(MAP_FAILED == buffers[n_buffers].start)
		{
			printf("mmap failed!\r\n");
			close(fd);
			exit(-1);
		}
	}
  
	// Enqueue an empty (capturing) or filled (output) buffer in the driver's incoming queue
	unsigned int i;
	enum v4l2_buf_type type;
	for(i = 0; i < 8; i++){
		struct v4l2_buffer buf;
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		ioctl(fd,VIDIOC_QBUF,&buf);
	}

	// Start streaming I/O
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ioctl(fd,VIDIOC_STREAMON,&type);

	// wait video raw-data ready
	for (;;) 
	{
		fd_set fds;
		struct timeval tv;
		int r;
	
		FD_ZERO (&fds);
		FD_SET (fd, &fds);
	
		/* Timeout. */
		tv.tv_sec = 10;
		tv.tv_usec = 0;

		r = select (fd + 1, &fds, NULL, NULL, &tv);
		if (-1 == r)
		{
			if (EINTR == errno)
				continue;
			printf ("select");
		}
	
		if (0 == r)
		{
			fprintf (stderr, "select timeout\r\n");
			exit(-1);
		}

		struct v4l2_buffer buf;
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		if(ioctl(fd,VIDIOC_DQBUF,&buf) != -1)
		{
			break;
		}
	}

	// Dequeue a filled (capturing) or displayed (output) buffer from the driver's outgoing queue
	struct v4l2_buffer buf;
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	ioctl(fd,VIDIOC_DQBUF,&buf);

	//char path[20];
	//snprintf(path,sizeof(path),"./test.yuv");
	//int fdyuyv = open(path,O_WRONLY|O_CREAT,00700);
	//int resultyuyv = fwrite(fdyuyv,buffers[buf.index].start,1280*720*2);
	//close(fdyuyv);	

	// write raw-data into file
	printf("buf.index = 0x%x\r\n",buf.index);
	printf("buffers[%d].length = 0x%x\r\n",buf.index,buffers[buf.index].length);
	
	file_fd = fopen(DEFAULT_YUV_FILE, "w");
	fwrite(buffers[buf.index].start, buffers[buf.index].length, 1, file_fd);
	fclose (file_fd);
	
	free (buffers);
	
	// Close the video device
	close(fd);

	printf("photo::----------\r\n");
	
	return 0;
}

