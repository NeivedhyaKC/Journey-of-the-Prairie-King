#include "Button.h"
#include <iostream>

void Button::InitButton(float width, float height, 
    sf::Text text, float positionX, float positionY,
    sf::Color textIdleColor,sf::Color textHoverColor,sf::Color textActiveColor,
    sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor,
    sf::Color outlineColor, float outlineThickness)
{
    this->textIdleColor = textIdleColor;
    this->textActiveColor = textActiveColor;
    this->textHoverColor = textHoverColor;

    this->activeColor = activeColor;
    this->hoverColor = hoverColor;
    this->idleColor = idleColor;

    this->text = text;

    shape.setOutlineThickness(outlineThickness);
    shape.setOutlineColor(outlineColor);
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(positionX, positionY);
    shape.setFillColor(idleColor);
    this->text.setPosition(shape.getPosition().x + shape.getGlobalBounds().width / 2
                           - this->text.getGlobalBounds().width / 2,
                           shape.getPosition().y + shape.getGlobalBounds().height / 2
                           - this->text.getGlobalBounds().height /1.5f);
    this->text.setFillColor(textIdleColor);
    this->text.setCharacterSize(30);
}

bool Button::Update(sf::RenderWindow* window)
{
    
    if (shape.getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
    {
        shape.setFillColor(hoverColor);
        text.setFillColor(textHoverColor);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            shape.setFillColor(activeColor);
            text.setFillColor(textActiveColor);
            return true;
        }
        return false;
    }
    text.setFillColor(textIdleColor);
    shape.setFillColor(idleColor);
    return false;
}

void Button::Render(sf::RenderTarget* target)
{
    target->draw(shape);
    target->draw(text);
}


