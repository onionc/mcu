#include "list.h"
#include "FreeRTOS.h"
#include "task.h"

uint32_t flag1, flag2, flag3;

// 任务控制块定义
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;
TaskHandle_t Task3_Handle;
TaskHandle_t TaskIdle_Handle; // 空闲任务控制块




// 软件延时
void delay(uint32_t count){
    for(; count!=0; count--);
}

// 任务1
void Task1_Entry(void *p_arg){
    for(;;){

        flag1 = 1;
        //vTaskDelay(2);
        delay(100);
        flag1 = 0;
        delay(100);
        //vTaskDelay(2);
    }
}

// 任务2
void Task2_Entry(void *p_arg){
    for(;;){

        flag2 = 1;
        //vTaskDelay(2);
        delay(100);
        flag2 = 0;
        delay(100);
        //vTaskDelay(2);
    }
}
// 任务3
void Task3_Entry(void *p_arg){
    for(;;){

        flag3 = 1;
        vTaskDelay(1);
        flag3 = 0;
        vTaskDelay(1);
    }
}

// 空闲任务
void prvIdleTask(void){
    for(;;){
    
    }
}

int main(void){
    
    // 创建任务
    Task1_Handle = xTaskCreateStatic(   (TaskFunction_t) Task1_Entry,
                                        (char*) "Task1",
                                        (uint32_t) TASK1_STACK_SIZE,
                                        (void *) NULL,
                                        2,
                                        (StackType_t *) Task1Stack,
                                        (TCB_t *) &Task1TCB);

    Task2_Handle = xTaskCreateStatic(   (TaskFunction_t) Task2_Entry,
                                        (char*) "Task2",
                                        (uint32_t) TASK2_STACK_SIZE,
                                        (void *) NULL,
                                        2,
                                        (StackType_t *) Task2Stack,
                                        (TCB_t *) &Task2TCB);
    Task3_Handle = xTaskCreateStatic(   (TaskFunction_t) Task3_Entry,
                                        (char*) "Task3",
                                        (uint32_t) TASK3_STACK_SIZE,
                                        (void *) NULL,
                                        3,
                                        (StackType_t *) Task3Stack,
                                        (TCB_t *) &Task3TCB);
    portDISABLE_INTERRUPTS();

    // 启动调度器，开始多任务调度
    vTaskStartScheduler();
    
    for(;;){
    
    }
}


// 链表功能测试
void ListFuncTest(){
    // 链表根节点
    List_t List_Test;

    // 节点
    ListItem_t List_Item1;
    ListItem_t List_Item2;
    ListItem_t List_Item3;
    
    // 链表根节点初始化
    vListInitialise(&List_Test);
    
    // 节点1初始化
    vListInitialseItem(&List_Item1);
    List_Item1.xItemValue = 1;
    
    vListInitialseItem(&List_Item2);
    List_Item2.xItemValue = 2;
    
    vListInitialseItem(&List_Item3);
    List_Item3.xItemValue = 3;
    
    // 将节点插入链表，升序排序
    vListInsert(&List_Test, &List_Item2);
    vListInsert(&List_Test, &List_Item1);
    vListInsert(&List_Test, &List_Item3);
}
