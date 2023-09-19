
#include "./spi/bsp_flash.h"

// ��ȡ FLASH ID
u32 SPI_FLASH_ReadId(void){
    u8 t[3]={0};
    u32 r;
    
    // ��ʼͨѶ��CS��
    SPI_FLASH_CS_LOW();
    
    // ���� JEDEC ָ�Ϊ�˶�ȡID
    SPI_FLASH_sendByte(W25X_JedecDeviceID);
    
    // ��ȡ3���ֽ�����
    t[0]=SPI_FLASH_sendByte(Dummy_Byte); // ����
    t[1]=SPI_FLASH_sendByte(Dummy_Byte); // �洢������
    t[2]=SPI_FLASH_sendByte(Dummy_Byte); // ����
    
    // ֹͣͨѶ��CS��
    SPI_FLASH_CS_HIGH();
    
    r = (t[0]<<16) | (t[1]<<8) | t[2];
    return r;

}

// FLASH дʹ��/ʧ��
void SPI_FLASH_WriteEnable(bool enable){
    // ��ʼͨѶ��CS��
    
    SPI_FLASH_CS_LOW();
    
    
    if(enable){
        // ����дʹ������
        SPI_FLASH_sendByte(W25X_WriteEnable);
    }else{
        // ����дʧ������
        SPI_FLASH_sendByte(W25X_WriteDisable);
    }
    
    // ֹͣͨѶ��CS��
    SPI_FLASH_CS_HIGH();
}

