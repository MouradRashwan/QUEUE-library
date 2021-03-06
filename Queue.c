#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "Queue.h"

void Queue_allocateStatic(Queue_t *ptQueue, void *pvQueueArray, int32_t i32QueueLen, int32_t i32ElementSize)
{
    ptQueue->i32ElementSize = i32ElementSize;
    ptQueue->i32Head = 0;
    ptQueue->i32Tail = 0;
    ptQueue->i32ActualLen = 0;
    ptQueue->i32QueueLen = i32QueueLen;
    ptQueue->pvArray = pvQueueArray;
}

bool Queue_allocateDynamic(Queue_t *ptQueue, int32_t i32QueueLen, int32_t i32ElementSize)
{
    ptQueue->i32ElementSize = i32ElementSize;
    ptQueue->i32Head = 0;
    ptQueue->i32Tail = 0;
    ptQueue->i32ActualLen = 0;
    ptQueue->i32QueueLen = i32QueueLen;
    ptQueue->pvArray = (void *)calloc(ptQueue->i32QueueLen, ptQueue->i32ElementSize);

    if(ptQueue->pvArray == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Queue_reallocateDynamic(Queue_t *ptQueue, int32_t i32NewQueueLen)
{
    void *pvData;

    pvData = (void *)realloc(ptQueue->pvArray, i32NewQueueLen * ptQueue->i32ElementSize);

    if(pvData == NULL)
    {
        return false;
    }
    else
    {
        ptQueue->pvArray = pvData;
        ptQueue->i32QueueLen = i32NewQueueLen;
        return true;
    }
}

void Queue_deallocateDynamic(Queue_t *ptQueue)
{
    free(ptQueue->pvArray);
    ptQueue->pvArray = NULL;
}

bool Queue_isEmpty(Queue_t *ptQueue)
{
    if(ptQueue->i32ActualLen == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Queue_isFull(Queue_t *ptQueue)
{
    if(ptQueue->i32ActualLen == ptQueue->i32QueueLen)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int32_t Queue_getActualLen(Queue_t *ptQueue)
{
    return ptQueue->i32ActualLen;
}

int32_t Queue_getQueueLen(Queue_t *ptQueue)
{
    return ptQueue->i32QueueLen;
}

bool Queue_getHead(Queue_t *ptQueue, void *pvElement)
{
    void *pvQueueElement;

    ptQueue->bHeadingToTail = true;
    ptQueue->i32Next = (ptQueue->i32Head + 1) % ptQueue->i32QueueLen;

    if(!Queue_isEmpty(ptQueue))
    {
        {
            pvQueueElement = ((uint8_t *)ptQueue->pvArray) + (ptQueue->i32Head * ptQueue->i32ElementSize);
            memcpy(pvElement, pvQueueElement, ptQueue->i32ElementSize);
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool Queue_getTail(Queue_t *ptQueue, void *pvElement)
{
    void *pvQueueElement;

    ptQueue->bHeadingToTail = false;
    ptQueue->i32Next = (ptQueue->i32Tail - 1 + ptQueue->i32QueueLen) % ptQueue->i32QueueLen;

    if(!Queue_isEmpty(ptQueue))
    {
        {
            pvQueueElement = ((uint8_t *)ptQueue->pvArray) + (ptQueue->i32Next * ptQueue->i32ElementSize);
            memcpy(pvElement, pvQueueElement, ptQueue->i32ElementSize);
        }
        ptQueue->i32Next = (ptQueue->i32Next - 1 + ptQueue->i32QueueLen) % ptQueue->i32QueueLen;
        return true;
    }
    else
    {
        return false;
    }
}

bool Queue_getNext(Queue_t *ptQueue, void *pvElement)
{
    void *pvQueueElement;

    if(ptQueue->bHeadingToTail == true)
    {
        if(ptQueue->i32Next != ptQueue->i32Tail)
        {
            {
                pvQueueElement = ((uint8_t *)ptQueue->pvArray) + (ptQueue->i32Next * ptQueue->i32ElementSize);
                memcpy(pvElement, pvQueueElement, ptQueue->i32ElementSize);
            }
            ptQueue->i32Next = (ptQueue->i32Next + 1) % ptQueue->i32QueueLen;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(ptQueue->i32Next != ((ptQueue->i32Head - 1 + ptQueue->i32QueueLen) % ptQueue->i32QueueLen))
        {
            {
                pvQueueElement = ((uint8_t *)ptQueue->pvArray) + (ptQueue->i32Next * ptQueue->i32ElementSize);
                memcpy(pvElement, pvQueueElement, ptQueue->i32ElementSize);
            }
            ptQueue->i32Next = (ptQueue->i32Next - 1 + ptQueue->i32QueueLen) % ptQueue->i32QueueLen;
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool Queue_dequeue(Queue_t *ptQueue, void *pvElement)
{
    void *pvQueueElement;

    if(!Queue_isEmpty(ptQueue))
    {
        {
            pvQueueElement = ((uint8_t *)ptQueue->pvArray) + (ptQueue->i32Head * ptQueue->i32ElementSize);
            memcpy(pvElement, pvQueueElement, ptQueue->i32ElementSize);
        }
        ptQueue->i32ActualLen -= 1;
        ptQueue->i32Head = (ptQueue->i32Head + 1) % ptQueue->i32QueueLen;
        return true;
    }
    else
    {
        return false;
    }
}

bool Queue_enqueue(Queue_t *ptQueue, void *pvElement)
{
    void *pvQueueElement;

    if(!Queue_isFull(ptQueue))
    {
        {
            pvQueueElement = ((uint8_t *)ptQueue->pvArray) + (ptQueue->i32Tail * ptQueue->i32ElementSize);
            memcpy(pvQueueElement, pvElement, ptQueue->i32ElementSize);
        }
        ptQueue->i32ActualLen += 1;
        ptQueue->i32Tail = (ptQueue->i32Tail + 1) % ptQueue->i32QueueLen;
        return true;
    }
    else
    {
        return false;
    }
}
