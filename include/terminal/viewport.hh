#pragma once
#include <cstddef>
#include <cstdint>

#include "shared/result.hh"
#include "terminal/grid.hh"


namespace cartty::term
{
    /**
     * @brief Owner of @ref grid , and the one who manages viewport maths.
     */
    class viewport
    {
    public:
        viewport(std::size_t rows, std::size_t columns, std::size_t scrollback_limit = 100'000);

        [[nodiscard]] auto max_scroll_offset() const noexcept -> std::size_t;
        [[nodiscard]] auto rows() const noexcept -> std::size_t;
        [[nodiscard]] auto columns() const noexcept -> std::size_t;

        [[nodiscard]] auto visible_rows() noexcept
            -> std::ranges::subrange<std::deque<row>::iterator>;
        [[nodiscard]] auto visible_rows() const noexcept
            -> std::ranges::subrange<std::deque<row>::const_iterator>;

        [[nodiscard]] auto operator[](std::size_t i) noexcept -> result<row *>;
        [[nodiscard]] auto operator[](std::size_t i) const noexcept -> result<const row *>;


        void resize(std::size_t rows, std::size_t columns);

        /** @brief Scrolls either up or down,
                   if `offset > 0` scroll down, if `offset < 0` scroll up. */
        void scroll(std::int64_t offset) noexcept;
        void push_back(row r);


    private:
        grid m_grid;

        /* The amount of rows visible at a time, set via constructor, or `viewport::resize()` */
        std::size_t m_visible_rows;

        /* Represents the bottom-most visible row of the viewport,
           offset of 0 means 0 rows between the newest row and the viewport. */
        std::size_t m_scroll_offset = 0;


        /** @brief Translate a viewport-relative index to the grid-relative (absolute) index. */
        [[nodiscard]]
        auto mf_translate_index(std::size_t relative) const noexcept -> result<std::size_t>;
    };
}
