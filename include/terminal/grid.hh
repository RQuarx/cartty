#pragma once
#include <deque>

#include "shared/result.hh"
#include "terminal/row.hh"


namespace cartty::term
{
    namespace _impl { struct logical_line; }


    class grid
    {
    public:
        struct range
        {
            static constexpr auto unset = -1;

            std::int32_t begin = unset;
            std::int32_t end   = unset;


            friend auto operator==(range, range) -> bool = default;
        };


        grid(std::int32_t rows, std::int32_t columns, std::int32_t scrollback_limit = 100'000);


        [[nodiscard]] auto columns() const noexcept -> std::int32_t;
        [[nodiscard]] auto rows() const noexcept -> std::int32_t;
        [[nodiscard]] auto scrollback_limit() const noexcept -> std::int32_t;


        [[nodiscard]] auto operator[](std::int32_t row) noexcept -> result<term::row *>;
        [[nodiscard]] auto operator[](std::int32_t row) const noexcept -> result<const term::row *>;


        [[nodiscard]]
        auto operator[](range r) noexcept
            -> result<std::ranges::subrange<std::deque<term::row>::iterator>>;

        [[nodiscard]]
        auto operator[](range r) const noexcept
            -> result<std::ranges::subrange<std::deque<term::row>::const_iterator>>;


        auto resize(std::int32_t new_rows, std::int32_t new_columns) noexcept -> result<>;
        void pop_front();
        void push_back(row r);
        void push_back();

        void set_scrollback_limit(std::int32_t n);

    private:
        std::deque<term::row> m_rows;
        std::int32_t          m_scrollback_limit;


        void mf_ensure_rows_is_under_limit();

        static auto rewrap(_impl::logical_line   &line,
                           std::int32_t           new_columns,
                           std::deque<term::row> &out_rows) noexcept -> result<>;


        /* Copies every range that overlaps [pos, end) and rebases it to the chunk. */
        static void clip(const auto &src, std::int32_t pos, std::int32_t end, auto &&add)
        {
            for (const auto &r : src)
            {
                if (r.end <= pos or r.begin >= end) continue;
                auto c  = r;
                c.begin = std::max(r.begin, pos) - pos;
                c.end   = std::min(r.end, end) - pos;
                add(std::move(c));
            }
        }
    };
}
