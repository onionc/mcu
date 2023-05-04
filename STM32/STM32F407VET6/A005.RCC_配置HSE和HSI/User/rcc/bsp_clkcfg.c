#include "./rcc/bsp_clkcfg.h"

/**
 * @brief 使用HSE设置系统时钟
 * 1、开启HSE ，并等待 HSE 稳定
 * 2、设置 AHB、APB2、APB1的预分频因子
 * 3、设置PLL的时钟来源
 *    设置VCO输入时钟 分频因子        m
 *    设置VCO输出时钟 倍频因子        n
 *    设置PLLCLK时钟分频因子          p
 *    设置OTG FS,SDIO,RNG时钟分频因子 q
 * 4、开启PLL，并等待PLL稳定
 * 5、把PLLCK切换为系统时钟SYSCLK
 * 6、读取时钟切换状态位，确保PLLCLK被选为系统时钟
 * 
 * @param m VCO输入时钟 分频因子 2~63
 * @param n VCO输出时钟 倍频因子 192~432
 * @param p PLLCLK时钟分频因子 2/4/6/8
 * @param q OTG FS,SDIO,RNG时钟分频因子 4~15
 * @return uint8_t SUCCESS/ERROR
 */
uint8_t HSE_SetSysClk(uint32_t m, uint32_t n, uint32_t p, uint32_t q){
    __IO uint32_t hseStatus = 0;

    // RCC外设复位
    RCC_DeInit();

    // 1、使能 HSE
    RCC_HSEConfig(RCC_HSE_ON);
    
    // 等待 HSE 启动稳定
    hseStatus = RCC_WaitForHSEStartUp();
    
    if(hseStatus == SUCCESS){
        // 调压器电压输出级别为1；工作时平衡性能和功耗
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        PWR->CR |= PWR_CR_VOS;
        
        // 2、设置分频因子
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    // hclk = sysclk / 1
        RCC_PCLK2Config(RCC_HCLK_Div2);     // pclk2 = hclk / 2
        RCC_PCLK1Config(RCC_HCLK_Div4);     // pclk1 = hclk / 4
        
        // 3、配置 主PLL 时钟来源，设置 M N P Q
        RCC_PLLConfig(RCC_PLLSource_HSE, m, n, p, q);
        
        // 4、使能 主PLL
        RCC_PLLCmd(ENABLE);
        
        // 等待 PLL 稳定
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        
        // 配置FLASH预取指,指令缓存,数据缓存和等待状态
        FLASH->ACR = FLASH_ACR_PRFTEN
                    |FLASH_ACR_ICEN
                    |FLASH_ACR_DCEN
                    |FLASH_ACR_LATENCY_5WS;
        
        // 5、选择 主PLL 为系统时钟源
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        
        // 6、确保 PLL 为系统时钟
        while(RCC_GetSYSCLKSource()!=0x08);
        
        return SUCCESS;
    }
    
    // HSE 启动出错
    return ERROR;
}

/**
 * @brief 使用HSI设置系统时钟
 * 1、开启HSE ，并等待 HSE 稳定
 * 2、设置 AHB、APB2、APB1的预分频因子
 * 3、设置PLL的时钟来源
 *    设置VCO输入时钟 分频因子        m
 *    设置VCO输出时钟 倍频因子        n
 *    设置PLLCLK时钟分频因子          p
 *    设置OTG FS,SDIO,RNG时钟分频因子 q
 * 4、开启PLL，并等待PLL稳定
 * 5、把PLLCK切换为系统时钟SYSCLK
 * 6、读取时钟切换状态位，确保PLLCLK被选为系统时钟
 * 
 * @param m VCO输入时钟 分频因子 2~63
 * @param n VCO输出时钟 倍频因子 192~432
 * @param p PLLCLK时钟分频因子 2/4/6/8
 * @param q OTG FS,SDIO,RNG时钟分频因子 4~15
 * @return uint8_t SUCCESS/ERROR
 */
uint8_t HSI_SetSysClk(uint32_t m, uint32_t n, uint32_t p, uint32_t q){
    __IO uint32_t startUpCounter=0, hsiStatus = 0;

    // RCC外设复位
    RCC_DeInit();

    // 1、使能 HSI, HSI=16M
    RCC_HSICmd(ENABLE);
    
    
    // 等待HSI启动稳定
    do {
        hsiStatus = RCC->CR & RCC_CR_HSERDY;
        startUpCounter++;
    } while ((hsiStatus==0)&&(startUpCounter
                            !=HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSIRDY) != RESET) {
        hsiStatus = (uint32_t)0x01;
    } else {
        hsiStatus = (uint32_t)0x00;
    }

    // HSI 启动成功
    if(hsiStatus == SUCCESS){
        // 调压器电压输出级别为1；工作时平衡性能和功耗
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        PWR->CR |= PWR_CR_VOS;
        
        // 2、设置分频因子
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    // hclk = sysclk / 1
        RCC_PCLK2Config(RCC_HCLK_Div2);     // pclk2 = hclk / 2
        RCC_PCLK1Config(RCC_HCLK_Div4);     // pclk1 = hclk / 4
        
        // 3、配置 主PLL 时钟来源，设置 M N P Q
        RCC_PLLConfig(RCC_PLLSource_HSI, m, n, p, q);
        
        // 4、使能 主PLL
        RCC_PLLCmd(ENABLE);
        
        // 等待 PLL 稳定
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        
        // 配置FLASH预取指,指令缓存,数据缓存和等待状态
        FLASH->ACR = FLASH_ACR_PRFTEN
                    |FLASH_ACR_ICEN
                    |FLASH_ACR_DCEN
                    |FLASH_ACR_LATENCY_5WS;
        
        // 5、选择 主PLL 为系统时钟源
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        
        // 6、确保 PLL 为系统时钟
        while(RCC_GetSYSCLKSource()!=0x08);
        
        return SUCCESS;
    }
    
    // HSE 启动出错
    return ERROR;
}
