#pragma once
#include <cstdint>
#include <format>


namespace cartty::term
{
    struct position
    {
        std::uint32_t column;
        std::uint32_t row;
    };


    struct range
    {
        position begin;
        position end;
    };


    enum class underline_style : std::uint8_t
    {
        none,
        single,
        doubles,
        curly,
        dotted,
        dashed
    };
}


template <typename CharT>
struct std::formatter<cartty::term::position, CharT>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) -> ParseContext::iterator
    { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const cartty::term::position &pos, FormatContext &ctx) const
    { return std::format_to(ctx.out(), "{}:{}", pos.column, pos.row); }
};


template <typename CharT>
struct std::formatter<cartty::term::range, CharT>
{
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) -> ParseContext::iterator
    { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const cartty::term::range &r, FormatContext &ctx) const
    { return std::format_to(ctx.out(), "[{}]->[{}]", r.begin, r.end); }
};
