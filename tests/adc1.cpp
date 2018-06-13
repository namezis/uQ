/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <uq.hpp>

namespace uq = cycfi::uq;

uq::adc<1, 1, 32> adc;

int main()
{
   adc.enable_channels<3>();
   adc.start();
   while (true) {}
}

void irq(adc_conversion_half_complete<1>)
{
}

void irq(adc_conversion_complete<1>)
{
}

// The actual "C" interrupt handlers are defined here:
#include <uq/irq.hpp>

