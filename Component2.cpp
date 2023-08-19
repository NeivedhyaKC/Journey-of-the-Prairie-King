#include"SceneNode.h"

EnemyComponent::EnemyComponent(SceneNode* node, SceneNode* player,int speed, int health,ComponentType type,
    std::function<void(float,float)> spawnExplosionPtr):
    Component(node),
    player(player),
    direction(sf::Vector2f(0,0)),
    health(health),
    speed(speed),
    type(type),
    spawnExplosionPtr(spawnExplosionPtr)
{
    node->componentOnCollision = std::bind(&EnemyComponent::OnCollision, this, std::placeholders::_1,
        std::placeholders::_2);
}
void EnemyComponent::Update()
{
    direction = player->GetWorldPosition() - node->GetWorldPosition();
    int quotient = 0;
    if (direction.x == 0)
    {
        node->Move(1 * speed, 0);
        return;
    }
    else if (direction.y == 0)
    {
        node->Move(0, 1 * speed);
        return;
    }
    quotient = abs(direction.x) / abs(direction.y);
    if (quotient >= 0.5f && quotient <= 2)
    {
        direction.x = direction.x / abs(direction.x);
        direction.y = direction.y / abs(direction.y);
    }
    else if (quotient > 2)
    {
        direction.x = direction.x / abs(direction.x);
        direction.y = 0;
    }
    else if (quotient < 0.5f)
    {
        direction.x = 0;
        direction.y = direction.y / abs(direction.y);
    }
    node->Move(direction.x * speed, direction.y * speed);
}

void EnemyComponent::OnCollision(std::function<void(Collider& other)> baseOnCollisionPtr, Collider& other)
{
    baseOnCollisionPtr(other);
    if (other.thisTag == ColliderTag::Bullet)
    {
        BulletComponent* bulletComponent = 
            dynamic_cast<BulletComponent*>(other.GetNode()->GetComponent(ComponentType::Bullet));
        health -= bulletComponent->bulletDamage;
        if (health <= 0)
        {
            SceneNode::GetDetachChildRequests().push(node);
            spawnExplosionPtr(node->GetWorldPosition().x, node->GetWorldPosition().y);
        }
    }
}

ExplosionComponent::ExplosionComponent(SceneNode* node, sf::Sprite explosionSprite,float counterPoint):
    Component(node),
    explosionSprite(explosionSprite),
    counterPoint(counterPoint)
{
    type = ComponentType::Explosion;
}

void ExplosionComponent::Update()
{
    counter += 4;
    if (counter > counterPoint)
    {
        counter -= counterPoint;
        if (explosionSprite.getTextureRect().left + explosionSprite.getTextureRect().width != explosionSprite.getTextureRect().width * 6)
        {
            node->mSprite = explosionSprite;
            explosionSprite.setTextureRect(sf::IntRect(explosionSprite.getTextureRect().left + explosionSprite.getTextureRect().width, 0,
                explosionSprite.getTextureRect().width, explosionSprite.getTextureRect().height));
        }
        else
        {
            SceneNode::GetDetachChildRequests().push(node);
        }
    }
}
