#include "stdafx.h"
#include "TopView.h"



TopView::TopView()
{
}


TopView::~TopView()
{
}

int TopView::draw(sf::RenderTarget& target, const sf::FloatRect& viewport) const
{
    if (this->m_gobjects == NULL)
        return -1;
    size_t i = 0;
    for (; i < this->m_gobjects->size(); i++) {
        if (viewport.intersects((*m_gobjects)[i]->getBoundingRect())) {
            //draw it
            (*this->m_gobjects)[i]->draw(target);
        }
    }

    return int(i);
}
