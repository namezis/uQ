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
      virtual        ~adc_base() = default;
      void           construct();
      void           setup();
      void           destruct();
      virtual void   init() = 0;
   };
}}}

#endif
