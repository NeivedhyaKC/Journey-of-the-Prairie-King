#include "SceneNode.h"
#include <assert.h>


sf::FloatRect SceneNode::GetRect()
{
    return sf::FloatRect(GetWorldPosition()- sf::Vector2f(mSprite.getGlobalBounds().width/3, mSprite.getGlobalBounds().height/3),
        sf::Vector2f(mSprite.getGlobalBounds().width/1.5f,mSprite.getGlobalBounds().height/1.5f));
}

Component* SceneNode::GetComponent(ComponentType type)
{
    for (unsigned int i = 0; i < components.size(); i++)
    {
        if (components[i]->type == type)
        {
            return components[i].get();
        }
    }
    assert(false);//Component not present in components.
    return nullptr;
}

std::queue<SceneNode*>& SceneNode::GetDetachChildRequests()
{
    static std::queue<SceneNode*> detachChildRequests;
    return detachChildRequests;
}

const int SceneNode::AssignID()
{
    static int IDnum = 0;
    int ID = IDnum;
    IDnum++;
    return ID;
}

void SceneNode::DetachChildren()
{
    while (!GetDetachChildRequests().empty())
    {
        SceneNode* front = GetDetachChildRequests().front();
        front->GetParent()->detachChild(*front);
        GetDetachChildRequests().pop();
    }
}

void SceneNode::OnCollision(Collider& other)
{
    if (other.thisTag != ColliderTag::Trigger ||(other.thisTag==ColliderTag::Trigger && (collider->thisTag==ColliderTag::Player || 
        collider->thisTag==ColliderTag::Bullet)))
    {
        if (componentOnCollision != nullptr && componentOnCollisionWasCalled == false)
        {
            componentOnCollisionWasCalled = true;
            componentOnCollision(std::bind(&SceneNode::OnCollision, this, std::placeholders::_1), other);
            return;
        }
        if (other.direction == Direction::Down)
        {
            Move(0, 4);
        }
        else if (other.direction == Direction::Up)
        {
            Move(0, -4);
        }
        else if (other.direction == Direction::Left)
        {
            Move(-4, 0);
        }
        else if (other.direction == Direction::Right)
        {
            Move(4, 0);
        }
        componentOnCollisionWasCalled = false;
    }
}

SceneNode::SceneNode(SceneNode* parent,sf::Sprite sprite):
    mParent(parent),
    mSprite(sprite),
    ID(AssignID())
{
    setOrigin(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);
}

SceneNode* SceneNode::CreateAttachChild(SceneNode* parent, sf::Sprite sprite)
{
    SceneNode::Ptr sceneNode(new SceneNode(parent, sprite));
    mChildren.push_back(std::move(sceneNode));
    return mChildren[mChildren.size() - 1].get();
}

SceneNode::Ptr SceneNode::detachChild(SceneNode& child)
{
    SceneNode::Ptr found;
    for (auto itr = mChildren.begin(); itr != mChildren.end(); itr++)
    {
        if (itr->get() == &child)
        {
            child.mParent = nullptr;
            CollisionSystem::getInstance()->RemoveCollider(child.collider);
            found = std::move(*itr);
            components.clear();
            mChildren.erase(itr);
            return found;
        }
    }
    assert(false);
    return nullptr;
}

sf::Sprite& SceneNode::GetSprite() 
{
    return mSprite;
}

SceneNode* SceneNode::GetParent()
{
    return mParent;
}

const sf::Vector2f SceneNode::GetVelocity()
{
    return velocity;
}

const sf::Vector2f SceneNode::GetWorldPosition() const
{
    if (mParent == nullptr)
    {
        return getPosition();
    }
    return getPosition() + mParent->getPosition();
}

const sf::Vector2f SceneNode::GetLocalPosition() const
{
    return getPosition();
}

void SceneNode::SetWorldPosition(float x, float y)
{
    if (mParent != nullptr)
    {
        setPosition(sf::Vector2f(x, y)- mParent->GetWorldPosition());
    }
    else
    {
        setPosition(x, y);
    }
    moved = true;
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (moved || [&]() ->bool 
                 { 
                     if(mParent==nullptr) return false;
                     else return mParent->moved;
                 }())
    {
        moved = true;
        worldTransform = states.transform *= getTransform();
        target.draw(mSprite, states);
        drawChildren(target,states);
        moved = false;
    }
    else
    {
        states.transform = worldTransform;
        target.draw(mSprite, states);
        drawChildren(target, states);
    }
    
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (unsigned int i = 0; i < mChildren.size(); i++)
    {
        mChildren[i]->draw(target, states);
    }
}

void SceneNode::Update()
{
    velocity.x = 0; velocity.y = 0;
    for (unsigned int i = 0; i < components.size(); i++)
    {
        components[i]->Update();
    }
    UpdateChildren();
    if (mParent == nullptr)
    {
        CollisionSystem::getInstance()->CollisionDetection();
        DetachChildren();
    }
}

void SceneNode::UpdateChildren()
{
    for (unsigned int i = 0; i < mChildren.size(); i++)
    {
        mChildren[i]->Update();
    }
}

void SceneNode::MoveAfterCollision()
{
    if (velocity.x != 0 && velocity.y != 0)
    {
        SetVelocity(sqrt((velocity.x * velocity.x) / 2) * velocity.x / abs(velocity.x),
                    sqrt((velocity.y * velocity.y) / 2) * velocity.y / abs(velocity.y));
    }
    move(velocity);
    for (unsigned int i = 0; i < mChildren.size(); i++)
    {
        mChildren[i]->MoveAfterCollision();
    }
}

void SceneNode::WipeAllEnemies()
{
    for (int i = 0; i < mChildren.size(); i++)
    {
        if (mChildren[i].get()->collider != nullptr)
        {
            if (mChildren[i].get()->collider->thisTag == ColliderTag::Enemy)
            {
                SceneNode::GetDetachChildRequests().push(mChildren[i].get());
            }
        }
    }
}

void SceneNode::ClearChildren()
{
    mChildren.clear();
}
