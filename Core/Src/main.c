/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define line(x) 8+8*(x)
#define MAX_LENGTH 20

//write in the middle
#define MIDDLE(x) (SSD1306_WIDTH/2)-(strlen(x)*6/2)

#define MIDDLE_TITLE(x) (SSD1306_WIDTH/2)-(strlen(x)*7/2)

//write from the end
#define END(x) SSD1306_WIDTH-(strlen(x)*6)

#define STATES \
		ENTITY(Idle,Idle_Function)\
		ENTITY(Start_Screen,Start_Screen_Function)\
		ENTITY(Main_Screen,Main_Screen_Function)\
		ENTITY(Coordinates_Screen,Coordinates_Screen_Function)\
		ENTITY(Skills_Screen,Skills_Screen_Function)\
		ENTITY(Languaes_Screen,Languaes_Screen_Function)\
		ENTITY(Projects_Screen,Projects_Screen_Function)\
		ENTITY(Diplomas_Screen,Diplomas_Screen_Function)\
		ENTITY(ProfessionalExperience_Screen,ProfessionalExperience_Screen_Function)

//write 2 words
#define PRINT2(l,a,b)\
		ssd1306_SetCursor(5,line(l));\
		ret=ssd1306_WriteString(a, Font_6x8, White);\
		ssd1306_SetCursor(END(b)-5,line(l));\
		ret=ssd1306_WriteString(b, Font_6x8, White);

//write 3 words
#define PRINT3(l,a,b,c)\
		ssd1306_SetCursor(0,line(l));\
		ret=ssd1306_WriteString(a, Font_6x8, White);\
		ssd1306_SetCursor(MIDDLE(b),line(l));\
		ret=ssd1306_WriteString(b, Font_6x8, White);\
		ssd1306_SetCursor(END(c),line(l));\
		ret=ssd1306_WriteString(c, Font_6x8, White);

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
char str[]="";
char ret;

typedef enum{
#define ENTITY(x,y) x,
	STATES
#undef ENTITY
}States;

States Current_State=Idle;
uint8_t Number_of_states=0;
int8_t Choice=1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void Idle_Function(void);
void Start_Screen_Function(void);
void Main_Screen_Function(void);
void Coordinates_Screen_Function(void);
void Skills_Screen_Function(void);
void Languaes_Screen_Function(void);
void Projects_Screen_Function(void);
void Diplomas_Screen_Function(void);
void ProfessionalExperience_Screen_Function(void);

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
	MX_I2C1_Init();
	/* USER CODE BEGIN 2 */
	ssd1306_Init();

#define ENTITY(x,y)  Number_of_states++;
	STATES
#undef ENTITY

	switch (Current_State){
#define ENTITY(x,y) case(x): { \
		y(); \
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==1) \
		Current_State=(Current_State+1)%Number_of_states; \
}\
break;
	STATES
#undef ENTITY
	default : {
		ssd1306_Fill(White);
		ssd1306_UpdateScreen();
	}
	}
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{switch (Current_State){
#define ENTITY(x,y) case(x): { \
		y(); \
}\
break;
	STATES
#undef ENTITY
	default : {
		ssd1306_Fill(White);
		ssd1306_UpdateScreen();
	}
	}
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
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 20;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV8;
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
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x00909BEB;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pins : PA4 PA5 PA6 PA7 */
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */
void Idle_Function(void)
{
	ssd1306_Fill(White);
	ssd1306_UpdateScreen();
}

void Start_Screen_Function(void)
{
	ssd1306_Fill(Black);
	//------------Title---------------//
	ssd1306_SetCursor(MIDDLE_TITLE("Med Taher Bazzazi"),0);
	ret=ssd1306_WriteString("Med Taher Bazzazi", Font_7x10, White);
	//------------body---------------//
	ssd1306_SetCursor(MIDDLE("Bonjouur"),line(1));
	ret=ssd1306_WriteString("Bonjouur", Font_6x8, White);
	ssd1306_SetCursor(MIDDLE("C'est mon CV"),line(2));
	ret=ssd1306_WriteString("C'est mon CV", Font_6x8, White);
	ssd1306_SetCursor(MIDDLE("j'espere que"),line(3));
	ret=ssd1306_WriteString("j'espere que", Font_6x8, White);
	ssd1306_SetCursor(MIDDLE("vous l'aimez"),line(4));
	ret=ssd1306_WriteString("vous l'aimez", Font_6x8, White);
	ssd1306_SetCursor(END(":)"),line(5));
	ret=ssd1306_WriteString(":)", Font_6x8, White);
	ssd1306_UpdateScreen();
}

