#include "State.h"

int* State::intState = nullptr;

State::State(sf::RenderWindow* window,sf::View* view):
    window(window),
    view(view)
{
    font.loadFromFile("Fonts/Harbinger.ttf");
}

void State::UpdateMousePositions()
{
    mousePosScreen = sf::Mouse::getPosition();
    mousePosWindow = sf::Mouse::getPosition(*window);
    if (view)
        window->setView(*view);
    view->setViewport(sf::FloatRect((window->getSize().x-350-window->getSize().x / 2)/ (float)window->getSize().x,
        (window->getSize().y-350-window->getSize().y / 2)/ (float)window->getSize().y,
        700 / (float)window->getSize().x, 700 / (float)window->getSize().y));
    mousePosView = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
}




void KeyboardButton::AssignKey(int button)
{
    this->button = sf::Keyboard::Key(button);   
}

bool KeyboardButton::Pressed() 
{
    if (sf::Keyboard::isKeyPressed(this->button))
    {
        PressedTwice();
        OnHold();
        return true;
    }
    return false;
}

void KeyboardButton::PressedTwice()
{

}

void KeyboardButton::OnHold()
{

}

void KeyboardButton::Released()
{

}
