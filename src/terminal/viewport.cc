#include "terminal/viewport.hh"

using cartty::term::viewport;


viewport::viewport(std::size_t rows, std::size_t columns, std::size_t scrollback_limit)
    : m_grid { rows, columns, scrollback_limit }, m_visible_rows { rows }
{
}


auto viewport::max_scroll_offset() const noexcept -> std::size_t
{ return m_grid.rows() - m_visible_rows; }


auto viewport::rows() const noexcept -> std::size_t { return m_grid.rows(); }


auto viewport::columns() const noexcept -> std::size_t { return m_grid.columns(); }


auto viewport::visible_rows() const noexcept
    -> std::ranges::subrange<std::deque<row>::const_iterator>
{ return const_cast<viewport &>(*this).visible_rows(); }


auto viewport::visible_rows() noexcept -> std::ranges::subrange<std::deque<row>::iterator>
{
    const std::size_t end   = m_grid.rows() - m_scroll_offset;
    const std::size_t begin = end - m_visible_rows;
    return *m_grid[{ begin, end }];
}


auto viewport::mf_translate_index(std::size_t relative) const noexcept -> result<std::size_t>
{
    if (relative >= m_visible_rows)
        return error { "Relative index is out-of-range. (relative ({}) >= rows ({}))", relative,
                       m_visible_rows }
            .unexpected();

    /* 0 = top of the viewport, m_visible_rows - 1 = bottom */
    return m_grid.rows() - m_scroll_offset - m_visible_rows + relative;
}


auto viewport::operator[](std::size_t i) noexcept -> result<row *>
{
    return mf_translate_index(i).and_then([this](std::size_t i) { return m_grid[i]; });
}

auto viewport::operator[](std::size_t i) const noexcept -> result<const row *>
{
    return const_cast<viewport &>(*this)[i].transform([](row *r) -> const row * { return r; });
}


void viewport::resize(std::size_t rows, std::size_t columns)
{
    m_grid.resize(rows, columns);
    m_visible_rows  = rows;
    m_scroll_offset = std::min(m_scroll_offset, max_scroll_offset());
}


void viewport::scroll(std::int64_t offset) noexcept
{
    m_scroll_offset = std::clamp(m_scroll_offset - offset, 0UZ, max_scroll_offset());
    for (auto &&row : visible_rows()) row.set_dirty();
}


void viewport::push_back(row r) { m_grid.push_back(std::move(r)); }
