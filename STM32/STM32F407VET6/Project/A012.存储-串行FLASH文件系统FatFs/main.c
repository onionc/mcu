
#include "stm32f4xx.h"

#include "./led/bsp_led.h"

#include "./usart/bsp_usart.h"
#include "./spi/bsp_flash.h"

#include "./ff.h"

// 变量定义
FATFS fs;           //文件系统对象
FIL file;           // 文件对象
FRESULT fRes;      // 文件操作结果
BYTE xBuf[FF_MAX_SS]={0}; // 操作数据buffer
BYTE rBuf[4000]={0}; // 读数据buffer
BYTE wBuf[] = "壬戌之秋，七月既望，苏子与客泛舟游于赤壁之下。清风徐来，水波不兴。举酒属客，诵明月之诗，歌窈窕之章。少焉，月出于东山之上，徘徊于斗牛之间。白露横江，水光接天。纵一苇之所如，凌万顷之茫然。浩浩乎如冯虚御风，而不知其所止；飘飘乎如遗世独立，羽化而登仙。(冯 通：凭)\
　　于是饮酒乐甚，扣舷而歌之。歌曰：“桂棹兮兰桨，击空明兮溯流光。渺渺兮予怀，望美人兮天一方。”客有吹洞箫者，倚歌而和之。其声呜呜然，如怨如慕，如泣如诉；余音袅袅，不绝如缕。舞幽壑之潜蛟，泣孤舟之嫠妇。\
　　苏子愀然，正襟危坐而问客曰：“何为其然也？”客曰：“‘月明星稀，乌鹊南飞’，此非曹孟德之诗乎？西望夏口，东望武昌，山川相缪，郁乎苍苍，此非孟德之困于周郎者乎？方其破荆州，下江陵，顺流而东也，舳舻千里，旌旗蔽空，酾酒临江，横槊赋诗，固一世之雄也，而今安在哉？况吾与子渔樵于江渚之上，侣鱼虾而友麋鹿，驾一叶之扁舟，举匏樽以相属。寄蜉蝣于天地，渺沧海之一粟。哀吾生之须臾，羡长江之无穷。挟飞仙以遨游，抱明月而长终。知不可乎骤得，托遗响于悲风。”\
　　苏子曰：“客亦知夫水与月乎？逝者如斯，而未尝往也；盈虚者如彼，而卒莫消长也。盖将自其变者而观之，则天地曾不能以一瞬；自其不变者而观之，则物与我皆无尽也，而又何羡乎！且夫天地之间，物各有主，苟非吾之所有，虽一毫而莫取。惟江上之清风，与山间之明月，耳得之而为声，目遇之而成色，取之无禁，用之不竭。是造物者之无尽藏也，而吾与子之所共适。”(共适 一作：共食)\
　　客喜而笑，洗盏更酌。肴核既尽，杯盘狼籍。相与枕藉乎舟中，不知东方之既白。";
const char *physicalNum = "1:";
u32 num; // 字节数


#define FORCE_FORMAT    0   // 强制格式化
#define WRITE_TEST      0   // 写数据测试
#define READ_TEST       0   // 读数据测试


FRESULT formatSpace(); // 格式化
FRESULT miscellaneous(); // 获取详情

