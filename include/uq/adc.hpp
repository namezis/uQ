/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(CYCFI_UQ_ADC_HPP_DECEMBER_31_2015)
#define CYCFI_UQ_ADC_HPP_DECEMBER_31_2015

#include <uq/detail/adc.hpp>
#include <uq/support.hpp>
#include <uq/startup.hpp>
#include <array>

namespace cycfi { namespace uq
{
   ////////////////////////////////////////////////////////////////////////////
   // adc
   ////////////////////////////////////////////////////////////////////////////
   template <
      std::size_t id_
    , std::size_t channels_
    , std::size_t buffer_size_ = 8>
   struct adc : public detail::adc_base
   {
      static constexpr std::size_t id = id_;
      static_assert(id >=1 && id <= 3, "Invalid ADC id");

      static constexpr std::size_t channels = channels_;
      static constexpr std::size_t buffer_size = buffer_size_;
      static constexpr std::size_t capacity = buffer_size * channels;

      using adc_type = adc;
      using sample_group_type = uint16_t[channels];
      using buffer_type = std::array<sample_group_type, buffer_size>;
      using buffer_iterator_type = typename buffer_type::const_iterator;

      adc()
      {
         constexpr auto adc = detail::get_adc(id_);
         adc_base::construct(adc);
      }

      // template <std::size_t channel>
      void enable_channel()
      {
         // Enable GPIO clock
         __HAL_RCC_GPIOA_CLK_ENABLE();

         adc_base::enable_channel(GPIOA, GPIO_PIN_6, ADC_CHANNEL_3, ADC_REGULAR_RANK_1);
      }

      void start()
      {
         if (HAL_ADC_Start_DMA(this, (uint32_t*) &_data[0][0], size()) != HAL_OK)
            error_handler();
      }

      void stop()
      {
         if (HAL_ADC_Stop_DMA(this))
            error_handler();
      }

      void clear()
      {
         for (auto& buff : _data)
            buff.fill(0);
      }

      constexpr std::size_t size() const           { return buffer_size; }
      constexpr std::size_t num_channels() const   { return channels; }

      buffer_iterator_type begin() const           { return _data.begin(); }
      buffer_iterator_type middle() const          { return _data.begin() + (buffer_size / 2); }
      buffer_iterator_type end() const             { return _data.end(); }

      buffer_type       _data;
   };
}}

#endif
