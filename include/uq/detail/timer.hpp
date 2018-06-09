/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(CYCFI_UQ_TIMER_DETAIL_HPP_DECEMBER_21_2015)
#define CYCFI_UQ_TIMER_DETAIL_HPP_DECEMBER_21_2015

#include "stm32h7xx_hal.h"
#include <cstdint>

namespace cycfi { namespace uq { namespace detail
{
   struct timer_base : TIM_HandleTypeDef {};

   constexpr TIM_TypeDef* get_timer(std::size_t id)
   {
      switch (id)
      {
         case 1:  return TIM1;
         case 2:  return TIM2;
         case 3:  return TIM3;
         case 4:  return TIM4;
         case 5:  return TIM5;
         case 6:  return TIM6;
         case 7:  return TIM7;
         case 8:  return TIM8;
         case 12: return TIM12;
         case 13: return TIM13;
         case 14: return TIM14;
         case 15: return TIM15;
         case 16: return TIM16;
         case 17: return TIM17;
      };
      return 0;
   }

   constexpr int timer_clock_div(std::size_t id)
   {
      switch (id)
      {
         case 2: case 3: case 4: case 5: case 6:
         case 7: case 12: case 13: case 14:
            return 2;

         case 1: case 8: case 15: case 16: case 17:
            return 1;
      };
      return -1;
   }

   template <std::size_t id>
   inline void enable_peripheral_clock()
   {
      switch (id)
      {
         case 1:  __HAL_RCC_TIM1_CLK_ENABLE();  break;
         case 2:  __HAL_RCC_TIM2_CLK_ENABLE();  break;
         case 3:  __HAL_RCC_TIM3_CLK_ENABLE();  break;
         case 4:  __HAL_RCC_TIM4_CLK_ENABLE();  break;
         case 5:  __HAL_RCC_TIM5_CLK_ENABLE();  break;
         case 6:  __HAL_RCC_TIM6_CLK_ENABLE();  break;
         case 7:  __HAL_RCC_TIM7_CLK_ENABLE();  break;
         case 8:  __HAL_RCC_TIM8_CLK_ENABLE();  break;
         case 12: __HAL_RCC_TIM12_CLK_ENABLE(); break;
         case 13: __HAL_RCC_TIM13_CLK_ENABLE(); break;
         case 14: __HAL_RCC_TIM14_CLK_ENABLE(); break;
         case 15: __HAL_RCC_TIM15_CLK_ENABLE(); break;
         case 16: __HAL_RCC_TIM16_CLK_ENABLE(); break;
         case 17: __HAL_RCC_TIM17_CLK_ENABLE(); break;
      };
   }

   template <std::size_t id>
   constexpr int get_timer_irq_id_main()
   {
      switch (id)
      {
         case 2:  return TIM2_IRQn;
         case 3:  return TIM3_IRQn;
         case 4:  return TIM4_IRQn;
         case 5:  return TIM5_IRQn;
         case 7:  return TIM7_IRQn;
         case 15: return TIM15_IRQn;
         case 16: return TIM16_IRQn;
         case 17: return TIM17_IRQn;
      };
      return -1;
   }

   template <std::size_t id>
   constexpr IRQn_Type get_timer_irq_id()
   {
      constexpr auto irq_id = get_timer_irq_id_main<id>();
      static_assert(irq_id != -1, "Timer has no interrupt capability.");
      return IRQn_Type(irq_id);
   }
}}}

#endif
