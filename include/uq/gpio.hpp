/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(CYCFI_UQ_GPIO_HPP_JUNE_5_2018)
#define CYCFI_UQ_GPIO_HPP_JUNE_5_2018

#include <cstdint>
#include <uq/detail/gpio.hpp>
#include <uq/startup.hpp>

////////////////////////////////////////////////////////////////////////////
// Interrupt Key (This should be placed in the global scope)
////////////////////////////////////////////////////////////////////////////
template <std::size_t N>
struct exti_task {};

namespace cycfi { namespace uq
{
   ////////////////////////////////////////////////////////////////////////////
   // Constants
   ////////////////////////////////////////////////////////////////////////////
   enum class port_enum
   {
      a, b, c, d, e, f, g, h, i, j, k
   };

   enum class port_speed_enum
   {
      // 2MHz low speed
      low_speed = GPIO_SPEED_FREQ_LOW,

      // 12.5 MHz to 50 MHz medium speed
      mid_speed  = GPIO_SPEED_FREQ_MEDIUM,

      // 25 MHz to 100 MHz high speed
      high_speed = GPIO_SPEED_FREQ_HIGH,

      // 50 MHz to 200 MHz very high speed
      very_high_speed = GPIO_SPEED_FREQ_VERY_HIGH
   };

   enum class port_mode_enum
   {
      push_pull = GPIO_MODE_OUTPUT_PP     // Output Push Pull Mode
    , open_drain = GPIO_MODE_OUTPUT_OD    // Output Open Drain Mode
   };

   enum class port_pull_enum
   {
      none = GPIO_NOPULL                  // No Pull-up or Pull-down
    , pull_up = GPIO_PULLUP               // Pull-up
    , pull_down = GPIO_PULLDOWN           // Pull-down
   };

   enum class port_edge_enum
   {
      rising = GPIO_MODE_IT_RISING        // Rising edge
    , falling = GPIO_MODE_IT_FALLING      // Falling edge
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

      auto constexpr rising_edge          = port_edge_enum::rising;
      auto constexpr falling_edge         = port_edge_enum::falling;
   }

   ////////////////////////////////////////////////////////////////////////////
   // Support
   ////////////////////////////////////////////////////////////////////////////
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

   template <typename T>
   struct inverse_port : port_base<inverse_port<T>>
   {
	   bool state() const
      {
         return port.state();
      }

      T port;
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

   ////////////////////////////////////////////////////////////////////////////
   // output_port
   ////////////////////////////////////////////////////////////////////////////
   template <
      port_enum port_
    , std::uint8_t pin_
    , port_speed_enum speed_ = port::high_speed
    , port_mode_enum mode_ = port::push_pull
    , port_pull_enum pull_ = port::no_pull
   >
   struct output_port : port_base<output_port<port_, pin_, speed_, mode_, pull_>>
   {
      static_assert(pin_ < 16, "Invalid pin number.");

      using self_type = output_port;
      using inverse_type = inverse_port<output_port>;

      constexpr static port_enum port = port_;
      constexpr static std::uint8_t pin = pin_;
      constexpr static port_speed_enum speed = speed_;
      constexpr static port_mode_enum mode = mode_;
      constexpr static port_pull_enum pull = pull_;
      constexpr static uint32_t mask = 1 << pin;
      constexpr static auto out = detail::gpio(port);

      output_port()
      {
         init();  // Initialize the system

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
         return (out->ODR & mask) != 0;
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

      output_port& operator=(inverse_type)
      {
         out->ODR ^= mask;
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
   };

   ////////////////////////////////////////////////////////////////////////////
   // Dev boards typically have a main led:
   using main_led = output_port<port::portb, 0>;

   ////////////////////////////////////////////////////////////////////////////
   // input_port
   ////////////////////////////////////////////////////////////////////////////
   template <
      port_enum port_
    , std::uint8_t pin_
    , port_pull_enum pull_ = port::no_pull
   >
   struct input_port : port_base<input_port<port_, pin_, pull_>>
   {
      static_assert(pin_ < 16, "Invalid pin number.");

      using self_type = input_port;

      constexpr static port_enum port = port_;
      constexpr static std::uint8_t pin = pin_;
      constexpr static port_pull_enum pull = pull_;
      constexpr static uint32_t mask = 1 << pin;
      constexpr static auto in = detail::gpio(port);

      input_port()
      {
         init();  // Initialize the system

         detail::enable_gpio(port);
         GPIO_InitTypeDef init =
         {
            std::uint32_t(mask)
          , GPIO_MODE_INPUT
          , std::uint32_t(pull)
          , GPIO_SPEED_FREQ_VERY_HIGH
         };
         HAL_GPIO_Init(in, &init);
      }

      input_port(port_edge_enum edge, std::size_t priority = 0)
      {
         detail::enable_gpio(port);
         GPIO_InitTypeDef init =
         {
            std::uint32_t(mask)
          , std::uint32_t(edge)
          , std::uint32_t(pull)
          , GPIO_SPEED_FREQ_VERY_HIGH
         };
         HAL_GPIO_Init(in, &init);

         HAL_NVIC_SetPriority(detail::exti_irq<pin>(), priority, 0);
         HAL_NVIC_EnableIRQ(detail::exti_irq<pin>());
      }

      ~input_port()
      {
         HAL_GPIO_DeInit(in, pin);
      }

      bool state() const
      {
         return (in->IDR & mask) != 0;
      }
   };

   ////////////////////////////////////////////////////////////////////////////
   // Dev boards typically have a main button:
   using main_btn = input_port<port::portc, 13, port::pull_down>;
}}

#endif
