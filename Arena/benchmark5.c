#include "mavalloc.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main( int argc, char * argv[] )
{
  clock_t begin = clock();

  unsigned char *arr[1000];

  for (int i = 0; i < 1000; i++)
  {
    arr[i] = malloc(100);
  }
  for (int i = 0; i < 1000; i++)
  {
    free(arr[i]);
    arr[i] = NULL;
  }

  for (int i = 0; i < 1000; i++)
  {
    arr[i] = malloc(100);
  }
  for (int i = 0; i < 1000; i++)
  {
    if (i % 2 == 0)
    {
      free(arr[i]);
      arr[i] = NULL;
    }   
  }  

  unsigned int *int_arr[50];

  for (int i = 0; i < 50; i++)
  {
    int_arr[i] = malloc(40);
  }
  for (int i = 0; i < 50; i++)
  {
    if (i % 3 == 1)
    {
      free(int_arr[i]);
      int_arr[i] = NULL;
    }
  }

  unsigned long *long_arr[25];

  for (int i = 0; i < 25; i++)
  {
    long_arr[i] = malloc(500);
  }
  for (int i = 0; i < 50; i++)
  {
    if (i % 5 == 0)
    {
      free(long_arr[i]);
      long_arr[i] = NULL;
    }
  }

  void *ptr1 = malloc(20000);
  void *ptr2 = malloc(15000);
  void *ptr3 = malloc(15000);
  void *ptr4 = malloc(10000);
  void *ptr5 = malloc(5000);

  free(ptr2);
  ptr2 = NULL;
  free(ptr4);
  ptr4 = NULL;

  void *ptr6 = malloc(25000);
  void *ptr7 = malloc(30000);

  free(ptr1);
  ptr1 = NULL;
  free(ptr7);
  ptr7 = NULL;

  void *ptr8 = malloc(10000);

  free(ptr3);
  ptr3 = NULL;
  free(ptr5);
  ptr5 = NULL;
  free(ptr8);
  ptr8 = NULL;
  free(ptr6);
  ptr6 = NULL;

  for (int j = 0; j < 100; j++)
  {
    //Creating bunch of different sized holes and processes
    void *p1 = malloc(20000);
    void *p2 = malloc(400);
    void *p3 = malloc(3500);
    void *p4 = malloc(10);
    void *p5 = malloc(1400);

    free(p2);
    p2 = NULL;
    free(p4);
    p4 = NULL;

    void *p6 = malloc(9000);
    void *p7 = malloc(150);

    free(p1);
    p1 = NULL;
    free(p7);
    p7 = NULL;

    void *p8 = malloc(85);

    void *p9 = malloc(12000);
    void *p10 = malloc(1200);
    void *p11 = malloc(35000);
    void *p12 = malloc(125);
    void *p13 = malloc(10000);

    free(p11);
    p11 = NULL;
    free(p12);
    p12 = NULL;

    void *p14 = malloc(500);
    void *p15 = malloc(77);
    void *p16 = malloc(9900);
    void *p17 = malloc(1700);
    void *p18 = malloc(15000);

    free(p14);
    p14 = NULL;
    free(p16);
    p16 = NULL;

    for (int i = 0; i < 1000; i++)
    {
      arr[i] = malloc(100);
    }
    for (int i = 0; i < 1000; i++)
    {
      if (i % 2 == 0)
      {
      free(arr[i]);
      arr[i] = NULL;
      }
    }  

    for (int i = 0; i < 50; i++)
    {
      int_arr[i] = malloc(40);
    }
    for (int i = 0; i < 50; i++)
    {
      if (i % 3 == 1)
      {
      free(int_arr[i]);
      int_arr[i] = NULL;
      }
    }

    free(p3);
    p3 = NULL;
    free(p5);
    p5 = NULL;
    free(p6);
    p6 = NULL;
    free(p8);
    p8 = NULL;
    free(p9);
    p9 = NULL;
    free(p10);
    p10 = NULL;
    free(p13);
    p13 = NULL;
    free(p15);
    p15 = NULL;
    free(p17);
    p17 = NULL;
    free(p18);
    p18 = NULL;
  }

  void *big_ptr = malloc(90000);
  free(big_ptr);
  big_ptr = NULL;

  clock_t end = clock();

  double time = (double) (end - begin) * 1000 / CLOCKS_PER_SEC;

  printf("Benchmark5 Time: %.2f miliseconds\n", time);
  
  return 0;
}
