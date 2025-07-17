/* USER CODE BEGIN Header */
/**
 ============================================================================
 Name        : E8DecoderCycleCount.c
 Board       : STM32L412KBU6 (Nucleo-32)
 Author      :
 Version     : 1
 Copyright   : Free
 Description : Implementation of E8 and Dn lattice decoders in
 Conway, J., Sloane, N.: Fast quantizing and decoding and algorithms for lattice quantizers and codes.
 IEEE Transactions on Information Theory 28(2), 227–232 (1982).
 https://doi.org/10.1109/TIT.1982.1056484
 ============================================================================
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // Required for memcpy()


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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */


volatile unsigned int *DWT_CYCCNT   = (volatile unsigned int *)0xE0001004; //address of the register
volatile unsigned int *DWT_CONTROL  = (volatile unsigned int *)0xE0001000; //address of the register
volatile unsigned int *DWT_LAR      = (volatile unsigned int *)0xE0001FB0; //address of the register
volatile unsigned int *SCB_DEMCR    = (volatile unsigned int *)0xE000EDFC; //address of the register




/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */


int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
	return len;
}



float* subtract_arrays(int* intArray, float* doubleArray, int size) {
    // Allocate memory for the result array (of type double)
	float* resultArray = (float*)malloc(size * sizeof(float));

	/*
    // Check if memory allocation was successful
    if (resultArray == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return NULL; // Return NULL if allocation fails
    }
    */

    // Perform element-wise subtraction
    for (int i = 0; i < size; i++) {
        resultArray[i] = (float)intArray[i] - doubleArray[i]; // Cast int to double for subtraction
    }

    return resultArray;
}

float* subtract_arrays_double(float* doubleArray0, float* doubleArray, int size) {
    // Allocate memory for the result array (of type double)
	float* resultArray = (float*)malloc(size * sizeof(float));

    /*
	// Check if memory allocation was successful
    if (resultArray == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return NULL; // Return NULL if allocation fails
    }
    */

    // Perform element-wise subtraction
    for (int i = 0; i < size; i++) {
        resultArray[i] = doubleArray0[i] - doubleArray[i]; // Cast int to double for subtraction
    }

    return resultArray;
}


int sumArrayElements(int arr[], int size) {
    int sum = 0; // Initialize sum to zero
    // Loop through each element of the array
    for (int i = 0; i < size; i++) {
        sum += arr[i]; // Add the current element to the sum
    }
    return sum; // Return the total sum
}



void calculateAbsoluteValues(float arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = fabs(arr[i]); // Apply abs() to each element
    }
}

int* roundArrayToNearestInteger(float arr[], int size) {
	int* newArray = (int*)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
    	newArray[i] = (int)round(arr[i]);
    }
    return newArray;
}


int findIndexOfLargest(float arr[], int size) {
    if (size <= 0) {
        // Handle empty or invalid array size
        return -1; // Or some other error indicator
    }

    int largest_val = arr[0];
    int largest_index = 0;

    for (int i = 1; i < size; i++) {
        if (arr[i] > largest_val) {
            largest_val = arr[i];
            largest_index = i;
        }
    }

    return largest_index;
}



int findLargestAbsoluteDifferenceIndex(int *intArray, float *floatArray, int size) {

	/*
	if (size <= 0 || intArray == NULL || floatArray == NULL) {
        return -1; // Handle invalid input
    }
    */

    float maxAbsoluteDiff = -1;
    int maxIndex = -1;

    for (int i = 0; i < size; i++) {
        float diff = (float)intArray[i] - floatArray[i];
        float absoluteDiff = fabs(diff); // Use fabs for float absolute value

        if (absoluteDiff > maxAbsoluteDiff) {
            maxAbsoluteDiff = absoluteDiff;
            maxIndex = i;
        }
    }

    return maxIndex;
}


int E8Decision(int *intLP1, float *floatLP2, float *floatArray, int size) {

	/*
	if (size <= 0 || intArray == NULL || floatArray == NULL) {
        return -1; // Handle invalid input
    }
    */

    float tempt_sum = 0;
    int Index = 0;

    for (int i = 0; i < size; i++) {
        tempt_sum = tempt_sum+ ((float)intLP1[i] - floatLP2[i])*((float)intLP1[i] + floatLP2[i]-floatArray[i]-floatArray[i]);
        //float absoluteDiff = fabs(diff); // Use fabs for float absolute value
    }

    if (tempt_sum < 0) {
        return Index=1;
    } else {
        return Index=2;
    }
}



