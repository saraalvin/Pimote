#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <asm/hardware.h>  
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <asm/io.h>
 
 

 
int my_init(void){
   printk("<1>Good morning Vietnam!\n");
   return 0;
}
void my_end(void) {
   printk("<1>thats all folks\n");
}

module_init(my_init);
module_exit(my_end);
 


int reg_open(struct inode *inode, struct file *filp);

int reg_release(struct inode *inode, struct file *filep);

ssize_t reg_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);

ssize_t reg_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos);
 

/*Set functions as file interface (for OS)*/
struct file_operations reg_fops = {
	read: reg_read,
	write: reg_write,
	open: reg_open,
	release: reg_release
};

/* Global variables */
int reg_major = 60;
 

int init_module(void){
   int result;
   //Register type of device: character (or block, net)
   result = register_chrdev(reg_major, "register", &reg_fops);
   if (result < 0){
      printk("<1>Fail major number %d\n", reg_major);
      return result;
   }
   //Lock access to REG_ADDR (just 1 byte)
   //Ex: #define REG_ADDR GPIO_PEDDR
   check_region(REG_ADDR, 1); //REG_ADDR must be defined
}
 

void cleanup_module(void) {
	//free major number	unregister_chrdev(reg_major, "register");

	//Unlock access to register
	release_region(REG_ADDR, 1);
	printk("<1>Bye bye!");
}
 


/* Open device (as file) */
int reg_open(struct inode *inode, struct file *filp) {
	MOD_INC_USE_COUNT;
	printk("<1>Opening_\n");
	return 0;
}
/* Close device (as file) */
int reg_release(struct inode *inode, struct file *filep) {
	MOD_DEC_USE_COUNT;
	printk("<1>Closing\n");
	return 0;
}
 

/* Read from device (read from register) */
ssize_t reg_read(struct file *filep, char *buf,
                 size_t count, loff_t *f_pos);
	*buf = inb(REG_ADDR);	//1-byte register
	return 1;
}
/* Write to device (write to register) */
ssize_t reg_write(struct file *filep, const char *buf,
                  size_t count, loff_t *f_pos);
	outb(*buf, REG_ADDR);	//1-byte register
	return 1;
}
 
