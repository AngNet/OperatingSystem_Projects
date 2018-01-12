# Traversing Linked List

birthday_list.c creates a linked list containing 5 struct birthday elements. The contents of each are outputted to the kernel log buffer. At the end, the elements are deleted from the linked list and the memory is freed back into the kernel. 

## Instructions for building and running

To compile the module, enter the following on the command line:

```
make
```

Kernel modules are loaded using the following command:

```
sudo insmod birthday_list.ko
```

Use "dmesg" to  check the contents of the message in the kernel log buffer. To remove the birthday list  type in the follow:

```
sudo rmmod birthday_list.ko
```

Recheck the kernal log with:

```
sudo dmesg -c
```
