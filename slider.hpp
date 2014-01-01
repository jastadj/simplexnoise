#ifndef CLASS_SLIDER
#define CLASS_SLIDER

#include <SFML\Graphics.hpp>

#include <cstdlib>
#include <cmath>
#include <iostream> // for printf debugging

#define LINE_THICKNESS 2
#define SLIDER_HEIGHT 10
#define SLIDER_WIDTH 7
#define SLIDER_OUTLINE_TH 1

// USAGE:
// Slider class is only to be used for SFML

// Slider(sf::RenderWindow *nscreen, float minr, float maxr, int nwidth, float *ntarget);
// *nscreen = target renderwindow screen to draw to
// minr = minimum range of slider
// maxr = maximum range of slider
// nwidth = the graphical width in pixels of slider
// ntarget = the floag variable to point to to make changes to
//
// The slider needs to be constructed, and draw() to the screen
// the slider also needs to be updated with an sf::Event and pass to update(sf::Event event)
// to handle mouse events on the slider
// other options available, look at the public functions



class Slider
{
private:

    //render target
    sf::RenderWindow *screen;

    //target value pointer
    std::string name;
    float *target;
    bool rounding;
    bool realtime_update;

    //graphical objects
    sf::RectangleShape scaleA;
    sf::RectangleShape scaleB;
    sf::RectangleShape slider;

    //slider range
    float minRange;
    float maxRange;
    float getSliderPosOffset();

    //dimensions
    float sliderPosition;
    int width;
    sf::Vector2f position;

    //slider mouse control
    bool sliding;
    sf::Vector2i mouseOffset;
    bool mouseInSlider();


public:
    Slider(sf::RenderWindow *nscreen, float minr, float maxr, int nwidth, float *ntarget);
    Slider();

    std::string getName() { return name;}
    void setName(std::string nname) { name = nname;}
    void setColor(sf::Color ncolor);

    void setPosition(sf::Vector2f npos);
    sf::Vector2f getPosition() { return position;}
    void setCenter(sf::Vector2f npos);
    void setSliderPosition(float nsliderpos);

    //set for rounding values of the slider, this will snap it to whole numbers
    void setRounding(bool nround) {rounding = nround;}
    bool isRounding() { return rounding;}

    //this will update the slider position <=> target value in realtime
    void setRealtime(bool nrealtime) { realtime_update = nrealtime;}
    bool isRealtime() {return realtime_update;}


    bool isSliding() {if(sliding) return true; else return false;}
    sf::Vector2f getSliderPosition() { return slider.getPosition();} //return actual screen position of slider
    float convertSliderPosition(); // return valude of slider in range

    void setMin(float nmin);
    void setMax(float nmax);

    //these functions are required to use the slider in a functional manner
    bool update(sf::Event *event);
    void draw();

};


#endif // CLASS_SLIDER
