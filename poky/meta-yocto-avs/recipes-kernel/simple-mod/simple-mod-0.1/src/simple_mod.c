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

// ------------------- Include Files List --------------------
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

#include <linux/debugfs.h>

#include <linux/slab.h>
#include <linux/delay.h>

#include <linux/semaphore.h>
#include <linux/err.h>

#include <linux/platform_device.h>

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/percpu.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/sched/signal.h>


// ----------------- Macro Define -------- ----------------
#define	SIMPLE_MODULE_DRV_NAME				"simple-module"
#define	SIMPLE_MODULE_CLASS_NODE_NAME	"simple_module_class"
#define	SIMPLE_MODULE_DEVICE_NODE_NAME	"simple_module_device"

// ------------------ Typedef Declaration ------------------
struct simple_module_dev {
	int inc;
	int val;
	struct semaphore sem;
	struct cdev dev;

	int major;
	int minor;

	struct class *dev_class;
	struct device *dev_device;

	struct dentry* debug_root; 
	bool debug_en;
	u32 app_ready;

	struct proc_dir_entry * proc_entry;

	char * buf;
};

struct simple_module_platform_data {

};

// ------------------ Functions Declaration -----------------
static ssize_t simple_module_val_show(struct device *dev,
                                     			struct device_attribute *attr, 
                                     			char *buf);
static ssize_t simple_module_val_store(struct device *dev,
								struct device_attribute *attr, 
								const char *buf, 
								size_t size);

static int simple_module_probe(struct platform_device *pdev);
static int simple_module_remove(struct platform_device *pdev);

static int simple_module_open(struct inode *inode,struct file *filp);
static int simple_module_release(struct inode *inode,struct file *filp);
static ssize_t simple_module_read(struct file *filp,char __user *buf,size_t count,loff_t *f_pos);
static ssize_t simple_module_write(struct file *filp,const char __user *buf,size_t count,loff_t *f_pos);

static void *simple_module_l_start(struct seq_file *m, loff_t * pos);
static void *simple_module_l_next(struct seq_file *m, void *p, loff_t * pos);
static void simple_module_l_stop(struct seq_file *m, void *p);
static int simple_module_l_show(struct seq_file *m, void *p);
static int simple_module_process_statistics(struct seq_file *file,void *v);
static int simple_module_seq_open(struct inode *inode, struct file *file);

// ------------------ Static Variable -----------------------
static char demo_buffer[256];

static struct device_attribute simple_on_off_attr = {
	.attr = {
		.name = "val",
		.mode = S_IRUSR | S_IWUSR,
	},
	.show = simple_module_val_show,
	.store = simple_module_val_store,
};

static const struct file_operations simple_module_fops={
	.owner=THIS_MODULE,
	.open=simple_module_open,
	.release=simple_module_release,
	.read=simple_module_read,
	.write=simple_module_write,
};


static struct simple_module_platform_data simple_module_platform_data = {
	
};

static struct platform_device simple_module_platform_device = {
	.name =  SIMPLE_MODULE_DRV_NAME,
	.dev    = {
		.platform_data = &simple_module_platform_data,
	}
};


static struct platform_driver simple_module_platform_driver = {
	.driver	= {
		.name	= SIMPLE_MODULE_DRV_NAME,
		.owner	= THIS_MODULE,
	},
	.probe		= simple_module_probe,
	.remove		= simple_module_remove,
};

static struct seq_operations simple_module_seq_fops = {
	.start = simple_module_l_start,
	.next  = simple_module_l_next,
	.stop  = simple_module_l_stop,
	.show  = simple_module_l_show
};

static const struct file_operations simple_module_proc_fops = {
	.owner = THIS_MODULE,
	.open = simple_module_seq_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};


