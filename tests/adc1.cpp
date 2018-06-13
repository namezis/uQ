/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq.hpp>

namespace uq = cycfi::uq;

uq::adc<1, 1> adc;
int x = 123;

int main()
{
   adc.enable_channels<3>();
   adc.start();
   while (true)
   {
      x = 456;
   }
}

// extern "C"
// {
//    /**
//     * @brief  Conversion complete callback in non-blocking mode
//     * @param  hadc: ADC handle
//     * @retval None
//     */
//    // void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
//    // {
//    // /* Invalidate Data Cache to get the updated content of the SRAM on the first half of the ADC converted data buffer: 32 bytes */
//    //    SCB_InvalidateDCache_by_Addr((uint32_t*) adc.begin(), adc.capacity / 2);
//    // }

//    // /**
//    //  * @brief  Conversion DMA half-transfer callback in non-blocking mode
//    //  * @param  hadc: ADC handle
//    //  * @retval None
//    //  */
//    // void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
//    // {
//    //    /* Invalidate Data Cache to get the updated content of the SRAM on the second half of the ADC converted data buffer: 32 bytes */
//    //    SCB_InvalidateDCache_by_Addr((uint32_t*) adc.middle(), adc.capacity / 2);
//    // }

//    /**
//    * @brief  This function handles DMA1_Stream1_IRQHandler interrupt request.
//    * @param  None
//    * @retval None
//    */
//    // void DMA1_Stream1_IRQHandler(void)
//    // {
//    //    HAL_DMA_IRQHandler(adc.DMA_Handle);
//    // }

// }








// #include <uq.hpp>
// namespace uq = cycfi::uq;

// #include "stm32h7xx_hal.h"
// #include "stm32h7xx_nucleo_144.h"


// /* ADC handle declaration */
// ADC_HandleTypeDef             AdcHandle;

// /* Variable containing ADC conversions data */
// ALIGN_32BYTES (static uint16_t   aADCxConvertedData[32]);

// int main(void)
// {
//    uq::init();

//  /* This sample code shows how to convert an analog input and read the converted
//     data using DMA transfer.
//     To proceed, 4 steps are required: */

//   /* Initialize LED3 */
//   BSP_LED_Init(LED3);

//   /* ### - 1 - Initialize ADC peripheral #################################### */
//   AdcHandle.Instance          = ADC1;
//   if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
//   {
//     /* ADC de-initialization Error */
//     // Error_Handler();
//   }

//   AdcHandle.Init.ClockPrescaler           = ADC_CLOCK_SYNC_PCLK_DIV4;      /* Synchronous clock mode, input ADC clock divided by 4*/
//   AdcHandle.Init.Resolution               = ADC_RESOLUTION_16B;            /* 16-bit resolution for converted data */
//   AdcHandle.Init.ScanConvMode             = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
//   AdcHandle.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */
//   AdcHandle.Init.LowPowerAutoWait         = DISABLE;                       /* Auto-delayed conversion feature disabled */
//   AdcHandle.Init.ContinuousConvMode       = ENABLE;                        /* Continuous mode enabled (automatic conversion restart after each conversion) */
//   AdcHandle.Init.NbrOfConversion          = 1;                             /* Parameter discarded because sequencer is disabled */
//   AdcHandle.Init.DiscontinuousConvMode    = DISABLE;                       /* Parameter discarded because sequencer is disabled */
//   AdcHandle.Init.NbrOfDiscConversion      = 1;                             /* Parameter discarded because sequencer is disabled */
//   AdcHandle.Init.ExternalTrigConv         = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
//   AdcHandle.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
//   AdcHandle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR; /* ADC DMA circular requested */
//   AdcHandle.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */
//   AdcHandle.Init.OversamplingMode         = DISABLE;                       /* No oversampling */
//   AdcHandle.Init.BoostMode                = ENABLE;                        /* Enable Boost mode as ADC clock frequency is bigger than 20 MHz */
//   /* Initialize ADC peripheral according to the passed parameters */
//   if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
//   {
//     // Error_Handler();
//   }


//   /* ### - 2 - Start calibration ############################################ */
//   if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
//   {
//     //Error_Handler();
//   }

//   /* ### - 3 - Channel configuration ######################################## */

//   /* ADC channel configuration structure declaration */
//   ADC_ChannelConfTypeDef        sConfig;

