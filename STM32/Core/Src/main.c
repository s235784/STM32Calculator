/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "calculate.h"
#define ITEM_MAX 20
#define RXBUFFERSIZE  256
void modifyAction(char buffer[]);
void initItem();
char RxBuffer[RXBUFFERSIZE];
char item[ITEM_MAX + 2];
uint8_t aRxBuffer;
uint8_t Uart1_Rx_Cnt = 0;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

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
int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xFFFF);
	return ch;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
	initItem();
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);
  /* USER CODE END 2 */
	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		
    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
 
	if(Uart1_Rx_Cnt >= 255)  //????
	{
		Uart1_Rx_Cnt = 0;
		memset(RxBuffer,0x00,sizeof(RxBuffer));
		HAL_UART_Transmit(&huart1, (uint8_t *)"error", 10,0xFFFF);
	}
	else
	{
		RxBuffer[Uart1_Rx_Cnt++] = aRxBuffer;   //??????
	
		if((RxBuffer[Uart1_Rx_Cnt-1] == 0x0A)&&(RxBuffer[Uart1_Rx_Cnt-2] == 0x0D)) //?????
		{
			int i;
			char RxBuffer2[RXBUFFERSIZE];
			for (i = 0; i < Uart1_Rx_Cnt - 2; i++) {
				RxBuffer2[i] = RxBuffer[i];
			}
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
			modifyAction(RxBuffer2);
			//HAL_UART_Transmit(&huart1, (uint8_t *)&RxBuffer2, Uart1_Rx_Cnt - 2,0xFFFF); //??????????
      //while(HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX);//??UART????
			Uart1_Rx_Cnt = 0;
			memset(RxBuffer,0x00,sizeof(RxBuffer)); //????
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);
		}
	}
	
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //???????
}

void printItem() {
	printf("t1.txt=\"%s\"\xff\xff\xff", item);
}

void backItem() {
	if (item[1] != '\0') {
		int len = strlen(item);
		if (item[len - 1] == 'n') { // sin tan ln
			if (item[len - 2] == 'i' || item[len - 2] == 'a') {
				item[len - 1] = '\0';
				item[len - 2] = '\0';
				item[len - 3] = '\0';
			} else {
				item[len - 1] = '\0';
				item[len - 2] = '\0';
			}
		} else if (item[len - 1] == 's') { // cos
			item[len - 1] = '\0';
			item[len - 2] = '\0';
			item[len - 3] = '\0';
		} else if (item[len - 1] == 'g') { // lg
			item[len - 1] = '\0';
			item[len - 2] = '\0';
		} else {
			char* p;
			for (p = item; *p; p++){}
			*(--p) = '\0';
		}
	} else {
		strcpy(item, "0");
	}
	
	if (item[0] == '\0') item[0] = '0';
}

void overItemCount() {
	initItem();
	printf("t1.txt=\"Error Count\"\xff\xff\xff");
}

void errorInput() {
	initItem();
	printf("t1.txt=\"Error Input\"\xff\xff\xff");
}

int isSign(char ch) {
	switch (ch) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '^':
		case '.':
			return 1;
		default:
			return 0;
	}
}

int addSign(char *sign) {
	int itemLen = strlen(item);
	if (item[itemLen - 1] >= '0' && item[itemLen - 1] <= '9') {
		strcat(item, "*");
		strcat(item, sign);
		return 0;
	}
	
	if (item[itemLen - 1] == '.') return 2;
	
	strcat(item, sign);
	return 0;
}

int checkNum() {
	int len = strlen(item);
	return item[len - 1] >= '0' && item[len - 1] <= '9';
}

