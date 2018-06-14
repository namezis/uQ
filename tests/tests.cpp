/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define GPIO_TEST1

#ifdef GPIO_TEST1
# include "gpio1.cpp"
#endif

#ifdef GPIO_TEST2
# include "gpio2.cpp"
#endif

#ifdef GPIO_TEST3
# include "gpio3.cpp"
#endif

#ifdef TIMER_TEST1
# include "timer1.cpp"
#endif

#ifdef ADC_TEST1
# include "adc1.cpp"
#endif
