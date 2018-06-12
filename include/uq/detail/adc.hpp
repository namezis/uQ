/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(CYCFI_UQ_ADC_DETAIL_HPP_DECEMBER_31_2015)
#define CYCFI_UQ_ADC_DETAIL_HPP_DECEMBER_31_2015

#include "stm32h7xx_hal.h"
#include <cstdint>
#include <uq/support.hpp>
#include <uq/startup.hpp>

namespace cycfi { namespace uq { namespace detail
{
   struct adc_base : ADC_HandleTypeDef
   {
                        template <std::size_t id>
      void              setup();

      void              adc_setup(ADC_TypeDef* adc);
      void              dma_setup();

      void              enable_channel(
                           GPIO_TypeDef* gpio
                         , std::uint32_t pin
                         , uint32_t channel
                         , uint32_t rank);

      DMA_HandleTypeDef _dma_handle;
   };

   inline void enable_adc_clock(std::size_t id)
   {
      switch (id)
      {
         case 1:
         case 2:
            __HAL_RCC_ADC12_CLK_ENABLE();
            break;

         case 3:
            __HAL_RCC_ADC3_CLK_ENABLE();
            break;
      }

      // ADC Periph interface clock configuration
      __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);
   }

   constexpr ADC_TypeDef* get_adc(std::size_t id)
   {
      switch (id)
      {
         case 1: return ADC1;
         case 2: return ADC2;
         case 3: return ADC3;
      }
      return 0;
   }

   constexpr DMA_Stream_TypeDef* get_dma_stream(std::size_t id)
   {
      switch (id)
      {
         case 1: return DMA1_Stream1;
         case 2: return DMA1_Stream2;
         case 3: return DMA1_Stream3;
      }
      return 0; // can't happen (we have a static assert in adc class)
   }

   constexpr std::uint32_t get_dma_request(std::size_t id)
   {
      switch (id)
      {
         case 1: return DMA_REQUEST_ADC1;
         case 2: return DMA_REQUEST_ADC2;
         case 3: return DMA_REQUEST_ADC3;
      }
      return 0; // can't happen (we have a static assert in adc class)
   }

   constexpr IRQn_Type get_dma_irq_n(std::size_t id)
   {
      switch (id)
      {
         case 1: return DMA1_Stream1_IRQn;
         case 2: return DMA1_Stream2_IRQn;
         case 3: return DMA1_Stream3_IRQn;
      }
      return IRQn_Type(0); // can't happen (we a have static assert in adc class)
   }

   template <std::size_t id>
   void adc_base::setup()
   {
      uq::init();
      enable_adc_clock(id);

      // Configure DMA
      constexpr auto dma_stream = get_dma_stream(id);
      constexpr auto dma_request = get_dma_request(id);
      _dma_handle.Instance = dma_stream;
      _dma_handle.Init.Request = dma_request;
      adc_base::dma_setup();

      // NVIC configuration for DMA Input data interrupt
      constexpr auto dma_irq_n = get_dma_irq_n(id);
      HAL_NVIC_SetPriority(dma_irq_n, 1, 0);
      HAL_NVIC_EnableIRQ(dma_irq_n);

      // Configure ADC
      constexpr auto adc = detail::get_adc(id);
      adc_base::adc_setup(adc);
   }
}}}

#endif
