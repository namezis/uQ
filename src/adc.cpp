/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq/adc.hpp>

namespace cycfi { namespace uq { namespace detail
{
   void adc_base::construct()
   {
      uq::init();

      Instance          = ADC1;
      if (HAL_ADC_DeInit(this) != HAL_OK)
      {
         /* ADC de-initialization Error */
         // Error_Handler();
      }

      Init.ClockPrescaler           = ADC_CLOCK_SYNC_PCLK_DIV4;         /* Synchronous clock mode, input ADC clock divided by 4*/
      Init.Resolution               = ADC_RESOLUTION_16B;               /* 16-bit resolution for converted data */
      Init.ScanConvMode             = DISABLE;                          /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
      Init.EOCSelection             = ADC_EOC_SINGLE_CONV;              /* EOC flag picked-up to indicate conversion end */
      Init.LowPowerAutoWait         = DISABLE;                          /* Auto-delayed conversion feature disabled */
      Init.ContinuousConvMode       = ENABLE;                           /* Continuous mode enabled (automatic conversion restart after each conversion) */
      Init.NbrOfConversion          = 1;                                /* Parameter discarded because sequencer is disabled */
      Init.DiscontinuousConvMode    = DISABLE;                          /* Parameter discarded because sequencer is disabled */
      Init.NbrOfDiscConversion      = 1;                                /* Parameter discarded because sequencer is disabled */
      Init.ExternalTrigConv         = ADC_SOFTWARE_START;               /* Software start to trig the 1st conversion manually, without external event */
      Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;    /* Parameter discarded because software trigger chosen */
      Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;  /* ADC DMA circular requested */
      Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;         /* DR register is overwritten with the last conversion result in case of overrun */
      Init.OversamplingMode         = DISABLE;                          /* No oversampling */
      Init.BoostMode                = ENABLE;                           /* Enable Boost mode as ADC clock frequency is bigger than 20 MHz */

      /* Initialize ADC peripheral according to the passed parameters */
      if (HAL_ADC_Init(this) != HAL_OK)
      {
         // Error_Handler();
      }

      /* ### - 2 - Start calibration ############################################ */
      if (HAL_ADCEx_Calibration_Start(this, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
      {
         //Error_Handler();
      }
   }

   void adc_base::setup()
   {
      static DMA_HandleTypeDef         DmaHandle;

      /*##-1- Enable peripherals and GPIO Clocks #################################*/
      /* Enable GPIO clock ****************************************/
      __HAL_RCC_GPIOA_CLK_ENABLE();
      /* ADC Periph clock enable */
      __HAL_RCC_ADC12_CLK_ENABLE();
      /* ADC Periph interface clock configuration */
      __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);
      /* Enable DMA clock */
      __HAL_RCC_DMA1_CLK_ENABLE();

      /*##- 2- Configure peripheral GPIO #########################################*/
      /* ADC Channel GPIO pin configuration */
      GPIO_InitTypeDef          GPIO_InitStruct;
      GPIO_InitStruct.Pin = GPIO_PIN_6;
      GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
      /*##- 3- Configure DMA #####################################################*/

      /*********************** Configure DMA parameters ***************************/
      DmaHandle.Instance                 = DMA1_Stream1;
      DmaHandle.Init.Request             = DMA_REQUEST_ADC1;
      DmaHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
      DmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
      DmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
      DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
      DmaHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
      DmaHandle.Init.Mode                = DMA_CIRCULAR;
      DmaHandle.Init.Priority            = DMA_PRIORITY_MEDIUM;
      /* Deinitialize  & Initialize the DMA for new transfer */
      HAL_DMA_DeInit(&DmaHandle);
      HAL_DMA_Init(&DmaHandle);

      /* Associate the DMA handle */
      __HAL_LINKDMA(this, DMA_Handle, DmaHandle);

      /* NVIC configuration for DMA Input data interrupt */
      HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 1, 0);
      HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
   }

   void adc_base::destruct()
   {
      /*##-1- Reset peripherals ##################################################*/
      __HAL_RCC_ADC12_FORCE_RESET();
      __HAL_RCC_ADC12_RELEASE_RESET();
      /* ADC Periph clock disable
         (automatically reset all ADC instances of the ADC common group) */
      __HAL_RCC_ADC12_CLK_DISABLE();

      /*##-2- Disable peripherals and GPIO Clocks ################################*/
      /* De-initialize the ADC Channel GPIO pin */
      HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);
   }
}}}

extern "C"
{
   void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
   {
      static_cast<cycfi::uq::detail::adc_base*>(hadc)->init();
   }
}



