/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq.hpp>

namespace uq = cycfi::uq;

///////////////////////////////////////////////////////////////////////////////
// Toggle led test using timers and interrupts. This test uses a timer to
// toggle the dev-board's main led at a rate of 1 per second. No setup
// required.
//
// The timer used has a clock frequency of 10kHz and a cycle period of 10000
// ticks. Hence, 10kHz/10000 == 1 second.
///////////////////////////////////////////////////////////////////////////////

uq::main_led   led;
uq::timer<3>   timer(10000, 10000);

int main(void)
{
   timer.enable_interrupt();
   timer.start();
   while (true) {}
}

void irq(timer_task<3>)
{
  led = !led;
}

// The actual "C" interrupt handlers are defined here:
#include <uq/irq.hpp>
