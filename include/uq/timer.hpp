/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(CYCFI_UQ_TIMER_HPP_DECEMBER_21_2015)
#define CYCFI_UQ_TIMER_HPP_DECEMBER_21_2015

#include <uq/detail/timer.hpp>
#include <uq/support.hpp>
#include <uq/startup.hpp>

////////////////////////////////////////////////////////////////////////////
// Interrupt Key (This should be placed in the global scope)
////////////////////////////////////////////////////////////////////////////
template <std::size_t N>
struct timer_task {};

namespace cycfi { namespace uq
{
   template <std::size_t N>
   class timer : public detail::timer_base
   {
   public:

      constexpr static auto timer_base = detail::get_timer(N);
      static_assert(timer_base, "Invalid Timer N");

      timer(uint32_t clock_frequency, uint32_t period)
      {
         uq::init(); // Initialize the system
         detail::enable_peripheral_clock<N>();

         auto const periph_clock = detail::timer_clock_div(N) * clock_frequency;
         auto const prescaler    = (clock_speed() / periph_clock) - 1;

         Init.Period             = period - 1;
         Init.Prescaler          = prescaler;
         Init.ClockDivision      = 0;
         Init.CounterMode        = TIM_COUNTERMODE_UP;
         Init.RepetitionCounter  = 0;

         Instance = timer_base;
         if (HAL_TIM_Base_Init(this) != HAL_OK)
            error_handler();
      }

      ~timer()
      {
         HAL_TIM_Base_MspDeInit(this);
      }

      void enable_interrupt(std::size_t priority = 0)
      {
         HAL_NVIC_SetPriority(detail::get_timer_irq_id<N>(), priority, 0);
         HAL_NVIC_EnableIRQ(detail::get_timer_irq_id<N>());
      }

      void start()
      {
         if (HAL_TIM_Base_Start_IT(this) != HAL_OK)
            error_handler();
      }

      void stop()
      {
         if (HAL_TIM_Base_Stop_IT(this) != HAL_OK)
            error_handler();
      }
   };


}}

#endif
