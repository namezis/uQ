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
         adc_base::construct();
      }

      virtual ~adc()
      {
         adc_base::destruct();
      }

      virtual void init() override
      {
         adc_base::setup();
      }

      // template <std::size_t channel, std::size_t pin, std::size_t rank>
      void enable_channel()
      {
         ADC_ChannelConfTypeDef config;
         config.Channel       = ADC_CHANNEL_3;           // Sampled channel number
         config.Rank          = ADC_REGULAR_RANK_1;      // Rank of sampled channel number ADCx_CHANNEL
         config.SamplingTime  = ADC_SAMPLETIME_1CYCLE_5; // Sampling time (number of clock cycles unit)
         config.SingleDiff    = ADC_SINGLE_ENDED;        // Single-ended input channel
         config.OffsetNumber  = ADC_OFFSET_NONE;         // No offset subtraction
         config.Offset        = 0;                       // Parameter discarded because offset correction is disabled

         if (HAL_ADC_ConfigChannel(this, &config) != HAL_OK)
            error_handler();
      }

      void start()
      {
         if (HAL_ADC_Start_DMA(this, (uint32_t*) &data[0][0], size()) != HAL_OK)
            error_handler();
      }

      constexpr std::size_t size() { return buffer_size; }
      constexpr std::size_t num_channels() { return channels; }

      buffer_iterator_type begin() const { return data.begin(); }
      buffer_iterator_type middle() const { return data.begin() + (buffer_size / 2); }
      buffer_iterator_type end() const { return data.end(); }

      buffer_type data;
   };
}}

#endif
