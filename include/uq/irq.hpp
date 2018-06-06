/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(CYCFI_QU_IRQ_HPP_DECEMBER_22_2015)
#define CYCFI_QU_IRQ_HPP_DECEMBER_22_2015

#include <uq/gpio.hpp>
// #include <inf/timer.hpp>
// #include <inf/adc.hpp>
// #include <type_traits>

// #if defined(STM32F4)
// # include <stm32f4xx_ll_dma.h>
// # include <stm32f4xx_ll_adc.h>
// #else
// # error "MCU not supported"
// #endif

///////////////////////////////////////////////////////////////////////////////
// Timer Interrupts
///////////////////////////////////////////////////////////////////////////////

namespace cycfi { namespace uq { namespace detail
{
   template <std::size_t N>
   void handle_exti()
   {
      static_assert(N < 16, "Invalid EXTI ID");

      // If the interrupt task is not handled anyway, this will be compiled
      // away by the compiler.

      constexpr auto pin = 1 << N;

      if (!std::is_same<decltype(irq(exti_task<N>{})), irq_not_handled>::value)
      {
         // Manage Flags
         if (__HAL_GPIO_EXTI_GET_IT(pin) != RESET)
         {
            // Clear EXTI flag
            __HAL_GPIO_EXTI_CLEAR_IT(pin);

            // Handle exti task
            irq(exti_task<N>{});
         }
      }
   }
}}}

extern "C"
{
   void EXTI0_IRQHandler(void)
   {
      cycfi::uq::detail::handle_exti<0>();
   }

   void EXTI1_IRQHandler(void)
   {
      cycfi::uq::detail::handle_exti<1>();
   }

   void EXTI2_IRQHandler(void)
   {
      cycfi::uq::detail::handle_exti<2>();
   }

   void EXTI3_IRQHandler(void)
   {
      cycfi::uq::detail::handle_exti<3>();
   }

   void EXTI4_IRQHandler(void)
   {
      cycfi::uq::detail::handle_exti<4>();
   }

   void EXTI9_5_IRQHandler(void)
   {
      cycfi::uq::detail::handle_exti<5>();
      cycfi::uq::detail::handle_exti<6>();
      cycfi::uq::detail::handle_exti<7>();
      cycfi::uq::detail::handle_exti<8>();
      cycfi::uq::detail::handle_exti<9>();
      cycfi::uq::detail::handle_exti<10>();
   }

   void EXTI15_10_IRQHandler(void)
   {
      cycfi::uq::detail::handle_exti<10>();
      cycfi::uq::detail::handle_exti<11>();
      cycfi::uq::detail::handle_exti<12>();
      cycfi::uq::detail::handle_exti<13>();
      cycfi::uq::detail::handle_exti<14>();
      cycfi::uq::detail::handle_exti<15>();
   }
}

#endif
