#include "./sdio/sdio_test.h"
#include "./usart/bsp_usart.h"

u8 buffer[BLOCK_SIZE]={3};
//u8 mulBuf[BLOCK_SIZE * NUM_OF_BLOCKS] = {0};

void SD_TEST(){
    SD_Error Status=SD_OK;

    // ��ʼ��
    if((Status = SD_Init())!=SD_OK){
        printf("err, status = %d\r\n", Status);
        return;
    }

    
    // ������д��ǰ�������Ǳ���ġ���������ʼ��ַ�ͽ�����ַ���ڶ�����������ָ�����ȣ�����պ���ָ��һ���鳤�ȶ���
    Status = SD_Erase(0x00, 0x00 + BLOCK_SIZE);
    
    // ������д��
    Status = SD_WriteBlock(buffer, 0x00, 512);
    Status = SD_WaitWriteOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
    
    // �������ȡ
    Status = SD_ReadBlock(buffer, 0x00, 512);
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
    
    // ��� д��Ͷ�ȡ��δ����
    /*
    Status = SD_WriteMultiBlocks(mulBuf, 0x00, BLOCK_SIZE, NUM_OF_BLOCKS);
    Status = SD_WaitWriteOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
    Status = SD_ReadMultiBlocks(mulBuf, 0x00, BLOCK_SIZE, NUM_OF_BLOCKS);
    Status = SD_WaitReadOperation();
    while(state=SD_GetStatus() != SD_TRANSFER_OK);
    */
    
    printf("status = %d\r\nSD card test completed.\r\n", Status);
}

