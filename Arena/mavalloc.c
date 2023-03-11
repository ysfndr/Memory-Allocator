// The MIT License (MIT)
// 
// Copyright (c) 2021, 2022 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES UTA OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//Yusuf Nadir Cavus

#include "mavalloc.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_LL_SIZE 10000

static void *global_arena = NULL;
static enum ALGORITHM global_algorithm;
static int last_alloc = 0;
static int initialized = 0;

// H = Hole, P = Process
enum TYPE
{
  H = 0,
  P
};

struct Node 
{ 
  size_t size; 
  enum TYPE type; 
  int next; 
  int previous;
  int in_use;
  void *arena; 
};

static struct Node LinkedList[MAX_LL_SIZE]; //Linked list as an array. max size = 10000

int mavalloc_init( size_t size, enum ALGORITHM algorithm )
{
  initialized = 1;

  if (size < 0) return -1;

  size = ALIGN4(size);

  for (int i = 0; i < MAX_LL_SIZE; i++)
  {
    LinkedList[i].in_use = 0;
    LinkedList[i].size = 0;
    LinkedList[i].arena = 0;
    LinkedList[i].type = H;
    LinkedList[i].previous = -1;
    LinkedList[i].next = -1;
  }

  global_arena = malloc(size);
  global_algorithm = algorithm;

  if (!global_arena) return -1; //if malloc fails then return -1

  LinkedList[0].in_use = 1;
  LinkedList[0].size = size;
  LinkedList[0].arena = global_arena;
  LinkedList[0].type = H;
  LinkedList[0].previous = -1;
  LinkedList[0].next = -1;

  return 0;
}

void mavalloc_destroy( )
{
  for (int i = 0; i < MAX_LL_SIZE; i++)
  {
    LinkedList[i].in_use = 0;
    LinkedList[i].size = 0;
    LinkedList[i].arena = 0;
    LinkedList[i].type = H;
    LinkedList[i].previous = -1;
    LinkedList[i].next = -1;
  }

  free(global_arena);
  global_arena = NULL;
  last_alloc = 0;

  initialized = 0;

  return;
}