//   sConfig.Channel      = ADC_CHANNEL_3;                /* Sampled channel number */
//   sConfig.Rank         = ADC_REGULAR_RANK_1;          /* Rank of sampled channel number ADC_CHANNEL_3 */
//   sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;   /* Sampling time (number of clock cycles unit) */
//   sConfig.SingleDiff   = ADC_SINGLE_ENDED;            /* Single-ended input channel */
//   sConfig.OffsetNumber = ADC_OFFSET_NONE;             /* No offset subtraction */
//   sConfig.Offset = 0;                                 /* Parameter discarded because offset correction is disabled */
//   if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
//   {
//     //Error_Handler();
//   }

//   /* ### - 4 - Start conversion in DMA mode ################################# */
//   if (HAL_ADC_Start_DMA(&AdcHandle,
//                         (uint32_t *)aADCxConvertedData,
//                         32
//                        ) != HAL_OK)
//   {
//     //Error_Handler();
//   }

//   /* Infinite Loop */
//   while (1)
//   {
//   }
// }

// extern "C"
// {
//    void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
//    {
//       GPIO_InitTypeDef          GPIO_InitStruct;
//       static DMA_HandleTypeDef         DmaHandle;

//       /*##-1- Enable peripherals and GPIO Clocks #################################*/
//       /* Enable GPIO clock ****************************************/
//       __HAL_RCC_GPIOA_CLK_ENABLE();
//       /* ADC Periph clock enable */
//       __HAL_RCC_ADC12_CLK_ENABLE();
//       /* ADC Periph interface clock configuration */
//       __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);
//       /* Enable DMA clock */
//       __HAL_RCC_DMA1_CLK_ENABLE();

//       /*##- 2- Configure peripheral GPIO #########################################*/
//       /* ADC Channel GPIO pin configuration */
//       GPIO_InitStruct.Pin = GPIO_PIN_6;
//       GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//       GPIO_InitStruct.Pull = GPIO_NOPULL;
//       HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//       /*##- 3- Configure DMA #####################################################*/

//       /*********************** Configure DMA parameters ***************************/
//       DmaHandle.Instance                 = DMA1_Stream1;
//       DmaHandle.Init.Request             = DMA_REQUEST_ADC1;
//       DmaHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
//       DmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
//       DmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
//       DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//       DmaHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
//       DmaHandle.Init.Mode                = DMA_CIRCULAR;
//       DmaHandle.Init.Priority            = DMA_PRIORITY_MEDIUM;
//       /* Deinitialize  & Initialize the DMA for new transfer */
//       HAL_DMA_DeInit(&DmaHandle);
//       HAL_DMA_Init(&DmaHandle);

//       /* Associate the DMA handle */
//       __HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);

//       /* NVIC configuration for DMA Input data interrupt */
//       HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 1, 0);
//       HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
//    }

//    /**
//     * @brief ADC MSP De-Initialization
//     *        This function frees the hardware resources used in this example:
//     *          - Disable the Peripheral's clock
//     *          - Revert GPIO to their default state
//     * @param hadc: ADC handle pointer
//     * @retval None
//     */
//    void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
//    {
//       /*##-1- Reset peripherals ##################################################*/
//       __HAL_RCC_ADC12_FORCE_RESET();
//       __HAL_RCC_ADC12_RELEASE_RESET();
//       /* ADC Periph clock disable
//          (automatically reset all ADC instances of the ADC common group) */
//       __HAL_RCC_ADC12_CLK_DISABLE();

//       /*##-2- Disable peripherals and GPIO Clocks ################################*/
//       /* De-initialize the ADC Channel GPIO pin */
//       HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);
//    }

//    /**
//     * @brief  Conversion complete callback in non-blocking mode
//     * @param  hadc: ADC handle
//     * @retval None
//     */
//    void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
//    {
//    /* Invalidate Data Cache to get the updated content of the SRAM on the first half of the ADC converted data buffer: 32 bytes */
//       SCB_InvalidateDCache_by_Addr((uint32_t *) &aADCxConvertedData[0], 32);
//    }

//    /**
//     * @brief  Conversion DMA half-transfer callback in non-blocking mode
//     * @param  hadc: ADC handle
//     * @retval None
//     */
//    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
//    {
//       /* Invalidate Data Cache to get the updated content of the SRAM on the second half of the ADC converted data buffer: 32 bytes */
//       SCB_InvalidateDCache_by_Addr((uint32_t *) &aADCxConvertedData[32/2], 32);
//    }

//    /**
//    * @brief  This function handles DMA1_Stream1_IRQHandler interrupt request.
//    * @param  None
//    * @retval None
//    */
//    void DMA1_Stream1_IRQHandler(void)
//    {
//       HAL_DMA_IRQHandler(AdcHandle.DMA_Handle);
//    }

// }
