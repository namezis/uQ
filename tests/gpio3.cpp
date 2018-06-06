/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq.hpp>

namespace uq = cycfi::uq;

///////////////////////////////////////////////////////////////////////////////
// Interrupt button test. The the dev-board's main button button is
// configured with a pull-down to ground (hence normally 0). The button is
// also configured to fire up an interrupt on the rising edge (when the
// button is pressed, it transitions from 0 to 1). An exti_task is setup to
// handle this interrupt. The task simply toggles the main LED. No setup
// required.
///////////////////////////////////////////////////////////////////////////////

uq::main_led      led;
uq::debouncer<>   debounce;
uq::main_btn      btn(uq::port::rising_edge); // interrupt on rising edge

int main()
{
   uq::init();
   while (true) {}
}

void irq(exti_task<13>)
{
   if (debounce())
      led = !led;
}

// The actual "C" interrupt handlers are defined here:
#include <uq/irq.hpp>
