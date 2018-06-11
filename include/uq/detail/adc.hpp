/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(CYCFI_UQ_ADC_DETAIL_HPP_DECEMBER_31_2015)
#define CYCFI_UQ_ADC_DETAIL_HPP_DECEMBER_31_2015

#include "stm32h7xx_hal.h"
#include <cstdint>

namespace cycfi { namespace uq { namespace detail
{
   struct adc_base : ADC_HandleTypeDef
   {
      void              construct(ADC_TypeDef* adc);
      void              dma_setup();

      void              enable_channel(
                           GPIO_TypeDef* gpio
                         , std::uint32_t pin
                         , uint32_t channel
                         , uint32_t rank);

      DMA_HandleTypeDef _dma_handle;
   };

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


}}}

#endif
