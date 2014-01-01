#include "slider.hpp"

Slider::Slider() {}

Slider::Slider(sf::RenderWindow *nscreen, float minr, float maxr, int nwidth, float *ntarget)
{

    screen = nscreen;
    target = ntarget;

    //set dimensions
    minRange = minr;
    maxRange = maxr;
    width = nwidth;
    //default slider position to min value
    sliderPosition = *ntarget;
    sliding = false;
    realtime_update = false;

    //create graphic objects
    scaleA = sf::RectangleShape(sf::Vector2f(width,LINE_THICKNESS));
    scaleB = sf::RectangleShape(sf::Vector2f(width,LINE_THICKNESS));
    slider = sf::RectangleShape( sf::Vector2f(SLIDER_WIDTH, SLIDER_HEIGHT));

    //set object colors
    scaleA.setFillColor( sf::Color(220,220,220));
    scaleB.setFillColor( sf::Color(180,180,180));
    slider.setFillColor( sf::Color(220,220,220));

    //slider outline
    slider.setOutlineThickness(SLIDER_OUTLINE_TH);
    slider.setOutlineColor( sf::Color(120,120,120));

    //set object origins
    slider.setOrigin( SLIDER_WIDTH/2, SLIDER_HEIGHT/2);
    scaleB.setOrigin( sf::Vector2f(0,-LINE_THICKNESS));


}

void Slider::setColor(sf::Color ncolor)
{
    slider.setFillColor(ncolor);

}

void Slider::setPosition(sf::Vector2f npos)
{
    //set position reference
    position = npos;

    //set graphical objects to new position

    scaleA.setPosition(position);
    scaleB.setPosition(position);
    slider.setPosition(sf::Vector2f(position.x + getSliderPosOffset(), position.y+LINE_THICKNESS));
}

void Slider::setCenter(sf::Vector2f npos)
{
    npos.x = npos.x - (width/2);
    setPosition(npos);
}

float Slider::getSliderPosOffset()
{
    float rangeD = maxRange - minRange;

    return (width/rangeD) * (sliderPosition - minRange);
}

float Slider::convertSliderPosition()
{
    float conpos = (slider.getPosition().x - position.x) / (width/(maxRange-minRange) ) + minRange;

    if(rounding) return round(conpos);
    else return conpos;
}

bool Slider::update(sf::Event *event)
{
    //this is to tell the main that a change was made and an update needs to be made to reflect value change
    bool needsUpdate = false;

    if(event == NULL)
    {
        setSliderPosition(*target);
        return false;
    }

    //if sliding is activated but mouse button is no longer pressed
    // assume it was released and set position data
    if(sliding && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        //create new proposed slider value
        float newpos = (slider.getPosition().x - position.x) / (width/(maxRange-minRange)) + minRange;

        //if rounding, round foat
        if(rounding) newpos = round(newpos);

        //set slider position based on new value
        setSliderPosition(newpos);
        //no longer sliding
        sliding = false;
        //requires an update to be made
        needsUpdate = true;
    }

    //event handler
    if( event->type == sf::Event::MouseButtonPressed)
    {

       if(event->mouseButton.button == sf::Mouse::Left)
       {
           if(mouseInSlider())
           {
                mouseOffset = sf::Vector2i(slider.getPosition()) - sf::Mouse::getPosition(*screen) ;
                sliding = true;

           }
       }

    }

    if(mouseInSlider()) slider.setOutlineColor(sf::Color(255,255,255));
    else slider.setOutlineColor(sf::Color(120,120,120));


    if(sliding)
    {
        if(sf::Mouse::getPosition(*screen).x + mouseOffset.x >= position.x &&
           sf::Mouse::getPosition(*screen).x + mouseOffset.x  <= position.x + width)
        slider.setPosition( sf::Mouse::getPosition(*screen).x + mouseOffset.x, slider.getPosition().y);

        //if slider is updating target in realtime
        if(realtime_update)
        {
            *target = convertSliderPosition();
            needsUpdate = true;
        }
    }

    return needsUpdate;

}

void Slider::draw()
{

    //draw objects to screen
    screen->draw(scaleA);
    screen->draw(scaleB);
    screen->draw(slider);
}

void Slider::setSliderPosition(float nsliderpos)
{

    if(nsliderpos < minRange || nsliderpos > maxRange) return;

    else
    {
        sliderPosition = nsliderpos;

        *target = nsliderpos;

        slider.setPosition(sf::Vector2f(position.x + getSliderPosOffset(), position.y+LINE_THICKNESS));
    }
}

bool Slider::mouseInSlider()
{
    //get mouse position
    sf::Vector2i mousePos = sf::Mouse::getPosition(*screen);

    //is mouse position inside the slider button?
    if( slider.getGlobalBounds().contains(mousePos.x, mousePos.y) ) return true;
    else return false;
}

void Slider::setMin(float nmin)
{
    if(nmin >= maxRange) return;

    minRange = nmin;
    if(minRange > sliderPosition) setSliderPosition(minRange);

    update(NULL);
}

void Slider::setMax(float nmax)
{
    if(nmax <= minRange) return;

    maxRange = nmax;
    if(maxRange < sliderPosition) setSliderPosition(maxRange);

    update(NULL);
}
