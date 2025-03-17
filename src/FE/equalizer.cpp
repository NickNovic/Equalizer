#pragma once

#include "equalizer.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

equalizer::equalizer (sf::RenderWindow *window) {
   this->window = window;
}

equalizer::~equalizer () {
   delete window;
}

void equalizer::draw_background () {
    sf::RectangleShape rectangle(sf::Vector2(width, height));
    rectangle.setPosition(pos_x, pos_y);
    rectangle.setFillColor(sf::Color::Black);
    window->draw(rectangle);   
}

void equalizer::set_position (float pos_x, float pos_y) {
   this->pos_x = pos_x;
   this->pos_y = pos_y;
}

void equalizer::set_equalizer_size (float height, float width) {
   this->height = height;
   this->width = width;
}

void equalizer::set_frequences_count (int count) {
   inds_count = count;
}

int equalizer::draw_indicator (float x, float height) { 
   int y = pos_y - (height - max_ind_height) + border_offset;
   
   sf::RectangleShape freq(sf::Vector2f(ind_width, height));
   freq.setPosition (x, y);
   freq.setFillColor(sf::Color::Red);
   window->draw(freq);
   
   return 0;
}

int equalizer::draw_indicators (float *percents) {
   if (percents == nullptr) return 1;

   max_ind_height = height - 2 * border_offset;

   ind_width = (width - border_offset * 2) / inds_count * 0.9;
   float distance = (width - border_offset * 2) / (inds_count - 1) * 0.1;

   float x = pos_x + border_offset;

   for (int i = 0; i < inds_count - 1; i++) {
      if (percents[i] < 0 || percents[i] > 1) return 1;
      
      float height = percents[i] * max_ind_height; 
   
      draw_indicator (x, height);
      x += ind_width + distance; 
   }

   if (percents[inds_count - 1] < 0 || percents[inds_count] > 1) return 1;   
   
   float height = percents[inds_count - 1] * max_ind_height; 
   
   draw_indicator(x, height);
   
   return 0;
}

int equalizer::update (float *percents) {
   draw_background ();
   return draw_indicators (percents);
}

int equalizer::update() {
   draw_background ();
   return draw_indicators (nullptr);
}
