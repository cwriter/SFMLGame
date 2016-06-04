#pragma once
#include "stdafx.h"
#include "Object.h"

class SOW_AI_Object : public Loadable
{
public:
    SOW_AI_Object() {

    }

    virtual ~SOW_AI_Object() {

    }

    void update() {

    }

protected:

private:
};


class SOW_AI_Engine
{
public:
    SOW_AI_Engine();
    virtual ~SOW_AI_Engine();

    int update(sf::Int64 max_time) {
        sf::Clock clock;

        for (size_t i = 0; i < m_ai_ele.size(); i++) {


            if (clock.getElapsedTime().asMicroseconds() >= max_time) {
                this->last_update_index = i;
                return 1;
            }
        }
        return 0;
    }
protected:

    size_t last_update_index;

    std::vector<SOW_AI_Object*> m_ai_ele;

private:

};