// ----------------- Function List  -------- ----------------
static ssize_t simple_module_val_show(struct device *dev,
                                     			struct device_attribute *attr, 
                                     			char *buf)
{
	ssize_t ret;
	int val;
	struct simple_module_dev * dev_simple_module = (struct simple_module_dev *)dev_get_drvdata(dev);

	printk("%s::++++++++++\r\n", __FUNCTION__);

	if(down_interruptible(&(dev_simple_module->sem))) {                
		return -ERESTARTSYS;        
	}        

	val = dev_simple_module->val;
	
	ret = sprintf(buf,"%d\n", val);

	up(&(dev_simple_module->sem));        

	printk("%s::----------\r\n", __FUNCTION__);

	return ret;
}
 
 
static ssize_t simple_module_val_store(struct device *dev,
								struct device_attribute *attr, 
								const char *buf, 
								size_t size)
{
	ssize_t ret = size;
	int val = 0;
	struct simple_module_dev * dev_simple_module = (struct simple_module_dev *)dev_get_drvdata(dev);
	
	printk("%s::++++++++++\r\n", __FUNCTION__);

	if(down_interruptible(&(dev_simple_module->sem))) {                
		return -ERESTARTSYS;        
	}     
	
	val = simple_strtoul(buf, NULL,0);
	
	printk("val = 0x%x\r\n", val);
	dev_simple_module->val = val;

	up(&(dev_simple_module->sem));        
	
	printk("%s::----------\r\n", __FUNCTION__);
	
	return ret;
}
 

static int simple_module_open(struct inode *inode,struct file *filp)
{
	struct simple_module_dev * dev_simple_module;        

	printk("%s::++++++++++\r\n", __FUNCTION__);
	  
	// 将自定义设备结构体保存在文件指针的私有数据域中，以便访问设备时拿来用*/
	dev_simple_module = container_of(inode->i_cdev, struct simple_module_dev, dev);
	filp->private_data = dev_simple_module;

	if(dev_simple_module->inc > 0) return -ERESTARTSYS;

	dev_simple_module->inc++;

	printk("%s::----------\r\n", __FUNCTION__);

	return 0;
}

static int simple_module_release(struct inode *inode,struct file *filp)
{
	struct simple_module_dev* dev_simple_module = filp->private_data; 

	printk("%s::++++++++++\r\n", __FUNCTION__);

	if(dev_simple_module->inc <= 0){
		printk("close error");
		return -EFAULT;
	}

	dev_simple_module->inc--;

	printk("%s::----------\r\n", __FUNCTION__);

	return 0;
}

static ssize_t simple_module_read(struct file *filp,char __user *buf,size_t count,loff_t *f_pos)
{
	struct simple_module_dev* dev_simple_module = filp->private_data; 	   

	printk("%s::++++++++++\r\n", __FUNCTION__);
	printk("count = %ld\r\n", count);
	printk("pos = %lld\r\n", *f_pos);

	if(down_interruptible(&(dev_simple_module->sem))) {
		return -ERESTARTSYS;
	}

	if(count + *f_pos > sizeof(dev_simple_module->buf)) {
		return -ENOBUFS;
	}

	if(copy_to_user(buf,dev_simple_module->buf + *f_pos,count)){
		return -EFAULT;
	}

	up(&(dev_simple_module->sem));

	printk("%s::----------\r\n", __FUNCTION__);
	
	return count;
}

static ssize_t simple_module_write(struct file *filp,const char __user *buf,size_t count,loff_t *f_pos)
{
	struct simple_module_dev* dev_simple_module = filp->private_data; 	  
	
	printk("%s::++++++++++\r\n", __FUNCTION__);

	printk("count = %ld \r\n", count);
	printk("pos = %lld \r\n", *f_pos);

	if(down_interruptible(&(dev_simple_module->sem))) {
		return -ERESTARTSYS;
	}
	

	if(count + *f_pos > sizeof(dev_simple_module->buf)) {
		return -ENOBUFS;
	}

	if (copy_from_user(dev_simple_module->buf + *f_pos,buf,count)){
		count = -EFAULT;
	}

	up(&(dev_simple_module->sem));

	printk("%s::----------\r\n", __FUNCTION__);

	return count;
}


