// SPDX-License-Identifier: GPL-2.0

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Hello1 Module");
MODULE_LICENSE("Dual BSD/GPL");

struct event_data {
	struct list_head list;
	ktime_t start_time;
	ktime_t end_time;
};

static LIST_HEAD(event_list);

void print_hello(unsigned int n)
{
	struct event_data *event;

	event = kmalloc(sizeof(*event), GFP_KERNEL);
	if (!event)
		return;

	event->start_time = ktime_get();
	pr_info("Hello, world!\n");
	event->end_time = ktime_get();

	list_add_tail(&event->list, &event_list);
}
EXPORT_SYMBOL(print_hello);

static void __exit hello1_exit(void)
{
	struct event_data *event, *tmp;

	list_for_each_entry_safe(event, tmp, &event_list, list) {
		pr_info("Event time: %lld ns\n",
			ktime_to_ns(event->end_time) -
			ktime_to_ns(event->start_time));
		list_del(&event->list);
		kfree(event);
	}
	pr_info("Goodbye from hello1!\n");
}

module_exit(hello1_exit);

