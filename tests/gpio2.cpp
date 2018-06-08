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
// main LED is toggled on each button press. The button is debounced in
// software. No setup required.
///////////////////////////////////////////////////////////////////////////////

int main()
{
   uq::init();

   uq::main_led      led;
   uq::main_btn      btn;
   uq::debouncer<>   debounce;

   while (true)
   {
      uq::delay_ms(30);
      if (debounce(btn))
         led = !led;
   }
}
