#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>



MODULE_AUTHOR("Moroz Denis IV-81");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint amt = 1;
module_param(amt, uint, S_IRUGO);
MODULE_PARM_DESC(amt, "Amount of times hello world message should be repeated");

struct my_data {
	struct list_head list_node;
	ktime_t time;
};

static LIST_HEAD(my_list_head);

static int __init hello_init(void)
{
	uint i = 0;
	uint done = 0;
	
	struct my_data *tail;
	
	BUG_ON(amt > 10);
	if(amt == 0){
		pr_warn("WARNING The amount is 0");
	}else if (amt >= 5 && amt <= 10){
		pr_warn("WARNING The amount is range 5 and 10");
	}
	

	for (i = 0 ; i < amt; i++) 
	{
		tail = kmalloc(sizeof(struct my_data), GFP_KERNEL); 
		if (i == 8){tail = 0;}
		
		if(ZERO_OR_NULL_PTR(tail)){
			done = 1;
			break;
		}
		
		tail->time = ktime_get();
		list_add_tail(&(tail->list_node), &my_list_head);
		pr_info("Hello, world!\n");
	}
	
	
	if(done == 1){
		struct my_data *md, *tmp;
		pr_err("ERROR");
		list_for_each_entry_safe(md, tmp, &my_list_head, list_node) {
			list_del(&md->list_node);
			kfree(md);
		}
		BUG();
		return -ENOMEM;
	}

	return 0;
	
	
}

static void __exit hello_exit(void){

	struct my_data *md, *tmp;
	list_for_each_entry_safe(md, tmp, &my_list_head, list_node) {
		pr_info("Time: %lld\n", md->time);
		list_del(&md->list_node);
		kfree(md);
	}
	BUG_ON(!list_empty(&my_list_head));
}

module_init(hello_init);
module_exit(hello_exit);
