/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(CYCFI_UQ_DETAIL_GPIO_HPP_JUNE_5_2018)
#define CYCFI_UQ_DETAIL_GPIO_HPP_JUNE_5_2018

#include <stm32h7xx_hal.h>

namespace cycfi { namespace uq { namespace detail
{
   template <typename Port>
   constexpr GPIO_TypeDef* gpio(Port port)
   {
      switch (int(port))
      {
         case 0:  return GPIOA;
         case 1:  return GPIOB;
         case 2:  return GPIOC;
         case 3:  return GPIOD;
         case 4:  return GPIOE;
         case 5:  return GPIOF;
         case 6:  return GPIOG;
         case 7:  return GPIOH;
         case 8:  return GPIOI;
         case 9:  return GPIOJ;
         case 10: return GPIOK;
      }
      return 0;
   }

   template <typename Port>
   inline void enable_gpio(Port port)
   {
      switch (int(port))
      {
         case 0:  __HAL_RCC_GPIOA_CLK_ENABLE(); break;
         case 1:  __HAL_RCC_GPIOB_CLK_ENABLE(); break;
         case 2:  __HAL_RCC_GPIOC_CLK_ENABLE(); break;
         case 3:  __HAL_RCC_GPIOD_CLK_ENABLE(); break;
         case 4:  __HAL_RCC_GPIOE_CLK_ENABLE(); break;
         case 5:  __HAL_RCC_GPIOF_CLK_ENABLE(); break;
         case 6:  __HAL_RCC_GPIOG_CLK_ENABLE(); break;
         case 7:  __HAL_RCC_GPIOH_CLK_ENABLE(); break;
         case 8:  __HAL_RCC_GPIOI_CLK_ENABLE(); break;
         case 9:  __HAL_RCC_GPIOJ_CLK_ENABLE(); break;
         case 10: __HAL_RCC_GPIOK_CLK_ENABLE(); break;
      }
   }

   template <typename Port>
   inline void disable_gpio(Port port)
   {
      switch (int(port))
      {
         case 0:  __HAL_RCC_GPIOA_CLK_DISABLE(); break;
         case 1:  __HAL_RCC_GPIOB_CLK_DISABLE(); break;
         case 2:  __HAL_RCC_GPIOC_CLK_DISABLE(); break;
         case 3:  __HAL_RCC_GPIOD_CLK_DISABLE(); break;
         case 4:  __HAL_RCC_GPIOE_CLK_DISABLE(); break;
         case 5:  __HAL_RCC_GPIOF_CLK_DISABLE(); break;
         case 6:  __HAL_RCC_GPIOG_CLK_DISABLE(); break;
         case 7:  __HAL_RCC_GPIOH_CLK_DISABLE(); break;
         case 8:  __HAL_RCC_GPIOI_CLK_DISABLE(); break;
         case 9:  __HAL_RCC_GPIOJ_CLK_DISABLE(); break;
         case 10: __HAL_RCC_GPIOK_CLK_DISABLE(); break;
      }
   }

   template <std::size_t const pin>
   constexpr IRQn_Type exti_irq()
   {
      static_assert(pin < 16, "Invalid pin number.");
      switch (pin)
      {
         case 0:  return EXTI0_IRQn;
         case 1:  return EXTI1_IRQn;
         case 2:  return EXTI2_IRQn;
         case 3:  return EXTI3_IRQn;
         case 4:  return EXTI4_IRQn;
         case 5:  return EXTI9_5_IRQn;
         case 6:  return EXTI9_5_IRQn;
         case 7:  return EXTI9_5_IRQn;
         case 8:  return EXTI9_5_IRQn;
         case 9:  return EXTI9_5_IRQn;
         case 10: return EXTI15_10_IRQn;
         case 11: return EXTI15_10_IRQn;
         case 12: return EXTI15_10_IRQn;
         case 13: return EXTI15_10_IRQn;
         case 14: return EXTI15_10_IRQn;
         case 15: return EXTI15_10_IRQn;
      }
   }
}}}

#endif
