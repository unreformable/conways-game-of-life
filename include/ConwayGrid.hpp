#pragma once

#include "Grid.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>



using State = bool;
constexpr bool ALIVE = true;
constexpr bool DEAD = false;

class ConwayGrid
{
public:
    ConwayGrid(int width, int height)
    :   m_neighbours_grid(width, height),
        m_states_grid(width, height, DEAD)
    {
        init_neighbours();

        std::remove("../data/grid.txt");
    }

    void step()
    {
        Grid<State> tmp = m_states_grid;

        for(int y = 0; y < m_states_grid.height(); ++y)
        {
            for(int x = 0; x < m_states_grid.width(); ++x)
            {
                int alive_neighbours = 0;

                for(int neighbour : m_neighbours_grid.get(x, y))
                {
                    if(tmp.get(neighbour) == ALIVE)
                        ++alive_neighbours;
                }

                State cell_state = tmp.get(x, y);

                if(cell_state == ALIVE && alive_neighbours != 2 && alive_neighbours != 3)
                {
                    m_states_grid.set(x, y, DEAD);
                }
                else if(cell_state == DEAD && alive_neighbours == 3)
                {
                    m_states_grid.set(x, y, ALIVE);
                }
            }
        }
    }

    void save(const std::string& path)
    {
        std::ofstream file;
        file.open(path);

        file << m_states_grid.width() << ' ' << m_states_grid.height() << '\n';

        for(int y = 0; y < m_states_grid.height(); ++y)
        {
            for(int x = 0; x < m_states_grid.width(); ++x)
            {
                file << m_states_grid.get(x, y) << ' ';
            }
            file << '\n';
        }
    }

    void load(const std::string& path)
    {
        std::ifstream file;
        file.open(path);

        if(file.is_open() == false)
            return;

        int grid_width;
        int grid_height;
        file >> grid_width >> grid_height;

        m_states_grid = Grid<State>(grid_width, grid_height);
        for(int y = 0; y < m_states_grid.height(); ++y)
        {
            for(int x = 0; x < m_states_grid.width(); ++x)
            {
                State state;
                file >> state;
                m_states_grid.set(x, y, state);
            }
        }
    }

    const Grid<State>& states_grid() const { return m_states_grid; }
    Grid<State>& states_grid() { return m_states_grid; }

private:
    void init_neighbours()
    {
        for(int y = 0; y < m_neighbours_grid.height(); ++y)
        {
            for(int x = 0; x < m_neighbours_grid.width(); ++x)
            {
                std::vector<int> neighbours;
                neighbours.reserve(8);

                for(int j = y-1; j <= y+1; ++j)
                {
                    for(int i = x-1; i <= x+1; ++i)
                    {
                        if(i == x && j == y)
                            continue;
                        
                        int neighbour_x = i < 0 ? m_neighbours_grid.width()-1  : i > m_neighbours_grid.width()-1  ? 0 : i;
                        int neighbour_y = j < 0 ? m_neighbours_grid.height()-1 : j > m_neighbours_grid.height()-1 ? 0 : j;

                        
                        neighbours.push_back(neighbour_y * m_neighbours_grid.width() + neighbour_x);
                    }
                }
                m_neighbours_grid.set(x, y, neighbours);
            }
        }
    }

private:
    Grid<State> m_states_grid;

    // Hold indexes of neighbours for each cell
    Grid<std::vector<int>> m_neighbours_grid;
};