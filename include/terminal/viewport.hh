#pragma once
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
        viewport(std::int32_t rows, std::int32_t columns, std::int32_t scrollback_limit = 100'000);

        [[nodiscard]] auto max_scroll_offset() const noexcept -> std::int32_t;
        [[nodiscard]] auto rows() const noexcept -> std::int32_t;
        [[nodiscard]] auto columns() const noexcept -> std::int32_t;

        [[nodiscard]] auto visible_rows() noexcept
            -> std::ranges::subrange<std::deque<row>::iterator>;
        [[nodiscard]] auto visible_rows() const noexcept
            -> std::ranges::subrange<std::deque<row>::const_iterator>;

        [[nodiscard]] auto operator[](std::int32_t i) noexcept -> result<row *>;
        [[nodiscard]] auto operator[](std::int32_t i) const noexcept -> result<const row *>;


        auto resize(std::int32_t rows, std::int32_t columns) noexcept -> result<>;

        /** @brief Scrolls either up or down,
                   if `offset > 0` scroll down, if `offset < 0` scroll up. */
        void scroll(std::int32_t offset) noexcept;
        void push_back(row r);


    private:
        grid m_grid;

        /* The amount of rows visible at a time, set via constructor, or `viewport::resize()` */
        std::int32_t m_visible_rows;

        /* Represents the bottom-most visible row of the viewport,
           offset of 0 means 0 rows between the newest row and the viewport. */
        std::int32_t m_scroll_offset = 0;


        /** @brief Translate a viewport-relative index to the grid-relative (absolute) index. */
        [[nodiscard]]
        auto mf_translate_index(std::int32_t relative) const noexcept -> result<std::int32_t>;
    };
}
