#include "mavalloc.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main( int argc, char * argv[] )
{
  clock_t begin = clock();

  mavalloc_init(100000, FIRST_FIT);

  unsigned char *arr[1000];

  for (int i = 0; i < 1000; i++)
  {
    arr[i] = mavalloc_alloc(100);
  }
  for (int i = 0; i < 1000; i++)
  {
    mavalloc_free(arr[i]);
    arr[i] = NULL;
  }

  for (int i = 0; i < 1000; i++)
  {
    arr[i] = mavalloc_alloc(100);
  }
  for (int i = 0; i < 1000; i++)
  {
    if (i % 2 == 0)
    {
      mavalloc_free(arr[i]);
      arr[i] = NULL;
    }   
  }  

  unsigned int *int_arr[50];

  for (int i = 0; i < 50; i++)
  {
    int_arr[i] = mavalloc_alloc(40);
  }
  for (int i = 0; i < 50; i++)
  {
    if (i % 3 == 1)
    {
      mavalloc_free(int_arr[i]);
      int_arr[i] = NULL;
    }
  }

  unsigned long *long_arr[25];

  for (int i = 0; i < 25; i++)
  {
    long_arr[i] = mavalloc_alloc(500);
  }
  for (int i = 0; i < 50; i++)
  {
    if (i % 5 == 0)
    {
      mavalloc_free(long_arr[i]);
      long_arr[i] = NULL;
    }
  }

  void *ptr1 = mavalloc_alloc(20000);
  void *ptr2 = mavalloc_alloc(15000);
  void *ptr3 = mavalloc_alloc(15000);
  void *ptr4 = mavalloc_alloc(10000);
  void *ptr5 = mavalloc_alloc(5000);

  mavalloc_free(ptr2);
  ptr2 = NULL;
  mavalloc_free(ptr4);
  ptr4 = NULL;

  void *ptr6 = mavalloc_alloc(25000);
  void *ptr7 = mavalloc_alloc(30000);

  mavalloc_free(ptr1);
  ptr1 = NULL;
  mavalloc_free(ptr7);
  ptr7 = NULL;

  void *ptr8 = mavalloc_alloc(10000);

  mavalloc_free(ptr3);
  ptr3 = NULL;
  mavalloc_free(ptr5);
  ptr5 = NULL;
  mavalloc_free(ptr8);
  ptr8 = NULL;
  mavalloc_free(ptr6);
  ptr6 = NULL;

  for (int j = 0; j < 100; j++)
  {
    //Creating bunch of different sized holes and processes
    void *p1 = mavalloc_alloc(20000);
    void *p2 = mavalloc_alloc(400);
    void *p3 = mavalloc_alloc(3500);
    void *p4 = mavalloc_alloc(10);
    void *p5 = mavalloc_alloc(1400);

    mavalloc_free(p2);
    p2 = NULL;
    mavalloc_free(p4);
    p4 = NULL;

    void *p6 = mavalloc_alloc(9000);
    void *p7 = mavalloc_alloc(150);

    mavalloc_free(p1);
    p1 = NULL;
    mavalloc_free(p7);
    p7 = NULL;

    void *p8 = mavalloc_alloc(85);

    void *p9 = mavalloc_alloc(12000);
    void *p10 = mavalloc_alloc(1200);
    void *p11 = mavalloc_alloc(35000);
    void *p12 = mavalloc_alloc(125);
    void *p13 = mavalloc_alloc(10000);

    mavalloc_free(p11);
    p11 = NULL;
    mavalloc_free(p12);
    p12 = NULL;

    void *p14 = mavalloc_alloc(500);
    void *p15 = mavalloc_alloc(77);
    void *p16 = mavalloc_alloc(9900);
    void *p17 = mavalloc_alloc(1700);
    void *p18 = mavalloc_alloc(15000);

    mavalloc_free(p14);
    p14 = NULL;
    mavalloc_free(p16);
    p16 = NULL;

    for (int i = 0; i < 1000; i++)
    {
      arr[i] = mavalloc_alloc(100);
    }
    for (int i = 0; i < 1000; i++)
    {
      if (i % 2 == 0)
      {
      mavalloc_free(arr[i]);
      arr[i] = NULL;
      }
    }  

    for (int i = 0; i < 50; i++)
    {
      int_arr[i] = mavalloc_alloc(40);
    }
    for (int i = 0; i < 50; i++)
    {
      if (i % 3 == 1)
      {
      mavalloc_free(int_arr[i]);
      int_arr[i] = NULL;
      }
    }

    mavalloc_free(p3);
    p3 = NULL;
    mavalloc_free(p5);
    p5 = NULL;
    mavalloc_free(p6);
    p6 = NULL;
    mavalloc_free(p8);
    p8 = NULL;
    mavalloc_free(p9);
    p9 = NULL;
    mavalloc_free(p10);
    p10 = NULL;
    mavalloc_free(p13);
    p13 = NULL;
    mavalloc_free(p15);
    p15 = NULL;
    mavalloc_free(p17);
    p17 = NULL;
    mavalloc_free(p18);
    p18 = NULL;
  }

  void *big_ptr = mavalloc_alloc(90000);
  mavalloc_free(big_ptr);
  big_ptr = NULL;

  mavalloc_destroy();

  clock_t end = clock();

  double time = (double) (end - begin) * 1000 / CLOCKS_PER_SEC;

  printf("Benchmark1 Time: %.2f miliseconds\n", time);
  
  return 0;
}