void addItem(char ch) {
	int flag = 0;
	int len = strlen(item);
	int restLen = ITEM_MAX - len;
	
	if (restLen == 0) {
		overItemCount();
		return;
	}
	
	if (item[len - 1] == '0' && !isSign(ch)) {
		char* p;
		for (p = item; *p; p++){}
		*(--p) = '\0';
	}
	
	if (ch == 's') {
			if (restLen < 4) flag = 1;
			else flag = addSign("sin");
	} else if (ch == 'c') {
			if (restLen < 4) flag = 1;
			else flag = addSign("cos");
	} else if (ch == 't') {
			if (restLen < 4) flag = 1;
			else flag = addSign("tan");
	} else if (ch == 'n') {
			if (restLen < 3) flag = 1;
			else flag = addSign("ln");
	} else if (ch == 'g') {
			if (restLen < 3) flag = 1;
			else flag = addSign("lg");
	} else if (ch == '(') {
			flag = addSign("(");
	} else if (ch == ')') {
			if (checkNum()) strcat(item, ")");
			else flag = 2;
	} else if (ch == '.') {
			if (checkNum()) strcat(item, ".");
			else flag = 2;
	} else if (ch == '+') {
			if (checkNum()) strcat(item, "+");
			else flag = 2;
	} else if (ch == '-') {
			if (checkNum()) strcat(item, "-");
			else flag = 2;
	} else if (ch == '*') {
			if (checkNum()) strcat(item, "*");
			else flag = 2;
	} else if (ch == '/') {
			if (checkNum()) strcat(item, "/");
			else flag = 2;
	} else if (ch == '^') {
			if (checkNum()) strcat(item, "^");
			else flag = 2;
	} else if (item[0] == '\0') {
			item[0] = ch;
	} else if (item[0] == '-' && item[1] == '0' && len == 2) {
			backItem();
			char strChar[2] = {ch};
			strncat(item, strChar, 1);
	} else {
			char strChar[2] = {ch};
			strncat(item, strChar, 1);
	}
	
	if (flag == 1) overItemCount();
	else if (flag == 2) errorInput();
	else printItem();
}

void initItem() {
	item[0] = '0';
	for (int i = 1; i < ITEM_MAX; i++) {
		item[i] = '\0';
	}
}

void clearPrint() {
	printf("t1.txt=\"0\"\xff\xff\xff");
}

void printResult() {
	int len = strlen(item);
	if ((item[len - 1] < '0' || item[len - 1] > '9') && item[len - 1] != ')') {
		printf("t1.txt=\"Input Error\"\xff\xff\xff");
		initItem();
		return;
	}
	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
	char str[ITEM_MAX];
	strcpy(str, item);
	startCalculate(str);
	initItem();
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
}

void clearAll() {
	initItem();
	clearPrint();
}

void modifyAction(char buffer[]) {
	if (buffer[0] == 0x01) {
		addItem('1');
	} else if (buffer[0] == 0x02) {
		addItem('2');
	} else if (buffer[0] == 0x03) {
		addItem('3');
	} else if (buffer[0] == 0x04) {
		addItem('4');
	} else if (buffer[0] == 0x05) {
		addItem('5');
	} else if (buffer[0] == 0x06) {
		addItem('6');
	} else if (buffer[0] == 0x07) {
		addItem('7');
	} else if (buffer[0] == 0x08) {
		addItem('8');
	} else if (buffer[0] == 0x09) {
		addItem('9');
	} else if (buffer[0] == 0x00) {
		addItem('0');
	} else if (buffer[0] == 0x0B) {
		addItem('.');
	} else if (buffer[0] == 0x0C) {
		addItem('+');
	} else if (buffer[0] == 0x0D) {
		addItem('-');
	} else if (buffer[0] == 0x0E) {
		addItem('*');
	} else if (buffer[0] == 0x0F) {
		addItem('/');
	} else if (buffer[0] == 0x10) {
		addItem('(');
	} else if (buffer[0] == 0x11) {
		addItem(')');
	} else if (buffer[0] == 0x12) {
		addItem('^');
	} else if (buffer[0] == 0x13) {
		addItem('s');
	} else if (buffer[0] == 0x14) {
		addItem('c');
	} else if (buffer[0] == 0x15) {
		addItem('t');
	} else if (buffer[0] == 0x16) {
		addItem('n');
	} else if (buffer[0] == 0x17) {
		addItem('g');
	} else if (buffer[0] == 0x1F) {
		printResult();
	} else if (buffer[0] == 0x20) {
		backItem();
		printItem();
	} else if (buffer[0] == 0x21) {
		clearAll();
	} else if (buffer[0] == 0x63) {
		printItem();
	}
}

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
