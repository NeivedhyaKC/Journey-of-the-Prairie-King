#include "GameState.h"

GameState::GameState(sf::RenderWindow* window, sf::View* view) :
    State(window, view)
{
    InitKeybinds();
    LoadTextures();
    LoadLevel(levelNumber);

    srand(time(0));
    enemySpawnTimer.restart();
    levelTimer.restart();
    spawnEnemyThreshold = RandomRange(0.5f, 3.0f);
    totalTime = 60.0f;
}

void GameState::LoadTextures()
{
    try
    {
        textureHolder.load(Textures::TextureID::Goblin, "Textures/Goblin.png",sf::IntRect(0,0,96,16));
        textureHolder.load(Textures::TextureID::Slime, "Textures/Slime.png",sf::IntRect(0,0,96,16));
        textureHolder.load(Textures::TextureID::Explosion, "Textures/Explosion.png",sf::IntRect(0,0,184,32));
        textureHolder.load(Textures::TextureID::Fly, "Textures/Fly.png",sf::IntRect(0,0,64,16));
        textureHolder.load(Textures::TextureID::Background, "Textures/GroundAsset.png", sf::IntRect(640, 0, 20, 20));
        textureHolder.load(Textures::TextureID::BackgroundSample, "Textures/Grass.png",sf::IntRect(0,0,30,30));
        textureHolder.load(Textures::TextureID::PlayerRightWalk, "Textures/Player!.png",sf::IntRect(0,0,256,32));
        textureHolder.load(Textures::TextureID::PlayerLeftWalk, "Textures/Player!.png", sf::IntRect(0, 256, 256, 32));
        textureHolder.load(Textures::TextureID::Bullet, "Textures/Bullet.png",sf::IntRect(0,0,10,10));
        textureHolder.get(Textures::TextureID::Background).setRepeated(true);
    }
    catch (std::runtime_error err)
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

void GameState::LoadLevel(int levelNumber)
{
    sceneGraph.reset(new SceneNode(nullptr, sf::Sprite()));
    sceneGraph->SetWorldPosition(0, 0);

    sf::Sprite backGroundSprite(textureHolder.get(Textures::TextureID::Background));
    backGroundSprite.setScale(sf::Vector2f(2.5f, 2.5f));
    backGroundSprite.setTextureRect(sf::IntRect(0, 0, 280, 280));
    SceneNode* backGroundNode = sceneGraph->CreateAttachChild(sceneGraph.get(), backGroundSprite);
    backGroundNode->SetWorldPosition(0,0);

    //Player
    sf::Sprite PlayerLeftSprite(textureHolder.get(Textures::TextureID::PlayerLeftWalk));
    sf::Sprite PlayerRightSprite(textureHolder.get(Textures::TextureID::PlayerRightWalk));
    PlayerLeftSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    PlayerLeftSprite.setScale(sf::Vector2f(2.5f,2.5f));
    PlayerRightSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    PlayerRightSprite.setScale(sf::Vector2f(2.5f, 2.5f));
    Player = sceneGraph->CreateAttachChild(sceneGraph.get(), PlayerLeftSprite);
    Player->components.push_back(std::move(Component::Ptr(new PlayerComponent(Player,
        textureHolder.get(Textures::TextureID::Bullet),std::bind(&GameState::PlayerDied,this)))));
    Player->SetWorldPosition(0, 0);
    Player->SetCollider(Player, ColliderTag::Player);
    Player->components.push_back(std::move(Component::Ptr(new AnimationComponent(Player,PlayerLeftSprite,PlayerRightSprite,
        32, 8, 18))));

    std::string filename;
    switch (levelNumber)
    {
    case 1 :
        filename = "Levels/LevelFile1.txt";
        break;
    case 2:
        filename = "Levels/LevelFile1.txt";
        break;
    case 3:
        filename = "Levels/LevelFile1.txt";
        break;
    }
    
    int num;
    std::ifstream levelFile(filename); 
    if (levelFile.is_open())
    {
        for (int i = -325; i <= 325; i += 50)
        {
            for (int j = -325; j <= 325; j += 50)
            {
                levelFile >> num;
                switch (num)
                {
                case 0 :
                    break;
                case 1 :
                    sf::Sprite backGroundSample(textureHolder.get(Textures::TextureID::BackgroundSample));
                    backGroundSample.setScale(sf::Vector2f(1.6667f, 1.6667f));
                    SceneNode * backGroundNode1 = sceneGraph->CreateAttachChild(sceneGraph.get(), backGroundSample);
                    backGroundNode1->SetWorldPosition(j, i);
                    backGroundNode1->SetCollider(backGroundNode1, ColliderTag::Default);
                    break;
                }
            }
        }
        sf::Vector2f enemySpawnLocation;
        for (int i = 0; i < 12; i++)
        {
            levelFile >> enemySpawnLocation.x;
            levelFile >> enemySpawnLocation.y;
            enemySpawnLocations[i] = enemySpawnLocation;
            SceneNode* invisibleBlock = sceneGraph->CreateAttachChild(sceneGraph.get(),sf::Sprite());
            invisibleBlock->SetWorldPosition(enemySpawnLocation.x, enemySpawnLocation.y);
            invisibleBlock->SetTrigger(invisibleBlock,60,60);
        }
    }
    levelFile.close();
}

void GameState::SpawnExplosion(float x, float y)
{
    sf::Sprite ExplosionSprite(textureHolder.get(Textures::TextureID::Explosion));
    ExplosionSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    ExplosionSprite.setScale(sf::Vector2f(2, 2));
    SceneNode* explosion = sceneGraph->CreateAttachChild(sceneGraph.get(), ExplosionSprite);
    explosion->components.push_back(std::move(Component::Ptr(new ExplosionComponent(explosion, ExplosionSprite, 32))));
    explosion->SetWorldPosition(x, y);
}

void GameState::SpawnEnemies(sf::Vector2f worldPosition, int enemy)
{
    switch (enemy)
    {
    case 0:
    {
        sf::Sprite goblinRightSprite(textureHolder.get(Textures::TextureID::Goblin));
        sf::Sprite goblinLeftSprite(textureHolder.get(Textures::TextureID::Goblin));
        goblinLeftSprite.setTextureRect(sf::IntRect(16, 0, -16, 16));
        goblinRightSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
        goblinLeftSprite.setScale(sf::Vector2f(3, 3));
        goblinRightSprite.setScale(sf::Vector2f(3, 3));
        SceneNode* goblin = sceneGraph->CreateAttachChild(sceneGraph.get(), goblinLeftSprite);
        goblin->components.push_back(std::move(Component::Ptr(new EnemyComponent(goblin, Player, 2, 5, ComponentType::Goblin,
            std::bind(&GameState::SpawnExplosion, this, std::placeholders::_1, std::placeholders::_2)))));
        goblin->components.push_back(std::move(Component::Ptr(new AnimationComponent(goblin, goblinLeftSprite, goblinRightSprite,
            16, 6, 16))));
        goblin->SetCollider(goblin, ColliderTag::Enemy);
        goblin->SetWorldPosition(worldPosition.x, worldPosition.y);
        break;
    }
    case 1:
    {
        sf::Sprite slimeRightSprite(textureHolder.get(Textures::TextureID::Slime));
        sf::Sprite slimeLeftSprite(textureHolder.get(Textures::TextureID::Slime));
        slimeRightSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
        slimeLeftSprite.setTextureRect(sf::IntRect(16, 0, -16, 16));
        slimeLeftSprite.setScale(sf::Vector2f(3.5f, 3.5f));
        slimeRightSprite.setScale(sf::Vector2f(3.5f, 3.5f));
        SceneNode* slime = sceneGraph->CreateAttachChild(sceneGraph.get(), slimeRightSprite);
        slime->components.push_back(std::move(Component::Ptr(new EnemyComponent(slime, Player, 1, 10, ComponentType::Slime,
            std::bind(&GameState::SpawnExplosion, this, std::placeholders::_1, std::placeholders::_2)))));
        slime->components.push_back(std::move(Component::Ptr(new AnimationComponent(slime, slimeLeftSprite, slimeRightSprite,
            16, 6, 10))));
        slime->SetCollider(slime, ColliderTag::Enemy);
        slime->SetWorldPosition(worldPosition.x, worldPosition.y);
        break;
    }
    case 2:
    {
        sf::Sprite flyRightSprite(textureHolder.get(Textures::TextureID::Fly));
        sf::Sprite flyLeftSprite(textureHolder.get(Textures::TextureID::Fly));
        flyRightSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
        flyLeftSprite.setTextureRect(sf::IntRect(16, 0, -16, 16));
        flyRightSprite.setScale(sf::Vector2f(3, 3));
        flyLeftSprite.setScale(sf::Vector2f(3, 3));
        SceneNode* fly = sceneGraph->CreateAttachChild(sceneGraph.get(), flyRightSprite);
        fly->components.push_back(std::move(Component::Ptr(new EnemyComponent(fly, Player, 2, 10, ComponentType::Fly,
            std::bind(&GameState::SpawnExplosion, this, std::placeholders::_1, std::placeholders::_2)))));
        fly->components.push_back(Component::Ptr(new AnimationComponent(fly, flyLeftSprite, flyRightSprite,
            16, 4, 32)));
        fly->SetWorldPosition(worldPosition.x, worldPosition.y);
        fly->SetCollider(fly, ColliderTag::Enemy);
    }
    }
}

void GameState::PlayerDied()
{
    Player->SetWorldPosition(0, 0);
    noOfLives--;
    if (noOfLives == 0)
    {
        GameOver();
    }
    totalTime += 25.0f;
    sceneGraph->WipeAllEnemies();
}

void GameState::GameOver()
{
    std::cout << "Game Over" << std::endl;
    window->close();
}

void GameState::PlayerWon()
{
    if (levelNumber == 3)
    {
        //GameWon()
    }
    levelNumber++;
    sceneGraph->ClearChildren();
    LoadLevel(levelNumber);
    enemySpawnTimer.restart();
    levelTimer.restart();
    spawnEnemyThreshold = RandomRange(0.5f, 3.0f);
    totalTime = 60.0f;
}

int GameState::RandomRange(int min, int max)
{
    return rand() % (max - min) + min;
}

float GameState::RandomRange(float min, float max)
{
    return (float)RandomRange((int)min,(int) max) + float(RandomRange(0, (int)max) / (float)max);
}

void GameState::RandomlyGenerateEnemies()
{
    if (enemySpawnTimer.getElapsedTime().asSeconds() > spawnEnemyThreshold)
    {
        enemySpawnTimer.restart();
        spawnEnemyThreshold = RandomRange(0.5f, 3.0f);
        SpawnEnemies(enemySpawnLocations[RandomRange(0, 12)], levelEnemies[levelNumber][RandomRange(0, 10)]);
    }
}

void GameState::Update(const float dt)
{
    timeLeft = totalTime - levelTimer.getElapsedTime().asSeconds();
    if (timeLeft <= 0.0f)
    {
        //PlayerWon();
    }
    RandomlyGenerateEnemies();
    UpdateMousePositions();
    sceneGraph->Update();
    sceneGraph->MoveAfterCollision();
}


void GameState::Render(sf::RenderTarget* target)
{
    if (!target)
    {
        target = window;
    }
    window->draw(*sceneGraph.get());
}

void GameState::InitKeybinds()
{
    //Do not change order of reading controls from file
    std::ifstream gameControlsConfig("Config/gameControls.txt");
    int button;
    if (gameControlsConfig.is_open())
    {
        gameControlsConfig >> button;    wButton.AssignKey(button);
        gameControlsConfig >> button;    aButton.AssignKey(button);
        gameControlsConfig >> button;    sButton.AssignKey(button);
        gameControlsConfig >> button;    dButton.AssignKey(button);
        gameControlsConfig >> button;    fButton.AssignKey(button);
        gameControlsConfig >> button;    spaceButton.AssignKey(button);
        gameControlsConfig >> button;    qButton.AssignKey(button);
        gameControlsConfig >> button;    eButton.AssignKey(button);
        gameControlsConfig >> button;    rButton.AssignKey(button);
        gameControlsConfig >> button;    tabButton.AssignKey(button);
        gameControlsConfig >> button;    escapeButton.AssignKey(button);
    }
    gameControlsConfig.close();
}

void GameState::ExitState()
{

    delete this;
}
