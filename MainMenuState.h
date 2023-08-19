#pragma once
#include "State.h"

class MainMenuState :
    public State
{

public:
    MainMenuState(sf::RenderWindow* window,sf::View* view);
    virtual void ExitState();
    
private:
    virtual void Update(const float dt);
    virtual void Render(sf::RenderTarget* target = nullptr);

    Button button;
};