static void * simple_module_l_start(struct seq_file *m, loff_t * pos)
{
	loff_t index = *pos;

	printk(KERN_DEBUG "%s::+++++++++++++++\r\n",__FUNCTION__);

	if (index == 0) {
		seq_printf(m, "Current all the processes in system:\n"
                           "%-24s%-5s%-10s%-15s\n", "name", "pid", "state", "par-name");
		return &init_task;
	}
	else {
		return NULL;
	}

	printk(KERN_DEBUG "%s::---------------\r\n",__FUNCTION__);
}

static void *simple_module_l_next(struct seq_file *m, void *p, loff_t * pos)
{
	struct task_struct * task = (struct task_struct *)p;

	printk(KERN_DEBUG "%s::+++++++++++++++\r\n",__FUNCTION__);

	task = next_task(task);

	if ((*pos != 0) && (task == &init_task)) {
		return NULL;
	}
	
	++*pos;

	printk(KERN_DEBUG "%s::---------------\r\n",__FUNCTION__);
	
	return task;
}

static void simple_module_l_stop(struct seq_file *m, void *p)
{
	
	printk(KERN_DEBUG "%s::+++++++++++++++\r\n",__FUNCTION__);
	printk(KERN_DEBUG "%s::---------------\r\n",__FUNCTION__);
}

static int simple_module_l_show(struct seq_file *m, void *p)
{
	struct task_struct * task = (struct task_struct *)p;

	
	printk(KERN_DEBUG "%s::+++++++++++++++\r\n",__FUNCTION__);

	seq_printf(m, "%-24s%-5d%-10d%-15s \n", task->comm, task->pid, task->state, task->real_parent->comm);

	printk(KERN_DEBUG "%s::---------------\r\n",__FUNCTION__);
		
	return 0;
}


static int simple_module_process_statistics(struct seq_file *file,void *v)
{
	int num_running = 0;			//the number of process whose status is running
	int num_interruptible = 0;		//the number of process whose status is interruptible
	int num_uninterruptible = 0;	//the ... status is uninterruptible
	int num_zombie = 0;			//the process exited with status zombie
	int num_stopped = 0;			//the ... status is stopped
	int num_traced = 0;			//the ... status is traced
	int num_dead = 0;				//the process has deaded;
	int num_unknown = 0;			//the process whose status is unknown
	int num_total = 0;				//the total number of process

	int t_exit_state;				//temp var to store task_struct.exit_state
	int t_state;					//temp var to store task_struct.state
	struct task_struct *p;			//pointer to task_struct

	seq_printf(file, "Current all the processes in system:\n"
                           "%-24s%-5s%-10s%-15s\n", "name", "pid", "state", "par-name");
	for(p=&init_task;(p=next_task(p))!=&init_task;)
	{
		
		seq_printf(file, "%-24s%-5d%-10ld%-15s \n", p->comm, p->pid, p->state, p->real_parent->comm);

		num_total++;				//total number of process plus one

		t_state = p->state;			//put p->state to variable t_state
		t_exit_state = p->exit_state;	//similar to above

		if(t_exit_state!=0)			//if the process has exited
		{
			switch(t_exit_state){
				case EXIT_ZOMBIE:	//if the exit state is zombie
					num_zombie++;	//variable plus one
					break;
					
				case EXIT_DEAD:	//if the exit state is dead
					num_dead++;	//variable plus one
					break;

				default: //other case
					break;
			}
		}
		else				// if the proess hasn't exited
		{
			switch(t_state){
				case TASK_RUNNING:			//if the state is running
					num_running++;
					break;
					
				case TASK_INTERRUPTIBLE:	//state is interruptible
					num_interruptible++;			
					break;
					
				case TASK_UNINTERRUPTIBLE:	//state is uninterruptible
					num_uninterruptible++;		
					break;

				case TASK_STOPPED:			//state is stopped
					num_stopped++;
					break;
					
				case TASK_TRACED:				//state is traced
					num_traced++;
					break;
					
				default:
					num_unknown++;
					break;
			}
		}
	}

	//below instruction is to print the statistics result in above code
	seq_printf(file,"--------------------\n",num_total);
	seq_printf(file,"%-40s %10d\n", "total tasks: ", num_total);
	seq_printf(file,"%-40s %10d\n", "TASK_RUNNING: ", num_running);
	seq_printf(file,"%-40s %10d\n", "TASK_INTERRUPTIBLE: ", num_interruptible);
	seq_printf(file,"%-40s %10d\n", "TASK_UNINTERRUPTIBLE: ", num_uninterruptible);
	seq_printf(file,"%-40s %10d\n", "TASK_TRACED: ", num_stopped);
	seq_printf(file,"%-40s %10d\n", "EXIT_ZOMBIE: ", num_zombie);
	seq_printf(file,"%-40s %10d\n", "EXIT_DEAD: ", num_dead);
	seq_printf(file,"%-40s %10d\n", "UNKNOWN: ", num_unknown);

	return 0;
} 

