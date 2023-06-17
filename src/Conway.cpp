#include "Conway.hpp"

#include <iostream>



Conway::Conway(const ConwaySettings& settings)
:   m_window(sf::VideoMode(settings.window_width, settings.window_height), "Conway's Game of Life", sf::Style::Close),
    m_grid(settings.grid_width, settings.grid_height),
    m_last_states_grid(settings.grid_width, settings.grid_height, DEAD),
    m_vertices(sf::PrimitiveType::Triangles, settings.grid_width * settings.grid_height * 6),
    m_settings(settings)
{
    m_window.setFramerateLimit(60);

    m_view.setCenter(settings.window_width/2.0f, settings.window_height/2.0f);
    m_view.setSize(settings.window_width, settings.window_height);

    for(int y = 0; y < settings.grid_height; ++y)
    {
        for(int x = 0; x < settings.grid_width; ++x)
        {
            int idx = 6 * (y*settings.grid_width + x);
            sf::Vertex& v0 = m_vertices[idx + 0];
            sf::Vertex& v1 = m_vertices[idx + 1];
            sf::Vertex& v2 = m_vertices[idx + 2];
            sf::Vertex& v3 = m_vertices[idx + 3];
            sf::Vertex& v4 = m_vertices[idx + 4];
            sf::Vertex& v5 = m_vertices[idx + 5];

            v0.position = settings.cell_size * sf::Vector2f(x, y);
            v0.color = sf::Color::Black;

            v1.position = settings.cell_size * sf::Vector2f(x+1, y);
            v1.color = sf::Color::Black;

            v2.position = settings.cell_size * sf::Vector2f(x+1, y+1);
            v2.color = sf::Color::Black;

            v3.position = settings.cell_size * sf::Vector2f(x+1, y+1);
            v3.color = sf::Color::Black;

            v4.position = settings.cell_size * sf::Vector2f(x, y+1);
            v4.color = sf::Color::Black;

            v5.position = settings.cell_size * sf::Vector2f(x, y);
            v5.color = sf::Color::Black;
        }
    }

    m_font.loadFromFile("../assets/fonts/SuperComic.ttf");

    m_fps_text.setFont(m_font);
    m_fps_text.setFillColor(sf::Color::Magenta);
    m_fps_text.setCharacterSize(20);
    m_fps_text.setString("FPS: 60");
    m_fps_text.setPosition(sf::Vector2f(10.0f, 10.0f));

    m_steps_text.setFont(m_font);
    m_steps_text.setCharacterSize(16);
    m_steps_text.setString("Steps: 0");
    m_steps_text.setPosition(sf::Vector2f(10.0f, m_settings.window_height-m_steps_text.getLocalBounds().height-30.0f));

    m_step_time_text.setFont(m_font);
    m_step_time_text.setCharacterSize(16);
    m_step_time_text.setString("Step time ms: " + std::to_string(m_step_time));
    m_step_time_text.setPosition(sf::Vector2f(10.0f, m_settings.window_height-m_step_time_text.getLocalBounds().height-10.0f));

    m_paused_text.setFont(m_font);
    m_paused_text.setFillColor(sf::Color::Red);
    m_paused_text.setCharacterSize(26);
    m_paused_text.setString("Paused");
    m_paused_text.setPosition(sf::Vector2f(m_settings.window_width/2-m_paused_text.getLocalBounds().width/2, 10.0f/*m_settings.window_height()/2-m_paused_text.getLocalBounds().height/2*/));

    m_running_text.setFont(m_font);
    m_running_text.setFillColor(sf::Color::Green);
    m_running_text.setCharacterSize(16);
    m_running_text.setString("Running");
    m_running_text.setPosition(sf::Vector2f(m_settings.window_width/2-m_running_text.getLocalBounds().width/2, 10.0f));

    m_state_text = &m_paused_text;

    m_rect_thickness = settings.cell_size/8.0f;
    m_horizontal_rect.setFillColor(sf::Color::Transparent);
    m_horizontal_rect.setOutlineColor(sf::Color::Yellow);
    m_horizontal_rect.setOutlineThickness(-m_rect_thickness);
    m_horizontal_rect.setOrigin(settings.cell_size/2.0f, 0.0f);
    m_horizontal_rect.setSize(sf::Vector2f(settings.window_width+settings.cell_size, settings.cell_size));

    m_vertical_rect.setFillColor(sf::Color::Transparent);
    m_vertical_rect.setOutlineColor(sf::Color::Yellow);
    m_vertical_rect.setOutlineThickness(-m_rect_thickness);
    m_vertical_rect.setOrigin(0.0f, settings.cell_size/2.0f);
    m_vertical_rect.setSize(sf::Vector2f(settings.cell_size, settings.window_height+settings.cell_size));
}

