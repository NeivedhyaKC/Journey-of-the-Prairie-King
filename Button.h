#pragma once
#include<SFML/Graphics.hpp>
class Button
{
public:
    void InitButton(float width,float height,
           sf::Text text,float positionX,float positionY,
           sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textActiveColor,
           sf::Color idleColor,sf::Color hoverColor,sf::Color activeColor,
           sf::Color outlineColor, float outlineThickness);
    bool Update(sf::RenderWindow* window);
    void Render(sf::RenderTarget* target);

private:
    
    sf::Text text;
    sf::RectangleShape shape;

    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;

    sf::Color textIdleColor;
    sf::Color textHoverColor;
    sf::Color textActiveColor;
};