#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("I'd rather not say");
MODULE_DESCRIPTION("Simple echo character device driver");
MODULE_VERSION("0:0.1");

static int echo_open(struct inode *inode, struct file *filp);
static int echo_release(struct inode *inode, struct file *filp);
ssize_t echo_read(struct file *filep, char __user *buff, size_t count, loff_t *offp);
ssize_t echo_write(struct file *filep, const char __user *buff, size_t count, loff_t *offp);
static int echo_init(void);
static void echo_exit(void);

static dev_t mydev;
static struct cdev *mycdev;
static struct file_operations myfops = {
    .owner = THIS_MODULE,
	.read = echo_read,
	.write = echo_write,
	.open = echo_open,
	.release = echo_release
};

static int echo_open(struct inode *inode, struct file *filp)
{
    // TODO (Exercise 5) Properly link file's private_data

    printk(KERN_INFO "echo_open(): Returning\n");
    return -1;
}

static int echo_release(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "echo_release(): Returning\n");
    return -1;
}

ssize_t echo_read(struct file *filep, char __user *buff, size_t count, loff_t *offp)
{
    printk(KERN_INFO "echo_read(): Returning\n");
    return -1;
}

ssize_t echo_write(struct file *filep, const char __user *buff, size_t count, loff_t *offp)
{
    printk(KERN_INFO "echo_write(): Returning\n");
    return -1;
}

static int echo_init(void)
{
	int result = -1;

	// TODO (Exercise 2) register device driver so that:
	// - Driver name is echo 
	// - Major number is dynamically assigned
	// - Minor number starts from 0
	// - Only one device needs to be managed by the driver

	if (result < 0){
		printk(KERN_ERR "Failed to register echo device driver\n");
		return result;
	}

    // TODO (Exercise 4) Create struture to represent char devices

    // TODO (Exercise 4) Register character device into the kernel

    if (result < 0){
        printk(KERN_ERR "Failed to register character device info\n");
        return result;
    }

	// TODO (Exercise 2) print "Echo device driver registered with major number X"
	// to the kernel logging buffer so that:
	// - X is the obtained major number during registration
	// - Message printed using the informational log evel

	return 0;
}

static void echo_exit(void)
{
    // TODO (Exercise 4) deregister character device

    // TODO (Exercise 2) deregister the device driver's device numbers

	// TODO (Exercise 2) print "Echo device driver deregistered" to the kernel
	// logginf buffer using the information log level
}

module_init(echo_init);
module_exit(echo_exit);
