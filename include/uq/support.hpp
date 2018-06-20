/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(CYCFI_UQ_SUPPORT_HPP_DECEMBER_20_2015)
#define CYCFI_UQ_SUPPORT_HPP_DECEMBER_20_2015

#include <type_traits>
#include <cstdint>
#include <cstring>
#include <limits>
#include <algorithm>
#include <cassert>
#include <stm32h7xx_hal.h>

namespace cycfi { namespace uq
{
   ////////////////////////////////////////////////////////////////////////////
   // Basic metaprogramming utils
   ////////////////////////////////////////////////////////////////////////////
   template <bool b>
   using bool_ = std::integral_constant<bool, b>;

   template <int i>
   using int_ = std::integral_constant<int, i>;

   template <std::size_t i>
   using uint_ = std::integral_constant<std::size_t, i>;

   template <int8_t i>
   using int8_ = std::integral_constant<std::int8_t, i>;

   template <uint8_t i>
   using uint8_ = std::integral_constant<std::uint8_t, i>;

   template <int16_t i>
   using int16_ = std::integral_constant<std::int16_t, i>;

   template <uint16_t i>
   using uint16_ = std::integral_constant<std::uint16_t, i>;

   template <int32_t i>
   using int32_ = std::integral_constant<std::int32_t, i>;

   template <std::uint32_t i>
   using uint32_ = std::integral_constant<std::uint32_t, i>;

   template <int64_t i>
   using int64_ = std::integral_constant<std::int64_t, i>;

   template <uint64_t i>
   using uint64_ = std::integral_constant<std::uint64_t, i>;

   template <typename T>
   struct identity
   {
      using type = T;
   };

   ////////////////////////////////////////////////////////////////////////////
   // The MCU clock speed
   ////////////////////////////////////////////////////////////////////////////
   inline std::uint32_t const clock_speed()
   {
      return SystemCoreClock;
   }

   ////////////////////////////////////////////////////////////////////////////
   // delay_ms function
	////////////////////////////////////////////////////////////////////////////
   inline void delay_ms(std::uint32_t ms)
   {
	   HAL_Delay(ms);
   }

   ////////////////////////////////////////////////////////////////////////////
   // Returns the current system tick, configured as milliseconds.
   ////////////////////////////////////////////////////////////////////////////
   inline auto millis()
   {
	   return HAL_GetTick();
   }

   ////////////////////////////////////////////////////////////////////////////
   // Disable and enable interrupt
	////////////////////////////////////////////////////////////////////////////
   struct disable_irq
   {
      disable_irq()
       : _state(__get_PRIMASK() != 0)
      {
         __disable_irq();
      }

      ~disable_irq()
      {
         if (!_state)
            __enable_irq();
      }

      bool _state;
   };

   ////////////////////////////////////////////////////////////////////////////
   // error_handler
   ////////////////////////////////////////////////////////////////////////////
   void error_handler();

   ////////////////////////////////////////////////////////////////////////////
   // debouncer
   ////////////////////////////////////////////////////////////////////////////
   template <std::size_t delay = 100 /*ms*/>
   struct debouncer
   {
      constexpr debouncer()
       : _time(millis())
      {}

      inline bool operator()(bool state)
      {
         if (state)
         {
            auto now = millis();
            auto elapsed = now - _time;
            _time = now;
            if (elapsed > delay)
               return true;
         }
         return false;
      }

      inline bool operator()()
      {
         return (*this)(true);
      }

      std::uint32_t _time;
   };

   ////////////////////////////////////////////////////////////////////////////
   // Practical and efficient to_string implementation.
   //
   //    n:    Integer to convert. frac: Number of decimal places.
   //
   //    Scale it by frac if you need fractional digits. For example, given
   //    n: 1234 and frac: 2, the result is "12.34"
   ////////////////////////////////////////////////////////////////////////////
   template <typename T, std::size_t frac_>
   struct to_string_impl
   {
      to_string_impl(T n)
      {
         auto frac = size-frac_-1;
         auto i = size-1;
         auto sign = n;
         n = std::abs(n);

         while (true)
         {
            if (frac && i == frac)
               s[i--] = '.';

            s[i--] = n % 10 + '0';

            if ((n /= 10) == 0)
            {
               if (i >= frac)
               {
                  while (i != frac)
                     s[i--] = '0';
                  s[i--] = '.';
                  s[i--] = '0';
               }
               break;
            }
         }

         if (sign < 0)
            s[i--] = '-';
         auto len = size-i-1;
         std::memmove(s, s+i+1, len);
         s[len] = '\0';
      }

      static constexpr std::size_t size = std::numeric_limits<T>::digits10 + 2;
      static_assert(frac_+2 < size, "frac is too big");

      char s[size];
   };

   template <std::size_t frac = 0, typename T>
   auto to_string(T n)
   {
      return to_string_impl<T, frac>{n}.s;
   }
}}

///////////////////////////////////////////////////////////////////////////////
// Interrupts (These should be placed in the global scope)
///////////////////////////////////////////////////////////////////////////////
struct irq_not_handled {};

template <typename Key>
inline irq_not_handled irq(Key)
{
	return {};
}

#endif
