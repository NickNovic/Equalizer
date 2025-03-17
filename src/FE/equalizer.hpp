#include <SFML/Graphics.hpp>

class equalizer {
private:
    float height = 200;
    float width = 300;
    
    float ind_width;
    float max_ind_height;
    float ind_k;

    int inds_count = 5;

    sf::RenderWindow *window;

    float pos_x = 200.f;
    float pos_y = 200.f;
    
    float border_offset = 3.f;

    void draw_background ();
    int draw_indicators (float *percents);

    int draw_indicator (float x, float height);
public:
    equalizer (sf::RenderWindow *window);
    ~equalizer ();
    
    void set_position (float pos_x, float pos_y);
    void set_equalizer_size (float height, float width);
    void set_frequences_count (int count);
    
    int update(float *frequences);
    int update();
};
