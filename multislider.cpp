#include "multislider.hpp"

MultiSlider::MultiSlider(sf::RenderWindow *nscreen, int nmin, int nmax, int nwidth, std::vector<segment> *nsegs)
{
    screen = nscreen;

    sliding = false;
    slider_sel = -1;

    segs = nsegs;

    for(int i = 0; i < int(segs->size()); i++) sliders.push_back(getSliderTemplate());


    //parameters
    minRange = nmin;
    maxRange = nmax;
    width = nwidth;

    //end segment
    endStop.value = maxRange;
}

void MultiSlider::setCenterPosition(sf::Vector2f npos)
{
    npos.x = npos.x - (width/2);
    position = npos;
    std::cout << "terSlider pos = (" << position.x << "," << position.y << ")" << std::endl;
}

bool MultiSlider::update(sf::Event *event)
{
    bool needs_update = false;
    int mouseInSeg = -1;
    //get mouse position
    sf::Vector2i mousePos = sf::Mouse::getPosition(*screen);



    //check for mouse over slider
    for(int i = 0; i < int(segs->size()); i++)
    {
        if(mouseIn(i))
        {
            sliders[i].setOutlineColor( sf::Color(220,220,220));
            mouseInSeg = i;
        }
        else sliders[i].setOutlineColor(sf::Color(120,120,120));
    }

    //check for sliding status
    if(sliding && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        slider_sel = -1;
        sliding = false;
    }
    else if(sliding)
    {
        if(segs->empty())
        {
            slider_sel = -1;
            sliding = false;
        }

        int slider_value = positionToValue(mousePos.x);
        needs_update = true;

        //determine which slider is selected
        //if its the first slider

        if( abs(mousePos.y - position.y) > 12)
        {
            deleteSegment(slider_sel);
            slider_sel = -1;
            sliding = false;
        }

        if(slider_sel == 0)
        {
            //if its the only slider
            if( int(segs->size()) == 1 )
            {
                //if slider is within ranges
                if( slider_value >= minRange && slider_value <= maxRange)
                {
                    (*segs)[slider_sel].value = slider_value;
                }
            }
            //else, check first slider against value of next slider in list
            else
            {
                if( slider_value >= minRange && slider_value < (*segs)[1].value)
                {
                    (*segs)[slider_sel].value = slider_value;
                }
            }
        }
        //if its the last slider
        else if(slider_sel == int(segs->size()-1))
        {
            if(slider_value > (*segs)[slider_sel-1].value && slider_value <= maxRange)
            {
                (*segs)[slider_sel].value = slider_value;
            }
        }
        else
        {
            if( (*segs)[slider_sel-1].value < slider_value && slider_value < (*segs)[slider_sel+1].value)
            {
                (*segs)[slider_sel].value = slider_value;
            }
        }

    }

    //check for mouse click even
    if(event->type == sf::Event::MouseButtonPressed)
    {
        if(mouseInSeg != -1)
        {
            slider_sel = mouseInSeg;
            sliding = true;
        }
    }

    return needs_update;

}

bool MultiSlider::mouseIn(int i)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*screen);

    if(sliders[i].getGlobalBounds().contains( mousePos.x, mousePos.y)) return true;
    else return false;
}

void MultiSlider::draw()
{
    //create segment lines

    //if no segments are populated, draw line to endstop
    if(segs->empty())
    {
        drawSegLine(minRange, &endStop);
    }
    //if segments available
    else
    {
        for(int i = 0; i < int(segs->size()); i++)
        {
            if(i == 0) drawSegLine(minRange, &(*segs)[i]);
            else drawSegLine( (*segs)[i-1].value, &(*segs)[i]);

        }
        if( endStop.value != segs->back().value) drawSegLine( segs->back().value, &endStop);
    }

    //draw buttons
    for(int i = 0; i < int(segs->size()); i++)
    {
        sliders[i].setPosition( sf::Vector2f(position.x + valueToPosition((*segs)[i].value), position.y));
        screen->draw(sliders[i]);
    }
}

