# Dining Philosopher

The situation is that there are 5 philosophers that alternate between thinking and eating. In order to eat, they need to pick up a fork from their left and right. Pthreads mutex locks and condition variables are used to allow philosophers to eat and not let anyone go hungry. 

## Instructions for building and running

To compile the program, use the command 
```
gcc dine.c -lpthread
```

Ignore the warnings and then run with 
```
./a.out
```
