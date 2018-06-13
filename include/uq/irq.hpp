/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(CYCFI_QU_IRQ_HPP_DECEMBER_22_2015)
#define CYCFI_QU_IRQ_HPP_DECEMBER_22_2015

#include <uq/gpio.hpp>
#include <uq/adc.hpp>


namespace cycfi { namespace uq { namespace detail
{
   ////////////////////////////////////////////////////////////////////////////
   // Timer Interrupts
   ////////////////////////////////////////////////////////////////////////////

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
   ////////////////////////////////////////////////////////////////////////////
   // External interrupts
   ////////////////////////////////////////////////////////////////////////////
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

   ////////////////////////////////////////////////////////////////////////////
   // Timer interrupts
   ////////////////////////////////////////////////////////////////////////////
#define UQ_TIM_GET_FLAG(timer, flag)                                          \
   (((timer)->SR & (flag)) == (flag))
   /***/

#define UQ_TIM_GET_IT_SOURCE(timer, interrupt)                                \
   ((((timer)->DIER & (interrupt)) == (interrupt)) ? SET : RESET)
   /***/

#define UQ_TIM_CLEAR_IT(timer, interrupt)                                     \
   ((timer)->SR = ~(uint32_t)(interrupt))
   /***/

#define TIMER_INTERRUPT_HANDLER(N)                                            \
   void TIM##N##_IRQHandler(void)                                             \
   {                                                                          \
      if (UQ_TIM_GET_FLAG(TIM##N, TIM_IT_UPDATE) != RESET)                    \
      {                                                                       \
         if (UQ_TIM_GET_IT_SOURCE(TIM##N, TIM_IT_UPDATE) !=RESET)             \
         {                                                                    \
            UQ_TIM_CLEAR_IT(TIM##N, TIM_IT_UPDATE);                           \
            irq(timer_task<N>{});                                             \
         }                                                                    \
      }                                                                       \
   }                                                                          \
   /***/

#if defined(TIM2)
 TIMER_INTERRUPT_HANDLER(2)
#endif

#if defined(TIM3)
 TIMER_INTERRUPT_HANDLER(3)
#endif

#if defined(TIM4)
 TIMER_INTERRUPT_HANDLER(4)
#endif

#if defined(TIM5)
 TIMER_INTERRUPT_HANDLER(5)
#endif

#if defined(TIM7)
 TIMER_INTERRUPT_HANDLER(7)
#endif

#if defined(TIM15)
 TIMER_INTERRUPT_HANDLER(15)
#endif

#if defined(TIM16)
 TIMER_INTERRUPT_HANDLER(16)
#endif

#if defined(TIM17)
 TIMER_INTERRUPT_HANDLER(17)
#endif

   ////////////////////////////////////////////////////////////////////////////
   // ADC Interrupts
   ////////////////////////////////////////////////////////////////////////////
   void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
   {
      // Invalidate Data Cache to get the updated content of the SRAM on the
      // first half of the ADC converted data buffer.
      using namespace cycfi::uq::detail;
      auto adc = static_cast<adc_base*>(hadc);
      auto size = adc._size / 2;
      SCB_InvalidateDCache_by_Addr(adc._pdata, size);
   }

   void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
   {
      // Invalidate Data Cache to get the updated content of the SRAM on the
      // second half of the ADC converted data buffer.
      using namespace cycfi::uq::detail;
      auto adc = static_cast<adc_base*>(hadc);
      auto size = adc._size / 2;
      SCB_InvalidateDCache_by_Addr(adc._pdata + size, size);
   }
}

#endif
