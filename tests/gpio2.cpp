/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq.hpp>

namespace uq = cycfi::uq;

///////////////////////////////////////////////////////////////////////////////
// Toggle the main LED on button press (using the dev-board's main button)
///////////////////////////////////////////////////////////////////////////////

int main()
{
   uq::init();

   uq::main_led      led;
   uq::main_btn      btn;
   uq::debouncer<>   debounce;

   while (true)
   {
      if (debounce(btn))
         led = !led;
   }
}
