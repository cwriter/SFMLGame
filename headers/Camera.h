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
		
		const float t_med = 10000.f;
		const float t_det = 100.f;
		
		//printf("Viewsize is %f|%f\n", x, y);
		if(m < t_det && rdl != Detailed) {
			rdl = Detailed;
			printf("Setting view to DETAILED\n");
			rdl_changed = true;
		}
		else if(m < t_med && m > t_det && rdl != Medium) {
			rdl = Medium;
			printf("Setting view to MEDIUM\n");
			rdl_changed = true;
		}
		else if(m >= t_med && rdl != Low){
			rdl = Low;
			printf("Setting view to LOW\n");
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
