/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq.hpp>

namespace uq = cycfi::uq;

///////////////////////////////////////////////////////////////////////////////
// Acquire analog inputs using the ADC set to get data at a rate of 100
// samples per second. The timer used has a clock frequency of 10kHz and a
// cycle period of 100 ticks. Hence, 10kHz/100 == 100Hz.
//
// Setup: Connect a 3.3v voltage divider using a potentiometer to pin PA6.
// Run the program. Set a breakpoint in either irq functions and inspect the
// adc._data.
///////////////////////////////////////////////////////////////////////////////
uq::timer<3>      timer(10000, 100);
uq::adc<1, 1, 32> adc(timer);

int main()
{
   adc.enable_channels<3>();
   adc.start();
   timer.start();
   while (true) {}
}

void irq(adc_conversion_half_complete<1>)
{
   // Conversion half complete
}

void irq(adc_conversion_complete<1>)
{
   // Conversion complete
}

// The actual "C" interrupt handlers are defined here:
#include <uq/irq.hpp>

