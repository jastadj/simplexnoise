#include "button.hpp"

Button::Button(sf::RenderWindow *nscreen, std::string btext, sf::Vector2f position)
{
    screen = nscreen;

    mPosition = position;
    mFont.loadFromFile("font.ttf");

    mText = sf::Text(btext, mFont, 12);
    mText.setOrigin(mText.getLocalBounds().width/2, (mText.getLocalBounds().height/2)+3);
    mText.setColor(sf::Color(0,0,0));

    //create button surface
    buttonShape = sf::RectangleShape( sf::Vector2f(mText.getLocalBounds().width+6, mText.getLocalBounds().height+8));
    buttonShape.setFillColor( sf::Color(200,200,200));
    buttonShape.setOutlineThickness(2);
    buttonShape.setOutlineColor(sf::Color(240,240,240));
    buttonShape.setOrigin( buttonShape.getLocalBounds().width/2, buttonShape.getLocalBounds().height/2);

}


void Button::update()
{
    //set member positions
    buttonShape.setPosition(mPosition);
    mText.setPosition(mPosition);

    if(mouseIn())
    {
        mText.setColor( sf::Color(255,255,0));
    } else mText.setColor( sf::Color(0,0,0));
}

void Button::draw()
{
    //draw to screen
    screen->draw(buttonShape);
    screen->draw(mText);
}

bool Button::mouseIn()
{
    if(buttonShape.getGlobalBounds().contains( sf::Mouse::getPosition(*screen).x, sf::Mouse::getPosition(*screen).y))
    {
        return true;
    }
    else return false;
}

void Button::setText(std::string ntext)
{
    mText.setString(ntext);
}
