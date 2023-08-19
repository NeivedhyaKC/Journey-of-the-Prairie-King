#pragma once
#include<SFML/Graphics.hpp>
class World
{
    //FSM for level with EnterState and ExitState functions to handle transitions

    //create 2d array for level( or a level class)    allocate 2d array dynamically
    //support for view(zoom in or out when level is large or small)
    //setView every RenderFrame
    //call Update and Render every frame
    //contain sceneGraph
    //parser funnctions for 2D array/level
    //support for transitions to cutscenes
    void Update();
    void Render();
    void LevelFactoryFunction(int gridSize,int levelNumber);
    void CreateLevel();
private:
    sf::Sprite backGround;
};

