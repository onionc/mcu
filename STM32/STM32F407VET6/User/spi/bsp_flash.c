
#include "./spi/bsp_flash.h"

// 读取 FLASH ID
u32 SPI_FLASH_ReadId(void){
    u8 t[3]={0};
    u32 r;
    
    // 开始通讯：CS低
    SPI_FLASH_CS_LOW();
    
    // 发送 JEDEC 指令，为了读取ID
    SPI_FLASH_sendByte(W25X_JedecDeviceID);
    
    // 读取3个字节数据
    t[0]=SPI_FLASH_sendByte(Dummy_Byte); // 厂商
    t[1]=SPI_FLASH_sendByte(Dummy_Byte); // 存储器类型
    t[2]=SPI_FLASH_sendByte(Dummy_Byte); // 容量
    
    // 停止通讯：CS高
    SPI_FLASH_CS_HIGH();
    
    r = (t[0]<<16) | (t[1]<<8) | t[2];
    return r;

}

// FLASH 写使能/失能
void SPI_FLASH_WriteEnable(bool enable){
    // 开始通讯：CS低
    
    SPI_FLASH_CS_LOW();
    
    
    if(enable){
        // 发送写使能命令
        SPI_FLASH_sendByte(W25X_WriteEnable);
    }else{
        // 发送写失能命令
        SPI_FLASH_sendByte(W25X_WriteDisable);
    }
    
    // 停止通讯：CS高
    SPI_FLASH_CS_HIGH();
}

// 等待FLASH不忙碌（ BUSY 标志被置0）
ErrorStatus SPI_FLASH_WaitForWriteEnd(u8 times){
    __IO u32 timeout = SPIT_FLAG_TIMEOUT*times; // 0x1000 不够
    u8 flashStatus = 0;
    
    SPI_FLASH_CS_LOW();
    
    // 发送 读取状态寄存器 命令
    SPI_FLASH_sendByte(W25X_ReadStatusReg);
    
    do{
        flashStatus = SPI_FLASH_sendByte(Dummy_Byte);
        if(timeout-- == 0){
            SPI_FLASH_CS_HIGH();
            return ERROR; 
        }
    }while((flashStatus & 0x01) == SET);
    
    SPI_FLASH_CS_HIGH();
    return SUCCESS;
}

// 擦除FLASH，按扇区
ErrorStatus SPI_FLASH_SectorErase(u32 sectorAddr){
    // 写使能
    SPI_FLASH_WriteEnable(true);
    // 等待
    //if(SPI_FLASH_WaitForWriteEnd()==ERROR){
    //    return ERROR;
    //}
    
    // 开始通讯
    SPI_FLASH_CS_LOW();
    
    // 扇区擦除指令
    SPI_FLASH_sendByte(W25X_SectorErase);
    // 擦除地址高位
    SPI_FLASH_sendByte(sectorAddr>>16 & 0xFF);
    // 擦除地址中位
    SPI_FLASH_sendByte(sectorAddr>>8 & 0xFF);
    // 擦除地址低位
    SPI_FLASH_sendByte(sectorAddr & 0xFF);
    // 结束通讯
    SPI_FLASH_CS_HIGH();
    
    // 等待，擦除扇区时比较耗时
    if(SPI_FLASH_WaitForWriteEnd(20)==ERROR){
        return ERROR;
    }
    return SUCCESS;
}

// 
/**
 * 按页写入数据
 * @param pBuf 要写入数据指针
 * @param writeAddr 写入地址
 * @param writeLen 写入长度，单位字节
 * @return Status
 */
ErrorStatus SPI_FLASH_PageWrite(u8 *pBuf, u32 writeAddr, u16 writeLen){
    if(writeLen > W25X_FLASH_PER_PAGE_BYTES_MAX){
        return ERROR;
    }
    
    // 写使能
    SPI_FLASH_WriteEnable(true);
    
    SPI_FLASH_CS_LOW(); // 开始通讯
    SPI_FLASH_sendByte(W25X_PageProgram); // 写指令
    SPI_FLASH_sendByte(writeAddr>>16 & 0xFF); // 写地址
    SPI_FLASH_sendByte(writeAddr>>8 & 0xFF);
    SPI_FLASH_sendByte(writeAddr & 0xFF);
    
    // 写入数据
    while(writeLen--){
        SPI_FLASH_sendByte(*pBuf++); // 发送一字节数据
    }
    SPI_FLASH_CS_HIGH(); // 结束通讯

    // 等待
    if(SPI_FLASH_WaitForWriteEnd(1)==ERROR){
        return ERROR;
    }
    return SUCCESS;
}

