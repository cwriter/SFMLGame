#pragma once
#include "stdafx.h"
#include "Object.h"
#include "SOW_Road.h"
#include "Context.h"

class SOW_Floor : public GObjectBase
{
public:
	SOW_Floor() {

	}

	~SOW_Floor() {

	}

	int load(XMLReader& data) override
	{
		bool b = false;
		this->level = data.asInt(L"level.", b);
		if (!b)
		{
			printf("[Error] Failed to get floor level in %s:%d\n", __FILE__, __LINE__);
		}
		//#TODO: Get different information

		//Load objects
		data.for_all("object", std::bind(&SOW_Floor::object_loader, this, std::placeholders::_1));



		return 0;
	}

	void unload() override {
		//Write contents to file
	}

private:

	///<summary>
	///Internal loader for Floors
	///</summary>
	void object_loader(const XMLGroup* group)
	{
		SFG::Pointer<GObjectBase> obj(new GObjectBase(this->m_governor));
		assert(obj.isValid());

        XMLReader reader(*group);
		auto ret = obj->load(reader);
		if (ret != 0)
		{
			printf("[Error] Failed to load object in %s:%d\n", __FILE__, __LINE__);
			return;
		}

		m_objects.push_back(obj);
	}

	std::string m_path;
	std::vector<SFG::Pointer<GObjectBase>> m_objects;

	int level;
};


class SOW_Building : public GObjectBase
{
public:
	SOW_Building() {

	}

	~SOW_Building() {

	}

	int load(XMLReader& data) override
	{
		auto name = data.getValue(L"name.");
		assert(name != L"__xml_failure");
		this->m_name = name;

		bool b = false;

		//Get the building rect
		this->m_building_rect = data.asFloatRect(L"rect.", b);
		if (!b)
		{
			printf("[Error] Failed to get building rect for building named %s in %s:%d\n", this->m_name.toAnsiString().c_str(), __FILE__, __LINE__);
		}

		//Get the rotation of the rect
		this->m_building_rotation = data.asDouble(L"rotation.", b);
		if (!b)
		{
			printf("[Error] Failed to get building rotation for building named %s in %s:%d\n", this->m_name.toAnsiString().c_str(), __FILE__, __LINE__);
		}

		//#TODO: Load more information


		//Load Floors now
		data.for_all("Floor", std::bind(&SOW_Building::floor_loader, this, std::placeholders::_1));


		return 0;
	}

private:

	///<summary>
	///Internal loader for Floors
	///</summary>
	void floor_loader(const XMLGroup* group)
	{
		SFG::Pointer<SOW_Floor> floor(new SOW_Floor());

		XMLReader reader(*group);

		auto ret = floor->load(reader);
		if (ret != 0)
		{
			printf("[Error] Failed to load floor in %s:%d\n", __FILE__, __LINE__);
			return;
		}

		this->m_floors.push_back(floor);
	}


	sf::FloatRect m_building_rect;
	double m_building_rotation;
	std::vector<SFG::Pointer<SOW_Floor>> m_floors;

	SOW_Road* m_road;
	int m_number;
};

class SOW_World : public MsgObject
{
public:
	enum {
		AllocateFirstDeleteLast = 1, //Requires more RAM as this will first get RAM for new objects and then start to unload unused ones
		DeleteFirstAllocateLast = 2, //Might lead to less performance as this makes place first and then allocates new memory for new objects
		Balanced = 3,				 //Does both, meaning that there is no priority setting, both are handled in 1 loop, needs less CPU
		NoDeleteBit = 4,			 //If set, deletion is disabled
		NoDelete = 5				 //Allocates memory, but does not delete it	(includes AllocateFirstDeleteLast and NoDeleteBit)
	};
	SOW_World();
	~SOW_World();

	///<summary>
	///Optimizes multiple areas.
	///</summary>
	int optimize_rects(sf::Int64 max_time, std::vector<sf::FloatRect*>& rects, int flags);

	template <typename T, typename S> int notify(int msg, T* sender, S* object);


	int dynamicNotify(int msg, const SFG::Pointer<MsgPackage>& pkg) override;
	//template <> int notify(int msg, Scene2D* sender);
	//template <> int notify(int msg, GObjectBase* sender);

	///<summary>
	///Manages the world by allocating memory we need and freeing memory we don't need anymore
	///</summary>
	///<param name="max_time">
	///The maximal time in microseconds this function can take. <br/>
	///Note that there is no guarantee that this function doesn't take longer than specified.
	///</param>
	int optimize(sf::Int64 max_time, sf::FloatRect& rect, int flags);

	///<summary>
	///Adds a new scene to m_active_scenes
	///</summary>
	int addScene(const sf::FloatRect& rect);

	///<summary>
	///Draw elements in srect to trect in target. Note that target can be a texture; but it has to exist already. This function calls addScene()
	///</summary>
	int draw(sf::RenderTarget& target, const sf::FloatRect& trect, const sf::FloatRect& srect);

	///<summary>
	///Draws All scenes that intersect with srect to target
	///</summary>
	int draw(sf::RenderTarget& target, const sf::FloatRect& srect);

	///<summary>
	///Gets a list of all possible actions in this context (at position pos), within a max_dist radius
	///</summary>
	SFG::Pointer<ContextOptionList> getContextOptions(const sf::Vector3f& pos, const float max_dist);

	///<summary>
	///Loads the world.
	///</summary>
	int load(const sf::String& path);

	template<typename T> int notify(int msg, T* sender)
	{
		printf("[Error] int notify was overriden, but does not provide solutions for this type of sender in %s:%d\n", __FILE__, __LINE__);
		return -1;
	}


private:

	///<summary>
	///Internal loader for Buildings
	///</summary>
	void building_loader(const XMLGroup* group);

	size_t last_optimize_index;

	int optimize_sub_loop(int flags, sf::Clock& clock, sf::FloatRect& rect, sf::Int64 max_time);

	std::vector<GObjectBase*> m_objects;

	std::vector<SFG::Pointer<SOW_Building>> m_buildings; //#TODO: Not sure if it's better to make this specific group for buildings or not (they might be integrated in "m_objects" as well)

	std::vector<SFG::Pointer<Scene2D>> m_active_scenes; //Contains all active scenes

	bool m_restricted_to_active_scenes;					//Determines whether the world's search actions should limit to scenes or not. True by default
};

