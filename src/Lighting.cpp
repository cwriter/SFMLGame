#include "stdafx.h"
#include "Lighting.h"



LightSource::LightSource()
{
}


LightSource::~LightSource()
{
}

int LightMapper::startMapping(float precision)
{
    //Lock thread
    std::lock_guard<std::mutex> guard(m_mutex);
    std::async(&LightMapper::Thread, this);


    return 0;
}