// 不定量写入
ErrorStatus SPI_FLASH_BufWrite(u8 *pBuf, u32 writeAddr, u16 writeLen){
    u8 numOfPage, numOfOth, addr, count, temp;
    
    // addr为0则刚好是一页开头
    addr = writeAddr % FLASH_PER_PAGE_BYTES;
    // 差 count 个字节刚好一页
    count = FLASH_PER_PAGE_BYTES - addr; 
    // 要写多少页
    numOfPage = writeLen / FLASH_PER_PAGE_BYTES;
    // 还剩不满一页的多少字节
    numOfOth = writeLen % FLASH_PER_PAGE_BYTES;
    
    // 数据不按页对齐
    if(addr!=0){
        // 先用剩余字节count 写满当前页
        if(SPI_FLASH_PageWrite(pBuf, writeAddr, count)==ERROR) return ERROR;
        writeAddr += count;
        pBuf += count;
        
        // 重新计算页和剩余，后面数据是按页对齐的
        writeLen-=count;
        numOfPage = writeLen / FLASH_PER_PAGE_BYTES;
        numOfOth = writeLen % FLASH_PER_PAGE_BYTES;
    }
        
    // writeLen < 页大小，小于一页直接按页写入一次
    if(numOfPage==0){
        if(SPI_FLASH_PageWrite(pBuf, writeAddr, writeLen)==ERROR) return ERROR;
    }else{
        // 先写整页
        while(numOfPage--){
            if(SPI_FLASH_PageWrite(pBuf, writeAddr, FLASH_PER_PAGE_BYTES)==ERROR) return ERROR;
            writeAddr += FLASH_PER_PAGE_BYTES;
            pBuf += FLASH_PER_PAGE_BYTES;
        }
        
        // 写剩余部分
        if(numOfOth){
            if(SPI_FLASH_PageWrite(pBuf, writeAddr, numOfOth)==ERROR) return ERROR;
        }
    }
    return SUCCESS;
 
    
    /* 跟示例写完，发现可以优化, 见上面
    u8 numOfPage, numOfOth, addr, count, temp;
    
    // addr为0则刚好是一页开头
    addr = writeAddr % FLASH_PER_PAGE_BYTES;
    // 差 count 个字节刚好一页
    count = FLASH_PER_PAGE_BYTES - addr; 
    // 要写多少页
    numOfPage = writeLen / FLASH_PER_PAGE_BYTES;
    // 还剩不满一页的多少字节
    numOfOth = writeLen % FLASH_PER_PAGE_BYTES;
    
    if(addr == 0){
        // 按页对齐
        
        // writeLen < 页大小，小于一页直接按页写入一次
        if(numOfPage==0){
            SPI_FLASH_PageWrite(pBuf, writeAddr, writeLen);
        }else{
            // 先写整页
            while(numOfPage--){
                SPI_FLASH_PageWrite(pBuf, writeAddr, writeLen);
                writeAddr += FLASH_PER_PAGE_BYTES;
                pBuf += FLASH_PER_PAGE_BYTES;
            }
            
            // 写剩余部分
            SPI_FLASH_PageWrite(pBuf, writeAddr, numOfOth);
        }
    }else{ 
        // 地址不对齐
        
        // writeLen < 页大小
        if(numOfPage==0){
            
            // 要写入的数据长度 大于 当前页剩余的字节
            if(numOfOth > count){
                // 先用剩余字节count 写满当前页
                SPI_FLASH_PageWrite(pBuf, writeAddr, count);
                writeAddr += count;
                pBuf += count;
                
                // 再写剩余数据
                SPI_FLASH_PageWrite(pBuf, writeAddr, numOfOth - count);
            }else{
                // 当前页剩余字节可以写完数据
                SPI_FLASH_PageWrite(pBuf, writeAddr, writeLen);
            }
            
        }else{
            // 先写当前页剩余字节count
            SPI_FLASH_PageWrite(pBuf, writeAddr, count);
            writeAddr += count;
            pBuf += count;
            
            // 重新计算页和剩余，之后就可从新页开始写入
            writeLen-=count;
            numOfPage = writeLen / FLASH_PER_PAGE_BYTES;
            numOfOth = writeLen % FLASH_PER_PAGE_BYTES;
            // 写整页
            while(numOfPage--){
                SPI_FLASH_PageWrite(pBuf, writeAddr, writeLen);
                writeAddr += FLASH_PER_PAGE_BYTES;
                pBuf += FLASH_PER_PAGE_BYTES;
            }
            
            // 写剩余部分
            SPI_FLASH_PageWrite(pBuf, writeAddr, numOfOth);
        }
    
    }
    
    */
    
    
    
}
/**
 * 读取 FLASH 数据
 * @param pBuf 存储读出数据的指针
 * @param readAddr 读取地址
 * @param readLen 读取长度，单位字节
 * @return Status
 */
ErrorStatus SPI_FLASH_ReadData(u8 *pBuf, u32 readAddr, u16 readLen){
    SPI_FLASH_CS_LOW(); // 开始通讯
    SPI_FLASH_sendByte(W25X_ReadData); // 读指令
    SPI_FLASH_sendByte(readAddr>>16 & 0xFF); // 读地址
    SPI_FLASH_sendByte(readAddr>>8 & 0xFF);
    SPI_FLASH_sendByte(readAddr & 0xFF);
    
    // 写入数据
    while(readLen--){
        *pBuf++ = SPI_FLASH_sendByte(Dummy_Byte); // 发送一字节数据
    }
    SPI_FLASH_CS_HIGH(); // 结束通讯
}

// 唤醒 FLASH
void SPI_FLASH_WAKEUP(){
    SPI_FLASH_CS_LOW(); // 开始通讯
    SPI_FLASH_sendByte(W25X_ReleasePowerDown); // 上电命令
    SPI_FLASH_CS_HIGH(); // 结束通讯
}
