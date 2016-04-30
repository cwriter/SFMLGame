#pragma once
#include "stdafx.h"
#include "Object.h"

class LightSource;
class LightObstacle;
class LightMapper;

///<summary>
///Base class of objects that impedes light by a given factor and/or changes the output angle by some degree
///</summary>
class LightObstacle : public MsgObject
{
public:

	template <typename T> int notify(int msg, T* sender) {
		printf("[Warning] LightObstacle::notify(int, T*) called, but this function hasn't been overridden by the child class: %s:%d\n", __FILE__, __LINE__);
		return -1;
	}




private:
	double m_impeding_factor; //impeding factor where negative values mean that the light gets more intense and 0 that it is opaque
	double m_in_angle_change; //in deg: Angle change
	double m_out_angle_change;
	double m_reflect;		//[0;1.00]: How much light gets reflected (in the impact angle)
	float m_reflection_diffusity; //How much of the reflected light is diffuse?
};

///<summary>
///Provides a lightmap buffer to blend with the final image. Shall be optimized for multithreading
///</summary>
class LightMapper : public MsgObject
{
public:
	LightMapper(sf::FloatRect& rect, std::vector<SFG::Pointer<LightSource>>* lightSources, std::vector<SFG::Pointer<LightObstacle>>* lightObstacles)
		: m_rect(rect), m_lightSources(lightSources), m_lightObstacles(lightObstacles)
	{

	}

	template <typename T> int notify(int msg, T* sender) {
		printf("[Warning] MsgObject::notify(int, T*) called, but this function hasn't been overridden by the child class: %s:%d\n", __FILE__, __LINE__);
		return -1;
	}

    int dynamicNotify(int msg, const SFG::Pointer<MsgPackage>& pkg) override
    {
        if (msg == GObjectBase::Destroyed)
		{
            LightSource* sender = (LightSource*)pkg->getValue(0);    //This message type requires the first argument to be the sender
			m_lightSources->erase(std::find(m_lightSources->begin(), m_lightSources->end(), sender));
		}
		else if (msg == GObjectBase::PositionChanged)
		{
			//Update the position of this object in the lightmap
		}

		return 0;
    }


	int Thread()
	{
		//Create an appropriate context
		m_mutex.lock();
		sf::Context ctx;

		//Create a texture that's appropriate for our usage
		sf::RenderTexture texture;
		texture.create(int(this->m_rect.width), int(this->m_rect.height));
		texture.clear(sf::Color(0,0,0)); //Fully black (representing the darkness)

		//Now, blend the lights into the texture


		m_mutex.unlock();
		return 0;
	}

	int startMapping(float precision);
private:
	std::mutex m_mutex;

	SFG::FloatRect m_rect;
	std::vector<SFG::Pointer<LightSource>>* m_lightSources;
	std::vector<SFG::Pointer<LightObstacle>>* m_lightObstacles;
};

///<summary>
///Base Class for lights. Contains information about the light intensitiy, color and position
///</summary>
class LightSource : public MsgObject
{
public:

	enum LightSourceMessages
	{
		//"Cheap" setters
		MsetIntensity = GObjectBase::GObjectMessagesCount + 1,
		MsetColor,
		MsetOffset,


		LightSourceMessagesCount
	};
	LightSource();
	virtual ~LightSource();

	int notify(int msg) override
	{
		if (msg == GObjectBase::Destroyed)
		{
			//We lost our generating object, so we'll be destroyed too.
			this->notifyAll(msg, this); //Forward to listener (which should be the lightmapper)
		}

		return 0;
	}

	template <typename T> int notify(int msg, T* sender) {
		printf("[Warning] MsgObject::notify(int, T*) called, but this function hasn't been overridden by the child class: %s:%d\n", __FILE__, __LINE__);
		return -1;
	}

    int dynamicNotify(int msg, const SFG::Pointer<MsgPackage>& pkg) override
    {
        if(msg == GObjectBase::GObjectMessages::PositionChanged)
        {
            GObjectBase* sender = (GObjectBase*)pkg->getValue(0); //This format requires parameter 0 to be the sender
            //Adjust position
			this->m_position = (const SFG::Vector2<float>&)sender->getSprite().getPosition() + (SFG::Vector2<float>)m_offset.rotated(sender->getSprite().getRotation());
			//Forward to Lightmapper
			this->notifyAll(GObjectBase::PositionChanged, this);
        }
        return 0;
    }

	//Setters
	inline void setOffset(const sf::Vector2f& offset) { this->m_offset = offset; }
	inline void setColor(const sf::Color& color) { this->m_color = color; }
	inline void setIntensity(float intensity) { this->m_intensity = intensity; }

	inline float getIntensity() { return this->m_intensity; }
	inline sf::Color& getColor() { return this->m_color; }
	inline SFG::Vector2f& getOffset() { return this->m_offset; }

	inline void setAngle(float a) { m_angle = a; }
	inline void setRotation(float r) { m_rotation = r; }
	inline void setGradient(float g) { m_gradient = g; }

	inline float getAngle() { return this->m_angle; }
	inline float getRotation() { return this->m_rotation; }
	inline float getGradient() { return this->m_gradient; }
	inline SFG::Vector2f getPosition() { return this->m_position; }
private:
	//GObjectBase* m_base;	//The object that has this lighting

	SFG::Vector2f m_offset; //The offset from the (unrotated) base object
	SFG::Vector2f m_position; //The absolute position
	sf::Color m_color;

	//Calculation: intensity is distance, angle angle (obviously)
	float m_angle;
	float m_rotation;
	float m_gradient;
	float m_intensity;
};