void Conway::run()
{
    sf::Clock update_clock;

    sf::Clock loop_clock;

    float dt = 0.0f;
    sf::Time last_loop_time;
    int frames_count = 0;

    int steps_count = 0;

    while(m_window.isOpen())
    {
        sf::Event event;
        while(m_window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                m_window.close();
            if(event.type == sf::Event::KeyReleased)
            {
                if(event.key.code == sf::Keyboard::Escape)
                    m_window.close();
                else if(event.key.code == sf::Keyboard::Space)
                {
                    m_simulate = !m_simulate;
                    if(m_simulate == false)
                    {
                        m_state_text = &m_paused_text;

                        steps_count = 0;
                        m_steps_text.setString("Steps: " + std::to_string(steps_count));

                        m_grid.states_grid() = m_last_states_grid;
                    }
                    else
                    {
                        m_state_text = &m_running_text;

                        m_last_states_grid = m_grid.states_grid();
                    }
                }
                else if(event.key.code == sf::Keyboard::C)
                {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                        if(m_simulate == false)
                            m_grid.save("../data/grid.txt");
                }
                else if(event.key.code == sf::Keyboard::V)
                {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                        if(m_simulate == false)
                            m_grid.load("../data/grid.txt");
                }
                else if(event.key.code == sf::Keyboard::R)
                {
                    if(m_simulate == false)
                        m_grid.states_grid() = Grid<State>(m_settings.grid_width, m_settings.grid_height);
                }
            }
            if(event.type == sf::Event::MouseWheelScrolled)
            {
                if(event.mouseWheelScroll.delta > 0.0f)
                {
                    if(m_view.getSize().x > 80.0f && m_view.getSize().y > 80.0f)
                    {
                        m_zoom *= 0.92f;
                        m_view.zoom(0.92f);

                        m_vertical_rect.setOutlineThickness(-m_rect_thickness*m_zoom);
                        m_horizontal_rect.setOutlineThickness(-m_rect_thickness*m_zoom);
                    }
                }
                else
                {
                    if(m_view.getSize().x < 2*m_settings.window_width && m_view.getSize().y < 2*m_settings.window_height)
                    {
                        m_zoom *= 1.08f;
                        m_view.zoom(1.08f);

                        m_vertical_rect.setOutlineThickness(-m_rect_thickness*m_zoom);
                        m_horizontal_rect.setOutlineThickness(-m_rect_thickness*m_zoom);
                    }
                }
            }
        }

        // Camera movement
        float camera_offset = 100.0f * dt;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            camera_offset *= 3.0f;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            camera_offset *= 0.333f;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            m_view.move(0.0f, -camera_offset);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            m_view.move(0.0f, camera_offset);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            m_view.move(-camera_offset, 0.0f);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            m_view.move(camera_offset, 0.0f);
        
        // Increase/decrease speed of simulation
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            if(m_step_time < 1000)
            {
                m_step_time += 5;
                m_step_time_text.setString("Step time ms: " + std::to_string(m_step_time));
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            if(m_step_time > 5)
            {
                m_step_time -= 5;
                m_step_time_text.setString("Step time ms: " + std::to_string(m_step_time));
            }
        }
        
        // If simulate -> then update grid
        if(m_simulate == true)
        {
            if(update_clock.getElapsedTime().asMilliseconds() > m_step_time)
            {
                m_grid.step();
                m_steps_text.setString("Steps: " + std::to_string(++steps_count));
                update_clock.restart();
            }
        }
        else // If not simulate -> then edit grid
        {
            const sf::Vector2i& mouse_pos = sf::Mouse::getPosition(m_window);
            
            const sf::Vector2f view_pos = m_window.mapPixelToCoords(mouse_pos);

            if(view_pos.x >= 0.0f && view_pos.x <= m_settings.window_width && view_pos.y >= 0.0f && view_pos.y <= m_settings.window_height)
            {
                int grid_x = view_pos.x/m_settings.cell_size;
                int grid_y = view_pos.y/m_settings.cell_size;
                
                m_horizontal_rect.setPosition(0, grid_y*m_settings.cell_size);
                m_vertical_rect.setPosition(grid_x*m_settings.cell_size, 0);

                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) == true)
                    m_grid.states_grid().set(grid_x, grid_y, ALIVE);
                
                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) == true)
                    m_grid.states_grid().set(grid_x, grid_y, DEAD);
            }
        }

        draw();

        ++frames_count;
        sf::Time current_time = loop_clock.getElapsedTime();
        dt = current_time.asSeconds() - last_loop_time.asSeconds();
        last_loop_time = loop_clock.getElapsedTime();
        if(current_time.asSeconds() > 1.0f)
        {
            m_fps_text.setString("FPS: " + std::to_string(frames_count));
            frames_count = 0;
            loop_clock.restart();
            last_loop_time = loop_clock.getElapsedTime();
        }
    }
}

void Conway::draw()
{
    const Grid<State>& states_grid = m_grid.states_grid();
    for(int y = 0; y < m_settings.grid_height; ++y)
    {
        for(int x = 0; x < m_settings.grid_width; ++x)
        {
            int idx = 6 * (y*m_settings.grid_width + x);

            if(states_grid.get(x, y) == ALIVE)
            {
                m_vertices[idx + 0].color = sf::Color::White;
                m_vertices[idx + 1].color = sf::Color::White;
                m_vertices[idx + 2].color = sf::Color::White;
                m_vertices[idx + 3].color = sf::Color::White;
                m_vertices[idx + 4].color = sf::Color::White;
                m_vertices[idx + 5].color = sf::Color::White;
            }
            else
            {
                m_vertices[idx + 0].color = sf::Color::Black;
                m_vertices[idx + 1].color = sf::Color::Black;
                m_vertices[idx + 2].color = sf::Color::Black;
                m_vertices[idx + 3].color = sf::Color::Black;
                m_vertices[idx + 4].color = sf::Color::Black;
                m_vertices[idx + 5].color = sf::Color::Black;
            }
        }
    }

    m_window.setView(m_view);
    m_window.clear(sf::Color(180, 180, 180));

    m_window.draw(m_vertices);
    m_window.draw(m_fps_text);
    m_window.draw(*m_state_text);
    m_window.draw(m_steps_text);
    m_window.draw(m_step_time_text);
    if(m_simulate == false)
    {
        m_window.draw(m_horizontal_rect);
        m_window.draw(m_vertical_rect);
    }

    m_window.display();
}