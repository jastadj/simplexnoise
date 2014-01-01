#ifndef CLASS_MULTISLIDER
#define CLASS_MULTISLIDER

#include <cstdlib>
#include <vector>
#include <cmath>
#include <iostream>

#include <SFML\Graphics.hpp>

#define LINE_THICKNESS 2
#define SLIDER_HEIGHT 10
#define SLIDER_WIDTH 7
#define SLIDER_OUTLINE_TH 1

/*
struct segment_old
{
    int *target;
    sf::RectangleShape slider;
    sf::Color segcolor;

};
*/

struct segment
{
    int value;
    int red;
    int green;
    int blue;

};

class MultiSlider
{
private:

    sf::RenderWindow *screen;

    bool sliding;
    int slider_sel;
    bool mouseIn(int i);
    int mouseXoffset;

    //parameters
    int minRange;
    int maxRange;
    int width;
    sf::Vector2f position;

    //std::vector< segment_old > segments;
    std::vector< segment> *segs;
    std::vector< sf::RectangleShape> sliders;

    segment endStop;
    void drawSegLine(int start, segment *stop);

    int valueToPosition(int val);
    int positionToValue(int pos);

    sf::RectangleShape getSliderTemplate();

    //segment adding/removing
    void deleteSegment(int id);


public:
    MultiSlider(sf::RenderWindow *nscreen, int nmin, int nmax, int nwidth, std::vector<segment> *nsegs);

    bool update(sf::Event *event);
    void draw();

    void setPosition(sf::Vector2f npos) {position = npos;}
    void setCenterPosition( sf::Vector2f npos);

    //debug
    void addSegment(segment *newseg);
};

#endif // CLASS_MULTISLIDER
