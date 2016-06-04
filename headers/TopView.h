#pragma once
#include "GameState.h"
///<summary>
///A Scene2D with height ordering and intelligent height drawing
///</summary>
class TopView :
    public Scene2D
{
public:
    TopView();
    virtual ~TopView();

    virtual int draw(sf::RenderTarget& target, const sf::FloatRect& viewport) const override;
private:
};

