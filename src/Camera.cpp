#include "stdafx.h"
#include "Camera.h"

namespace SFG
{

Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::update(float dt)
{
    if(!m_animated) return;	//Return if there is no panning happening

    if(	std::abs(m_targetx - view.getCenter().x) < 5000.f &&
            std::abs(m_targety - view.getCenter().y) < 5000.f)
    {
        m_animated = false;
        //printf("Shortcutting: %f|%f\n", m_targetx - view.getCenter().x, m_targety - view.getCenter().y);
        //printf("Shortcutting: %f|%f\n", view.getCenter().x, view.getCenter().y);
        view.setCenter(m_targetx, m_targety);
        return;
    }


    move(m_stepx * dt,
         m_stepy * dt);

    //printf("Left: %f|%f\n", m_targetx - view.getCenter().x, m_targety - view.getCenter().y);
}

void Camera::animatedPanTo(float x, float y)
{
    m_targetx = x;
    m_targety = y;

    m_stepx = (m_targetx - this->view.getCenter().x) / 100.f;
    m_stepy = (m_targety - this->view.getCenter().y) / 100.f;

    //printf("Dist is %f|%f\n", m_stepx, m_stepy);

    if(std::abs(m_stepx) < 5.f && std::abs(m_stepy) < 5.f)
    {
        view.setCenter(x, y);
        return;
    }

    m_animated = true;
}

}

