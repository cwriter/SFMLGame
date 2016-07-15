#pragma once
#include "stdafx.h"
#include "Object.h"
#include <list>
#include "Physics.h"


enum SSG_Material
{
	Steel,
	Plastic,
	Ceramic
	
};


///<summary>
///Represents a physically interactable, drawable part of a bigger object
///</summary>
class SSG_Part :
    public GObjectBase, public PE::PhysicObject
{
public:
	enum SSG_PartMessages
	{
		//Start at GObjectMessagesCount
		
	};
	
	enum part_IntType
	{
		Polygon,
		SimpleRect
	};
	
	enum part_StrucType
	{
		Hull
		
	};
	
    SSG_Part();
    virtual ~SSG_Part();

    int notify(int msg) override
    {
        return 0;
    }

    virtual int load(XMLReader& data) override
    {
		//First, clear any previous data
		m_points.clear();
		
		//Get the type
		auto type = data.getValue("type.");
		if(type == "hull")
			this->m_StrucType = Hull;
		
		
		auto param = data.getValue("param.");
		
		auto name = data.getValue("name.");
		//#TODO: Check if we have to prefix this one
		this->m_name = name;
		
		if(param == "straight")
		{
			//If we have just a straight bar, we can simplify from polygon to a line with thickness, rotation and length (~= a rectangle)
			m_InternalType = part_IntType::SimpleRect;
			auto thickness = data.getValue("thickness.");
			auto rotation = data.getValue("rotation.");
			auto length = data.getValue("length.");
		}
		else if(param.find("polygon:") == 0) //i.e. if it starts with "polygon"
		{
			m_InternalType = part_IntType::Polygon;
			const size_t start_of_data = strlen("polygon:");
			size_t lastpos = start_of_data;
			do
			{
				//Get the x coordinate first
				size_t nextpos = param.find(";", lastpos);
				auto tmp = param.substring(lastpos, nextpos - lastpos);
				auto x = std::stod(tmp.toAnsiString());
				 
				lastpos = nextpos;
				
				//Get the y coordinate next
				nextpos = param.find(";", lastpos);
				tmp = param.substring(lastpos, nextpos - lastpos);
				auto y = std::stod(tmp.toAnsiString());
				 
				lastpos = nextpos;
				
				
				//We now have our 2d vertex coordinates, now add them to the polygon
				m_points.push_back(SFG::Vector2f(x, y));
				
				 
			} while (lastpos != sf::String::InvalidPos);
		}
		else
		{
			//Print an error
			SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__, "Failed to get information format");
		}
		
		auto material = data.getValue("material.");
		//#TODO: Find a way to apply texture and physical resistance (et al) values from this string.
		
		
		return 0;
	}
    
    
    virtual mpf_class x() const override
    {
        return ((PE::PhysicObject*)this)->x();
    }

    virtual mpf_class y() const override
    {
        return ((PE::PhysicObject*)this)->y();
    }
    
    
private:
	
	int m_InternalType;
	int m_StrucType;

	int m_material;		//This holds the type of material the part consists of (if it is a simple type)
	
	std::list<SFG::Vector2f> m_points;	//The polygon points of the element (should not be many;
										//the count shall not exceed 100)
	
	

};

