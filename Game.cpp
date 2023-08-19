#include "Game.h"

void Game::initWindow()
{
    window.create(sf::VideoMode(1080,720), "Top-Down Game",sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(120);
    window.setVerticalSyncEnabled(false);
    backgroundView.setCenter(1000,0);
    backgroundView.setSize(window.getSize().x, window.getSize().y);
    gameView.setCenter(0, 0);
    gameView.setSize(700, 700);
    gameView.setViewport(sf::FloatRect((window.getSize().x - 350- window.getSize().x/2) / (float)window.getSize().x, 
        (window.getSize().y - 350-window.getSize().y / 2) / (float)window.getSize().y,
        700 / (float)window.getSize().x, 700 / (float)window.getSize().y));
    //Dont need to set viewport , it is automatically set to window size in setView   
    window.setView(backgroundView);
    window.setView(gameView);
}

void Game::initStates()
{
    State::intState = &intState;
    currentState = new MainMenuState(&window,&gameView);
}

Game::Game()
{
    initWindow();
    initStates();
}

void Game::UpdateSFMlEvents()
{
    if (window.pollEvent(windowEvent))
    {
        if (windowEvent.type == sf::Event::Closed)
        {
            window.close();
        }
    }
    
}

void Game::UpdateDt()
{
    dt += dtClock.restart().asSeconds();
}

void Game::Update(float dt)
{
    window.setView(backgroundView);
    UpdateSFMlEvents();
    
    if (currentState != nullptr)
    {
        currentState->Update(dt);
    }


    if (prevIntState != intState)
    {
        ChangeState();
    }
    prevIntState = intState;
}

void Game::Render()
{
    window.clear(sf::Color::Blue);
    if (currentState != nullptr)
    {
        currentState->Render();
    }

    window.display();
}

void Game::Quit()
{
    window.close();
}

void Game::ChangeState()
{
    currentState->ExitState();
    switch (intState)
    {
    case 0: currentState = new MainMenuState(&window, &gameView);
        break;
    case 1: currentState = new GameState(&window, &gameView);
        break;
    }
}

void Game::Run()
{
    while (window.isOpen())
    {
        UpdateDt();
        while (dt > DELTATIME)
        {
            Update(DELTATIME);
            dt -= DELTATIME;
        }
        Render();
    }
}