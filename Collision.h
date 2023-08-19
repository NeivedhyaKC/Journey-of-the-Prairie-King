#pragma once 
#include<SFML/Graphics.hpp>
#include<iostream>
#include<functional>
#include<list>

class SceneNode;
enum class ColliderTag { Default, Player, Enemy, Powerup, Bullet,Trigger };
enum class Direction { Down, Left, Right, Up };

class Collider
{
    friend class CollisionSystem;
public:
    Direction direction = Direction::Left;
    const ColliderTag thisTag = ColliderTag::Default;
    SceneNode* GetNode();
    bool operator==(const Collider& other) const;
private:
    Collider(SceneNode* node, std::function<sf::FloatRect()> rectFuncPtr,
        std::function<sf::Vector2f()> posFuncPtr,std::function<void(Collider& other)> onCollisionPtr,
        const int ID, const ColliderTag tag);
    std::function<sf::FloatRect()> rectFuncPtr;
    std::function<sf::Vector2f()> posFuncPtr;
    std::function<void(Collider& other)> onCollisionPtr;
    SceneNode* node = nullptr;
    const int ID = 0;
};

class CollisionSystem
{
public:
    static CollisionSystem* getInstance();
    Collider* AddCollider(SceneNode* node, std::function<sf::FloatRect()> rectFuncPtr,
        std::function<sf::Vector2f()> posFuncPtr,std::function<void(Collider& other)> onCollisionPtr,
        const int ID,const ColliderTag tag);
    void RemoveCollider(Collider* collider);
    void CollisionDetection();
private:
    CollisionSystem();
    static CollisionSystem* instance;
    std::list<Collider> colliders;
};

