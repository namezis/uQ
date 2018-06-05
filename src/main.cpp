/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq/startup.hpp>

#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo_144.h"

namespace uq = cycfi::uq;

int main()
{
   uq::init();
   BSP_LED_Init(LED1);

   while (true)
   {
      HAL_Delay(1000);
      BSP_LED_Toggle(LED1);
   }
}
