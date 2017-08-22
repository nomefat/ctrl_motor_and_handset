/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define led3_Pin GPIO_PIN_13
#define led3_GPIO_Port GPIOC
#define led2_Pin GPIO_PIN_14
#define led2_GPIO_Port GPIOC
#define led1_Pin GPIO_PIN_15
#define led1_GPIO_Port GPIOC
#define beep_Pin GPIO_PIN_0
#define beep_GPIO_Port GPIOA
#define rf_ce_Pin GPIO_PIN_2
#define rf_ce_GPIO_Port GPIOA
#define rf_irq_Pin GPIO_PIN_3
#define rf_irq_GPIO_Port GPIOA
#define rf_irq_EXTI_IRQn EXTI3_IRQn
#define rf_cs_Pin GPIO_PIN_4
#define rf_cs_GPIO_Port GPIOA
#define led4_Pin GPIO_PIN_0
#define led4_GPIO_Port GPIOB
#define led5_Pin GPIO_PIN_1
#define led5_GPIO_Port GPIOB
#define V1_Pin GPIO_PIN_2
#define V1_GPIO_Port GPIOB
#define V2_Pin GPIO_PIN_10
#define V2_GPIO_Port GPIOB
#define V3_Pin GPIO_PIN_11
#define V3_GPIO_Port GPIOB
#define R7_Pin GPIO_PIN_12
#define R7_GPIO_Port GPIOB
#define R6_Pin GPIO_PIN_13
#define R6_GPIO_Port GPIOB
#define R5_Pin GPIO_PIN_14
#define R5_GPIO_Port GPIOB
#define R4_Pin GPIO_PIN_15
#define R4_GPIO_Port GPIOB
#define R3_Pin GPIO_PIN_8
#define R3_GPIO_Port GPIOA
#define R2_Pin GPIO_PIN_9
#define R2_GPIO_Port GPIOA
#define R1_Pin GPIO_PIN_10
#define R1_GPIO_Port GPIOA
#define power_btn_Pin GPIO_PIN_11
#define power_btn_GPIO_Port GPIOA
#define power_enable_Pin GPIO_PIN_12
#define power_enable_GPIO_Port GPIOA
#define g_Pin GPIO_PIN_15
#define g_GPIO_Port GPIOA
#define c_Pin GPIO_PIN_3
#define c_GPIO_Port GPIOB
#define h_Pin GPIO_PIN_4
#define h_GPIO_Port GPIOB
#define d_Pin GPIO_PIN_5
#define d_GPIO_Port GPIOB
#define e_Pin GPIO_PIN_6
#define e_GPIO_Port GPIOB
#define a_Pin GPIO_PIN_7
#define a_GPIO_Port GPIOB
#define f_Pin GPIO_PIN_8
#define f_GPIO_Port GPIOB
#define b_Pin GPIO_PIN_9
#define b_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
