/*
Name: Natalie Ang
Course: CPSC 351 - 01, Operating Systems, Fall 2017
Assignment 1: Chapter 2 Project 1 Part 2
Instructor: Professor William McCarthy
Code Last Updated: 9/4/17

Purpose: Create a linked list of 5 struct, traverse the 
list and ouput contents to kernel log buffer. Use command "dmesg"
to confirm properly constructed list. Afterward, Delete elements and
free memory back to kernel.
*/

#include <linux/module.h>   
#include <linux/kernel.h>   
#include <linux/init.h>     
#include <linux/list.h>    
#include <linux/types.h>    
#include <linux/slab.h>     

/* Birthday struct  */
typedef struct birthday_ {
    
    int day;
    int month;
    int year;
    struct list_head list;

}birthday;

/*Macro for inserting elements into linked list */
static LIST_HEAD(birthday_list);

/* 

The function where birthday struct are initialized,
memory is allocated for thse struct and inserts 5 
birthdays into linked list. 

*/
int birthdayList_init(void) {

    printk(KERN_INFO "Loading Module\n");

    /* Allocate 5 birthdays structs */
    birthday *person;
    int i = 0;
    for(i = 0; i < 5; i++) {
        /* Request malloc to the kernel. */
        person = kmalloc(sizeof(*person), GFP_KERNEL);

        /* Assign value to the struct. */
        person->day = 2 + i;
        person->month = 8 + i;
        person->year = 1995 + i;

        /* Init the list within the struct. */
        INIT_LIST_HEAD(&person->list);

        /* Add this struct to the tail of the list. */
        list_add_tail(&person->list, &birthday_list);

    }

    /* Go thru the list and print. */
    birthday *ptr;
    list_for_each_entry(ptr, &birthday_list, list) {

        printk(KERN_INFO "Birthday: Month %d Day %d Year %d\n",
            ptr->month,
            ptr->day,
            ptr->year);
    }

    return 0;
}

/* 
   This function is called when the module is removed. 
   It prints the list of birthdays being removed, and 
   then deletes the list from kernel memory           
*/
void birthdayList_exit(void) {

    printk(KERN_INFO "Removing Module\n");

    /* Go thru the list and free the memory. */
    birthday *ptr, *next;
    list_for_each_entry_safe(ptr, next, &birthday_list, list) {

        printk(KERN_INFO "Removing Birthday: Month %d Day %d Year %d\n",
                ptr->month,
                ptr->day,
                ptr->year);
        /*Delete list using included functions*/
        list_del(&ptr->list);
        /*free list memory*/
        kfree(ptr);
    
    }

}

/*Macros for module entry and exit points */
module_init(birthdayList_init);
module_exit(birthdayList_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module that displays 5 birthday structs");
MODULE_AUTHOR("Natalie Ang");