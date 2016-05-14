#pragma once
#include "stdafx.h"
#include "UI.h"
#include "Message.h"
#include "Object.h"
#include "XML.h"
#include "System.h"
#include "DLL.h"
#include "KeyMapper.h"
#include "GameSettings.h"

class UIManager;
class MsgObject;
class Loadable;
class GObjectBase;
class Animation;

class CMTAnimation;

///<summary>
///Module base class
///</summary>
class Module
{
public:
	virtual ~Module();

	virtual int load(const char* data) {
		m_datastring = std::string(data); //Only suitable for string-based data!
		return 0;
	}

	virtual int load(const XMLReader& xmldata) {
		//Mostly usable for stat-heavy objects
		return 0;
	}

	virtual int unload();

	SFG::Pointer<Loadable> request(const sf::String& source);

	SFG::Pointer<Loadable> requestByName(const sf::String& name);

	std::vector<SFG::Pointer<Loadable>>* getData() { return &m_data; }

	void addObject(const SFG::Pointer<Loadable>& obj) {
		this->m_data.push_back(obj);
	}

	inline void setLanguage(std::string& lang) {
		this->m_language = lang;
	}

	inline std::string getLanguage() { return this->m_language; }

	inline std::string& getType() { return this->m_type; }

protected:
	std::string m_language;

	std::string m_name;		//The name of the module
	std::string m_type;		//The type of the module

	std::string m_datastring;

	std::vector<SFG::Pointer<Loadable>> m_data;

};

///<summary>
///The module responsible for managing scenes
///</summary>
class ModuleG2D : public Module
{
public:
	ModuleG2D()
	{
		m_type = "moduleG2D";
	}
	virtual ~ModuleG2D();
	virtual int unload() override;
	int load(const XMLReader& xmldata) override;
protected:
	int load_animation(const sf::String& path, GObjectBase* ob, const XMLReader& xmldata);
private:

};

///<summary>
///The module responsible for storing, translating and drawing text
///</summary>
class ModuleG2DText : public Module
{
public:

	int load(const XMLReader& xmldata) override;
private:
};


///<summary>
///Loads data into any module
///</summary>
class ModuleLoader
{
public:

	virtual int load(const sf::String& xmlpath, Module& target, sf::String modname = sf::String());
	virtual int load(Module& target, sf::String modname = sf::String()) {
		return target.load(XMLReader(*reader.getXMLGroupHandle(modname)));
	}

	virtual int unload() {
		return 0;
	}

private:
	XMLReader reader;
};

///<summary>
///Basic 2D scene that manages and draws a scene (potentially in a bigger world)<br>
///The basic way is to copy-construct from a given vector pointer (as provided by some "World"-class).
///</summary>
class Scene2D : public MsgObject
{
public:
	enum Scene2DMessages
	{
		ObjectAbandoned = 0,
		RequireReEvaluation,
		Scene2dMessagesCount
	};

	Scene2D();

	Scene2D(const Scene2D& c);

	Scene2D(Scene2D&& c);

	Scene2D(std::vector<GObjectBase*>* vec);

	virtual ~Scene2D();

	//int notify(int msg, GObjectBase* sender) override;
	template<typename T> int notify(int msg, T* sender)
	{

		return 0;
	}

	///<summary>
	///Sets the vector to the specified type. When using this function,
	///the Scene2D-Object will just be a reference holder, i.e. it will not manage any memory.
	///</summary>
	///<param name="copy">
	///The vector to be passed to the Scene. This vector should not incorporate any concurrent writing access.
	///</param>
	void setObjects(std::vector<GObjectBase*>* copy);

	std::vector<GObjectBase*>* objects()
	{
		return this->m_gobjects;
	}

	///<summary>
	///Adds every object in vec for which cond returns true to this object's vector. <br/>
	///Using this functions makes this object managing an own vector; you can't add objects if <c>this</c> is just a reference holder.
	///</summary>
	///<param name="autodelete">
	///Determines whether the destructor should clean up the objects in the vector "this" holds.
	///</param>
	void selectObjects(std::vector<GObjectBase*>* vec, std::function<bool(const GObjectBase*)> cond, bool autodelete = false);

