#pragma once
#include <functional>

enum class ComponentType { Player, Bullet, Animation, Goblin, Fly,Slime ,Explosion};
class SceneNode;
class Component
{
public:
    typedef std::unique_ptr<Component> Ptr;
    Component(SceneNode* node);
    virtual void Update() = 0;

    ComponentType type = ComponentType::Player;
protected:
    SceneNode* node;
};

class PlayerComponent :public Component
{
public:
    PlayerComponent(SceneNode* node,sf::Texture& bulletTexture,std::function<void()> playerDiedFuncPtr);
    virtual void Update() override;
    float bulletDamage = 5;
private:
    void OnCollision(std::function<void(Collider& other)>baseOnCollisionPtr, Collider& other);
    sf::Texture& bulletTexture;
    std::function<void()> playerDiedFuncPtr = nullptr;
    sf::Vector2f bulletDirection;
    float bulletSpeed = 5.f;
    sf::Clock bulletTimer;
    float bulletCooldown=0.5f;
};

class AnimationComponent:public Component
{
private:
    sf::Sprite walkingLeftSprite;
    sf::Sprite walkingRightSprite;
    std::function<void(std::function<void()>)> componentAnimationUpdate = nullptr;
    bool componenetAnimationUpdateWasCalled = false;
    int noOfSprites;
    int spriteSize;
    int counter = 0;
    const int counterPoint;
    int textureRectLeft = 0;
    bool wasFacingRight = false;
public:
    AnimationComponent(SceneNode* node,sf::Sprite walkingLeftSprite,sf::Sprite walkingRightSprite,
                       int spriteWidth,int noOfSprites,int counterPoint);
    virtual void Update() override;
};

class BulletComponent :public Component
{
public:
    BulletComponent(SceneNode* node, sf::Vector2f velocity,float bulletDamage);
    float bulletDamage = 5.0f;
private:
    void OnCollision(std::function<void(Collider& other)>baseOnCollisionPtr, Collider& other);
    virtual void Update() override;
    sf::Vector2f velocity;
};

class EnemyComponent:public Component
{
public:
    EnemyComponent(SceneNode* node, SceneNode* player,int speed,int health,ComponentType type,
    std::function<void(float, float)> spawnExplosionPtr);
    virtual void Update() override;
    void OnCollision(std::function<void(Collider& other)>baseOnCollisionPtr, Collider& other);
    ComponentType type;
private:
    int health = 5;
    int speed = 2;
    sf::Vector2f direction;
    SceneNode* player;
    std::function<void(float, float)> spawnExplosionPtr;
};

class ExplosionComponent :public Component
{
public:
    ExplosionComponent(SceneNode* node, sf::Sprite explosionSprite,float counterPoint);
    virtual void Update() override;
    sf::Sprite explosionSprite;
    float counter = 0.f;
    float counterPoint = 0;
};
