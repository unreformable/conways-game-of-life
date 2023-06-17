#pragma once

#include <vector>



template <typename T>
class Grid
{
public:
    Grid(int width, int height, T value = T())
    :   m_width(width),
        m_height(height),
        m_cells(width*height, std::move(value))
    {
    }

    void resize(int width, int height, T value = T())
    {
        m_width = width;
        m_height = height;
        m_cells.resize(width*height, std::move(value));
    }

    const T& get(int x, int y) const
    {
        const T& cell = m_cells[to_index(x, y)];
        return cell;
    }
    void set(int x, int y, T value)
    {
        m_cells[to_index(x, y)] = std::move(value);
    }

    const T& get(int idx) const
    {
        const T& cell = m_cells[idx];
        return cell;
    }
    void set(int idx, T value)
    {
        m_cells[idx] = std::move(value);
    }

    int width() const  { return m_width;  }
    int height() const { return m_height; }

private:
    int to_index(int x, int y) const
    {
        return y*m_width + x;
    }

private:
    std::vector<T> m_cells;
    int m_width;
    int m_height;
};