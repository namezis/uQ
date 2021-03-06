/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq.hpp>

namespace uq = cycfi::uq;

///////////////////////////////////////////////////////////////////////////////
// Interrupt button test. The the dev-board's main button is configured with
// a pull-down to ground (hence normally 0). The button is also configured to
// fire up an interrupt on the rising edge (when the button is pressed, it
// transitions from 0 to 1). An exti_task is setup to handle this interrupt.
// The task simply toggles the dev-board's main led. No setup required.
//
// For demonstration only. Typically, buttons are polled. Using interrupts
// with non hardware-debounced buttons is not a good idea because buttons
// bounce and can generate a lot of edges which invoke the interrupt function
// many times.
///////////////////////////////////////////////////////////////////////////////

uq::main_led      led;
uq::debouncer<>   debounce;
uq::main_btn      btn(uq::port::rising_edge); // interrupt on rising edge

using exti_task_id = exti_task<uq::main_btn::pin>;

int main()
{
   while (true) {}
}

void irq(exti_task_id)
{
   if (debounce())
      led = !led;
}

// The actual "C" interrupt handlers are defined here:
#include <uq/irq.hpp>
