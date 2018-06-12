/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(CYCFI_UQ_ADC_DETAIL_HPP_DECEMBER_31_2015)
#define CYCFI_UQ_ADC_DETAIL_HPP_DECEMBER_31_2015

#include "stm32h7xx_hal.h"
#include <cstdint>
#include <uq/support.hpp>
#include <uq/startup.hpp>
#include <uq/gpio.hpp>

namespace cycfi { namespace uq { namespace detail
{
   struct adc_base : ADC_HandleTypeDef
   {
                        template <std::size_t id>
      void              setup();

      void              adc_setup(ADC_TypeDef* adc);
      void              dma_setup();

      void              enable_channel(
                           GPIO_TypeDef* gpio
                         , std::uint32_t pin
                         , uint32_t channel
                         , uint32_t rank);

      DMA_HandleTypeDef _dma_handle;
   };

   inline void enable_adc_clock(std::size_t id)
   {
      switch (id)
      {
         case 1:
         case 2:
            __HAL_RCC_ADC12_CLK_ENABLE();
            break;

         case 3:
            __HAL_RCC_ADC3_CLK_ENABLE();
            break;
      }

      // ADC Periph interface clock configuration
      __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);
   }

   constexpr ADC_TypeDef* get_adc(std::size_t id)
   {
      switch (id)
      {
         case 1: return ADC1;
         case 2: return ADC2;
         case 3: return ADC3;
      }
      return 0;
   }

   constexpr DMA_Stream_TypeDef* get_dma_stream(std::size_t id)
   {
      switch (id)
      {
         case 1: return DMA1_Stream1;
         case 2: return DMA1_Stream2;
         case 3: return DMA1_Stream3;
      }
      return 0; // can't happen (we have a static assert in adc class)
   }

   constexpr std::uint32_t get_dma_request(std::size_t id)
   {
      switch (id)
      {
         case 1: return DMA_REQUEST_ADC1;
         case 2: return DMA_REQUEST_ADC2;
         case 3: return DMA_REQUEST_ADC3;
      }
      return 0; // can't happen (we have a static assert in adc class)
   }

   constexpr IRQn_Type get_dma_irq_n(std::size_t id)
   {
      switch (id)
      {
         case 1: return DMA1_Stream1_IRQn;
         case 2: return DMA1_Stream2_IRQn;
         case 3: return DMA1_Stream3_IRQn;
      }
      return IRQn_Type(0); // can't happen (we a have static assert in adc class)
   }

   template <std::size_t id>
   void adc_base::setup()
   {
      uq::init();
      enable_adc_clock(id);

      // Configure DMA
      constexpr auto dma_stream = get_dma_stream(id);
      constexpr auto dma_request = get_dma_request(id);
      constexpr auto dma_irq_n = get_dma_irq_n(id);

      _dma_handle.Instance = dma_stream;
      _dma_handle.Init.Request = dma_request;
      adc_base::dma_setup();

      // NVIC configuration for DMA Input data interrupt
      HAL_NVIC_SetPriority(dma_irq_n, 1, 0);
      HAL_NVIC_EnableIRQ(dma_irq_n);

      // Configure ADC
      constexpr auto adc = detail::get_adc(id);
      adc_base::adc_setup(adc);
   }

   ////////////////////////////////////////////////////////////////////////////
   // adc gpio config
   ////////////////////////////////////////////////////////////////////////////
   struct adc_gpio_config
   {
      std::uint8_t   channel;
      port_enum      port;
      std::uint8_t   pin;
   };

   constexpr adc_gpio_config adc1_gpio_config[] =
   {
      // Differential
      {  2, port::portf, 11 }
    , {  3, port::porta,  6 }  // common with adc2!
    , {  4, port::portc,  4 }  // common with adc2!
    , {  5, port::portb,  0 }  // common with adc2!
    , { 10, port::portc,  0 }  // common with adc2 and adc3!
    , { 16, port::porta,  0 }
    , { 18, port::porta,  4 }  // common with adc2!

      // Single ended
    , {  6, port::portf, 12 }
    , {  7, port::porta,  7 }  // common with adc2!
    , {  8, port::portc,  5 }  // common with adc2!
    , {  9, port::portb,  0 }  // common with adc2!
    , { 11, port::portc,  1 }  // common with adc2 and adc3!
    , { 14, port::porta,  2 }  // common with adc2!
    , { 15, port::porta,  3 }  // common with adc2!
    , { 17, port::porta,  1 }
    , { 19, port::porta,  5 }  // common with adc2!
   };

