#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>

int first, second;
char operation;

ssize_t calc_write(struct file *filp, const char *buf, size_t count, loff_t *offp)
{
	operation = buf[0];
	return count;
}

ssize_t calc_read(struct file *filp, char *buf, size_t count, loff_t *offp)
{
	char str[15];
	int length = 15;
	static int finished = 0;
	length = sprintf(str,"%c", operation);
	if ( finished ) {
		finished = 0;
		return 0;
	}
	finished = 1;
	strcpy(buf, str);
	return length;
}

static const struct file_operations calc_read_fops = {
	.owner = THIS_MODULE,
	.read = calc_read
};

static const struct file_operations calc_write_fops = {
	.owner = THIS_MODULE,
	.write = calc_write
};
	

static int __init proc_init(void)
{
	proc_create("calc_write", 0666, NULL, &calc_write_fops);
	proc_create("calc_read", 0, NULL, &calc_read_fops);
	return 0;
}
static void __exit proc_exit(void)
{
	remove_proc_entry("calc_read", NULL);
	remove_proc_entry("calc_write", NULL);
}

MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_exit);