void * mavalloc_alloc( size_t size )
{
  if (initialized == 0) return NULL;
  if (size == 0) return NULL;

  void *ptr = NULL;

  size = ALIGN4(size);

  if (global_algorithm == FIRST_FIT)
  {
    for (int i = 0; i < MAX_LL_SIZE; i++)
    {
      if (LinkedList[i].in_use && LinkedList[i].type == H && LinkedList[i].size >= size)
      {
        //if the size of the hole is the same as the allocation size
        //then we don't need to update or shift any node other than the ith node.
        if (LinkedList[i].size == size)
        {
          LinkedList[i].type = P;
          ptr = LinkedList[i].arena;
          return ptr;
        }

        LinkedList[i].type = P;
        ptr = LinkedList[i].arena;

        if (LinkedList[i].next == -1)
        {
          size_t remainder_size = LinkedList[i].size - size;

          LinkedList[i].size = size;
          LinkedList[i].next = i + 1;
          
          LinkedList[i + 1].in_use = 1;
          LinkedList[i + 1].size = remainder_size;
          LinkedList[i + 1].type = H;
          LinkedList[i + 1].previous = i;
          LinkedList[i + 1].arena = LinkedList[i].arena + size;
        }
        else
        {
          size_t remainder_size = LinkedList[i].size - size;
          LinkedList[i].size = size;

          //get to the last used node in order to start shifting from the end
          //in this case x will be the unused node right after the last used node
          int x = i + 1;
          while (LinkedList[x].in_use == 1) x++;

          //Shift
          for (int j = x; j > i + 1; j--)
          {
            LinkedList[j].in_use = LinkedList[j - 1].in_use;
            LinkedList[j].size = LinkedList[j - 1].size;
            LinkedList[j].arena = LinkedList[j - 1].arena;
            LinkedList[j].type = LinkedList[j - 1].type;
            LinkedList[j].previous = j - 1;

            if (j == x) LinkedList[j].next = -1; //if it is the last node. next should be -1
            else LinkedList[j].next = j + 1;
          }

          //After shifting we can now update LinkedList[i + 1] accordingly
          LinkedList[i].next = i + 1;
          
          LinkedList[i + 1].in_use = 1;
          LinkedList[i + 1].size = remainder_size;
          LinkedList[i + 1].type = H;
          LinkedList[i + 1].previous = i;
          LinkedList[i + 1].arena = LinkedList[i].arena + size;
        }

        break;
      }
    }
  }
  else if (global_algorithm == BEST_FIT)
  {
    int best = -1;
    int b_fit = 1000000000; //assuming max arena size is less than 100,000,000

    //we first find the best fit
    for (int i = 0; i < MAX_LL_SIZE; i++)
    {
      if (LinkedList[i].in_use && LinkedList[i].type == H && LinkedList[i].size >= size)
      {
        //if it's a better fit, update best index and the best fit
        if (LinkedList[i].size - size < b_fit)
        {
          best = i;
          b_fit = LinkedList[i].size - size;
        } 
      }
    }

    if (best == -1) return NULL;

    //if the size of the hole is the same as the allocation size
    //then we don't need to update or shift any node other than the ith node.
    if (LinkedList[best].size == size)
    {
      LinkedList[best].type = P;
      ptr = LinkedList[best].arena;
      return ptr;
    }

    LinkedList[best].type = P;
    ptr = LinkedList[best].arena;

    if (LinkedList[best].next == -1)
    {
      size_t remainder_size = LinkedList[best].size - size;

      LinkedList[best].size = size;
      LinkedList[best].next = best + 1;
      
      LinkedList[best + 1].in_use = 1;
      LinkedList[best + 1].size = remainder_size;
      LinkedList[best + 1].type = H;
      LinkedList[best + 1].previous = best;
      LinkedList[best + 1].arena = LinkedList[best].arena + size;
    }
    //if the size of the hole is the same as the allocation size
    //then we don't need to shift. we only need to update the node.
    else if (LinkedList[best].size != size)
    {
      size_t remainder_size = LinkedList[best].size - size;
      LinkedList[best].size = size;
      
      //get to the last used node in order to start shifting from the end
      //in this case x will be the unused node right after the last used node
      int x = best + 1;
      while (LinkedList[x].in_use == 1) x++;

      //Shift
      for (int j = x; j > best + 1; j--)
      {
        LinkedList[j].in_use = LinkedList[j - 1].in_use;
        LinkedList[j].size = LinkedList[j - 1].size;
        LinkedList[j].arena = LinkedList[j - 1].arena;
        LinkedList[j].type = LinkedList[j - 1].type;
        LinkedList[j].previous = j - 1;

        if (j == x) LinkedList[j].next = -1; //if it is the last node. next should be -1
        else LinkedList[j].next = j + 1;
      }

      //After shifting we can now update LinkedList[i + 1] accordingly
      LinkedList[best].next = best + 1;
      
      LinkedList[best + 1].in_use = 1;
      LinkedList[best + 1].size = remainder_size;
      LinkedList[best + 1].type = H;
      LinkedList[best + 1].previous = best;
      LinkedList[best + 1].arena = LinkedList[best].arena + size;
    }
  }
  else if (global_algorithm == WORST_FIT)
  {
    int best = -1;
    int w_fit = 0;

    //we first find the worst fit
    for (int i = 0; i < MAX_LL_SIZE; i++)
    {
      if (LinkedList[i].in_use && LinkedList[i].type == H && LinkedList[i].size >= size)
      {
        //if it's a worse fit, update best index and the fit
        if (LinkedList[i].size - size >= w_fit)
        {
          best = i;
          w_fit = LinkedList[i].size - size;
        } 
      }
    }

    if (best == -1) return NULL;

    //if the size of the hole is the same as the allocation size
    //then we don't need to update or shift any node other than the ith node.
    if (LinkedList[best].size == size)
    {
      LinkedList[best].type = P;
      ptr = LinkedList[best].arena;
      return ptr;
    }

    LinkedList[best].type = P;
    ptr = LinkedList[best].arena;

    if (LinkedList[best].next == -1)
    {
      size_t remainder_size = LinkedList[best].size - size;

      LinkedList[best].size = size;
      LinkedList[best].next = best + 1;
      
      LinkedList[best + 1].in_use = 1;
      LinkedList[best + 1].size = remainder_size;
      LinkedList[best + 1].type = H;
      LinkedList[best + 1].previous = best;
      LinkedList[best + 1].arena = LinkedList[best].arena + size;
    }
    //if the size of the hole is the same as the allocation size
    //then we don't need to shift. Shifting in this case would create an extra 0 sized hole
    else if (LinkedList[best].size != size)
    {
      size_t remainder_size = LinkedList[best].size - size;
      LinkedList[best].size = size;

      //get to the last used node in order to start shifting from the end
      //in this case x will be the unused node right after the last used node
      int x = best + 1;
      while (LinkedList[x].in_use == 1) x++;

      //Shift
      for (int j = x; j > best + 1; j--)
      {
        LinkedList[j].in_use = LinkedList[j - 1].in_use;
        LinkedList[j].size = LinkedList[j - 1].size;
        LinkedList[j].arena = LinkedList[j - 1].arena;
        LinkedList[j].type = LinkedList[j - 1].type;
        LinkedList[j].previous = j - 1;

        if (j == x) LinkedList[j].next = -1; //if it is the last node. next should be -1
        else LinkedList[j].next = j + 1;
      }

      //After shifting we can now update LinkedList[i + 1] accordingly
      LinkedList[best].next = best + 1;
      
      LinkedList[best + 1].in_use = 1;
      LinkedList[best + 1].size = remainder_size;
      LinkedList[best + 1].type = H;
      LinkedList[best + 1].previous = best;
      LinkedList[best + 1].arena = LinkedList[best].arena + size;
    }
  }
  else if (global_algorithm == NEXT_FIT)
  {
    for (int i = last_alloc; i < MAX_LL_SIZE; i++)
    {
      if (LinkedList[i].in_use && LinkedList[i].type == H && LinkedList[i].size >= size)
      {
        last_alloc = i;
        LinkedList[i].type = P;
        ptr = LinkedList[i].arena;

        size_t remainder_size = LinkedList[i].size - size;
        LinkedList[i].size = size;

        //In Next-fit, we won't allocate a hole that is in between processes
        //which means we won't need to shift
        LinkedList[i].next = i + 1;
        
        LinkedList[i + 1].in_use = 1;
        LinkedList[i + 1].size = remainder_size;
        LinkedList[i + 1].type = H;
        LinkedList[i + 1].previous = i;
        LinkedList[i + 1].arena = LinkedList[i].arena + size;

        break;    
      }
    }

    //if not allocated. then loop around
    if (ptr == NULL)
    {
      for (int i = 0; i < last_alloc; i++)
      {
        if (LinkedList[i].in_use && LinkedList[i].type == H && LinkedList[i].size >= size)
        {
          last_alloc = i;
          LinkedList[i].type = P;
          ptr = LinkedList[i].arena;

          size_t remainder_size = LinkedList[i].size - size;
          LinkedList[i].size = size;

          //In Next-fit, we won't allocate a hole that is in between processes
          //which means we won't need to shift
          LinkedList[i].next = i + 1;
          
          LinkedList[i + 1].in_use = 1;
          LinkedList[i + 1].size = remainder_size;
          LinkedList[i + 1].type = H;
          LinkedList[i + 1].previous = i;
          LinkedList[i + 1].arena = LinkedList[i].arena + size;

          break;    
        }
      }
    }
  }

  return ptr;
}

