/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq.hpp>

namespace uq = cycfi::uq;

///////////////////////////////////////////////////////////////////////////////
// Simplest toggle the led test. This test uses simple delay to toggle the
// dev-board's main LED at a rate of 1 per second. No setup required.
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