	///<summary>
	///Moves the rectangle that the scene holds. Moving the rect so that the new rect doesn't intersect the old rect is not recommended.
	///</summary>
	int move(const sf::Vector2f& offset);


	///<summary>
	///Stops listening to the object's changes
	///</summary>
	void releaseObjects();

	virtual int draw(sf::RenderTarget& target, const sf::FloatRect& viewport) const;

	void optimize();

	///<summary>
	///Adds an object to the scene
	///</summary>
	bool addObject(GObjectBase* obj, bool force_check = false);

	///<summary>
	///Removes an object from the scene
	///</summary>
	void removeObject(GObjectBase* obj);

	///<summary>
	///Removes every object for which cond returns true
	///</summary>
	void removeObjects(const std::function<bool(const GObjectBase*)>& cond);

	sf::FloatRect& rect() {
		return this->m_rect;
	}
protected:
	std::vector<GObjectBase*>* m_gobjects;
	sf::FloatRect m_rect;
	size_t* share_counter;
	bool autodelete;
};

///<summary>
///Handles the specific gamestate while also being the gamestate's module manager
///</summary>
class GameState : public SFG::KeyMapper
{
public:
	GameState();
	virtual ~GameState();

	///<summary>
	///Update your logic.
	///<param name="dt>
	///Time that has elapsed since the last update in milliseconds with microsecond resolution (expected values are between 1 and 100)
	///</param>
	///</summary>
	virtual int update(double dt) {
		return 0;
	}

	///<summary>
	///New list of inputs to process. The Inputlag of this operation is the render time
	///</summary>
	virtual int processEvents(SFG::Window& window, std::vector<sf::Event>& events) {

		return 0;
	}

	///<summary>
	///Draw the current scene
	///</summary>
	virtual void draw(sf::RenderTarget* t) {
		//Draw items
	}

	///<summary>
	///Loads the references to the modules. This does/should <i>not</i><br/>
	///load all data, but will load it on demand. Use <c>preload()</c> to load everything instantly.
	///</summary>
	virtual int load(const sf::String& path) {
		//Load the ui manager stuff

		return 0;
	}

	///<summary>
	///Action to be executed when the player requires a pause or wants to continue
	///</summary>
	///<returns>True if the game should switch to the default pause gamestate, false if you want to handle it by yourself (or ignore pauses)
	virtual bool pause(bool p) {
		//The code here are just recommendations.
		//For example, if you want to be able to load data during pause, you should not stop the system
		if (p)
		{
			this->m_system.halt_thread(); //Care: Can block (Estimated time < 60 ms)
		}

		if (!p)
		{
			this->m_system.continue_thread(); //Will resume the system task execution
		}
		return false;
	}

	///<summary>
	///Preloads the content needed for this specific gamestate
	///</summary>
	virtual int preload() {
		return 0;
	}

	///<summary>
	///Unloads all data from this gamestate while begin able to instantly reload (=keeping filepaths for example)
	///</summary>
	virtual int unload() {
		for (auto m : this->m_modules)
		{
			m->unload();
		}
		return 0;
	}

	///<summary>
	///Loads a list of assets
	///</summary>
	///<param name="reader">The reader providing the data</param>
	virtual int loadAssets(const XMLReader& reader);

	virtual int init(SFG::Window& win)
	{
		return -1;
	}


	inline SFG::UIManager* UI() { return &this->m_uiman; }

	std::string switch_to;	//When set, the Game will try to switch to the specified gamestate

	inline std::string& getName() { return name; }
	inline void setName(const std::string& name) { this->name = name; }

	void addModule(const SFG::Pointer<Module>& mod) {
		//Require: Not present yet
		assert(std::find(m_modules.begin(), m_modules.end(), mod) == m_modules.end());
		m_modules.push_back(mod);
	}

