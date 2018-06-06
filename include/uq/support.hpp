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
   // Quick to_string implementation.
   //
   //    n: integer to convert, scaled it by frac if you need
   //       fractional digits.
   //    s: the output string. should hold enough chars to represent
   //       the digits.
   //    frac: number of decimal places.
   //
   //    For example, if n: 1234 and frac: 2, the result is 12.34
   ////////////////////////////////////////////////////////////////////////////
   inline void reverse(char s[])
   {
      int i, j;
      for (i = 0, j = std::strlen(s)-1; i<j; i++, j--)
      {
         char c = s[i];
         s[i] = s[j];
         s[j] = c;
      }
   }

   inline void to_string(int n, char s[], int frac = 0)
   {
      int i, sign;

      if ((sign = n) < 0)                 // record sign
         n = -n;                          // make n positive
      i = 0;
      while (true)                        // generate digits in reverse order
      {
         if (frac && i == frac)
            s[i++] = '.';
         s[i++] = n % 10 + '0';           // get next digit

         if ((n /= 10) == 0)
         {
            if (i <= frac)
            {
               while (i != frac)
                  s[i++] = '0';
               s[i++] = '.';
               s[i++] = '0';
            }
            break;
         }
      }

      if (sign < 0)
         s[i++] = '-';
      s[i] = '\0';
      reverse(s);
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
