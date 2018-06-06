/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq.hpp>

namespace uq = cycfi::uq;

///////////////////////////////////////////////////////////////////////////////
// Simplest button test. We poll the dev-board's main button, which is
// configured with a pull-down to ground, hence normally 0. The dev-board's
// main LED is turned ON if the button is pushed, otherwise the main led is
// turned OFF. No setup required.
///////////////////////////////////////////////////////////////////////////////

int main()
{
   uq::init();

   uq::main_led      led;
   uq::main_btn      btn;

   while (true)
   {
      uq::delay_ms(30);
      led = btn;
   }
}
