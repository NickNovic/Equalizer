#include "application.hpp"
#include "equalizer.hpp"

#include <bits/stdc++.h>

void application::setup_main_window () {
    main_window = new sf::RenderWindow(sf::VideoMode(800, 600), 
                                     "Equalizer main window");
    main_window->clear(sf::Color::White);
}

application::application () {
    setup_main_window ();
}

application::~application () {
    delete main_window;
}

void application::process_events () {
    sf::Event event;
    while (main_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
        main_window->close();
    }
}

void application::reset_main_window () {
    // Clear the window with a color
    main_window->clear(sf::Color::White);
}

int application::run () {
    equalizer *eqlzr = new equalizer(main_window);
    float percents[] = {0.1, 0.2, 0.3, 0.4, 0.5};   
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 100);

    // Main loop
    while (main_window->isOpen()) {
        process_events ();
        
        for (int i = 0; i < 5; i++) {
            percents[i] = static_cast<float>(distr(gen)) / 100;
        }

        eqlzr->update(percents);

        main_window->display();
        usleep(5000);
        reset_main_window ();   
    }
    return 0;
}
