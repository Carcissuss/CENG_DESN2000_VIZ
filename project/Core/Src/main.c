/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "coast.h"
#include "lcd.h"
#include "state.h"
#include "time.h"
#include "interrupt.h"
#include "buzzer.h"
#include "vibration.h"
#include "options.h"
#include "fitness.h"
#include "ldr.h"
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
ADC_HandleTypeDef hadc2;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */
ADC_HandleTypeDef hadc2;

uint32_t last_tick = 0;
uint32_t seconds = 0;
uint32_t period_count = 0;

uint32_t millisecond = 0;
uint32_t lastCountdownSecond = 0;
uint32_t lastStopwatchSecond = 0;

uint32_t decimal_second_count = 0;
uint32_t button_double_press_time[4] = {0, 0, 0, 0};
uint32_t button_holding_time[4] = {0, 0, 0, 0};

bool timeFormatChanged = false;
bool screenNeedsRefresh = false;
/* button press check */
bool button1 = false;
bool button2 = false;
bool button3 = false;
bool buttonB = false;

/* double press interval and holding time */
uint32_t double_press_interval = 3;
uint32_t holding_bound = 10;
/* press check */
bool is_single_press[4] = {false, false, false, false};
bool is_double_press[4] = {false, false, false, false};
bool is_holding[4] = {false, false, false, false};

/* sound setting */
bool enable_sound = true;
bool button_sound = false;
bool enable_vibration = true;
bool button_vibration = false;
bool enable_time_update = false;

/* flashlight feature */
volatile bool flash = false;

/* screen navigation */
typedef struct {
	bool screen_homepage;
	bool screen_countdown;
	bool screen_stopwatch;
	bool screen_alarm;
	bool screen_setting_1;
	bool screen_setting_2;
}Navigation;

Navigation screen = {
    .screen_homepage = true,
    .screen_countdown = false,
    .screen_stopwatch = false,
    .screen_alarm = false,
    .screen_setting_1 = false,
    .screen_setting_2 = false
};

/* countdown */
Countdown countdown = {
	.minute = 0,
	.second = 0,
	.countdown_enable = false
};
/* stopwatch */
Stopwatch stopwatch = {
	.hour = 0,
	.minute = 0,
	.second = 0,
	.stopwatch_enable = false
};


/* bool lap stopwatch */
bool lapStopwatchFlag = false;

/* ldr value get */
uint32_t modulus = 0;
uint32_t ldrValue = 0;
uint32_t dutyCycle = 0;
//static float s_ldr_filt = 0.0f;     // 滤波后的LDR
//static int   s_inited   = 0;
//static float s_duty     = DUTY_MIN; // 当前占空(0~100)
float alpha = 0.000025;
static float ema = 0;


/* Alarm variables*/
RTC_AlarmTypeDef sAlarm = {0};   // <-- single global definition