void MultiSlider::drawSegLine(int start, segment *stop)
{
    //calculate segment length
    int length = valueToPosition(stop->value) - valueToPosition(start);

    //create topline
    sf::RectangleShape topLine( sf::Vector2f( length, LINE_THICKNESS) );
    topLine.setPosition(position.x + valueToPosition(start), position.y);
    topLine.setFillColor( sf::Color(200,200,200));
    //create bottom line
    sf::RectangleShape bottomLine( sf::Vector2f( length, LINE_THICKNESS) );
    bottomLine.setPosition( position.x + valueToPosition(start),  position.y + LINE_THICKNESS );
    bottomLine.setFillColor( sf::Color(110,110,110));

    //apply color
    sf::RectangleShape colormask( sf::Vector2f( length, LINE_THICKNESS*2));
    colormask.setPosition(position.x + valueToPosition(start),  position.y);
    colormask.setFillColor( sf::Color(stop->red, stop->green, stop->blue, 150));

    //drow line segment
    screen->draw(topLine);
    screen->draw(bottomLine);
    screen->draw(colormask);

}


int MultiSlider::valueToPosition(int xval)
{
    return width/(maxRange-minRange) * (xval - minRange);
}

int MultiSlider::positionToValue(int pos)
{
    pos = pos - position.x;

    return int(  (float(maxRange-minRange)/ float(width) ) * pos);

}

sf::RectangleShape MultiSlider::getSliderTemplate()
{
    //create slider vector
    sf::RectangleShape slider_template;
    slider_template = sf::RectangleShape(sf::Vector2f(SLIDER_WIDTH, SLIDER_HEIGHT));
    slider_template.setOutlineThickness(SLIDER_OUTLINE_TH);
    slider_template.setOutlineColor(sf::Color(120,120,120));
    slider_template.setOrigin( SLIDER_WIDTH/2, SLIDER_HEIGHT/2);

    return slider_template;
}


/*
void MultiSlider::addSegment(int *ntarget, sf::Color segc)
{
    if( *ntarget < minRange || *ntarget > maxRange) return;

    segment_old newseg;

    newseg.target = ntarget;
    newseg.slider = sf::RectangleShape( sf::Vector2f(SLIDER_WIDTH, SLIDER_HEIGHT));
    newseg.slider.setOrigin( sf::Vector2f(SLIDER_WIDTH/2, SLIDER_HEIGHT/2));
    newseg.slider.setFillColor( sf::Color(220,220,220));
    newseg.slider.setOutlineThickness(SLIDER_OUTLINE_TH);
    newseg.slider.setOutlineColor( sf::Color(120,120,120));
    newseg.segcolor = segc;

    //insert slider into segment vector
    if(segments.empty()) segments.push_back(newseg);
    else
    {
        segments.push_back(newseg);
    }

}
*/


void MultiSlider::deleteSegment(int id)
{
    if(id < 0 || id >= int(segs->size())) return;

    segs->erase( segs->begin()+id);
    sliders.erase( sliders.begin()+id);
}

void MultiSlider::addSegment(segment *newseg)
{
    //find where to insert new segment
    if(newseg == NULL) return;

    if(segs->empty())
    {
        segs->push_back(*newseg);
        sliders.push_back(getSliderTemplate());
    }
    else
    {
        for(int i = 0; i < int(segs->size()); i++)
        {
            if(newseg->value < (*segs)[i].value)
            {
                std::cout << newseg->value << " is < " <<  (*segs)[i].value << std::endl;
                segs->insert(segs->begin()+i, *newseg);
                sliders.insert(sliders.begin() + i, getSliderTemplate());

                return;
            }
        }

        if(newseg->value > (*segs).back().value) {segs->push_back(*newseg); sliders.push_back(getSliderTemplate());}

    }

}
