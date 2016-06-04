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
private:
    sf::View view;
};

}
