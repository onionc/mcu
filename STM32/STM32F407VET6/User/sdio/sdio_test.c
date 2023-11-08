#include "./sdio/sdio_test.h"
#include "./usart/bsp_usart.h"

u8 buffer[BLOCK_SIZE]={3};
//u8 mulBuf[BLOCK_SIZE * NUM_OF_BLOCKS] = {0};

void SD_TEST(){
    SD_Error Status=SD_OK;

    // 初始化
    if((Status = SD_Init())!=SD_OK){
        printf("err, status = %d\r\n", Status);
        return;
    }

    
    // 擦除，写入前擦除不是必须的。参数是起始地址和结束地址，第二个参数不是指定长度，这里刚好是指定一个块长度而已
    Status = SD_Erase(0x00, 0x00 + BLOCK_SIZE);
    
    // 单独块写入
    Status = SD_WriteBlock(buffer, 0x00, 512);
    Status = SD_WaitWriteOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
    
    // 单独块读取
    Status = SD_ReadBlock(buffer, 0x00, 512);
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
    
    // 多块 写入和读取，未测试
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

