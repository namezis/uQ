/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq.hpp>

namespace uq = cycfi::uq;

int main()
{
   uq::init();

   uq::main_led led;
   led = 1;

   while (true)
   {
      HAL_Delay(1000);
      led = !led;
   }
}
