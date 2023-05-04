#include "./rcc/bsp_clkcfg.h"

/**
 * @brief ʹ��HSE����ϵͳʱ��
 * 1������HSE �����ȴ� HSE �ȶ�
 * 2������ AHB��APB2��APB1��Ԥ��Ƶ����
 * 3������PLL��ʱ����Դ
 *    ����VCO����ʱ�� ��Ƶ����        m
 *    ����VCO���ʱ�� ��Ƶ����        n
 *    ����PLLCLKʱ�ӷ�Ƶ����          p
 *    ����OTG FS,SDIO,RNGʱ�ӷ�Ƶ���� q
 * 4������PLL�����ȴ�PLL�ȶ�
 * 5����PLLCK�л�Ϊϵͳʱ��SYSCLK
 * 6����ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
 * 
 * @param m VCO����ʱ�� ��Ƶ���� 2~63
 * @param n VCO���ʱ�� ��Ƶ���� 192~432
 * @param p PLLCLKʱ�ӷ�Ƶ���� 2/4/6/8
 * @param q OTG FS,SDIO,RNGʱ�ӷ�Ƶ���� 4~15
 * @return uint8_t SUCCESS/ERROR
 */
uint8_t HSE_SetSysClk(uint32_t m, uint32_t n, uint32_t p, uint32_t q){
    __IO uint32_t hseStatus = 0;

    // RCC���踴λ
    RCC_DeInit();

    // 1��ʹ�� HSE
    RCC_HSEConfig(RCC_HSE_ON);
    
    // �ȴ� HSE �����ȶ�
    hseStatus = RCC_WaitForHSEStartUp();
    
    if(hseStatus == SUCCESS){
        // ��ѹ����ѹ�������Ϊ1������ʱƽ�����ܺ͹���
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        PWR->CR |= PWR_CR_VOS;
        
        // 2�����÷�Ƶ����
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    // hclk = sysclk / 1
        RCC_PCLK2Config(RCC_HCLK_Div2);     // pclk2 = hclk / 2
        RCC_PCLK1Config(RCC_HCLK_Div4);     // pclk1 = hclk / 4
        
        // 3������ ��PLL ʱ����Դ������ M N P Q
        RCC_PLLConfig(RCC_PLLSource_HSE, m, n, p, q);
        
        // 4��ʹ�� ��PLL
        RCC_PLLCmd(ENABLE);
        
        // �ȴ� PLL �ȶ�
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        
        // ����FLASHԤȡָ,ָ���,���ݻ���͵ȴ�״̬
        FLASH->ACR = FLASH_ACR_PRFTEN
                    |FLASH_ACR_ICEN
                    |FLASH_ACR_DCEN
                    |FLASH_ACR_LATENCY_5WS;
        
        // 5��ѡ�� ��PLL Ϊϵͳʱ��Դ
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        
        // 6��ȷ�� PLL Ϊϵͳʱ��
        while(RCC_GetSYSCLKSource()!=0x08);
        
        return SUCCESS;
    }
    
    // HSE ��������
    return ERROR;
}

/**
 * @brief ʹ��HSI����ϵͳʱ��
 * 1������HSE �����ȴ� HSE �ȶ�
 * 2������ AHB��APB2��APB1��Ԥ��Ƶ����
 * 3������PLL��ʱ����Դ
 *    ����VCO����ʱ�� ��Ƶ����        m
 *    ����VCO���ʱ�� ��Ƶ����        n
 *    ����PLLCLKʱ�ӷ�Ƶ����          p
 *    ����OTG FS,SDIO,RNGʱ�ӷ�Ƶ���� q
 * 4������PLL�����ȴ�PLL�ȶ�
 * 5����PLLCK�л�Ϊϵͳʱ��SYSCLK
 * 6����ȡʱ���л�״̬λ��ȷ��PLLCLK��ѡΪϵͳʱ��
 * 
 * @param m VCO����ʱ�� ��Ƶ���� 2~63
 * @param n VCO���ʱ�� ��Ƶ���� 192~432
 * @param p PLLCLKʱ�ӷ�Ƶ���� 2/4/6/8
 * @param q OTG FS,SDIO,RNGʱ�ӷ�Ƶ���� 4~15
 * @return uint8_t SUCCESS/ERROR
 */
uint8_t HSI_SetSysClk(uint32_t m, uint32_t n, uint32_t p, uint32_t q){
    __IO uint32_t startUpCounter=0, hsiStatus = 0;

    // RCC���踴λ
    RCC_DeInit();

    // 1��ʹ�� HSI, HSI=16M
    RCC_HSICmd(ENABLE);
    
    
    // �ȴ�HSI�����ȶ�
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

    // HSI �����ɹ�
    if(hsiStatus == SUCCESS){
        // ��ѹ����ѹ�������Ϊ1������ʱƽ�����ܺ͹���
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        PWR->CR |= PWR_CR_VOS;
        
        // 2�����÷�Ƶ����
        RCC_HCLKConfig(RCC_SYSCLK_Div1);    // hclk = sysclk / 1
        RCC_PCLK2Config(RCC_HCLK_Div2);     // pclk2 = hclk / 2
        RCC_PCLK1Config(RCC_HCLK_Div4);     // pclk1 = hclk / 4
        
        // 3������ ��PLL ʱ����Դ������ M N P Q
        RCC_PLLConfig(RCC_PLLSource_HSI, m, n, p, q);
        
        // 4��ʹ�� ��PLL
        RCC_PLLCmd(ENABLE);
        
        // �ȴ� PLL �ȶ�
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        
        // ����FLASHԤȡָ,ָ���,���ݻ���͵ȴ�״̬
        FLASH->ACR = FLASH_ACR_PRFTEN
                    |FLASH_ACR_ICEN
                    |FLASH_ACR_DCEN
                    |FLASH_ACR_LATENCY_5WS;
        
        // 5��ѡ�� ��PLL Ϊϵͳʱ��Դ
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        
        // 6��ȷ�� PLL Ϊϵͳʱ��
        while(RCC_GetSYSCLKSource()!=0x08);
        
        return SUCCESS;
    }
    
    // HSE ��������
    return ERROR;
}
