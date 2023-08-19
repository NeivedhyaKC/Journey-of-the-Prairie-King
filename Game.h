#pragma once
#include "GameState.h"
#include "MainMenuState.h"
class Game
{
private:
#define DELTATIME 0.016f
public :
    Game();

    void Run();

private:
    void initWindow();
    void initStates();

    void UpdateSFMlEvents();
    void UpdateDt();
    void Update(float dt);
    void Render();
    void Quit();
    void ChangeState();

    sf::RenderWindow window;
    sf::Event windowEvent;
    sf::View backgroundView;
    sf::View gameView;
    sf::Clock dtClock;
    float dt = 0;
    int intState = 0;
    int prevIntState = 0;
    State* currentState = nullptr;
};