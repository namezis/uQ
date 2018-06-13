/*=============================================================================
   Copyright (c) 2014-2018 Joel de Guzman. All rights reserved.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(CYCFI_UQ_ADC_HPP_DECEMBER_31_2015)
#define CYCFI_UQ_ADC_HPP_DECEMBER_31_2015

#include <uq/detail/adc.hpp>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
// Interrupt Keys (This should be placed in the global scope)
///////////////////////////////////////////////////////////////////////////////
template <std::size_t id>
struct adc_conversion_half_complete {};

template <std::size_t id>
struct adc_conversion_complete {};

namespace cycfi { namespace uq
{
   ////////////////////////////////////////////////////////////////////////////
   // adc
   ////////////////////////////////////////////////////////////////////////////
   template <
      std::size_t id_
    , std::size_t channels_
    , std::size_t buffer_size_ = 8>
   struct adc : detail::adc_base
   {
      static constexpr std::size_t id = id_;
      static_assert(id >=1 && id <= 3, "Invalid ADC id");

      static constexpr std::size_t channels = channels_;
      static constexpr std::size_t buffer_size = buffer_size_;
      static constexpr std::size_t capacity = buffer_size * channels;

      using base_type = detail::adc_base;
      using adc_type = adc;

      adc()
       : adc_base(id, _data, capacity)
      {
         base_type::setup<id>();
      }

      template <std::size_t... channels>
      void enable_channels()
      {
         static_assert(sizeof...(channels) == channels_,
            "Invalid number of channnels");

         using iseq = std::index_sequence<channels...>;
         detail::enable_all_adc_channels<id>(iseq{}, *this, 1);
      }

      void start()
      {
         auto data = reinterpret_cast<uint32_t*>(_data);
         if (HAL_ADC_Start_DMA(this, data, capacity) != HAL_OK)
            error_handler();
      }

      void stop()
      {
         if (HAL_ADC_Stop_DMA(this))
            error_handler();
      }

      void clear()
      {
         std::fill(_data, _data + capacity, 0);
      }

      uint16_t const*   begin() const     { return _data; }
      uint16_t const*   middle() const    { return _data + (capacity / 2); }
      uint16_t const*   end() const       { return _data + capacity; }

      uint16_t _data[capacity] __attribute__((aligned(32)));
   };
}}

#endif
