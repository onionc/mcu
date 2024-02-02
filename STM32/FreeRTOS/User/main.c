#include "list.h"
#include "FreeRTOS.h"
#include "task.h"

uint32_t flag1, flag2;

// ������ƿ鶨��
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;





// �����ʱ
void delay(uint32_t count){
    for(; count!=0; count--);
}

// ����1
void Task1_Entry(void *p_arg){
    for(;;){
        flag1 = 1;
        delay(100);
        flag1 = 0;
        delay(100);
        
        // �ֶ��л�����
        taskYIELD();
        
    }
}

// ����2
void Task2_Entry(void *p_arg){
    for(;;){
        flag2 = 1;
        delay(100);
        flag2 = 0;
        delay(100);
        
        // �ֶ��л�����
        taskYIELD();
    }
}

int main(void){
    
    // ��ʼ�������б�
    prvInitialiseTaskLists();
    // ��������
    Task1_Handle = xTaskCreateStatic(   (TaskFunction_t) Task1_Entry,
                                        (char*) "Task1",
                                        (uint32_t) TASK1_STACK_SIZE,
                                        (void *) NULL,
                                        (StackType_t *) Task1Stack,
                                        (TCB_t *) &Task1TCB);
    // ��������ӵ������б�
    vListInsertEnd(&(pxReadyTasksLists[1]), &((&Task1TCB)->xStateListItem) );
    
    Task2_Handle = xTaskCreateStatic(   (TaskFunction_t) Task2_Entry,
                                        (char*) "Task2",
                                        (uint32_t) TASK2_STACK_SIZE,
                                        (void *) NULL,
                                        (StackType_t *) Task2Stack,
                                        (TCB_t *) &Task2TCB);
    vListInsertEnd(&(pxReadyTasksLists[2]), &((&Task2TCB)->xStateListItem) );
    
    // ��������������ʼ���������
    vTaskStartScheduler();
    
    for(;;){
    
    }
}


// �����ܲ���
void ListFuncTest(){
    // ������ڵ�
    List_t List_Test;

    // �ڵ�
    ListItem_t List_Item1;
    ListItem_t List_Item2;
    ListItem_t List_Item3;
    
    // ������ڵ��ʼ��
    vListInitialise(&List_Test);
    
    // �ڵ�1��ʼ��
    vListInitialseItem(&List_Item1);
    List_Item1.xItemValue = 1;
    
    vListInitialseItem(&List_Item2);
    List_Item2.xItemValue = 2;
    
    vListInitialseItem(&List_Item3);
    List_Item3.xItemValue = 3;
    
    // ���ڵ����������������
    vListInsert(&List_Test, &List_Item2);
    vListInsert(&List_Test, &List_Item1);
    vListInsert(&List_Test, &List_Item3);
}
