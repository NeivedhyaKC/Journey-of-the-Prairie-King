#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include "Button.h"

class State
{
public :
    State(sf::RenderWindow* window,sf::View* view);
    virtual void Update(const float dt) = 0;
    virtual void Render(sf::RenderTarget* target = nullptr) = 0;
    virtual void ExitState() = 0;
    
    static int* intState;

protected :
    sf::RenderWindow* window;
    sf::View* view;
    sf::Font font;
    sf::Vector2i mousePosScreen;
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    void UpdateMousePositions();
};

class KeyboardButton
{
private:
    //sf::Clock keyClock;  Uncomment when implementing the below three functions
public:
    sf::Keyboard::Key button;

    void AssignKey(int button);
    bool Pressed();

    //Still have to implement these three functions
    void PressedTwice();
    void OnHold();
    void Released();
};