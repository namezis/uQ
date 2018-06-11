/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq/adc.hpp>

namespace cycfi { namespace uq { namespace detail
{
   void adc_base::construct(ADC_TypeDef* adc)
   {
      uq::init();

      Instance = adc;
      if (HAL_ADC_DeInit(this) != HAL_OK)
         error_handler();

      Init.ClockPrescaler           = ADC_CLOCK_SYNC_PCLK_DIV4;         // Synchronous clock mode, input ADC clock divided by 4
      Init.Resolution               = ADC_RESOLUTION_16B;               // 16-bit resolution for converted data
      Init.ScanConvMode             = DISABLE;                          // Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1)
      Init.EOCSelection             = ADC_EOC_SINGLE_CONV;              // EOC flag picked-up to indicate conversion end
      Init.LowPowerAutoWait         = DISABLE;                          // Auto-delayed conversion feature disabled
      Init.ContinuousConvMode       = ENABLE;                           // Continuous mode enabled (automatic conversion restart after each conversion)
      Init.NbrOfConversion          = 1;                                // Parameter discarded because sequencer is disabled
      Init.DiscontinuousConvMode    = DISABLE;                          // Parameter discarded because sequencer is disabled
      Init.NbrOfDiscConversion      = 1;                                // Parameter discarded because sequencer is disabled
      Init.ExternalTrigConv         = ADC_SOFTWARE_START;               // Software start to trig the 1st conversion manually, without external event
      Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;    // Parameter discarded because software trigger chosen
      Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;  // ADC DMA circular requested
      Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;         // DR register is overwritten with the last conversion result in case of overrun
      Init.OversamplingMode         = DISABLE;                          // No oversampling
      Init.BoostMode                = ENABLE;                           // Enable Boost mode as ADC clock frequency is bigger than 20 MHz

      // Initialize ADC peripheral according to the passed parameters
      if (HAL_ADC_Init(this) != HAL_OK)
         error_handler();

      // Start calibration
      if (HAL_ADCEx_Calibration_Start(this, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
         error_handler();
   }

   void adc_base::dma_setup()
   {
      __HAL_RCC_ADC12_CLK_ENABLE();                // ADC Periph clock enable
      __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP); // ADC Periph interface clock configuration
      __HAL_RCC_DMA1_CLK_ENABLE();                 // Enable DMA clock

      _dma_handle.Instance                 = DMA1_Stream1;
      _dma_handle.Init.Request             = DMA_REQUEST_ADC1;
      _dma_handle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
      _dma_handle.Init.PeriphInc           = DMA_PINC_DISABLE;
      _dma_handle.Init.MemInc              = DMA_MINC_ENABLE;
      _dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
      _dma_handle.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
      _dma_handle.Init.Mode                = DMA_CIRCULAR;
      _dma_handle.Init.Priority            = DMA_PRIORITY_MEDIUM;

      // Deinitialize  & Initialize the DMA for new transfer
      HAL_DMA_DeInit(&_dma_handle);
      HAL_DMA_Init(&_dma_handle);

      // Associate the DMA handle
      __HAL_LINKDMA(this, DMA_Handle, _dma_handle);

      // NVIC configuration for DMA Input data interrupt
      HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 1, 0);
      HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
   }

   void adc_base::enable_channel(
      GPIO_TypeDef* gpio
    , std::uint32_t pin
    , uint32_t channel
    , uint32_t rank)
   {
      // ADC Channel GPIO pin configuration
      GPIO_InitTypeDef GPIO_InitStruct;
      GPIO_InitStruct.Pin = pin;
      GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(gpio, &GPIO_InitStruct);

      ADC_ChannelConfTypeDef config;
      config.Channel       = channel;                 // Sampled channel number
      config.Rank          = rank;                    // Rank of sampled channel
      config.SamplingTime  = ADC_SAMPLETIME_1CYCLE_5; // Sampling time (number of clock cycles unit)
      config.SingleDiff    = ADC_SINGLE_ENDED;        // Single-ended input channel
      config.OffsetNumber  = ADC_OFFSET_NONE;         // No offset subtraction
      config.Offset        = 0;                       // Parameter discarded because offset correction is disabled

      if (HAL_ADC_ConfigChannel(this, &config) != HAL_OK)
         error_handler();
   }
}}}

extern "C"
{
   void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
   {
      static_cast<cycfi::uq::detail::adc_base*>(hadc)->dma_setup();
   }
}



