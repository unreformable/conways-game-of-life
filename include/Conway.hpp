#pragma once

#include "ConwayGrid.hpp"
#include "Grid.hpp"
#include <SFML/Graphics.hpp>



struct ConwaySettings
{
    int grid_width;
    int grid_height;
    float cell_size;
    int window_width;
    int window_height;

    ConwaySettings(int grid_width, int grid_height, int cell_size)
    :   grid_width(grid_width),
        grid_height(grid_height),
        cell_size(static_cast<float>(cell_size)),
        window_width(grid_width*cell_size),
        window_height(grid_height*cell_size)
    {
    }
};

class Conway
{
public:
    Conway(const ConwaySettings& settings);

    void run();

private:
    void draw();

private:
    ConwayGrid m_grid;
    
    bool m_simulate = false;

    int m_step_time = 100;

    float m_rect_thickness;
    float m_zoom = 1.0f;

    Grid<State> m_last_states_grid;

    sf::RenderWindow m_window;
    sf::View m_view;

    sf::VertexArray m_vertices;

    const ConwaySettings& m_settings;

    sf::Text* m_state_text = nullptr;

    sf::Font m_font;
    sf::Text m_fps_text;

    sf::Text m_step_time_text;
    sf::Text m_steps_text;

    sf::Text m_running_text;
    sf::Text m_paused_text;

    sf::RectangleShape m_horizontal_rect;
    sf::RectangleShape m_vertical_rect;
};