	///<summary>
	///Gets the vector of modules when in Debug Mode, nullptr otherwise.
	///</summary>
	std::vector<SFG::Pointer<Module>>* getModules()
	{
#ifdef _DEBUG
		return &m_modules;
#else
		return nullptr;
#endif
	}

	inline GameSettingsInterface& getGameSettingsInterface() {
		return m_gs;
	}

protected:
	GameSettingsInterface m_gs;

	SFG::UIManager m_uiman;
	std::string name;		//The name of the gamestate
	SFG::System m_system;	//The gamestate's system
	std::vector<SFG::Pointer<Module>> m_modules;
};

///<summary>
///Gamestate that is loaded from a DLL. Usage doesn't differ.
///</summary>
class DLLLoadedGameState : public GameState
{
public:
	DLLLoadedGameState()
	{
		data = nullptr;
		f_destructor = nullptr;
		f_draw = nullptr;
		f_load = nullptr;
		f_pause = nullptr;
		f_preload = nullptr;
		f_processEvents = nullptr;
		f_unload = nullptr;
		f_update = nullptr;
	}

	virtual ~DLLLoadedGameState()
	{
		//Call the loaded destructor
		if(f_destructor != nullptr)
			f_destructor(this->data);
	}
	typedef void* (*GameStateMemory)();
	typedef void (*GameStateDestructor)(void* memory);
	typedef int (*GameStateUpdateFunc)(void* memory, double dt);
	typedef void (*GameStateDrawFunc)(void* memory, sf::RenderTarget* t);
	typedef int (*GameStateLoadFunc)(void* memory, const sf::String& path);
	typedef int (*GameStateUnloadFunc)(void* memory);
	typedef int (*GameStateProcessFunc)(void* memory, std::vector<sf::Event>& events);
	typedef bool (*GameStatePauseFunc)(void* memory, bool p);
	typedef int (*GameStatePreloadFunc)(void* memory);

	///<summary>
	///Load the gamestate
	///</summary>
	///<param name="path">The path pointing to the file to be loaded</param>
	int loadFromDll(const char* path);

	///<summary>
	///Resolves all functions required by this gamestate from m_dll
	///</summary>
	int resolveAll();

	int update(double dt) override
	{
		//call the specific update function
		assert(f_update != nullptr);
		return f_update(data, dt);
	}

	void draw(sf::RenderTarget* t) override
	{
		assert(f_draw != nullptr);
		return f_draw(data, t);
	}

	int unload() override
	{
		assert(f_unload != nullptr);
		return f_unload(data);
	}

	bool pause(bool p) override
	{
		assert(f_pause != nullptr);
		return f_pause(data, p);
	}

	int preload() override
	{
		assert(f_preload != nullptr);
		return f_preload(data);
	}

	int processEvents(SFG::Window& window, std::vector<sf::Event>& events) override
	{
		assert(f_processEvents != nullptr);
		return f_processEvents(data, events);
	}

	int load(const sf::String& path) override
	{
		assert(f_load != nullptr);
		return f_load(data, path);
	}

private:

	void* data;

	GameStateDestructor f_destructor;
	GameStateUpdateFunc f_update;
	GameStateDrawFunc f_draw;
	GameStateLoadFunc f_load;
	GameStatePauseFunc f_pause;
	GameStateProcessFunc f_processEvents;
	GameStateUnloadFunc f_unload;
	GameStatePreloadFunc f_preload;


	SFG::DLL m_dll;
};

class StartupGameState : public GameState, public MsgObject
{
public:
	StartupGameState();

	virtual ~StartupGameState()
	{

	}

	virtual int notify(int msg) override
	{

		return 0;
	}

	virtual int dynamicNotify(int msg, const SFG::Pointer<MsgPackage>& pkg) override;

	virtual int load(const sf::String& path) override;

	virtual int update(double dt) override;

	virtual void draw(sf::RenderTarget* target) override;

