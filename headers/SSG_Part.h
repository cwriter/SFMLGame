#pragma once
#include "Object.h"

///<summary>
///Represents a physically interactable, drawable part of a bigger object
///</summary>
class SSG_Part :
    public GObjectBase, public PE::PhysicObject
{
public:
    SSG_Part();
    virtual ~SSG_Part();

    int notify(int msg) override
    {
        return 0;
    }

    virtual mpf_class x() const override
    {
        return double(GObjectBase::getSprite().getPosition().x);
    }

    virtual mpf_class y() const override
    {
        return double(GObjectBase::getSprite().getPosition().y);
    }
private:



};