static int simple_module_seq_open(struct inode *inode, struct file *file)
{
	// return seq_open(file, &simple_module_seq_fops);

	return single_open(file,simple_module_process_statistics,NULL);		// bind seq_file with simple_module_show function
}

// --------------------------------
static int simple_module_probe(struct platform_device *pdev)
{
	int ret;
	dev_t devno = 0;
	struct dentry *ent;
	struct simple_module_dev *dev_simple_module;

	printk("%s::++++++++++\r\n", __FUNCTION__);
	
	// 分配simple_module  设备结构体变量
	dev_simple_module = devm_kzalloc(&pdev->dev,sizeof(struct simple_module_dev), GFP_KERNEL);
	if (dev_simple_module == NULL)
	{
		dev_err(&pdev->dev, "failed to alloc platform device memory! \r\n");
		return -ENOMEM;
	}

	memset(dev_simple_module, 0, sizeof(struct simple_module_dev));
	sema_init(&(dev_simple_module->sem),1);
	dev_simple_module->debug_en = 0;
	dev_simple_module->app_ready = 0;
	dev_simple_module->buf = demo_buffer;

	// --------------------
	// 动态分配主设备和从设备号
	ret = alloc_chrdev_region(&devno, 0, 1, SIMPLE_MODULE_DRV_NAME);
	if(ret < 0)
	{
		devm_kfree(&pdev->dev,dev_simple_module);
		dev_err(&pdev->dev, "failed to alloc char dev region! \r\n");
		return ret;
	}
	dev_simple_module->major = MAJOR(devno);
	dev_simple_module->minor = MINOR(devno);

	printk("major = %d \r\n", dev_simple_module->major);
	printk("minor = %d \r\n", dev_simple_module->minor);

	// 初始化一个字符设备(通过初始化cdev 结构体)
	cdev_init(&(dev_simple_module->dev), &simple_module_fops);
	
	// 注册一个字符设备到系统
	ret = cdev_add(&(dev_simple_module->dev),devno, 1);
	if(ret) {
		devm_kfree(&pdev->dev,dev_simple_module);

		dev_err(&pdev->dev, "failed to add char dev region! \r\n");
		return ret;
	}		 

	// --------------------
	dev_simple_module->dev_class = class_create(THIS_MODULE, SIMPLE_MODULE_CLASS_NODE_NAME);
	dev_simple_module->dev_device = device_create(dev_simple_module->dev_class, NULL, MKDEV(dev_simple_module->major,dev_simple_module->minor), NULL, SIMPLE_MODULE_DEVICE_NODE_NAME);

	ret = device_create_file(dev_simple_module->dev_device, &simple_on_off_attr);
	if (ret < 0)
	{
		dev_err(&pdev->dev, "failed to  create device file! \r\n");

		devm_kfree(&pdev->dev,dev_simple_module);
		return ret;
	}

	// --------------------
	// new debugfs/simple_module
	ent = debugfs_create_dir("simple_module", NULL);
	if (IS_ERR(ent))
	{		
		dev_err(&pdev->dev,"Couldn't create debug root dir\n");
		devm_kfree(&pdev->dev,dev_simple_module);

		ret=PTR_ERR(ent);
		return ret;
	}

	dev_simple_module->debug_root = ent;
	
	// new debugfs/simple_module/app_ready
	ent = debugfs_create_u32("app_ready", S_IFREG | S_IRWXUGO,
					  dev_simple_module->debug_root,
					  &dev_simple_module->app_ready);
	if (IS_ERR(ent)){
		devm_kfree(&pdev->dev,dev_simple_module);
		
		dev_err(&pdev->dev,"Couldn't create debug app_ready file.\n");
		ret=PTR_ERR(ent);

		return ret;
	}
	
	// new debugfs/simple_module/en
	ent = debugfs_create_bool("en", S_IFREG | S_IRWXUGO,
					  dev_simple_module->debug_root,
					  &dev_simple_module->debug_en);
	if (IS_ERR(ent)){
		printk(KERN_ERR "Failed to create debugfs en file.\r\n");
		
		dev_err(&pdev->dev,"Couldn't create debug heart_beat file.\n");
		ret=PTR_ERR(ent);

		return ret;
	}

	// --------------------
	// new /proc/simple_module_proc
	dev_simple_module->proc_entry = proc_create("simple_module_proc", 0444, NULL,&simple_module_proc_fops);
	if(dev_simple_module->proc_entry == NULL)
	{
		dev_err(&pdev->dev,"Couldn't create proc fs.\n");
		
		ret = -ENOMEM;
       	return ret;
	}
	
	// --------------------
	// 保存成设备的私有数据
	dev_set_drvdata(dev_simple_module->dev_device, dev_simple_module);
	
	// 保存成平台总线设备的私有数据
	platform_set_drvdata(pdev, dev_simple_module);

	printk("%s::----------\r\n", __FUNCTION__);

	return ret;
}

