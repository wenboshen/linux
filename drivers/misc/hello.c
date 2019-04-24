#include <linux/module.h>
#include <linux/init_task.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>	/* for copy_*_user */

#define MAX_SIZE	1024

static char msg[MAX_SIZE];

/*static int hello_proc_show(struct seq_file *m, void *v) {*/
/*seq_printf(m, "Hello proc!\n");*/
/*return 0;*/
/*}*/

/*static int hello_proc_open(struct inode *inode, struct  file *file) {*/
/*return single_open(file, hello_proc_show, NULL);*/
/*}*/


static void traversal_thread_group(struct task_struct * tsk){
	struct task_struct * curr_thread = NULL;
	unsigned long tg_offset = offsetof(struct task_struct, thread_group);

	curr_thread = (struct task_struct *) (((unsigned long)tsk->thread_group.next) - tg_offset);
	/*if (curr_thread == tsk){*/
	/*printk("Thread Group is empty!");*/
	/*return;*/
	/*}*/
	while (curr_thread != tsk){
		printk("THREAD PID=%d\tCANARY=%lx\n", curr_thread->pid, curr_thread->stack_canary);
		curr_thread = (struct task_struct *) (((unsigned long)curr_thread->thread_group.next) - tg_offset);
	}
}

static void traversal_process(void) {
	struct task_struct * tsk = NULL;

	traversal_thread_group(&init_task);
	for_each_process(tsk){
		printk("PROC PID=%d\tCANARY=%lx\n", tsk->pid, tsk->stack_canary);
		traversal_thread_group(tsk);
	}
}

static noinline void canary_test_stub(u64 * a1, u64 *a2) {
	printk("ADDR: %llx %llx\n", (u64)a1, (u64)a2);
}

void canary_test(void) {
	u64 array[4] = {0x1111, 0x2222, 0x3333, 0x4444};
	u64 berry[4] = {0xaaaa, 0xbbbb, 0xcccc, 0xdddd};

	printk("PID %d CANARY %lx\n", current->pid, current->stack_canary);

	canary_test_stub(array, berry);
}

ssize_t proc_read(struct file *filp,char __user *buf,size_t count,loff_t *offp ) 
{
	/*sprintf(msg, "%s", "hello proc is read");*/

	printk("proc_read:%s\n", msg);
	/*canary_test();*/
	traversal_process();
	return 0;
}

ssize_t proc_write(struct file *filp,const char *buf,size_t count,loff_t *offp)
{
	int ret = 0;
	if (count > MAX_SIZE){
		count =  MAX_SIZE;
	}

	/*ret = copy_from_user(msg,buf,count);*/
	printk("proc_write: copy_from_user return =%d\n", ret);
	return 0;
}

static const struct file_operations hello_proc_fops = {
	/*.owner = THIS_MODULE,*/
	/*.open = hello_proc_open,*/
	.read = proc_read,
	.write = proc_write,
	/*.llseek = seq_lseek,*/
	/*.release = single_release,*/
};

static int __init hello_proc_init(void) {
	proc_create("hello_proc", 0, NULL, &hello_proc_fops);
	return 0;
}

static void __exit hello_proc_exit(void) {
	remove_proc_entry("hello_proc", NULL);
}

MODULE_LICENSE("GPL");
module_init(hello_proc_init);
module_exit(hello_proc_exit);
