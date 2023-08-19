#include "Collision.h"
#include "SceneNode.h"

CollisionSystem* CollisionSystem::getInstance()
{
    if (instance==nullptr)
    {
        instance = new CollisionSystem();
    }
    return instance;
}
CollisionSystem* CollisionSystem::instance = nullptr;

Collider* CollisionSystem::AddCollider(SceneNode* node, std::function<sf::FloatRect()> rectFuncPtr,
                           std::function<sf::Vector2f()> posFuncPtr, 
                           std::function<void(Collider& other)> onCollisionPtr,const int ID,const ColliderTag tag)
{
    colliders.push_back(Collider(node, rectFuncPtr, posFuncPtr, onCollisionPtr,ID,tag));
    return &colliders.back();
}

void CollisionSystem::RemoveCollider(Collider* collider)
{
    if (collider != nullptr)
    {
        colliders.remove(*collider);
    }
}

CollisionSystem::CollisionSystem()
{

}

void CollisionSystem::CollisionDetection()
{
    for (Collider& collider : colliders)
    {
        if (collider.thisTag == ColliderTag::Player || collider.thisTag == ColliderTag::Enemy||
            collider.thisTag==ColliderTag::Bullet)
        {
            for (Collider& collider2 : colliders)
            {
                if (collider.ID != collider2.ID && collider.rectFuncPtr().intersects(collider2.rectFuncPtr()))
                {
                    sf::Vector2f distance = collider2.posFuncPtr() - collider.posFuncPtr();
                    if (abs(distance.x) > abs(distance.y))
                    {
                        if (distance.x >= 0)
                        {
                            collider.direction = Direction::Right;
                            collider2.direction = Direction::Left;
                            collider.onCollisionPtr(collider2);
                        }
                        else
                        {
                            collider.direction = Direction::Left;
                            collider2.direction = Direction::Right;
                            collider.onCollisionPtr(collider2);
                        }
                    }
                    else
                    {
                        if (distance.y >= 0)
                        {
                            collider.direction = Direction::Down;
                            collider2.direction = Direction::Up;
                            collider.onCollisionPtr(collider2);
                        }
                        else
                        {
                            collider.direction = Direction::Up;
                            collider2.direction = Direction::Down;
                            collider.onCollisionPtr(collider2);
                        }
                    }
                }
            }
        }
    }
}

SceneNode* Collider::GetNode()
{
    return node;
}

bool Collider::operator==(const Collider& other) const
{
    if (ID == other.ID)
    {
        return true;
    }
    return false;
}

Collider::Collider(SceneNode* node,std::function<sf::FloatRect()> rectFuncPtr,
          std::function<sf::Vector2f()> posFuncPtr, std::function<void(Collider& other)> onCollisionPtr,
          const int ID,const ColliderTag tag):
    node(node),
    rectFuncPtr(rectFuncPtr),
    posFuncPtr(posFuncPtr),
    onCollisionPtr(onCollisionPtr),
    ID(ID), thisTag(tag)
{

}