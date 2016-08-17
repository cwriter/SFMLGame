#pragma once
#include "stdafx.h"

namespace SFG
{

class Camera
{
public:
    Camera();
    ~Camera();

    operator sf::View&() {
        return getView();
    }

    void move(float x, float y) {
        this->view.move(x, y);
    }

    void setCenter(float x, float y) {
        this->view.setCenter(x, y);
    }
    
    void setCenter(const sf::Vector2f& pos) {
        this->view.setCenter(pos.x, pos.y);
    }

    void setSize(float x, float y) {
        this->view.setSize(x, y < 0.f ? y : -y);
		
		auto m = std::min(std::abs(x), std::abs(y)) / scale;
		
		if(m < 10000.f && rdl != Medium) {
			rdl = Medium;
			rdl_changed = true;
		}
		else if(m < 100.f && rdl != Detailed) {
			rdl = Detailed;
			rdl_changed = true;
		}
		else if(rdl != Low){
			rdl = Low;
			rdl_changed = true;
		}
    }

    sf::View& getView() {
        return this->view;
    }
    
    void update(float dt);
    
    void animatedPanTo(float x, float y);
    
	enum requiredDetailLevel
	{
		Low,
		Medium,
		Detailed
	} rdl;
	
	float scale;
	bool rdl_changed;
	
private:
    sf::View view;
	
	
	
	bool m_animated;
	
	float m_targetx;
	float m_targety;
	
	float m_stepx;
	float m_stepy;
	
};

}