volatile bool alarm_active = false;
static uint32_t alarm_start_tick = 0;
static uint32_t last_blink_tick = 0;
static uint32_t last_beep_tick = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM7_Init(void);
static void MX_ADC2_Init(void);
/* USER CODE BEGIN PFP */
extern void coast_asm_delay(uint32_t milliseconds);
void SystemClock_Config(void);
static void system_clock_setup(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == B1_Pin) {
		/* B1 is pressed */
		if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 0) {
			/* sound indication */
			if (enable_sound) generate_sound(460, 50, htim1);

			if (enable_vibration) {
				vibration_call(STEPS_PER_REV);
			}
			if (is_single_press[0] == true &&
				is_double_press[0] == false &&
				(decimal_second_count - button_double_press_time[0]) <= double_press_interval) {
				is_double_press[0] = true;
				is_single_press[0] = false;
				is_holding[0] = false;

			} else {
				is_single_press[0] = true;
				is_double_press[0] = false;
				is_holding[0] = false;
			}

			button_holding_time[0] = decimal_second_count;
		}
		/* B1 is released */
		else {
			stop_sound(htim1);

			if ((decimal_second_count - button_holding_time[0]) >= holding_bound) {
				is_holding[0] = true;
				is_double_press[0] = false;
				is_single_press[0] = false;
			} else {
				button_double_press_time[0] = decimal_second_count;
			}
			if (is_holding[0]) {
				switch (currentScreen) {
					case TIME:
						switchTimeFormat();
						break;
					default:
					}
			} else if (is_single_press[0]) {
				switch (currentScreen) {
					case HOME:
						currentScreen = TIME;
						break;
					case SETTINGS:
						flash = !flash;
						flash_state();
						screenNeedsRefresh = true;
						break;
					case ALARM:
						changeAlarmMin();
						break;
					case COUNTDOWN:
						toggleCountdown(&countdown);
						break;
					case STOPWATCH:
						toggleStopwatch(&stopwatch);
						break;
					default:
				}
			}
		}
	} else if (GPIO_Pin == SW1_Pin) {
		/* The sw1 pin is pressed */

		if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == 1) {
			/* sound indication */
			if (enable_sound) generate_sound(460, 50, htim1);

			if (enable_vibration) {
				vibration_call(STEPS_PER_REV);
			}

			if (is_single_press[1] == true &&
				is_double_press[1] == false &&
				(decimal_second_count - button_double_press_time[1]) <= double_press_interval) {
				is_double_press[1] = true;
				is_single_press[1] = false;
				is_holding[1] = false;

			} else {
				is_single_press[1] = true;
				is_double_press[1] = false;
				is_holding[1] = false;
			}

			button_holding_time[1] = decimal_second_count;
		}
		/* The sw1 pin is released */
		else {
			stop_sound(htim1);

			if ((decimal_second_count - button_holding_time[1]) >= holding_bound) {
				is_holding[1] = true;
				is_double_press[1] = false;
				is_single_press[1] = false;
			} else {
				button_double_press_time[1] = decimal_second_count;
			}
			/* navigation */
			if (is_holding[1]) {
				switch (currentScreen){
				//SW1 held, BACK for ALARM // no feature here thus far

				}
			} else if (is_single_press[1]) {
				switch (currentScreen) {
					case SETTINGS:
						currentScreen = OPT;
						break;
					case OPT:
						currentScreen = HOME;
						break;
					case TIME:
						currentScreen = HOME;
						break;
					case ALARM:
						currentScreen = TIME;
						break;
					case ALARM_SET:
						currentScreen = ALARM;
						break;
					case FITNESS:
						currentScreen = HOME;
						break;
					case COUNTDOWN:
						currentScreen = FITNESS;
						break;
					case STOPWATCH:
						currentScreen = FITNESS;
						break;
				}
			}
		}
	} else if (GPIO_Pin == SW2_Pin) {
		/* The sw2 pin is pressed */

		if (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == 1) {
			/* sound indication */
			if (enable_sound) {
				generate_sound(460, 50, htim1);
			}
			if (enable_vibration) {
				vibration_call(STEPS_PER_REV);
			}

			if (enable_vibration) {
				button_vibration = true;
			}

		    if (is_single_press[2] == true &&
		        is_double_press[2] == false &&
		        (decimal_second_count - button_double_press_time[2]) <= double_press_interval) {
		        is_double_press[2] = true;
		        is_single_press[2] = false;
		        is_holding[2] = false;

		    } else {
		        is_single_press[2] = true;
		        is_double_press[2] = false;
		        is_holding[2] = false;
		    }

		    button_holding_time[2] = decimal_second_count;
		}
		/* The sw2 pin is released */
		else {
			stop_sound(htim1);

			if ((decimal_second_count - button_holding_time[2]) >= holding_bound) {
				is_holding[2] = true;
				is_double_press[2] = false;
				is_single_press[2] = false;
			} else {
				button_double_press_time[2] = decimal_second_count;
			}
			if (is_holding[2] == true) {
				switch (currentScreen){
				//SW2 Held, HOME
					case TIME:
						currentScreen = HOME;
						break;
					case ALARM:
						currentScreen = HOME;
						break;
					case ALARM_SET:
						currentScreen = HOME;
						break;
					case SETTINGS:
						currentScreen = HOME;
						break;
					case OPT:
						currentScreen = HOME;
						break;
					default:
						currentScreen = HOME;
						break;
				}
			} else if (is_single_press[2]) {
				switch (currentScreen) {
					case ALARM:
						currentScreen = ALARM_SET;  // request alarm set
						break;
					case HOME:
						currentScreen = FITNESS;
						break;
					case FITNESS:
						currentScreen = COUNTDOWN;
						break;
					case COUNTDOWN:
						mintueCountdown(&countdown);
						break;
					case STOPWATCH:
						lapStopwatchFlag = true;
						break;
					case SETTINGS:
						enable_sound = !enable_sound;   // toggle first
						screenNeedsRefresh = true;
						break;
				    }
			}
		}
	} else {
		/* The sw3 pin is pressed */
		if (HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == 1) {
			/* sound indication */
			if (enable_sound) generate_sound(460, 50, htim1);

			if (enable_vibration) {
				vibration_call(STEPS_PER_REV);
			}
			if (is_single_press[3] == true &&
				is_double_press[3] == false &&
				(decimal_second_count - button_double_press_time[3]) <= double_press_interval) {
				is_double_press[3] = true;
				is_single_press[3] = false;
				is_holding[3] = false;

			} else {
				is_single_press[3] = true;
				is_double_press[3] = false;
				is_holding[3] = false;
			}

			button_holding_time[3] = decimal_second_count;
		}
		/* The sw3 pin is released */
		else {
			stop_sound(htim1);

			if ((decimal_second_count - button_holding_time[3]) >= holding_bound) {
				is_holding[3] = true;
				is_double_press[3] = false;
				is_single_press[3] = false;
			} else {
				button_double_press_time[3] = decimal_second_count;
			}
			if (is_holding[3]) {
				switch(currentScreen) {
					case COUNTDOWN:
						resetCountdown(&countdown);
						break;
					case STOPWATCH:
						resetStopwatch(&stopwatch);
						break;
				}
			}
			if (is_double_press[3]) {
				is_single_press[3] = false; // cancel single press
				switch (currentScreen){
					case ALARM:
						switchAMPM();
						break;
				}
			}
			// Only act as single press if no double press
			else if (is_single_press[3]) {
				switch (currentScreen) {
					case HOME:
						previousScreen = currentScreen;
						currentScreen = OPT;
						break;
					case OPT:
						previousScreen = currentScreen;
						currentScreen = SETTINGS;
						break;
					case SETTINGS:
						enable_vibration = !enable_vibration;
						if (enable_vibration) {
							vibration_call(32);
						} else {
							vibration_stop();
						}
						screenNeedsRefresh = true;
						break;
					case TIME:
						currentScreen = ALARM;
						break;
					case ALARM:
						changeAlarmHour();
						break;
					case FITNESS:
						currentScreen = STOPWATCH;
						break;
					case COUNTDOWN:
						secondCountdown(&countdown);
						break;
				}
			}
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim6) {
		seconds++;
	} else if (htim == &htim7) {
		period_count++;
		if (period_count >= 100) {
			period_count = 0;
			decimal_second_count++;
		}
		vibration_tick_1ms(); // vibration ticker
		if (!flash) {
			/* ldr running code */
			if (period_count == dutyCycle) {
				HAL_GPIO_WritePin(LED_D1_GPIO_Port, LED_D1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_D2_GPIO_Port, LED_D2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_D3_GPIO_Port, LED_D3_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_D4_GPIO_Port, LED_D4_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

				HAL_GPIO_WritePin(GPIOB, SER_Data_IN_Pin, GPIO_PIN_RESET);
				for (int i = 0; i < 16; i++) {
					HAL_GPIO_WritePin(SRCLK_GPIO_Port, SRCLK_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(SRCLK_GPIO_Port, SRCLK_Pin, GPIO_PIN_RESET);
				}
				HAL_GPIO_WritePin(RCLK_Latch_GPIO_Port, RCLK_Latch_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(RCLK_Latch_GPIO_Port, RCLK_Latch_Pin, GPIO_PIN_RESET);
			} else if (period_count == 0) {
				HAL_GPIO_WritePin(LED_D1_GPIO_Port, LED_D1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_D2_GPIO_Port, LED_D2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_D3_GPIO_Port, LED_D3_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_D4_GPIO_Port, LED_D4_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

				HAL_GPIO_WritePin(GPIOB, SER_Data_IN_Pin, GPIO_PIN_SET);
				for (int i = 0; i < 16; i++) {
					HAL_GPIO_WritePin(SRCLK_GPIO_Port, SRCLK_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(SRCLK_GPIO_Port, SRCLK_Pin, GPIO_PIN_RESET);
				}
				HAL_GPIO_WritePin(RCLK_Latch_GPIO_Port, RCLK_Latch_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(RCLK_Latch_GPIO_Port, RCLK_Latch_Pin, GPIO_PIN_RESET);
			}
		}
	}
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
	alarm_active = true;
	alarm_start_tick = HAL_GetTick();
	last_blink_tick = alarm_start_tick;
	last_beep_tick = alarm_start_tick;
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
  system_clock_setup();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_TIM1_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */

  coast_lcd_init();
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (countdown.countdown_enable) {
	  	  runCountdown(&countdown, &lastCountdownSecond, seconds, htim1, enable_sound, enable_vibration);
	  	  }
	  if (stopwatch.stopwatch_enable) {
		  runStopwatch(&stopwatch, &lastStopwatchSecond, seconds);
	  }

	  // Rings, Vibrates and Flashes for 5 seconds
	  if (alarm_active) {
	      if (HAL_GetTick() - alarm_start_tick >= 5000) {
	          alarm_active = false;
	          HAL_GPIO_WritePin(LED_D3_GPIO_Port, LED_D3_Pin, GPIO_PIN_RESET);
	          stop_sound(htim1);
	      } else {
	          if (HAL_GetTick() - last_beep_tick >= 500) {
	              play_note(460, 100, 50, htim1);
	              vibration_call(32);
	              flash_on();
	              last_beep_tick = HAL_GetTick();
	          }
	      }
	  }

	  if (currentScreen != previousScreen || timeFormatChanged || screenNeedsRefresh) {
			LCD_SendCmd(LCD_CLEAR_DISPLAY);
			coast_asm_delay(2);

			switch (currentScreen) {
				case HOME:
					homePage(); // draw layout only
					updateTime(1, 4);
					break;
				case TIME:
					timePage();
					updateTime(0, 4);
					break;
				case ALARM:
					alarmPage();
					updateAlarm(1, 0);
					break;
				case ALARM_SET:
				    alarmConfirm();
				    break;
				case OPT:
					OPTpage(ldrValue);
					break;
				case SETTINGS:
					settingsPage();
					break;
				case FITNESS:
					fitnessPage();
					updateFitness(1, 0);
					break;
				case COUNTDOWN:
					countdownPage(countdown);
					break;
				case STOPWATCH:
					stopwatchPage(stopwatch);
					break;

			}
			previousScreen = currentScreen;
			screenNeedsRefresh = false;  // clear the flags
			timeFormatChanged = false;  // clear the flag
			last_tick = HAL_GetTick();

	  }

	  /* UPDATE TIME EVERY SECOND ELAPSED */
	  if ((HAL_GetTick() - last_tick) >= 1000) {
	  	switch (currentScreen) {
	  		case HOME:
	  			updateTime(1, 4);  // row 1 (second line), column 4
	  			break;
	  		case TIME:
	  			updateTime(0, 4);  // row 0, col 6 (or wherever)
	  			break;
	  		case ALARM:
	  			updateAlarm(1, 0);
	  			break;
	  		case FITNESS:
	  			 updateFitness(1, 0);
	  			 break;
	  		case COUNTDOWN:
				updateCountdown(countdown);
				break;
			case STOPWATCH:
				if (lapStopwatchFlag) {
					lapStopwatch(stopwatch);
					lapStopwatchFlag = false;
				}
				updateStopwatch(stopwatch);
				break;
			case OPT:
				updateOpt(ldrValue);
				break;
	  		default:
	  			break;
	  	}
	  	last_tick += 1000;
	  }
	  /* ldr */
	  HAL_ADC_Start(&hadc2);
	  HAL_ADC_PollForConversion(&hadc2, 1);
	  ldrValue = HAL_ADC_GetValue(&hadc2);

	  // Exponential Moving Average Filter
	  ema = alpha * ldrValue + (1 - alpha) * ema;

	  if (ema >= 600) {
		  dutyCycle = 0;
	  } else if (ema <= 100) {
		  dutyCycle = 100;
	  } else {
		  dutyCycle = (600 - (uint32_t)ema) / 5;
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_TIM1
                              |RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x8;
  sTime.Minutes = 0x34;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_SUNDAY;
  sDate.Month = RTC_MONTH_AUGUST;
  sDate.Date = 0x3;
  sDate.Year = 0x25;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 1000 -1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 296;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 9999;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 7199;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 999;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 71;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SRCLK_Pin|COILA_Pin|Control_RW_Pin|COILC_Pin
                          |Data_D4_Pin|Data_D5_Pin|Data_D6_Pin|Data_D7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|COILB_Pin|Control_RS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, COILD_Pin|LED_D1_Pin|SER_Data_IN_Pin|LED_D4_Pin
                          |LED_D2_Pin|LED_D3_Pin|RCLK_Latch_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Control_E_GPIO_Port, Control_E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SRCLK_Pin COILA_Pin Control_RW_Pin COILC_Pin
                           Data_D4_Pin Data_D5_Pin Data_D6_Pin Data_D7_Pin */
  GPIO_InitStruct.Pin = SRCLK_Pin|COILA_Pin|Control_RW_Pin|COILC_Pin
                          |Data_D4_Pin|Data_D5_Pin|Data_D6_Pin|Data_D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SW1_Pin SW2_Pin */
  GPIO_InitStruct.Pin = SW1_Pin|SW2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin COILB_Pin Control_RS_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|COILB_Pin|Control_RS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SW3_Pin */
  GPIO_InitStruct.Pin = SW3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SW3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : COILD_Pin LED_D1_Pin SER_Data_IN_Pin LED_D4_Pin
                           LED_D2_Pin LED_D3_Pin RCLK_Latch_Pin */
  GPIO_InitStruct.Pin = COILD_Pin|LED_D1_Pin|SER_Data_IN_Pin|LED_D4_Pin
                          |LED_D2_Pin|LED_D3_Pin|RCLK_Latch_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Control_E_Pin */
  GPIO_InitStruct.Pin = Control_E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Control_E_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void system_clock_setup() {
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
	Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_TIM1;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
	Error_Handler();
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
#ifdef USE_FULL_ASSERT
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