int* DnDecoder(float arr[],int size) {
    // Allocate memory for the new array dynamically
    int* f = (int*)malloc(size * sizeof(int));
    int* g = (int*)malloc(size * sizeof(int));
    int i = 0;
    int totalSum=0;


    //int* LP = (int*)malloc(size * sizeof(int));

    /*
    // Check if memory allocation was successful
    if (f == NULL) {
        printf("Memory allocation failed!\n");
        return NULL; // Return NULL to indicate an error
    }
    */


    //f =roundArrayToNearestInteger(arr, size);
    //g =roundArrayToNearestInteger(arr, size);
    //memcpy(g, f, size * sizeof(int));

    for (i = 0; i < size; i++) {
    	f[i] = (int)roundf(arr[i]);
    	g[i]=f[i];
    	totalSum=totalSum+f[i];
    }


    //int index =findLargestAbsoluteDifferenceIndex(f, arr, size);
    int index = -1;
    float maxAbsoluteDiff = -1;

        for (i = 0; i < size; i++) {
            float diff = (float)f[i] - arr[i];
            float absoluteDiff = fabs(diff); // Use fabs for float absolute value

            if (absoluteDiff > maxAbsoluteDiff) {
                maxAbsoluteDiff = absoluteDiff;
                index = i;
            }
        }

    /*
    float* result = subtract_arrays(f, arr, size);
    calculateAbsoluteValues(result, size); // Call the function
    int index = findIndexOfLargest(result, size);
    */


    if (arr[index]>f[index]) {
        g[index]=g[index]+1;
    } else {
    	g[index]=g[index]-1;
    }

    //int totalSum = sumArrayElements(f, size);

    if ((totalSum & 1) == 0) {
    //if (totalSum % 2 == 0) {
        return f;
    } else {
        return g;
    }

}






int main(void) {

	//Example of input vector
	float y[] = { 1, 0.5000, 0.5000, 0.5000, 0.5000, 0.5000, 1.5000, 0.5000 };

	//Input to E8 lattice decoding
	float shift[] = { 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5 };
	float shift_n[] = { -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5 };
	int arraySize = 8;
	int flag = 0;
	int *LP1;
	float *LP2;

	//Other variables
	uint32_t t1, t2, numCycles;
	float *numbers_shift;
	int *tempt;


	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	// uint8_t tx_buff[]="adsfas";

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */

	*SCB_DEMCR |= 0x01000000;
	*DWT_LAR = 0xC5ACCE55; // enable access
	*DWT_CYCCNT = 0; // reset the counter
	/*--------------------------- End of  MCU Configuration----------------------*/


	/*E8 lattice decoder:
	     Step 1: call D8 decoding twice, obtain two estimated E8 lattice points LP1 and LP2
	     Step 2: choose the closest E8 point to y, by comparing ||LP1-y|| and ||LP2-y||
	*/

	//First call of D8 decoder, output the first estimated E8 point LP1
	*DWT_CONTROL |= 1; // enable the counter
	t1 = *DWT_CYCCNT;

	LP1 = DnDecoder(y, arraySize);

	//Second call of D8 decoder, output the second estimated E8 point LP2
	numbers_shift = subtract_arrays_double(y, shift, arraySize);
	tempt = DnDecoder(numbers_shift, arraySize);

	LP2 = subtract_arrays(tempt, shift_n, arraySize);

	//choose the closest E8 point to the input vector y, by comparing ||LP1-y|| and ||LP2-y||
	flag = E8Decision(LP1, LP2, y, arraySize); // The total clock cycles for E8 Lattice CVP decoder

	t2 = *DWT_CYCCNT;
	*DWT_CONTROL |= 0; // close the counter
	numCycles = (t2 - t1);

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */


	while (1) {

		printf("clock cycles for E8 lattice decoding: %" PRIu32 "\r\n",numCycles);
		HAL_Delay(500);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
