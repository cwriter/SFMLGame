#pragma once
#include "stdafx.h"

namespace SFG
{

class Camera
{
public:
    Camera();
    ~Camera();

    operator sf::View() const {
        return this->view;
    }

    void move(float x, float y) {
        this->view.move(x, y);
    }

    void setCenter(float x, float y) {
        this->view.setCenter(x, y);
    }

    void setSize(float x, float y) {
        this->view.setSize(x, y);
    }

    sf::View& getView() {
        return this->view;
    }
    
    void update(float dt);
    
    void animatedPanTo(float x, float y);
    
private:
    sf::View view;
	
	bool m_animated;
	
	float m_targetx;
	float m_targety;
	
	float m_stepx;
	float m_stepy;
	
};

}