static int simple_module_remove(struct platform_device *pdev)
{
	struct simple_module_dev *dev_simple_module;

	printk("%s::++++++++++\r\n", __FUNCTION__);
	
	dev_simple_module = (struct simple_module_dev *)platform_get_drvdata(pdev);

	// 移除proc 中分配的dentry
	if (dev_simple_module->proc_entry)
	{
		remove_proc_entry("simple_module_proc", NULL);
	}
	
	// 移除debugfs  中每个分配的dentry
	if (dev_simple_module->debug_root)
	{
		debugfs_remove_recursive(dev_simple_module->debug_root);
	}
	
	// 销毁sysfs  中设备类别和设备
	if(dev_simple_module->dev_class)
	{
		device_destroy(dev_simple_module->dev_class, MKDEV(dev_simple_module->major, dev_simple_module->minor));
		class_destroy(dev_simple_module->dev_class);
	}

	// 删除字符设备和释放设备内存
	cdev_del(&(dev_simple_module->dev));

	// 释放设备号
	unregister_chrdev_region(MKDEV(dev_simple_module->major, dev_simple_module->minor), 1);

	// 释放simple_module  设备结构体变量
	devm_kfree(&pdev->dev,dev_simple_module);

	printk("%s::----------\r\n", __FUNCTION__);

	return 0;
}

static int __init simple_module_init(void)
{
	int ret;

	printk("%s::++++++++++\r\n", __FUNCTION__);

	ret = platform_device_register(&simple_module_platform_device);
	if (ret) {
		printk(KERN_ERR "platform_device_register failed!\r\n");
		return ret;
	}

	ret = platform_driver_register(&simple_module_platform_driver);
	if (ret) {
		printk(KERN_ERR "platform_driver_register failed!\r\n");
		return ret;
	}

	printk("simple_init_module OK!\n");

	printk("%s::----------\r\n", __FUNCTION__);

	return 0;
}

static void __exit simple_module_cleanup(void)
{
	printk("%s::++++++++++\r\n", __FUNCTION__);

	platform_driver_unregister(&simple_module_platform_driver);
	platform_device_unregister(&simple_module_platform_device);
	
	printk("simple_cleanup_module!\n");

	printk("%s::----------\r\n", __FUNCTION__);
}


module_init(simple_module_init);
module_exit(simple_module_cleanup);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module Driver for /dev /sys /proc /debugfs");
MODULE_AUTHOR("David Xu <david.xu3@flex.com");
MODULE_VERSION("v1.0");

