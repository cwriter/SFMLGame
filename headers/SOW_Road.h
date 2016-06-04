#pragma once
#include "stdafx.h"
#include "Object.h"
#include "Physics.h"

class SOW_RoadSegment : public GObjectBase
{
public:
    SOW_RoadSegment();
    virtual ~SOW_RoadSegment();

    void setVectors(PE::Vector2df& pos, PE::Vector2df vec) {

        this->m_r_position = pos;
        this->m_r_vector = vec;
        //REQUIRES REPEATED TEXTURES
        assert(this->getSprite().getTexture()->isRepeated());
        this->setScale(float(m_r_vector.absLength()), 1.f); //Stretch the texture
        //Now rotate the sprite accordingly
        float desired_rotation = (float)atan(m_r_vector.y / m_r_vector.x);
        if (m_r_vector.x < 0.) {
            desired_rotation += float(PI);
        }
        this->setRotation(desired_rotation * 180.f / float(PI));
    }

    void draw(sf::RenderTarget& target) override {
        target.draw(this->getSprite()); //Draw segment
    }
protected:
    PE::Vector2df m_r_position; //The vector from (0,0) to the point where the vector originates
    PE::Vector2df m_r_vector; //The vector with length and direction
};

class SOW_Road : public Loadable
{
public:
    SOW_Road();
    virtual ~SOW_Road();

    int load(const char* data) override {
        return 0;
    }

protected:
    std::vector<SOW_RoadSegment*> m_segments;
};

