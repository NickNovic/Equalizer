#include <SFML/Graphics.hpp>

class application {
private:
    sf::RenderWindow *main_window;
    /*
     * Resets window for next while() loop iteration
     * */
    void reset_main_window ();
    /*
     * Sets window up in start of application
     * */
    void setup_main_window ();
    /*
     * Processes all events what happen
     * */
    void process_events ();
public:
    application ();
    ~application ();
    int run();
};