	virtual int processEvents(SFG::Window& window, std::vector<sf::Event>& events) override;
private:
	int progess_event(int msg, const SFG::Pointer<MsgPackage>& pkg)
	{
		m_index++;
		return 0;
	}

	size_t m_elements_loaded;
	size_t m_elements_to_load;

	double m_passed_time;

	int m_index;

	SFG::Pointer<GObjectBase> m_e_logo;
	SFG::Pointer<GObjectBase> m_a_logo;
	SFG::Pointer<GObjectBase> m_g_logo;

	sf::View m_view;
};

class ClickableMenuText : public sf::Text
{
public:
	ClickableMenuText()
	{
		mouse_held = false;
		hover_time = 0.f;
		hoverAnim.reset(nullptr);
	}
	std::function<void(void)> on_click;

	bool mouse_held;
	float hover_time;

	SFG::Pointer<CMTAnimation> hoverAnim;
private:
};

class MenuAnimation
{
public:
	MenuAnimation()
	{
		m_done = false;
		m_time = 0.0f;
	}

	virtual ~MenuAnimation()
	{
	}

	virtual bool update(double dt) = 0;

	double m_time; //Time in ms

	bool m_done;
};

///<summary>
///ClickableMenuText Animation class
///</summary>
class CMTAnimation
	: public MenuAnimation
{
public:
	CMTAnimation()
	{
		m_target = nullptr;
	}
	bool update(double dt)
	{
		m_time += dt;
		m_func(m_target, m_time, &m_done);
		return m_done;
	}

	ClickableMenuText* m_target;

	std::function<void(ClickableMenuText*, double, bool*)> m_func;


private:

};

class ViewAnimation
	: public MenuAnimation
{
public:
	ViewAnimation()
	{

	}
	bool update(double dt)
	{
		//This animation is relative to time, not absolute
		m_func(m_target, dt, &m_done);
		return m_done;
	}


	sf::View* m_target;

	std::function<void(sf::View*, double, bool*)> m_func;
private:
};

///<summary>
///Provides the various menus for the game (Main Menu, Options, Loader)
///</summary>
class MenuGameState : public GameState
{
public:
	MenuGameState();

	virtual ~MenuGameState() {

	}

	virtual int init(SFG::Window& win) override;
	
	virtual int update(double dt) override;

	virtual int processEvents(SFG::Window& window, std::vector<sf::Event>& events) override;

	virtual void draw(sf::RenderTarget* t) override;

	virtual bool pause(bool p) override {
		return false;
	}

	virtual int load(const sf::String& path) override {
		//Load an XML-File and set the references
		return 0;
	}

private:

	void clickAnimation(ClickableMenuText* target, double time, bool* done) const;

	void hoverAnimation(ClickableMenuText* target, double time, bool* done) const;

	void startGameClicked(void)
	{
		this->switch_to = "main_gamestate";
	}

	void loadGameClicked(void)
	{
		//Go to load game screen
		//#TODO: NOT FINAL (Loading screen needed etc...)
		this->switch_to = "main_gamestate";
	}

	void optionsClicked(void);

	void exitClicked(void)
	{
		//Exit
		this->switch_to = "exit_gamestate";
	}

	void generalPan(sf::View* target, double time, bool* done, const SFG::Vector2f& to);

	void panToMain(sf::View* target, double time, bool* done)
	{
		generalPan(target, time, done, m_main_position);
	}

	void panToOptions(sf::View* target, double time, bool* done)
	{
		generalPan(target, time, done, m_options_position);
	}

	void backToMainClicked(void);

	void resolutionClicked(void)
	{
		//Switch through different resolutions
	}

	//Some main text stuff
	sf::Font m_font;
	std::vector<ClickableMenuText> m_texts;

	std::vector<SFG::Pointer<MenuAnimation>> m_animations;

	sf::View m_view;

	SFG::Vector2f m_options_position;
	SFG::Vector2f m_main_position;
};
