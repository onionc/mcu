/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TCAN4550.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/*
 * Configure the TCAN4550
 */ int r=0;
void
Init_CAN(void)
{
    TCAN4x5x_Device_ClearSPIERR();                              // Clear any SPI ERR flags that might be set as a result of our pin mux changing during MCU startup

    /* Step one attempt to clear all interrupts */
	TCAN4x5x_Device_Interrupt_Enable dev_ie = {0};				// Initialize to 0 to all bits are set to 0.
	r=TCAN4x5x_Device_ConfigureInterruptEnable(&dev_ie);	        // Disable all non-MCAN related interrupts for simplicity
    printf("[2-%d]", r);
	TCAN4x5x_Device_Interrupts dev_ir = {0};					// Setup a new MCAN IR object for easy interrupt checking
	TCAN4x5x_Device_ReadInterrupts(&dev_ir);					// Request that the struct be updated with current DEVICE (not MCAN) interrupt values

	if (dev_ir.PWRON)                                           // If the Power On interrupt flag is set
		TCAN4x5x_Device_ClearInterrupts(&dev_ir);               // Clear it because if it's not cleared within ~4 minutes, it goes to sleep

	/* Configure the CAN bus speeds */
	TCAN4x5x_MCAN_Nominal_Timing_Simple TCANNomTiming = {0};	// 500k arbitration with a 40 MHz crystal ((40E6 / 2) / (32 + 8) = 500E3)
	TCANNomTiming.NominalBitRatePrescaler = 2;
	TCANNomTiming.NominalTqBeforeSamplePoint = 32;
	TCANNomTiming.NominalTqAfterSamplePoint = 8;

	TCAN4x5x_MCAN_Data_Timing_Simple TCANDataTiming = {0};		// 2 Mbps CAN FD with a 40 MHz crystal (40E6 / (15 + 5) = 2E6)
	TCANDataTiming.DataBitRatePrescaler = 2;
	TCANDataTiming.DataTqBeforeSamplePoint = 32;
	TCANDataTiming.DataTqAfterSamplePoint = 8;

	/* Configure the MCAN core settings */
	TCAN4x5x_MCAN_CCCR_Config cccrConfig = {0};					// Remember to initialize to 0, or you'll get random garbage!
	cccrConfig.FDOE = 1;										// CAN FD mode enable
	cccrConfig.BRSE = 1;										// CAN FD Bit rate switch enable

	/* Configure the default CAN packet filtering settings */
	TCAN4x5x_MCAN_Global_Filter_Configuration gfc = {0};
	gfc.RRFE = 1;                                               // Reject remote frames (TCAN4x5x doesn't support this)
	gfc.RRFS = 1;                                               // Reject remote frames (TCAN4x5x doesn't support this)
	gfc.ANFE = TCAN4x5x_GFC_ACCEPT_INTO_RXFIFO0;                // Default behavior if incoming message doesn't match a filter is to accept into RXFIO0 for extended ID messages (29 bit IDs)
	gfc.ANFS = TCAN4x5x_GFC_ACCEPT_INTO_RXFIFO0;                // Default behavior if incoming message doesn't match a filter is to accept into RXFIO0 for standard ID messages (11 bit IDs)

	/* ************************************************************************
	 * In the next configuration block, we will set the MCAN core up to have:
	 *   - 1 SID filter element
	 *   - 1 XID Filter element
	 *   - 5 RX FIFO 0 elements
	 *   - RX FIFO 0 supports data payloads up to 64 bytes
	 *   - RX FIFO 1 and RX Buffer will not have any elements, but we still set their data payload sizes, even though it's not required
	 *   - No TX Event FIFOs
	 *   - 2 Transmit buffers supporting up to 64 bytes of data payload
	 */
	TCAN4x5x_MRAM_Config MRAMConfiguration = {0};
    
	MRAMConfiguration.SIDNumElements = 1;						// Standard ID number of elements, you MUST have a filter written to MRAM for each element defined
	MRAMConfiguration.XIDNumElements = 1;						// Extended ID number of elements, you MUST have a filter written to MRAM for each element defined
	MRAMConfiguration.Rx0NumElements = 5;						// RX0 Number of elements
	MRAMConfiguration.Rx0ElementSize = MRAM_64_Byte_Data;		// RX0 data payload size
	MRAMConfiguration.Rx1NumElements = 0;						// RX1 number of elements
	MRAMConfiguration.Rx1ElementSize = MRAM_64_Byte_Data;		// RX1 data payload size
	MRAMConfiguration.RxBufNumElements = 0;						// RX buffer number of elements
	MRAMConfiguration.RxBufElementSize = MRAM_64_Byte_Data;		// RX buffer data payload size
	MRAMConfiguration.TxEventFIFONumElements = 0;				// TX Event FIFO number of elements
	MRAMConfiguration.TxBufferNumElements = 2;					// TX buffer number of elements
	MRAMConfiguration.TxBufferElementSize = MRAM_64_Byte_Data;	// TX buffer data payload size


	/* Configure the MCAN core with the settings above, the changes in this block are write protected registers,      *
	 * so it makes the most sense to do them all at once, so we only unlock and lock once                             */

	r=TCAN4x5x_MCAN_EnableProtectedRegisters();					// Start by making protected registers accessible
	printf("[5-%d]", r);
    r=TCAN4x5x_MCAN_ConfigureCCCRRegister(&cccrConfig);			// Enable FD mode and Bit rate switching
    printf("[6-%d]", r);
	r=TCAN4x5x_MCAN_ConfigureGlobalFilter(&gfc);                  // Configure the global filter configuration (Default CAN message behavior)
	printf("[7-%d]", r);
    r=TCAN4x5x_MCAN_ConfigureNominalTiming_Simple(&TCANNomTiming);// Setup nominal/arbitration bit timing
	printf("[8-%d]", r);
    r=TCAN4x5x_MCAN_ConfigureDataTiming_Simple(&TCANDataTiming);	// Setup CAN FD timing
	printf("[9-%d]", r);
    TCAN4x5x_MRAM_Clear();										// Clear all of MRAM (Writes 0's to all of it)

    r=TCAN4x5x_MRAM_Configure(&MRAMConfiguration);				// Set up the applicable registers related to MRAM configuration
    printf("[10-%d]", r);
	r=TCAN4x5x_MCAN_DisableProtectedRegisters();					// Disable protected write and take device out of INIT mode
    printf("[11-%d]", r);

	/* Set the interrupts we want to enable for MCAN */
	TCAN4x5x_MCAN_Interrupt_Enable mcan_ie = {0};				// Remember to initialize to 0, or you'll get random garbage!
	mcan_ie.RF0NE = 1;											// RX FIFO 0 new message interrupt enable

	TCAN4x5x_MCAN_ConfigureInterruptEnable(&mcan_ie);			// Enable the appropriate registers


	/* Setup filters, this filter will mark any message with ID 0x055 as a priority message */
	TCAN4x5x_MCAN_SID_Filter SID_ID = {0};
	SID_ID.SFT = TCAN4x5x_SID_SFT_CLASSIC;						// SFT: Standard filter type. Configured as a classic filter
	SID_ID.SFEC = TCAN4x5x_SID_SFEC_PRIORITYSTORERX0;			// Standard filter element configuration, store it in RX fifo 0 as a priority message
	SID_ID.SFID1 = 0x055;										// SFID1 (Classic mode Filter)
	SID_ID.SFID2 = 0x7FF;										// SFID2 (Classic mode Mask)
	TCAN4x5x_MCAN_WriteSIDFilter(0, &SID_ID);					// Write to the MRAM


	/* Store ID 0x12345678 as a priority message */
	TCAN4x5x_MCAN_XID_Filter XID_ID = {0};
	XID_ID.EFT = TCAN4x5x_XID_EFT_CLASSIC;                      // EFT
	XID_ID.EFEC = TCAN4x5x_XID_EFEC_PRIORITYSTORERX0;           // EFEC
	XID_ID.EFID1 = 0x12345678;                                  // EFID1 (Classic mode filter)
	XID_ID.EFID2 = 0x1FFFFFFF;                                  // EFID2 (Classic mode mask)
	TCAN4x5x_MCAN_WriteXIDFilter(0, &XID_ID);                   // Write to the MRAM

	/* Configure the TCAN4550 Non-CAN-related functions */
	TCAN4x5x_DEV_CONFIG devConfig = {0};                        // Remember to initialize to 0, or you'll get random garbage!
	devConfig.SWE_DIS = 0;                                      // Keep Sleep Wake Error Enabled (it's a disable bit, not an enable)
	devConfig.DEVICE_RESET = 0;                                 // Not requesting a software reset
	devConfig.WD_EN = 0;                                        // Watchdog disabled
	devConfig.nWKRQ_CONFIG = 0;                                 // Mirror INH function (default)
	devConfig.INH_DIS = 0;                                      // INH enabled (default)
	devConfig.GPIO1_GPO_CONFIG = TCAN4x5x_DEV_CONFIG_GPO1_MCAN_INT1;    // MCAN nINT 1 (default)
	devConfig.FAIL_SAFE_EN = 0;                                 // Failsafe disabled (default)
	devConfig.GPIO1_CONFIG = TCAN4x5x_DEV_CONFIG_GPIO1_CONFIG_GPO;      // GPIO set as GPO (Default)
	devConfig.WD_ACTION = TCAN4x5x_DEV_CONFIG_WDT_ACTION_nINT;  // Watchdog set an interrupt (default)
	devConfig.WD_BIT_RESET = 0;                                 // Don't reset the watchdog
	devConfig.nWKRQ_VOLTAGE = 0;                                // Set nWKRQ to internal voltage rail (default)
	devConfig.GPO2_CONFIG = TCAN4x5x_DEV_CONFIG_GPO2_MCAN_INT0; // GPO2 中断开启，中断中处理接收数据
	devConfig.CLK_REF = 1;                                      // Input crystal is a 40 MHz crystal (default)
	devConfig.WAKE_CONFIG = TCAN4x5x_DEV_CONFIG_WAKE_BOTH_EDGES;// Wake pin can be triggered by either edge (default)
	TCAN4x5x_Device_Configure(&devConfig);                      // Configure the device with the above configuration

	TCAN4x5x_Device_SetMode(TCAN4x5x_DEVICE_MODE_NORMAL);       // Set to normal mode, since configuration is done. This line turns on the transceiver

	TCAN4x5x_MCAN_ClearInterruptsAll();                         // Resets all MCAN interrupts (does NOT include any SPIERR interrupts)
}


