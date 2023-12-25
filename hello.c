#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("IO-11 Maria Tulub");
MODULE_DESCRIPTION("Lab3");
MODULE_LICENSE("GPL");

static uint count = 1; 

module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "A counter");

struct my_custom_struct {
  struct list_head list_item;
  ktime_t time_stamp;
};

static LIST_HEAD(my_custom_list_head);

static int __init my_custom_init(void)
{
  int i;  
  
  if (count == 0) {
    printk(KERN_WARNING "Count == 0\n");
  } else if (count > 10) {
    printk(KERN_WARNING "Count > 10, because WARN!\n");
    return -EINVAL;
  }

  for (i = 0; i < count; i++) {
    struct my_custom_struct *item = kmalloc(sizeof(struct my_custom_struct), GFP_KERNEL);
    item->time_stamp = ktime_get();
    list_add_tail(&item->list_item, &my_custom_list_head);
    printk(KERN_INFO "Hello Maria! %d\n", i);
  }

  return 0;
}

static void __exit my_custom_exit(void)
{
  struct my_custom_struct *entry, *temp;
  int idx = 0;

  list_for_each_entry_safe(entry, temp, &my_custom_list_head, list_item) {
    pr_info("Row: %d; Time = %lld\n", idx++, ktime_to_ns(entry->time_stamp));
    list_del(&entry->list_item);
    kfree(entry);
  }
  printk(KERN_INFO "Custom module successfully removed\n");
}

module_init(my_custom_init);
module_exit(my_custom_exit);

