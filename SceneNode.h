#pragma once
#include <vector>
#include<queue>
#include "Collision.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <assert.h>
#include "Component.h"

namespace Textures
{
    enum class TextureID { Goblin, Slime, Fly, Explosion, Background, BackgroundSample, PlayerRightWalk, PlayerLeftWalk ,
    Bullet};
}

namespace SoundBuffers
{
    enum class SoundBufferID{ };
}

template<typename Resource,typename Identifier>
class ResouceHolder
{
private:
    std::map<Identifier, std::unique_ptr<Resource>> resourceMap;
public:
    void load(Identifier key, const std::string& filename);
    Resource& get(const Identifier key);
    void load(Identifier key,const std::string& fileName, sf::IntRect intRect);
};


//SceneNodes which are not inside the viewport are being rendered.(No occlusion culling)
class SceneNode :public sf::NonCopyable,public sf::Drawable, public sf::Transformable
{
public :
    typedef std::unique_ptr<SceneNode> Ptr;

private:
    sf::Vector2f velocity;
    SceneNode* mParent = nullptr;
    std::vector<SceneNode::Ptr> mChildren;
    mutable sf::Transform worldTransform;
    mutable bool moved = true;
    bool componentOnCollisionWasCalled = false;
    
    void OnCollision(Collider& other);//OnCollisionEnter ,OnCollision and OnCollisionExit could be implemented
    const int  AssignID();
    void DetachChildren();
    SceneNode::Ptr detachChild(SceneNode& child);

public:
    SceneNode(SceneNode* parent, sf::Sprite sprite);
    sf::FloatRect GetRect();
    Component* GetComponent(ComponentType type);
    static std::queue<SceneNode*>& GetDetachChildRequests();

    inline void attachChild(SceneNode::Ptr child);
    SceneNode* CreateAttachChild(SceneNode* parent, sf::Sprite sprite);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

    virtual void Update();
    virtual void UpdateChildren();
    void MoveAfterCollision();
    void WipeAllEnemies();
    void ClearChildren();

    inline void Move(float x, float y);

    inline void SetCollider(SceneNode* node, ColliderTag tag);
    inline void SetTrigger(SceneNode* node,int triggerSizeX,int triggerSizeY);

    sf::Sprite& GetSprite();
    SceneNode* GetParent();
    const sf::Vector2f GetVelocity();
    const sf::Vector2f GetWorldPosition() const;
    const sf::Vector2f GetLocalPosition() const;
    void SetWorldPosition(float x,float y);
    inline void SetLocalPosition(float x, float y);
    inline void SetVelocity(float x, float y);
public:
    const int ID = 0;
    std::vector<Component::Ptr> components;
    sf::Sprite mSprite;
    Collider* collider = nullptr;
    std::function<void(std::function<void(Collider& other)>,Collider& other)> componentOnCollision;
};

template<typename Resource, typename Identifier>
inline void ResouceHolder<Resource, Identifier>::load(Identifier key, const std::string& filename)
{
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(filename))
    {
        throw std::runtime_error("failed to load texture" + filename);
    }
    auto inserted = resourceMap.insert(std::make_pair(key, std::move(resource)));
    assert(inserted.second);
}

template<typename Resource, typename Identifier>
Resource& ResouceHolder<Resource, Identifier>::get(const Identifier key)
{
    auto resource = resourceMap.find(key);
    assert(resource != resourceMap.end());//Trying to get a resource that is not there in the map.
    return *(resource->second);
}

template<typename Resource, typename Identifier>
inline void ResouceHolder<Resource, Identifier>::load(Identifier key, const std::string& fileName, sf::IntRect intRect)
{
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(fileName,intRect))
    {
        throw std::runtime_error("failed to load texture" + fileName);
    }
    auto inserted = resourceMap.insert(std::make_pair(key, std::move(resource)));
    assert(inserted.second);
}

void SceneNode::attachChild(SceneNode::Ptr child)
{
    mChildren.push_back(std::move(child));
}

inline void SceneNode::SetCollider(SceneNode* node, ColliderTag tag)
{
    if (collider == nullptr)
    {
        collider = CollisionSystem::getInstance()->AddCollider(node, std::bind(&SceneNode::GetRect, this),
            std::bind(&SceneNode::GetWorldPosition, this),
            std::bind(&SceneNode::OnCollision, this, std::placeholders::_1), ID, tag);
    }
}

inline void SceneNode::SetTrigger(SceneNode* node,int triggerSizeX,int triggerSizeY)
{
    mSprite.setTextureRect(sf::IntRect(0, 0, triggerSizeX, triggerSizeY));
    if (collider == nullptr)
    {
        collider = CollisionSystem::getInstance()->AddCollider(node, [&]()->sf::FloatRect
            {
                return sf::FloatRect(GetWorldPosition() - sf::Vector2f(mSprite.getTextureRect().width / 2, mSprite.getTextureRect().height / 2),
                    sf::Vector2f(mSprite.getTextureRect().width, mSprite.getTextureRect().height));
            }, std::bind(&SceneNode::GetWorldPosition, this), std::bind(&SceneNode::OnCollision, this, std::placeholders::_1),
                ID, ColliderTag::Trigger);
    }
}

void SceneNode::SetLocalPosition(float x, float y)
{
    setPosition(x, y);
    moved = true;
}   

inline void SceneNode::SetVelocity(float x, float y)
{
    velocity.x = x;
    velocity.y = y;
    moved = true;
}

void SceneNode::Move(float x, float y)
{
    velocity.x += x;
    velocity.y += y;
    moved = true;
}