// �ȴ�FLASH��æµ�� BUSY ��־����0��
ErrorStatus SPI_FLASH_WaitForWriteEnd(u8 times){
    __IO u32 timeout = SPIT_FLAG_TIMEOUT*times; // 0x1000 ����
    u8 flashStatus = 0;
    
    SPI_FLASH_CS_LOW();
    
    // ���� ��ȡ״̬�Ĵ��� ����
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

// ����FLASH��������
ErrorStatus SPI_FLASH_SectorErase(u32 sectorAddr){
    // дʹ��
    SPI_FLASH_WriteEnable(true);
    // �ȴ�
    //if(SPI_FLASH_WaitForWriteEnd()==ERROR){
    //    return ERROR;
    //}
    
    // ��ʼͨѶ
    SPI_FLASH_CS_LOW();
    
    // ��������ָ��
    SPI_FLASH_sendByte(W25X_SectorErase);
    // ������ַ��λ
    SPI_FLASH_sendByte(sectorAddr>>16 & 0xFF);
    // ������ַ��λ
    SPI_FLASH_sendByte(sectorAddr>>8 & 0xFF);
    // ������ַ��λ
    SPI_FLASH_sendByte(sectorAddr & 0xFF);
    // ����ͨѶ
    SPI_FLASH_CS_HIGH();
    
    // �ȴ�����������ʱ�ȽϺ�ʱ
    if(SPI_FLASH_WaitForWriteEnd(20)==ERROR){
        return ERROR;
    }
    return SUCCESS;
}

// 
/**
 * ��ҳд������
 * @param pBuf Ҫд������ָ��
 * @param writeAddr д���ַ
 * @param writeLen д�볤�ȣ���λ�ֽ�
 * @return Status
 */
ErrorStatus SPI_FLASH_PageWrite(u8 *pBuf, u32 writeAddr, u16 writeLen){
    if(writeLen > W25X_FLASH_PER_PAGE_BYTES_MAX){
        return ERROR;
    }
    
    // дʹ��
    SPI_FLASH_WriteEnable(true);
    
    SPI_FLASH_CS_LOW(); // ��ʼͨѶ
    SPI_FLASH_sendByte(W25X_PageProgram); // дָ��
    SPI_FLASH_sendByte(writeAddr>>16 & 0xFF); // д��ַ
    SPI_FLASH_sendByte(writeAddr>>8 & 0xFF);
    SPI_FLASH_sendByte(writeAddr & 0xFF);
    
    // д������
    while(writeLen--){
        SPI_FLASH_sendByte(*pBuf++); // ����һ�ֽ�����
    }
    SPI_FLASH_CS_HIGH(); // ����ͨѶ

    // �ȴ�
    if(SPI_FLASH_WaitForWriteEnd(1)==ERROR){
        return ERROR;
    }
    return SUCCESS;
}

// ������д��
ErrorStatus SPI_FLASH_BufWrite(u8 *pBuf, u32 writeAddr, u16 writeLen){
    u8 numOfPage, numOfOth, addr, count, temp;
    
    // addrΪ0��պ���һҳ��ͷ
    addr = writeAddr % FLASH_PER_PAGE_BYTES;
    // �� count ���ֽڸպ�һҳ
    count = FLASH_PER_PAGE_BYTES - addr; 
    // Ҫд����ҳ
    numOfPage = writeLen / FLASH_PER_PAGE_BYTES;
    // ��ʣ����һҳ�Ķ����ֽ�
    numOfOth = writeLen % FLASH_PER_PAGE_BYTES;
    
    // ���ݲ���ҳ����
    if(addr!=0){
        // ����ʣ���ֽ�count д����ǰҳ
        if(SPI_FLASH_PageWrite(pBuf, writeAddr, count)==ERROR) return ERROR;
        writeAddr += count;
        pBuf += count;
        
        // ���¼���ҳ��ʣ�࣬���������ǰ�ҳ�����
        writeLen-=count;
        numOfPage = writeLen / FLASH_PER_PAGE_BYTES;
        numOfOth = writeLen % FLASH_PER_PAGE_BYTES;
    }
        
    // writeLen < ҳ��С��С��һҳֱ�Ӱ�ҳд��һ��
    if(numOfPage==0){
        if(SPI_FLASH_PageWrite(pBuf, writeAddr, writeLen)==ERROR) return ERROR;
    }else{
        // ��д��ҳ
        while(numOfPage--){
            if(SPI_FLASH_PageWrite(pBuf, writeAddr, FLASH_PER_PAGE_BYTES)==ERROR) return ERROR;
            writeAddr += FLASH_PER_PAGE_BYTES;
            pBuf += FLASH_PER_PAGE_BYTES;
        }
        
        // дʣ�ಿ��
        if(numOfOth){
            if(SPI_FLASH_PageWrite(pBuf, writeAddr, numOfOth)==ERROR) return ERROR;
        }
    }
    return SUCCESS;
 
    
    /* ��ʾ��д�꣬���ֿ����Ż�, ������
    u8 numOfPage, numOfOth, addr, count, temp;
    
    // addrΪ0��պ���һҳ��ͷ
    addr = writeAddr % FLASH_PER_PAGE_BYTES;
    // �� count ���ֽڸպ�һҳ
    count = FLASH_PER_PAGE_BYTES - addr; 
    // Ҫд����ҳ
    numOfPage = writeLen / FLASH_PER_PAGE_BYTES;
    // ��ʣ����һҳ�Ķ����ֽ�
    numOfOth = writeLen % FLASH_PER_PAGE_BYTES;
    
    if(addr == 0){
        // ��ҳ����
        
        // writeLen < ҳ��С��С��һҳֱ�Ӱ�ҳд��һ��
        if(numOfPage==0){
            SPI_FLASH_PageWrite(pBuf, writeAddr, writeLen);
        }else{
            // ��д��ҳ
            while(numOfPage--){
                SPI_FLASH_PageWrite(pBuf, writeAddr, writeLen);
                writeAddr += FLASH_PER_PAGE_BYTES;
                pBuf += FLASH_PER_PAGE_BYTES;
            }
            
            // дʣ�ಿ��
            SPI_FLASH_PageWrite(pBuf, writeAddr, numOfOth);
        }
    }else{ 
        // ��ַ������
        
        // writeLen < ҳ��С
        if(numOfPage==0){
            
            // Ҫд������ݳ��� ���� ��ǰҳʣ����ֽ�
            if(numOfOth > count){
                // ����ʣ���ֽ�count д����ǰҳ
                SPI_FLASH_PageWrite(pBuf, writeAddr, count);
                writeAddr += count;
                pBuf += count;
                
                // ��дʣ������
                SPI_FLASH_PageWrite(pBuf, writeAddr, numOfOth - count);
            }else{
                // ��ǰҳʣ���ֽڿ���д������
                SPI_FLASH_PageWrite(pBuf, writeAddr, writeLen);
            }
            
        }else{
            // ��д��ǰҳʣ���ֽ�count
            SPI_FLASH_PageWrite(pBuf, writeAddr, count);
            writeAddr += count;
            pBuf += count;
            
            // ���¼���ҳ��ʣ�֮࣬��Ϳɴ���ҳ��ʼд��
            writeLen-=count;
            numOfPage = writeLen / FLASH_PER_PAGE_BYTES;
            numOfOth = writeLen % FLASH_PER_PAGE_BYTES;
            // д��ҳ
            while(numOfPage--){
                SPI_FLASH_PageWrite(pBuf, writeAddr, writeLen);
                writeAddr += FLASH_PER_PAGE_BYTES;
                pBuf += FLASH_PER_PAGE_BYTES;
            }
            
            // дʣ�ಿ��
            SPI_FLASH_PageWrite(pBuf, writeAddr, numOfOth);
        }
    
    }
    
    */
    
    
    
}
/**
 * ��ȡ FLASH ����
 * @param pBuf �洢�������ݵ�ָ��
 * @param readAddr ��ȡ��ַ
 * @param readLen ��ȡ���ȣ���λ�ֽ�
 * @return Status
 */
ErrorStatus SPI_FLASH_ReadData(u8 *pBuf, u32 readAddr, u16 readLen){
    SPI_FLASH_CS_LOW(); // ��ʼͨѶ
    SPI_FLASH_sendByte(W25X_ReadData); // ��ָ��
    SPI_FLASH_sendByte(readAddr>>16 & 0xFF); // ����ַ
    SPI_FLASH_sendByte(readAddr>>8 & 0xFF);
    SPI_FLASH_sendByte(readAddr & 0xFF);
    
    // д������
    while(readLen--){
        *pBuf++ = SPI_FLASH_sendByte(Dummy_Byte); // ����һ�ֽ�����
    }
    SPI_FLASH_CS_HIGH(); // ����ͨѶ
}

// ���� FLASH
void SPI_FLASH_WAKEUP(){
    SPI_FLASH_CS_LOW(); // ��ʼͨѶ
    SPI_FLASH_sendByte(W25X_ReleasePowerDown); // �ϵ�����
    SPI_FLASH_CS_HIGH(); // ����ͨѶ
}
