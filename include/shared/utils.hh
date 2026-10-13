#pragma once
#include <utility>

#include <spdlog/spdlog.h>

#include "shared/result.hh"


namespace cartty::utils
{
    template <std::integral T>
    [[nodiscard]]
    consteval auto integral_to_string() noexcept -> std::string_view
    {
        using type = std::remove_cvref_t<T>;

        if constexpr (std::same_as<type, std::uint8_t>)
            return "std::uint8_t";
        else if constexpr (std::same_as<type, std::int8_t>)
            return "std::int8_t";
        else if constexpr (std::same_as<type, std::uint16_t>)
            return "std::uint16_t";
        else if constexpr (std::same_as<type, std::int16_t>)
            return "std::int16_t";
        else if constexpr (std::same_as<type, std::uint32_t>)
            return "std::uint32_t";
        else if constexpr (std::same_as<type, std::int32_t>)
            return "std::int32_t";
        else if constexpr (std::same_as<type, std::uint64_t>)
            return "std::uint64_t";
        else if constexpr (std::same_as<type, std::int64_t>)
            return "std::int64_t";
        else if constexpr (std::same_as<type, std::size_t>)
            return "std::size_t";
        else
            static_assert("Unrecognized integral type.");
    }


    template <std::integral To, std::integral From>
    auto cast(From from) noexcept -> result<To>
    {
        if (std::in_range<To>(from)) return static_cast<To>(from);
        return error { "Value {}: {} is too large to store inside a {}", from,
                       integral_to_string<From>(), integral_to_string<To>() }
            .unexpected();
    }


    struct log_instead_tag
    {
    };


    template <std::integral To, std::integral From>
    auto cast(From from, log_instead_tag /* tag */) noexcept -> To
    {
        if (std::in_range<To>(from)) return static_cast<To>(from);
        spdlog::error("Value {}: {} is too large to store inside a {}", from,
                      integral_to_string<From>(), integral_to_string<To>());
        return 0;
    }


#define UNWRAP(...)                              \
    ({                                           \
        auto &&_result = __VA_ARGS__;            \
        if (!_result)                            \
            return _result.error().unexpected(); \
        std::move(*_result);                     \
    })
}
