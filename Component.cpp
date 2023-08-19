#pragma once
#include "SceneNode.h"

Component::Component(SceneNode* node):
    node(node)
{
    
}

PlayerComponent::PlayerComponent(SceneNode* node,sf::Texture& bulletTexture,std::function<void()> playerDiedFuncPtr):
    Component(node),
    bulletTexture(bulletTexture),
    playerDiedFuncPtr(playerDiedFuncPtr)
{
    bulletTimer.restart();
    type = ComponentType::Player;
    bulletDirection = sf::Vector2f(0, 0);
    node->componentOnCollision = std::bind(&PlayerComponent::OnCollision, this, std::placeholders::_1, std::placeholders::_2);
}

void PlayerComponent::Update()
{
    bulletDirection.x = 0; bulletDirection.y = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        node->Move(0, -4);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        node->Move(-4, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        node->Move(0, 4);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        node->Move(4, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        bulletDirection.y += -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        bulletDirection.x += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        bulletDirection.x += -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        bulletDirection.y += 1;
    

    if ((bulletDirection.x != 0 || bulletDirection.y != 0) && bulletTimer.getElapsedTime().asSeconds()>=bulletCooldown)
    {
        sf::Sprite bulletSprite(bulletTexture);
        bulletSprite.setScale(sf::Vector2f(2, 2));
        SceneNode* bullet = node->GetParent()->CreateAttachChild(node->GetParent(), bulletSprite);
        bullet->SetCollider(bullet, ColliderTag::Bullet);
        bullet->components.push_back(std::move(Component::Ptr(new BulletComponent(bullet, bulletDirection * bulletSpeed,
            bulletDamage))));
        bullet->SetWorldPosition(node->GetWorldPosition().x + bulletDirection.x * 50,
            node->GetWorldPosition().y + bulletDirection.y * 50);
        bulletTimer.restart();
    }
}

void PlayerComponent::OnCollision(std::function<void(Collider& other)> baseOnCollisionPtr, Collider& other)
{
    baseOnCollisionPtr(other);
    if (other.thisTag == ColliderTag::Enemy)
    {
        playerDiedFuncPtr();
    }
}

AnimationComponent::AnimationComponent(SceneNode* node,sf::Sprite walkingLeftSprite,sf::Sprite walkingRightSprite,
                                       int spriteSize, int noOfSprites,int counterPoint):
    Component(node),
    walkingLeftSprite(walkingLeftSprite),
    walkingRightSprite(walkingRightSprite),
    spriteSize(spriteSize),
    noOfSprites(noOfSprites),
    counterPoint(counterPoint)
{
    type = ComponentType::Animation;
}

void AnimationComponent::Update()
{
    if (componentAnimationUpdate != nullptr && componenetAnimationUpdateWasCalled == false)
    {
        componenetAnimationUpdateWasCalled = true;
        componentAnimationUpdate(std::bind(&AnimationComponent::Update, this));
        return;
    }
    if (abs(node->GetVelocity().x) >= abs(node->GetVelocity().y))
        counter += abs(node->GetVelocity().x);
    else
        counter += abs(node->GetVelocity().y);
    if (counter >= counterPoint)
    {
        if (node->mSprite.getTextureRect().left + spriteSize == spriteSize * noOfSprites)
        {
            if (node->mSprite.getTextureRect().width < 0)
                textureRectLeft = 0 + spriteSize;
            else
                textureRectLeft = 0;
        }
        else
        {
            textureRectLeft = node->mSprite.getTextureRect().left + spriteSize;
        }
        counter -= counterPoint;
        node->mSprite.setTextureRect(sf::IntRect(textureRectLeft, 0, node->mSprite.getTextureRect().width,
            node->mSprite.getTextureRect().height));
    }
    if (node->GetVelocity().x == 0 && node->GetVelocity().y == 0)
    {
        if (node->mSprite.getTextureRect().width < 0)
            textureRectLeft = 0 + spriteSize;
        else
            textureRectLeft = 0;
        node->mSprite.setTextureRect(sf::IntRect(textureRectLeft, 0, node->mSprite.getTextureRect().width,
            node->mSprite.getTextureRect().height));
    }
    
    if (wasFacingRight == true && node->GetVelocity().x < 0)
    {
        node->mSprite = walkingLeftSprite;
        node->mSprite.setTextureRect(sf::IntRect(textureRectLeft, 0, node->mSprite.getTextureRect().width,
            node->mSprite.getTextureRect().height));
        wasFacingRight = false;
    }
    else if(wasFacingRight == false && node->GetVelocity().x > 0)
    {
        node->mSprite = walkingRightSprite;
        node->mSprite.setTextureRect(sf::IntRect(textureRectLeft, 0, node->mSprite.getTextureRect().width,
            node->mSprite.getTextureRect().height));
        wasFacingRight = true;
    }
    componenetAnimationUpdateWasCalled = false;
}

BulletComponent::BulletComponent(SceneNode* node,sf::Vector2f velocity,float bulletDamage):
    Component(node),
    velocity(velocity),
    bulletDamage(bulletDamage)
{
    type = ComponentType::Bullet;
    node->componentOnCollision = std::bind(&BulletComponent::OnCollision, this, std::placeholders::_1,
        std::placeholders::_2);
}

void BulletComponent::OnCollision(std::function<void(Collider& other)> baseOnCollisionPtr,Collider& other)
{
    baseOnCollisionPtr(other);
    if (other.thisTag != ColliderTag::Bullet)
    {
        SceneNode::GetDetachChildRequests().push(node);
    }
}

void BulletComponent::Update()
{
    node->Move(velocity.x, velocity.y);
}
