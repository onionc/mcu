/* 片上外设基地址 */
#define ON_CHIP_REPIPH_BASE ((unsigned int)0x40000000)

/* 总线基地址 */
#define AHB1_BASE (ON_CHIP_REPIPH_BASE + 0x20000)

/* GPIO外设基地址 */
#define GPIOF_BASE (AHB1_BASE + 0x1400)

/* GPIO寄存器地址，转为指针 */
#define GPIOF_MODER     (*(unsigned int*)(GPIO_F_BASE + 0x00))
#define GPIOF_OTYPER    (*(unsigned int*)(GPIO_F_BASE + 0x04))
#define GPIOF_OSPEEDR   (*(unsigned int*)(GPIO_F_BASE + 0x08))
#define GPIOF_PUPDR     (*(unsigned int*)(GPIO_F_BASE + 0x0C))
#define GPIOF_IDR       (*(unsigned int*)(GPIO_F_BASE + 0x10))
#define GPIOF_ODR       (*(unsigned int*)(GPIO_F_BASE + 0x14))
#define GPIOF_BSRR      (*(unsigned int*)(GPIO_F_BASE + 0x18))
#define GPIOF_LCKR      (*(unsigned int*)(GPIO_F_BASE + 0x1C))
#define GPIOF_AFRL      (*(unsigned int*)(GPIO_F_BASE + 0x20))
#define GPIOF_AFRH      (*(unsigned int*)(GPIO_F_BASE + 0x24))
    
/* RCC 外设基地址 */
#define RCC_BASE        (AHB1_BASE + 0x3800)
/* RCC 的 AHB1时钟使能寄存器地址，指针 */
#define RCC_AHB1ENR     (*(unsigned int*)(RCC_BASE + 0x30))