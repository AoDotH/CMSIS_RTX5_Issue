/**
 * @file    main.c
 *
 * @brief   Test effect of double free.
*/

#include "cmsis_os2.h"
#include "RTE_Components.h"
#include  CMSIS_device_header
#include <string.h>
#include <stdio.h>
#include <stdint.h>


static osMemoryPoolId_t memPoolId;
static void test_task(void *arg);

int main (void) 
{
    osThreadAttr_t attr;
    osThreadId_t tid;
    static uint64_t stack[1024U/8U];
    
    /* System Initialization */
    SystemCoreClockUpdate();
    
    osKernelInitialize();                 // Initialize CMSIS-RTOS

    /* Create a thread */
    (void)memset(&attr, 0, sizeof(osThreadAttr_t));
    attr.name = NULL;
    attr.stack_mem = stack;
    attr.stack_size = 1024U;
    attr.priority = osPriorityNormal;
    tid = osThreadNew(test_task, NULL, &attr);
    if(tid == NULL){
        printf("Failed to create new thread\n");
    }
    
    /* Init memory pool */
    memPoolId = osMemoryPoolNew(2U, 128U, NULL);
    if(memPoolId == NULL){
        printf("Error osMemoryPoolNew failed\n");
    }
    
    osKernelStart();                      // Start thread execution
    
    
    for (;;) {}
}

static void test_task(void *arg)
{
    (void) arg;
    osStatus_t status;
    uint8_t *tempPtr;
    
        printf("test_task started\n");   

        tempPtr = osMemoryPoolAlloc (memPoolId, 0U);
        if(tempPtr == NULL){
            printf("failed to allocate memory\n");
        }
        else{
            printf("Allocated %u\n", (uint32_t)tempPtr);
        }
        
        tempPtr = osMemoryPoolAlloc (memPoolId, 0U);
        if(tempPtr == NULL){
            printf("failed to allocate memory\n");
        }
        else{
            printf("Allocated %u\n", (uint32_t)tempPtr);
        }
        
        status = osMemoryPoolFree (memPoolId, tempPtr);
        if(status != osOK){
            printf("Free failed %u\n",status);
        }
        else{
            printf("Freed %u\n", (uint32_t)tempPtr);
        }
        
        status = osMemoryPoolFree (memPoolId, tempPtr);
        if(status != osOK){
            printf("Free failed %u\n",status);
        }
        else{
            printf("Freed %u\n", (uint32_t)tempPtr);
        }
        
        tempPtr = osMemoryPoolAlloc (memPoolId, 0U);
        if(tempPtr == NULL){
            printf("failed to allocate memory\n");
        }
        else{
            printf("Allocated %u\n", (uint32_t)tempPtr);
        }
        tempPtr = osMemoryPoolAlloc (memPoolId, 0U);
        if(tempPtr == NULL){
            printf("failed to allocate memory\n");
        }
        else{
            printf("Allocated %u\n", (uint32_t)tempPtr);
        }
        tempPtr = osMemoryPoolAlloc (memPoolId, 0U);
        if(tempPtr == NULL){
            printf("failed to allocate memory\n");
        }
        else{
            printf("Allocated %u\n", (uint32_t)tempPtr);
        }

        tempPtr = osMemoryPoolAlloc (memPoolId, 0U);
        if(tempPtr == NULL){
            printf("failed to allocate memory\n");
        }
        else{
            printf("Allocated %u\n", (uint32_t)tempPtr);
        }

        osDelay(40000);
}
