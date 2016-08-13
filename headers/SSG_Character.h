#include "stdafx.h"
#include "AI.h"

///<summary>
///The class to represent figurines
///</summary>
class SSG_Character
	:	public GObjectBase,
		public PE::PhysicObject,
		public SFG::AI
{
public:
	SSG_Character()
	{
	}
	
	~SSG_Character()
	{	
	}
	
	virtual void draw(sf::RenderTarget& target) override
	{
		GObjectBase::draw(target);
	}
	
	virtual int update(float dt) override
	{
		int gobjret = GObjectBase::update(dt);
		
		return 0;
	}
	
	virtual int load(const XMLReader& data) override
	{
		return 0;
	}
	
private:
	
	
};