void Main_Screen_Function(void){
	ssd1306_Fill(Black);
	//------------Title---------------//
	ssd1306_SetCursor(MIDDLE_TITLE("Menu principale"),0);
	ret=ssd1306_WriteString("Menu principale", Font_7x10, White);
	//------------body---------------//
	ssd1306_SetCursor(7,line(1));
	ret=ssd1306_WriteString("Coordonnees", Font_6x8, White);
	ssd1306_SetCursor(7,line(2));
	ret=ssd1306_WriteString("Competences", Font_6x8, White);
	ssd1306_SetCursor(7,line(3));
	ret=ssd1306_WriteString("Langues", Font_6x8, White);
	ssd1306_SetCursor(7,line(4));
	ret=ssd1306_WriteString("Projets", Font_6x8, White);
	ssd1306_SetCursor(7,line(5));
	ret=ssd1306_WriteString("Formation", Font_6x8, White);
	ssd1306_SetCursor(7,line(6));
	ret=ssd1306_WriteString("Experience Pro.", Font_6x8, White);

	ssd1306_SetCursor(0,line(Choice));
	ret=ssd1306_WriteString("-", Font_6x8, White);
	ssd1306_UpdateScreen();
}
void Coordinates_Screen_Function(void)
{
	ssd1306_Fill(Black);
	//------------Title---------------//
	ssd1306_SetCursor(MIDDLE_TITLE("Coordonnees"),0);
	ret=ssd1306_WriteString("Coordonnees", Font_7x10, White);
	//------------body---------------//
	ssd1306_SetCursor(0,line(1));
	ret=ssd1306_WriteString("taher.bazzazi@ieeeorg", Font_6x8, White);
	ssd1306_SetCursor(0,line(2));
	ret=ssd1306_WriteString("LinkedIn:mohamed-tahe", Font_6x8, White);
	ssd1306_SetCursor(MIDDLE("r-bazzazi-408395204"),line(3));
	ret=ssd1306_WriteString("r-bazzazi-408395204", Font_6x8, White);
	ssd1306_SetCursor(MIDDLE("Nancy (54000)"),line(4));
	ret=ssd1306_WriteString("Nancy (54000)", Font_6x8, White);
	ssd1306_SetCursor(MIDDLE("+33 7 57 67 96 07"),line(5));
	ret=ssd1306_WriteString("+33 7 57 67 96 07", Font_6x8, White);
	ssd1306_SetCursor(MIDDLE("22/04/1999"),line(6));
	ret=ssd1306_WriteString("22/04/1999", Font_6x8, White);

	ssd1306_UpdateScreen();
}
void Skills_Screen_Function(void){
	ssd1306_Fill(Black);
	//------------Title---------------//
	ssd1306_SetCursor(MIDDLE_TITLE("Competences"),0);
	ret=ssd1306_WriteString("Competences", Font_7x10, White);
	//------------body---------------//

	PRINT3(1,"C","C++","VHDL");

	PRINT3(4,"Jave","Python","Rust");

	PRINT2(2,"STM32","RaspberryPi");

	ssd1306_SetCursor(MIDDLE("Arduino"),line(3));
	ret=ssd1306_WriteString("Arduino", Font_6x8, White);

	PRINT2(5,"Matlab","Simulink");

	ssd1306_SetCursor(MIDDLE("Git/Github"),line(6));
	ret=ssd1306_WriteString("Git/Github", Font_6x8, White);

	ssd1306_UpdateScreen();
}
void Languaes_Screen_Function(void){
	ssd1306_Fill(Black);
	//------------Title---------------//
	ssd1306_SetCursor(MIDDLE_TITLE("Langues"),0);
	ret=ssd1306_WriteString("Langues", Font_7x10, White);
	//------------body---------------//
	ssd1306_SetCursor(MIDDLE("Francais"),line(2));
	ret=ssd1306_WriteString("Francais", Font_6x8, White);
	ssd1306_SetCursor(MIDDLE("Anglais"),line(3));
	ret=ssd1306_WriteString("Anglais", Font_6x8, White);
	ssd1306_SetCursor(MIDDLE("Arabe"),line(4));
	ret=ssd1306_WriteString("Arabe", Font_6x8, White);
	ssd1306_SetCursor(MIDDLE("Allemand"),line(5));
	ret=ssd1306_WriteString("Allemand", Font_6x8, White);

	ssd1306_UpdateScreen();
}
void Projects_Screen_Function(void){
	ssd1306_Fill(Black);
	//------------Title---------------//
	ssd1306_SetCursor(MIDDLE_TITLE("Projets"),0);
	ret=ssd1306_WriteString("Projets", Font_7x10, White);
	//------------body---------------//
	ssd1306_SetCursor(7,line(1));
	ret=ssd1306_WriteString("EUROBOT", Font_6x8, White);
	ssd1306_SetCursor(7,line(2));
	ret=ssd1306_WriteString("Systeme VTOL", Font_6x8, White);
	ssd1306_SetCursor(7,line(3));
	ret=ssd1306_WriteString("Twizzy", Font_6x8, White);
	ssd1306_SetCursor(7,line(4));
	ret=ssd1306_WriteString("Bras Robotique", Font_6x8, White);
	ssd1306_SetCursor(7,line(5));
	ret=ssd1306_WriteString("Not Zedla", Font_6x8, White);
	ssd1306_SetCursor(7,line(6));
	ret=ssd1306_WriteString("Ce projet :)", Font_6x8, White);

	ssd1306_UpdateScreen();
}

