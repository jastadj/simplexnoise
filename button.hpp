#ifndef CLASS_BUTTON
#define CLASS_BUTTON

#include <SFML\Graphics.hpp>

class Button
{
private:

    sf::RenderWindow *screen;

    sf::Vector2f mPosition;
    sf::RectangleShape buttonShape;
    sf::Text mText;
    sf::Font mFont;

public:
    Button(sf::RenderWindow *nscreen, std::string btext, sf::Vector2f position);

    void update();
    void draw();
    void updateDraw() {update(); draw();}

    bool mouseIn();

    void setText(std::string ntext);

};
#endif // CLASS_BUTTON
