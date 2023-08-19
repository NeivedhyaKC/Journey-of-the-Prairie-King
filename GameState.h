#pragma once
#include "State.h"
#include "SceneNode.h"

class GameState : public State
{
public :
    GameState(sf::RenderWindow* window, sf::View* view);
    virtual void ExitState();
private:
    int RandomRange(int min, int max);
    float RandomRange(float min, float max);
    void RandomlyGenerateEnemies();
    virtual void Update(const float dt);
    virtual void Render(sf::RenderTarget* target = nullptr);
    virtual void InitKeybinds();
    void LoadTextures();
    void LoadLevel(int levelNumber);
    void SpawnExplosion(float x,float y);
    void SpawnEnemies(sf::Vector2f worldPosition, int enemy);
    void PlayerDied();
    void GameOver();
    void PlayerWon();

    int noOfLives = 3;
    float timeLeft = 0.f;
    float totalTime = 0.f;
    sf::Vector2f enemySpawnLocations[12];
    SceneNode::Ptr sceneGraph;
    SceneNode* Player = nullptr;
    ResouceHolder<sf::Texture, Textures::TextureID> textureHolder;

    sf::Clock levelTimer;
    int levelNumber = 1;
    sf::Clock enemySpawnTimer;
    float spawnEnemyThreshold = 0.f;
    int levelEnemies[3][10] = { { 0,0,0,0,0,0,0,1,1,2 },
                                { 0,0,0,0,1,1,1,1,2,2 },
                                { 0,0,0,1,1,1,1,2,2,2 } };

protected:
    KeyboardButton wButton;
    KeyboardButton aButton;
    KeyboardButton sButton;
    KeyboardButton dButton;
    KeyboardButton fButton;
    KeyboardButton spaceButton;
    KeyboardButton eButton;
    KeyboardButton qButton;
    KeyboardButton rButton;
    KeyboardButton tabButton;
    KeyboardButton escapeButton;
};  