int main(){
    u32 id;

    // led 初始化
    LED_GPIO_Config();
    LED1_OFF; // 1灯亮代表文件系统问题
    LED2_OFF;
    LED3_OFF;
    
    // 初始化usart
    Usart1_Cfg(9600);
    printf("串口初始化成功\r\n");
    
    // 在SPI FLASH挂载文件系统，会初始化设备
    fRes = f_mount(&fs, physicalNum, 1); // 文件对象，设备编号，立即重载
    
    /************* 格式化测试 **************/
    // 没有文件系统
    if(FORCE_FORMAT || fRes == FR_NO_FILESYSTEM){
        printf("没有文件系统\r\n");
        fRes=formatSpace();
    }
    
    if(fRes==FR_OK){
        printf("文件系统挂载成功\r\n");
    }else{
        printf("文件系统挂载失败 code=%d\r\n", fRes);
        LED1_ON;
    }

    printf("-----------------基本功能测试:-----------------\r\n");
#if WRITE_TEST==1
    /************* 写数据测试 **************/
    fRes = f_open(&file, "1:a.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if(fRes == FR_OK){
        printf("打开文件成功，准备写入数据\r\n");
        fRes = f_write(&file, wBuf, sizeof wBuf, &num);
        if(fRes == FR_OK){
            printf("文件写入成功，写入字节数：%u\r\n", num);
            
        }else{
            printf("文件写入失败\r\n");
        }
        // 关闭文件
        f_close(&file);
    }else{
        printf("打开文件失败\r\n");
        LED1_ON;
    }
#endif

#if READ_TEST==1
    /************* 读数据测试 **************/
    fRes = f_open(&file, "1:a.txt", FA_OPEN_EXISTING | FA_READ);
    if(fRes == FR_OK){
        printf("打开文件成功，准备读取数据\r\n");
        fRes = f_read(&file, rBuf, sizeof rBuf, &num);
        if(fRes == FR_OK){
            printf("文件读取成功，字节数：%u\r\n 内容：\r\n%s\r\n", num, rBuf);
        }else{
            printf("文件读写失败\r\n");
        }
        // 关闭文件
        f_close(&file);
    }else{
        printf("打开文件失败\r\n");
        LED1_ON;
    }
    
    // 取消挂载
    f_mount(NULL, physicalNum, 1);
#endif
    
    /************ 获取设备信息等常用功能测试 ***************/
    printf("-----------------常用功能测试-----------------\r\n");
    if((fRes=miscellaneous()) != FR_OK){
        printf("遇到错误 code=%d\r\n", fRes);
    }
    
    while(1){
        
        
    }
    
    return 0;
}

FRESULT formatSpace(){
    MKFS_PARM parm;
    printf("即将进行格式化……\r\n");
        
    // 格式化
    parm.fmt = FM_FAT | FM_SFD;
    parm.n_fat = 0; // 逻辑驱动器号
    parm.align = 512; // 扇区大小
    parm.n_root = 0;
    parm.au_size = 0;
    fRes = f_mkfs(physicalNum, &parm, xBuf, sizeof xBuf);
    
    if(fRes==FR_OK){
        printf("已格式化文件系统\r\n");
        // 重新挂载
        fRes = f_mount(NULL, physicalNum, 1);
        fRes = f_mount(&fs, physicalNum, 1);
    }
    return fRes;
}

FRESULT miscellaneous(){
    DIR dir;
    FATFS *pFs;
    u32 freeClust, freeSect, totalSect;
    
    printf("设备信息获取：\r\n");
    {
        // 获取设备信息和簇大小（等于1扇区）
        fRes = f_getfree(physicalNum, &freeClust, &pFs);
        if(fRes!=FR_OK) return fRes;
        // 计算总扇区个数和空扇区个数
        totalSect = (pFs->n_fatent - 2)* pFs->csize;
        freeSect = freeClust * pFs->csize;
        
        // 可用空间按扇区更新，4K变化才刷新一次。
        printf("设备总空间：%10lu KB, 可用空间 %10lu KB\r\n", totalSect*4, freeSect*4);
    }
    
    printf("文件定位功能：\r\n");
    if(1){
        fRes = f_open(&file, "1:b.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
        if(fRes!=FR_OK) return fRes;

        // 文件定位到末尾
        printf("file seek:%lu\r\n", f_size(&file));
        fRes = f_lseek(&file, f_size(&file));
        if(fRes!=FR_OK) return fRes;
        
        f_printf(&file, "\n追加一行, 当前文件大小：%lu\n 设备总空间：%10lu KB, 可用空间 %10lu KB\n", f_size(&file), totalSect*4, freeSect*4);
        
        // 定位到起始位置
        fRes = f_lseek(&file, 0);
        if(fRes!=FR_OK) return fRes;
        
        // 读取数据
        //fRes = f_read(&file, rBuf, f_size(&file), &num);
        //if(fRes!=FR_OK) return fRes;
        //printf("文件内容为：%s\r\n", rBuf);
        
        f_close(&file);
    }
    
    
    
    return FR_OK;
}