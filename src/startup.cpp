/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq/startup.hpp>
#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo_144.h"

namespace cycfi { namespace uq
{
   namespace
   {
      // This function is executed in case of error occurrence.
      void error_handler()
      {
         BSP_LED_On(LED3);
         while (1)
         {
         }
      }

      // System Clock Configuration
      //    The system Clock is configured as follows:
      //
      //    System Clock source            = PLL (HSE BYPASS)
      //    SYSCLK(Hz)                     = 400000000 (CPU Clock)
      //    HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
      //    AHB Prescaler                  = 2
      //    D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
      //    D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
      //    D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
      //    D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
      //    HSE Frequency(Hz)              = 8000000
      //    PLL_M                          = 4
      //    PLL_N                          = 400
      //    PLL_P                          = 2
      //    PLL_Q                          = 4
      //    PLL_R                          = 2
      //    VDD(V)                         = 3.3
      //    Flash Latency(WS)              = 4
      //
      void system_clock_config()
      {
         RCC_ClkInitTypeDef RCC_ClkInitStruct;
         RCC_OscInitTypeDef RCC_OscInitStruct;
         HAL_StatusTypeDef ret = HAL_OK;

         // Supply configuration update enable
         MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

         // The voltage scaling allows optimizing the power consumption when
         // the device is clocked below the maximum system frequency, to
         // update the voltage scaling value regarding system frequency refer
         // to product datasheet.
         __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

         while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
         {
         }

         /* Enable HSE Oscillator and activate PLL with HSE as source */
         RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
         RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
         RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
         RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
         RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
         RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

         RCC_OscInitStruct.PLL.PLLM = 4;
         RCC_OscInitStruct.PLL.PLLN = 400;
         RCC_OscInitStruct.PLL.PLLP = 2;
         RCC_OscInitStruct.PLL.PLLR = 2;
         RCC_OscInitStruct.PLL.PLLQ = 4;

         RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
         RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
         ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
         if (ret != HAL_OK)
            error_handler();

         // Select PLL as system clock source and configure  bus clocks dividers
         RCC_ClkInitStruct.ClockType =
            RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1
          | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1
         ;

         RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
         RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
         RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
         RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
         RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
         RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
         RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
         ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
         if (ret != HAL_OK)
            error_handler();

         // activate CSI clock mondatory for I/O Compensation Cell
         __HAL_RCC_CSI_ENABLE() ;

         // Enable SYSCFG clock mondatory for I/O Compensation Cell
         __HAL_RCC_SYSCFG_CLK_ENABLE() ;

         // Enables the I/O Compensation Cell
         HAL_EnableCompensationCell();
      }

      //  CPU L1-Cache enable.
      void CPU_CACHE_Enable()
      {
         // Enable I-Cache
         SCB_EnableICache();

         // Enable D-Cache
         SCB_EnableDCache();
      }
   }

   void init()
   {
      // Enable the CPU Cache
      CPU_CACHE_Enable();

      // STM32H7xx HAL library initialization:
      // - Systick timer is configured by default as source of time base.
      // - Set NVIC Group Priority to 4
      // - Low Level Initialization
      HAL_Init();

      // Configure the system clock to 400 MHz
      system_clock_config();
   }
}}


