# Multithread Sorting

Takes in a list of integers and sort them by using multithreaded programming. This is done by dividing the list into two similar size smaller list and then using two separate threads to sort each of the sublist.

## Instructions for building and running

To compile the program, use the command 
```
gcc thread.c -lpthread
```

Run with:
```
./a.out 
```

Sample Output: 
```
0, 1, 18, 24, 28, 31, 42, 43, 55, 81, 92, 102
```
