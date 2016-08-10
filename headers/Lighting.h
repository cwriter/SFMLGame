#pragma once
#include "stdafx.h"
#include "PE_Vector.h"
#include "Polygon.h"

///<summary>
///The class provides functionality for both lighting and getting lit.
///</summary>
class LightingObject
{
public:
	
	enum RayType
	{
		LightOnly = 0,
		LightAndTemperature
	} type;
	
	
	LightingObject()
	{
		intensity = 1000.f;
		type = RayType::LightAndTemperature;
	}
	
	float intensity;
	
	SFG::Pointer<SFG::Polygon> getIlluminatedBy(const LightingObject& obj)
	{
		SFG::Pointer<SFG::Polygon> ret(new SFG::Polygon());
		PE::Vector2df vec;
		vec.x = obj.lightingY() - this->lightingY();
		vec.y = -(obj.lightingX() - this->lightingX());
		
		auto thisvec = PE::Vector2df({lightingX(), lightingY()}) + vec.normalized() * this->lightingRadius();
		auto objvec = PE::Vector2df({obj.lightingX(), obj.lightingY()}) + vec.normalized() * obj.lightingRadius();
		
		
		
		return ret;
	}
	
	virtual mpf_class lightingX() const;
	
	virtual mpf_class lightingY() const;
	
	virtual mpf_class lightingRadius() const;
	
private:
	
};