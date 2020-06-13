/******************************************************************************
 *
 *   Copyright (C) 2011  Intel Corporation. All rights reserved.
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; version 2 of the License.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *****************************************************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <asm/io.h>
//#include <asm/system.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define simple_MAJOR 200  

static unsigned char simple_inc=0;
static char demoBuffer[256];

int simple_open(struct inode *inode,struct file *filp)
{
	if(simple_inc>0) return -ERESTARTSYS;

	simple_inc++;
	memset(demoBuffer,0,256);

	return 0;
}

int simple_release(struct inode *inode,struct file *filp)
{
	if(simple_inc<=0){
		printk("close error");
		return -EFAULT;
	}

	simple_inc--;

	return 0;
}

ssize_t simple_read(struct file *filp,char __user *buf,size_t count,loff_t *f_pos)
{
	printk("%s::++++++++++\r\n", __FUNCTION__);

	if(copy_to_user(buf,demoBuffer,count)){
		count=-EFAULT;
	}

	printk("%s::----------\r\n", __FUNCTION__);

	return count;
}

ssize_t simple_write(struct file *file,const char __user *buf,size_t count,loff_t *f_pos)
{
	printk("%s::++++++++++\r\n", __FUNCTION__);

	if (copy_from_user(demoBuffer + *f_pos,buf,count)){
		count = -EFAULT;
	}

	printk("%s::----------\r\n", __FUNCTION__);

	return count;
}

static const struct file_operations simple_fops={
	.owner=THIS_MODULE,
	.open=simple_open,
	.release=simple_release,
	.read=simple_read,
	.write=simple_write,
};

int simple_init_module(void)
{
	int ret;
	struct class *simple_class;

	memset(demoBuffer,0,256);
	ret=register_chrdev(simple_MAJOR,"simple",&simple_fops);
	if(ret<0){
		printk("Unable to register character device %d!/n",simple_MAJOR);
		return ret;
	}

	simple_class = class_create(THIS_MODULE, "simple_driver");
	device_create(simple_class, NULL, MKDEV(simple_MAJOR,0), NULL, "simple_device");

	printk("simple_init_module OK!\n");

	return 0;
}

void simple_cleanup_module(void)
{
	unregister_chrdev(simple_MAJOR,"simple");
	printk("simple_cleanup_module!\n");
}
module_init(simple_init_module);
module_exit(simple_cleanup_module);

MODULE_LICENSE("GPL");