volatile uint8_t TCAN_Int_Cnt = 0;					// A variable used to keep track of interrupts the MCAN Interrupt pin

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch(GPIO_Pin){
      case CAN_INT_Pin:
         
        TCAN_Int_Cnt++;
        break;
  }
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  int i,j;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
  
    // can 初始化
    Init_CAN();
    printf("can init.");

    // 发送两帧数据，一个是普通CAN，一个是CANFD
    TCAN4x5x_MCAN_TX_Header header = {0};			// Remember to initialize to 0, or you'll get random garbage!
	uint8_t data[4] = {0x55, 0x66, 0x77, 0x88};		// Define the data payload
	header.DLC = MCAN_DLC_4B;						// Set the DLC to be equal to or less than the data payload (it is ok to pass a 64 byte data array into the WriteTXFIFO function if your DLC is 8 bytes, only the first 8 bytes will be read)
	header.ID = 0x144;								// Set the ID
	header.FDF = 1;									// CAN FD frame enabled
	header.BRS = 1;									// Bit rate switch enabled
	header.EFC = 0;
	header.MM  = 0;
	header.RTR = 0;
	header.XTD = 0;									// We are not using an extended ID in this example
	header.ESI = 0;									// Error state indicator
    


	TCAN4x5x_MCAN_WriteTXBuffer(0, &header, data);	// This function actually writes the header and data payload to the TCAN's MRAM in the specified TX queue number. It returns the bit necessary to write to TXBAR,
													// but does not necessarily require you to use it. In this example, we won't, so that we can send the data queued up at a later point.

    data[0] = 0x11;
	data[1] = 0x22;
	data[2] = 0x33;
	data[3] = 0x44;									// Define the data payload

	header.DLC = MCAN_DLC_4B;						// Set the DLC to be equal to or less than the data payload (it is ok to pass a 64 byte data array into the WriteTXFIFO function if your DLC is 8 bytes, only the first 8 bytes will be read)
	header.ID = 0x123;								// Set the ID
	header.FDF = 0;									// CAN FD frame enabled
	header.BRS = 1;									// Bit rate switch enabled
	header.EFC = 0;
	header.MM  = 0;
	header.RTR = 0;
	header.XTD = 0;									// We are not using an extended ID in this example
	header.ESI = 0;									// Error state indicator

	TCAN4x5x_MCAN_WriteTXBuffer(1, &header, data);	// This line writes the data and header to TX FIFO 1
	TCAN4x5x_MCAN_TransmitBufferContents(1);		// Request that TX Buffer 1 be transmitted


	r=TCAN4x5x_MCAN_TransmitBufferContents(0);		// Now we can send the TX FIFO element 0 data that we had queued up earlier but didn't send.
    printf("[%d.]",r);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    //HAL_Delay(500);
    //HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
      
      // 接收数据
      if (TCAN_Int_Cnt > 0 )
		{
			TCAN_Int_Cnt--;
			TCAN4x5x_Device_Interrupts dev_ir = {0};            // Define a new Device IR object for device (non-CAN) interrupt checking
			TCAN4x5x_MCAN_Interrupts mcan_ir = {0};				// Setup a new MCAN IR object for easy interrupt checking
			TCAN4x5x_Device_ReadInterrupts(&dev_ir);            // Read the device interrupt register
			TCAN4x5x_MCAN_ReadInterrupts(&mcan_ir);		        // Read the interrupt register

			if (dev_ir.SPIERR)                                  // If the SPIERR flag is set
			    TCAN4x5x_Device_ClearSPIERR();                  // Clear the SPIERR flag

			if (mcan_ir.RF0N)									// If a new message in RX FIFO 0
			{
				TCAN4x5x_MCAN_RX_Header MsgHeader = {0};		// Initialize to 0 or you'll get garbage
				uint8_t numBytes = 0;                           // Used since the ReadNextFIFO function will return how many bytes of data were read
				uint8_t dataPayload[64] = {0};                  // Used to store the received data

				TCAN4x5x_MCAN_ClearInterrupts(&mcan_ir);	    // Clear any of the interrupt bits that are set.

				numBytes = TCAN4x5x_MCAN_ReadNextFIFO( RXFIFO0, &MsgHeader, dataPayload);	// This will read the next element in the RX FIFO 0

				// numBytes will have the number of bytes it transfered in it. Or you can decode the DLC value in MsgHeader.DLC
				// The data is now in dataPayload[], and message specific information is in the MsgHeader struct.
                
				if (MsgHeader.ID >0)		// Example of how you can do an action based off a received address
				{
                    printf("r...0x%x, len=%d\n", MsgHeader.ID, numBytes);
                    if(numBytes>0){
                        for(i=0; i<numBytes; i++){
                            printf("%02x ", dataPayload[i]);
                        }
                        printf("\n");
                    }
				}
			}
		}
    
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 128;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV16;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
