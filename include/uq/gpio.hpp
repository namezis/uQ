/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(CYCFI_UQ_GPIO_HPP_JUNE_5_2018)
#define CYCFI_UQ_GPIO_HPP_JUNE_5_2018

#include <cstdint>
#include <uq/detail/gpio.hpp>

namespace cycfi { namespace uq
{
   enum class port_enum
   {
      a, b, c, d, e, f, g, h, i, j, k
   };

   enum class port_speed_enum
   {
      low_speed         = GPIO_SPEED_FREQ_LOW,        // 2MHz low speed
      mid_speed         = GPIO_SPEED_FREQ_MEDIUM,     // 12.5 MHz to 50 MHz medium speed
      high_speed        = GPIO_SPEED_FREQ_HIGH,       // 25 MHz to 100 MHz high speed
      very_high_speed   = GPIO_SPEED_FREQ_VERY_HIGH   // 50 MHz to 200 MHz very high speed
   };

   enum class port_mode_enum
   {
      push_pull = GPIO_MODE_OUTPUT_PP                 // Output Push Pull Mode
    , open_drain = GPIO_MODE_OUTPUT_OD                // Output Open Drain Mode
   };

   enum class port_pull_enum
   {
      none = GPIO_NOPULL                              // No Pull-up or Pull-down
    , pull_up = GPIO_PULLUP                           // Pull-up
    , pull_down = GPIO_PULLDOWN                       // Pull-down
   };

   namespace port
   {
      auto constexpr porta                = port_enum::a;
      auto constexpr portb                = port_enum::b;
      auto constexpr portc                = port_enum::c;
      auto constexpr portd                = port_enum::d;
      auto constexpr porte                = port_enum::e;
      auto constexpr portf                = port_enum::f;
      auto constexpr portg                = port_enum::g;
      auto constexpr porth                = port_enum::h;
      auto constexpr porti                = port_enum::i;
      auto constexpr portj                = port_enum::j;
      auto constexpr portk                = port_enum::k;

      auto constexpr low_speed            = port_speed_enum::low_speed;
      auto constexpr mid_speed            = port_speed_enum::mid_speed;
      auto constexpr high_speed           = port_speed_enum::high_speed;
      auto constexpr very_high_speed      = port_speed_enum::very_high_speed;

      auto constexpr push_pull            = port_mode_enum::push_pull;
      auto constexpr open_drain           = port_mode_enum::open_drain;

      auto constexpr no_pull              = port_pull_enum::none;
      auto constexpr pull_up              = port_pull_enum::pull_up;
      auto constexpr pull_down            = port_pull_enum::pull_down;
   }

   template <typename Derived>
   struct port_base
   {
      operator bool() const
      {
         return derived().state();
      }

      bool operator!() const
      {
         return !derived().state();
      }

      template <typename Derived2>
      Derived& operator=(port_base<Derived2> const& rhs)
      {
         return derived() = rhs.state();
      }

      Derived& derived()
      {
         return *static_cast<Derived*>(this);
      }

      Derived const& derived() const
      {
         return *static_cast<Derived const*>(this);
      }
   };

   struct on_type : port_base<on_type>
   {
      constexpr bool state() const
      {
         return true;
      }
   };

   struct off_type : port_base<off_type>
   {
      constexpr bool state() const
      {
         return false;
      }
   };

   namespace port
   {
      constexpr on_type on = {};
      constexpr off_type off = {};
   }

   template <
      port_enum port
    , std::uint8_t pin
    , port_speed_enum speed = port::high_speed
    , port_mode_enum mode = port::push_pull
    , port_pull_enum pull = port::no_pull
   >
   struct output_port : port_base<output_port<port, pin, speed, mode, pull>>
   {
      constexpr static uint32_t mask = 1 << pin;
      using self_type = output_port;

      output_port()
       : out(detail::gpio(port))
      {
         detail::enable_gpio(port);
         GPIO_InitTypeDef init =
         {
            std::uint32_t(mask)
          , std::uint32_t(mode)
          , std::uint32_t(pull)
          , std::uint32_t(speed)
         };
         HAL_GPIO_Init(out, &init);
      }

      ~output_port()
      {
         HAL_GPIO_DeInit(out, pin);
      }

      bool state() const
      {
         return (out->IDR & mask) != 0;
      }

      output_port& operator=(bool val)
      {
         if (val)
            out->BSRRL = mask;
         else
            out->BSRRH = mask;
         return *this;
      }

      output_port& operator=(self_type)
      {
         return *this;
      }

      output_port& operator=(on_type)
      {
         out->BSRRL = mask;
         return *this;
      }

      output_port& operator=(off_type)
      {
         out->BSRRL = mask;
         return *this;
      }

      GPIO_TypeDef* out = detail::gpio(port);
   };

   // Dev boards typically have a main led:
   using main_led = output_port<port::portb, 0>;
}}

#endif