void Diplomas_Screen_Function(void)
{
	ssd1306_Fill(Black);
	//------------Title---------------//
	ssd1306_SetCursor(MIDDLE_TITLE("Formation"),0);
	ret=ssd1306_WriteString("Formation", Font_7x10, White);
	//------------body---------------//
	ssd1306_SetCursor(MIDDLE("ENSEM Nancy: Ing."),line(1));
	ret=ssd1306_WriteString("ENSEM Nancy: Ing.", Font_6x8, White);
	ssd1306_SetCursor(0,line(2));
	ret=ssd1306_WriteString("en Sys. Numeriques", Font_6x8, White);
	ssd1306_SetCursor(MIDDLE("INSAT Tunis: Ing."),line(3));
	ret=ssd1306_WriteString("INSAT Tunis: Ing.", Font_6x8, White);
	ssd1306_SetCursor(0,line(4));
	ret=ssd1306_WriteString("Info Indus. & Auto", Font_6x8, White);
	ssd1306_SetCursor(MIDDLE("INSAT Tunis: Prep."),line(5));
	ret=ssd1306_WriteString("INSAT Tunis: Prep.", Font_6x8, White);
	ssd1306_SetCursor(0,line(6));
	ret=ssd1306_WriteString("Math. Phy. Info.", Font_6x8, White);

	ssd1306_UpdateScreen();
}
void ProfessionalExperience_Screen_Function(void)
{
	ssd1306_Fill(Black);
	//------------Title---------------//
	ssd1306_SetCursor(MIDDLE_TITLE("Experience Pro."),0);
	ret=ssd1306_WriteString("Experience Pro.", Font_7x10, White);
	//------------body---------------//
	ssd1306_SetCursor(MIDDLE("STAGE EN GS2I"),line(1));
	ret=ssd1306_WriteString("STAGE EN GS2I", Font_6x8, White);

	PRINT2(2,"S7-1200","Modbus");

	PRINT2(3,"TIA Portal","Python");

	ssd1306_SetCursor(MIDDLE("RaspberyPi"),line(4));
	ret=ssd1306_WriteString("RaspberyPi", Font_6x8, White);

	ssd1306_SetCursor(MIDDLE("STAGE EN SGEA"),line(5));
	ret=ssd1306_WriteString("STAGE EN SGEA", Font_6x8, White);

	PRINT2(6,"Automate","Indus.");

	ssd1306_UpdateScreen();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	//if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7) == GPIO_PIN_RESET)	//SW4
	//	Current_State=(Current_State+1)%Number_of_states;
	if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4) == GPIO_PIN_RESET)	//SW1
		Current_State=Main_Screen;
	if ((Current_State== Main_Screen) && (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)) == GPIO_PIN_RESET)	//SW2
		Choice=1+((Choice)%6);

	if ((Current_State== Main_Screen) && (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)) == GPIO_PIN_RESET)	//SW3
		Choice=1+(4+Choice)%6;

	if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7) == GPIO_PIN_RESET)	//SW4
		Current_State=(Choice)+Main_Screen;

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
