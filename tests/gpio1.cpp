/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq.hpp>

namespace uq = cycfi::uq;

///////////////////////////////////////////////////////////////////////////////
// Toggle the LED once every second (using the dev-board's main LED)
///////////////////////////////////////////////////////////////////////////////

int main()
{
   uq::init();
   uq::main_led led;

   while (true)
   {
      uq::delay_ms(1000);
      led = !led;
   }
}