   constexpr adc_gpio_config adc2_gpio_config[] =
   {
      // Differential
      {  2, port::portf, 13 }
    , {  3, port::porta,  6 }  // common with adc1!
    , {  4, port::portc,  4 }  // common with adc1!
    , {  5, port::portb,  0 }  // common with adc1!
    , { 10, port::portc,  0 }  // common with adc1 and adc3!
    , { 18, port::porta,  4 }  // common with adc1!

      // Single ended
    , {  6, port::portf, 14 }
    , {  7, port::porta,  7 }  // common adc1!
    , {  8, port::portc,  5 }  // common adc1!
    , {  9, port::portb,  0 }  // common adc1!
    , { 11, port::portc,  1 }  // common adc2 and adc3!
    , { 14, port::porta,  2 }  // common with adc1!
    , { 15, port::porta,  3 }  // common with adc1!
    , { 19, port::porta,  5 }  // common adc1!
   };

   constexpr adc_gpio_config adc3_gpio_config[] =
   {
      // Differential
      {  1, port::portf,  2 }
    , {  2, port::portf,  9 }
    , {  3, port::portf,  7 }
    , {  4, port::portf,  5 }
    , {  5, port::portf,  3 }
    , { 10, port::portc,  0 }  // common with adc1 and adc2!

      // Single ended
    , {  0, port::portc,  2 }
    , {  6, port::portf, 10 }
    , {  7, port::portf,  8 }
    , {  8, port::portf,  6 }
    , {  9, port::portf,  4 }
    , { 11, port::portc,  1 }  // common with adc1 and adc2!
   };

   constexpr int get_adc_channel(std::size_t channel)
   {
      switch (channel)
      {
         case 0:     return ADC_CHANNEL_0;
         case 1:     return ADC_CHANNEL_1;
         case 2:     return ADC_CHANNEL_2;
         case 3:     return ADC_CHANNEL_3;
         case 4:     return ADC_CHANNEL_4;
         case 5:     return ADC_CHANNEL_5;
         case 6:     return ADC_CHANNEL_6;
         case 7:     return ADC_CHANNEL_7;
         case 8:     return ADC_CHANNEL_8;
         case 9:     return ADC_CHANNEL_9;
         case 10:    return ADC_CHANNEL_10;
         case 11:    return ADC_CHANNEL_11;
         case 12:    return ADC_CHANNEL_12;
         case 13:    return ADC_CHANNEL_13;
         case 14:    return ADC_CHANNEL_14;
         case 15:    return ADC_CHANNEL_15;
         case 16:    return ADC_CHANNEL_16;
      }
      return -1;
   }

   template <std::uint8_t channel, std::size_t N>
   constexpr adc_gpio_config
   find_adc_gpio_config(adc_gpio_config const (&config_list)[N])
   {
      for (auto i = 0; i != N; ++i)
      {
         if (config_list[i].channel == channel)
            return config_list[i];
      }
      return { channel, port_enum::none };
   }

   template <std::size_t id, std::uint8_t channel>
   constexpr adc_gpio_config
   get_adc_gpio_config()
   {
      switch (id)
      {
         case 1: return find_adc_gpio_config<channel>(adc1_gpio_config);
         case 2: return find_adc_gpio_config<channel>(adc2_gpio_config);
         case 3: return find_adc_gpio_config<channel>(adc3_gpio_config);
      }
      return { channel, port_enum::none };
   }

   template <std::size_t id, std::uint8_t channel>
   constexpr bool is_valid_adc_channel()
   {
      return channel <= 16 && get_adc_gpio_config<id, channel>().port != port_enum::none;
   }

   template <std::size_t id, std::uint8_t channel>
   void enable_adc_channel(adc_base& base, uint32_t rank = 1)
   {
      static_assert(is_valid_adc_channel<id, channel>(), "Invalid ADC channel");

      constexpr auto config = get_adc_gpio_config<id, channel>();
      constexpr auto gpio = detail::gpio(config.port);
      constexpr auto channel_ = get_adc_channel(channel);

      detail::enable_gpio(config.port);
      base.enable_channel(gpio, config.pin, channel_, rank);
   };
}}}

#endif