void mavalloc_free( void * ptr )
{
  for (int i = 0; i < MAX_LL_SIZE; i++)
  {
    if (ptr == LinkedList[i].arena && LinkedList[i].type == P)
    {
      LinkedList[i].type = H;

      if (i < 9999 && LinkedList[i + 1].type == H)
      {
        LinkedList[i].size += LinkedList[i + 1].size;

        //Shift starting from i+1
        for (int j = i + 1; j < MAX_LL_SIZE; j++)
        {
          LinkedList[j].in_use = LinkedList[j + 1].in_use;
          LinkedList[j].size = LinkedList[j + 1].size;
          LinkedList[j].arena = LinkedList[j + 1].arena;
          LinkedList[j].type = LinkedList[j + 1].type;
          LinkedList[j].previous = j - 1;
          LinkedList[j].next = j + 1;
        }
      }
      if (i > 0 && LinkedList[i - 1].type == H)
      {
        LinkedList[i - 1].size += LinkedList[i].size;

        //Shift starting from i
        for (int j = i; j < MAX_LL_SIZE; j++)
        {
          LinkedList[j].in_use = LinkedList[j + 1].in_use;
          LinkedList[j].size = LinkedList[j + 1].size;
          LinkedList[j].arena = LinkedList[j + 1].arena;
          LinkedList[j].type = LinkedList[j + 1].type;
          LinkedList[j].previous = j - 1;
          LinkedList[j].next = j + 1;
        }
      }
      break;
    }
  }

  return;
}

int mavalloc_size( )
{


  int number_of_nodes = 0;

  for (int i = 0; i < MAX_LL_SIZE; i++)
  {
    if (LinkedList[i].in_use) number_of_nodes++;
  }

  return number_of_nodes;
}
