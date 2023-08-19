#include "MainMenuState.h"
#include <fstream>

MainMenuState::MainMenuState(sf::RenderWindow* window,sf::View* view):
    State(window,view)
{
    button.InitButton(200,100, sf::Text("New Game", font), 0, 0, 
                      sf::Color::White, sf::Color::White, sf::Color::White,
                      sf::Color::Black, sf::Color::Black,sf::Color::Black,sf::Color::White,5.f);
}

void MainMenuState::Update(const float dt)
{
    UpdateMousePositions();
    if (button.Update(window))
    {
        *intState = 1;
    }
}

void MainMenuState::Render(sf::RenderTarget* target)
{
    if (target == nullptr)
    {
        target = window;
    }
    button.Render(window);
}

void MainMenuState::ExitState()
{
    delete this;
}
