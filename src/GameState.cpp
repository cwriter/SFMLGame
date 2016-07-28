#include "stdafx.h"
#include "GameState.h"


GameState::GameState()
{
}

GameState::~GameState()
{
    for (auto m : m_modules) {
        m.release();
    }
}

int DLLLoadedGameState::loadFromDll(const char* path)
{
    int ret = m_dll.load(path) ? 0 : -1;

    if(ret != -1)
        ret = resolveAll();

    return ret;
}

int DLLLoadedGameState::resolveAll()
{
    int ret = this->m_dll.isOpen() ? 0 : -1;
    if (ret == 0)
    {
        //Get the data pointer
        SFG::SFGFunc func = m_dll.resolve("data");
        if (func != NULL)
        {
            //Set the correct data (also working as constructor)
            GameStateMemory mem = (GameStateMemory)func;
            this->data = mem();
        }
        else
        {
            printf("[Critical] Failed to load memory getter function from GameState DLL in %s:%d\n", __FILE__, __LINE__);
            return -2;
        }
        f_destructor = (GameStateDestructor)m_dll.resolve("destructor");
        f_update = (GameStateUpdateFunc)m_dll.resolve("update");
        f_draw = (GameStateDrawFunc)m_dll.resolve("draw");
        f_load = (GameStateLoadFunc)m_dll.resolve("load");
        f_pause = (GameStatePauseFunc)m_dll.resolve("pause");
        f_processEvents = (GameStateProcessFunc)m_dll.resolve("processEvent");
        f_unload = (GameStateUnloadFunc)m_dll.resolve("unload");
        f_preload = (GameStatePreloadFunc)m_dll.resolve("preload");

        //Check if any func didn't get loaded correctly
        int issue = 0;
        if (f_destructor == nullptr) return -1;
        if (f_update == nullptr) return -2;
        if (f_draw == nullptr) return -3;
        if (f_load == nullptr) return -4;
        if (f_pause == nullptr) return -5;
        if (f_processEvents == nullptr) return -6;
        if (f_unload == nullptr) return -7;
        if (f_preload == nullptr) return -8;

    }
    return 0;
}

int Scene2D::draw(sf::RenderTarget& target, const sf::FloatRect& viewport) const
{
    //Target should be created
    assert(target.getSize().x > 0 && target.getSize().y > 0);

    //Set the correct view (draw offset)
    target.setView(sf::View(viewport));
    int count = 0;
    for (auto* o : *m_gobjects)
    {
        if (viewport.intersects(o->getBoundingRect())) {
            //Need to draw
            o->draw(target);
            count++;
        }
    }
    return count;
}

Scene2D::~Scene2D()
{
    //Release all objects!
    this->releaseObjects();
    //If it is standalone, it has a sharecounter
    if (share_counter != NULL) {
        (*share_counter)--;
        if (*share_counter == 0) {
            if (m_gobjects != NULL) {
                //Last holder, delete memory
                if (autodelete)
                {
                    for (auto g : *m_gobjects) {
                        delete g;	//Delete elements
                    }
                }
                delete m_gobjects; //Delete (and clear) vector
            }
        }
        delete share_counter; //Deleting the share counter
    }
}

void Scene2D::removeObject(GObjectBase* obj)
{
    assert(this->m_gobjects != nullptr);

    assert(obj != NULL);

    //Stop listening
    obj->removeListener(this);
    this->notifyAll(Scene2D::Scene2DMessages::ObjectAbandoned, this, obj); //Send to all that abandoned this object

    auto it = std::find(this->m_gobjects->begin(),
                        this->m_gobjects->end(),
                        obj);
    this->m_gobjects->erase(it);
}

bool Scene2D::addObject(GObjectBase* obj, bool force_check)
{
    assert(this->m_gobjects != nullptr);
    if (force_check)
    {
        //Check if already available (only debug as this would be really slow
        for (auto c : *this->m_gobjects)
        {
            if (c == obj)
            {
                printf("[Warning] Object already added %s:%d\n", __FILE__, __LINE__);
                return false;
            }
        }
    }
    obj->addListener(this);
    this->m_gobjects->push_back(obj);
    return true;
}

void Scene2D::releaseObjects()
{
    for (auto a : *m_gobjects)
    {
        //Stop listening
        a->removeListener(this);
    }
}

void Scene2D::removeObjects(const std::function<bool(const GObjectBase*)>& func)
{
    for (auto a : *m_gobjects)
    {
        if (func(a))
        {
            //Remove
            removeObject(a);
        }
    }
}

void Scene2D::selectObjects(std::vector<GObjectBase*>* vec, std::function<bool(const GObjectBase*)> cond, bool autodelete)
{
    this->autodelete = autodelete;
    //NOTE: THIS _ADDS_ OBJECTS TO THE VECTOR
    if (this->share_counter == nullptr) {
        //Allocate a new vector
        //this->m_gobjects can't be managed by this object atm (no leaks possible)
        this->m_gobjects = new std::vector<GObjectBase*>();
        if (this->m_gobjects == nullptr) {
            //Critical error
            printf("[Error] Memory allocation failed in %s:%d\n", __FILE__, __LINE__);
            return;
        }
        this->share_counter = new size_t(1);
        if (this->share_counter == nullptr) {
            //Critical error
            printf("[Error] Memory allocation failed in %s:%d\n", __FILE__, __LINE__);
            return;
        }
    }
    for (auto g : *vec)
    {
        if (cond(g))
        {
            //Catch position changes
            g->addListener(this);
            this->m_gobjects->push_back(g); //Add elements for which cond is true
        }
    }
}

template <> int Scene2D::notify(int msg, GObjectBase* sender)
{
    if (msg == GObjectBase::GObjectMessages::PositionChanged)
    {
        //Position changed, so check if it is still in this scene
        if (m_rect.intersects(sender->getBoundingRect()))
        {
            //do nothing
        }
        else
        {
            //Remove object from this scene
            this->removeObject(sender);
            this->notifyAll(Scene2DMessages::ObjectAbandoned);
        }
    }
    return 0;
}

void Scene2D::setObjects(std::vector<GObjectBase*>* copy)
{
    //As this is just a reference, we are not allowed to delete it afterwards
    if (this->share_counter != NULL)
    {
        if (this->m_gobjects != NULL)
            delete m_gobjects; //Leak prevention
        delete share_counter; //No leaks please
        share_counter = NULL;
        autodelete = false;
    }
    this->m_gobjects = copy;
}

int Scene2D::move(const sf::Vector2f& offset)
{
    //For debugging:
    assert(abs(offset.x) <= this->rect().width);
    assert(abs(offset.y) <= this->rect().height);

    sf::FloatRect nrect = this->rect();
    nrect.left += offset.x;
    nrect.top += offset.y;

    //For runtime:
    if (std::abs(offset.x) > this->rect().width || std::abs(offset.y) > this->rect().height)
    {
        //Bascially, just create from a new data set
        //Release all objects first (no need to listen to them anymore)
        this->releaseObjects();
        this->notifyAll(Scene2DMessages::RequireReEvaluation, this); //Let our parent do the job
        return 1; //Done, but not in the way usually intended
    }
    //Normal case: We just translate the scene a bit. 2 Tasks to fullfill then:
    //1) Remove entities that are now out of bounds
    //2) Add entities that are now in bounds

    //Set the new rect
    this->m_rect = nrect;

    //Remove objects that aren't in bounds anymore
    this->removeObjects([=](const GObjectBase* obj) {
        return (!obj->getBoundingRect().intersects(nrect)); //Doesn't intersect -> remove; intersects -> maintain
    });


    return 0;
}

void Scene2D::optimize() {
    //Sort by depth
    std::sort(m_gobjects->begin(),
              m_gobjects->end(),
    [=](GObjectBase* l, GObjectBase* r) {
        return (l->getDepth() < r->getDepth());
    });
}

int ModuleLoader::load(const sf::String& xmlpath, Module& target, sf::String modname)
{
    sf::String data;
    if (basicLoadFile(xmlpath, data) != 0) {
        //Error
        printf("Failed to load file %s", xmlpath.toAnsiString().c_str());
        return -1;
    }

    reader.setSource(data);
    reader.parse();

    int ret = 0;

    if ((ret = this->load(target, modname)) != 0)
    {
        printf("[Error] Module::load returned unexpected %d in %s:%d\n", ret, __FILE__, __LINE__);
    }

    /*int test = reader.asInt(L"xml/module[G2D]/object#0/animation/frame#1/duration/");
    printf("Fetching values: MP: %d\n", test);*/

    return 0;
}

void Module::listObjects() const
{
	for(auto o : m_data)
	{
		puts(o->m_name.toAnsiString().c_str());
	}
}

ModuleG2D::~ModuleG2D()
{
    unload();
}

int ModuleG2D::load(const XMLReader& xmldata)
{

    //Load from xml
    sf::String path;
	
	xmldata.for_all("object", [=](const XMLGroup* g){
		
		XMLReader r(*g);
		
		 //Object with the name
        SFG::Pointer<GObjectBase> ob(new GObjectBase(this));
		
		ob.cast<Loadable>()->Loadable::load(r);
		
		XMLReader r2(*r.getXMLGroupHandle("GObjectBase/"));
		
		ob->load(r2);
		
		this->m_data.push_back(ob);
		
	});

    return 0;
}

int GameState::loadAssets(const XMLReader& reader)
{
    auto list = reader.getXMLGroupHandle(L"");
    if (list == nullptr) return -1;
    for (auto a : list->lowerElements)
    {

        if (a.name == L"module")
        {
            //get the name
            sf::String name;
            for (auto attr : a.attributes)
            {
                if (attr.str1 == L"name")
                {
                    name = attr.str2;
                    break;
                }
            }
            //Get the corresponding module
            for (auto m : m_modules)
            {
                if (m->getType() == name)
                {
                    //Load into this module
                    m->load(XMLReader(a));
                }
            }
        }
    }
    return 0;
}

Scene2D::Scene2D()
{
    share_counter = new size_t(1);
    if (share_counter == NULL) {
        //Allocation error
        perror("ALLOCATION ERROR");
        return;
    }
    this->m_gobjects = new std::vector<GObjectBase*>;
    if (this->m_gobjects == NULL) {
        perror("ALLOCATION ERROR");
        return;
    }
};

Scene2D::Scene2D(const Scene2D& c)
{
    this->share_counter = c.share_counter;
    if (share_counter != NULL)
        (*share_counter)++;	//Increase counter if the other scene was not just a reference
    this->m_gobjects = c.m_gobjects;
}

Scene2D::Scene2D(Scene2D&& c)
{
    //Move
    this->share_counter = c.share_counter;
    c.share_counter = NULL;
    this->m_gobjects = c.m_gobjects;
    c.m_gobjects = NULL;
}

Scene2D::Scene2D(std::vector<GObjectBase*>* vec)
{
    //We don't share (the object is still handled by the world after all)
    this->share_counter = NULL;
    //Assign
    this->m_gobjects = vec;
}

int ModuleG2D::unload()
{
    for (auto l : m_data) {
        //l.release();
    }
    m_data.clear();
    return 0;
}

int ModuleG2DText::load(const XMLReader& xmldata)
{

    return 0;
}

Module::~Module()
{
    unload();
}

int StartupGameState::init(SFG::Window& win)
{
	UI()->setTarget(&win.getSFMLWindow());
	return 0;
}


int StartupGameState::load(const sf::String& path)
{
    SFG_UNUSED_STR(path, __FILE__, __LINE__);
    for (auto m : m_modules)
    {
        if (m->getType() == "moduleG2D")
        {
            //Get the objects named "Engine_Logo", "Additional_Logos", "Game_Logo"
            m_e_logo = m->requestByName(L"Engine_Logo");
            if (!m_e_logo.isValid())
            {
                printf("Name not resolved. Elements in the vector:\n");
                //Failed to load
                for (auto d : *m->getData())
                {
                    printf("%s\n", d->m_name.toAnsiString().c_str());
                }
            }
            m_a_logo = m->requestByName(L"Additional_Logos");
            m_g_logo = m->requestByName(L"Game_Logo");
            break;
        }
    }

    auto progress_func = std::bind(
                             &StartupGameState::progess_event,
                             this,
                             std::placeholders::_1,
                             std::placeholders::_2);

    m_e_logo->addListener(this, MsgObjectAction(progress_func));
    m_a_logo->addListener(this, MsgObjectAction(progress_func));
    m_g_logo->addListener(this, MsgObjectAction(progress_func));

    return 0;
}

int StartupGameState::processEvents(SFG::Window& window, std::vector<sf::Event>& events)
{
    for (size_t i = 0; i < events.size(); i++)
    {
        if (events[i].type == sf::Event::KeyReleased)
        {
            //Any key-up event will cancel the intro if possible (by default)
            //#TODO: Remove comment in the line below
            //if (m_elements_loaded == m_elements_to_load)
            {
                //Break this gamestate; switch to menu
                this->switch_to = "menugamestate";
                break;
            }
        }
    }
    return 0;
}

StartupGameState::StartupGameState()
{
    m_elements_loaded = 0;
    m_elements_to_load = 0;
    name = "startupgamestate";
    m_passed_time = 0.;

    m_index = 0;
}

int StartupGameState::update(double dt)
{
    //Increase the overall passed time counter
    m_passed_time += dt;

    //#TODO: Replace the code with the comments above them
    switch (m_index)
    {
    case 0:
        m_e_logo->update(float(dt));
        break;
    case 1:
        m_a_logo->update(float(dt));
        //m_e_logo->update(float(dt));
        break;
    case 2:
        m_g_logo->update(float(dt));
        //m_e_logo->update(float(dt));
        break;
    case 3:
        //Break out, hand off
        this->switch_to = "menugamestate";
        this->unload();
        break;
    default:
        printf("[Error] Index in the default Startup Gamestate was out of expected bounds in %s:%d\n", __FILE__, __LINE__);
        break;
    }
    return 0;
}

void StartupGameState::draw(sf::RenderTarget* target)
{
    if (target == nullptr)
    {
        printf("[Error] Trying to draw on nullptr in %s:%d. Aborting.\n", __FILE__, __LINE__);
        return;
    }
    //m_screen->draw(*target);

    switch (m_index)
    {
    case 0:
        m_view.setCenter(m_e_logo->getBoundingRect().center());
        m_view.setSize(m_e_logo->getBoundingRect().width, m_e_logo->getBoundingRect().height);
        m_e_logo->draw(*target);
        break;
    case 1:
        m_view.setCenter(m_a_logo->getBoundingRect().center());
        m_view.setSize(m_a_logo->getBoundingRect().width, m_a_logo->getBoundingRect().height);
        m_a_logo->draw(*target);
        //m_e_logo->draw(*target);
        break;
    case 2:
        m_view.setCenter(m_g_logo->getBoundingRect().center());
        m_view.setSize(m_g_logo->getBoundingRect().width, m_g_logo->getBoundingRect().height);
        m_g_logo->draw(*target);
        //m_e_logo->draw(*target);
        break;
    case 3:
        //Still trying to switch
        break;
    default:
        printf("[Error] Index in the default Startup Gamestate was out of expected bounds in %s:%d\n", __FILE__, __LINE__);
        break;
    }
    target->setView(m_view);
}

int StartupGameState::dynamicNotify(int msg, const SFG::Pointer<MsgPackage>& pkg)
{
    if (msg == Animation::AnimationMessages::AnimationFinished)
    {
        //We want to fetch this very event to make sure we don't loop our animations
        m_index++;
    }
    else
    {
        //Mark as unhandled
        return -1;
    }
    return 0;
}

MenuGameState::MenuGameState()
{
    name = "menugamestate";

    //Set the menu positions
    m_main_position.x = 0.f;
    m_main_position.y = 0.f;

    m_options_position.x = 1000.f;
    m_options_position.y = 1000.f;

    //#TODO: This should be translateable

    if (!m_font.loadFromFile("Fonts/arial.ttf"))
    {
        printf("[Error] Failed load font in %s:%d\n", __FILE__, __LINE__);
    }


    ClickableMenuText startGame;
    startGame.on_click = std::bind(&MenuGameState::startGameClicked, this);
    ClickableMenuText loadGame;
    loadGame.on_click = std::bind(&MenuGameState::loadGameClicked, this);
    ClickableMenuText Options;
    Options.on_click = std::bind(&MenuGameState::optionsClicked, this);
    ClickableMenuText Exit;
    Exit.on_click = std::bind(&MenuGameState::exitClicked, this);

    startGame.setFont(m_font);
    startGame.setString("Start game");
    startGame.setPosition(m_main_position);


    loadGame.setFont(m_font);
    loadGame.setString("Load game");
    loadGame.setPosition(m_main_position + SFG::Vector2f(0.f, 50.f));


    Options.setFont(m_font);
    Options.setString("Settings");
    Options.setPosition(m_main_position + SFG::Vector2f(0.f, 100.f));


    Exit.setFont(m_font);
    Exit.setString("Exit");
    Exit.setPosition(m_main_position + SFG::Vector2f(0.f, 150.f));

    //Set the correct Centers
    startGame.setOrigin(SFG::Rect<float>(startGame.getLocalBounds()).center());
    loadGame.setOrigin(SFG::Rect<float>(loadGame.getLocalBounds()).center());
    Options.setOrigin(SFG::Rect<float>(Options.getLocalBounds()).center());
    Exit.setOrigin(SFG::Rect<float>(Exit.getLocalBounds()).center());

    m_view.setCenter(SFG::Rect<float>(startGame.getGlobalBounds()).center());

    this->m_texts.push_back(startGame);
    this->m_texts.push_back(loadGame);
    this->m_texts.push_back(Options);
    this->m_texts.push_back(Exit);





    //Options menu
    ClickableMenuText optText;

    optText.setFont(m_font);
    optText.setString("Resolution");
    optText.setPosition(m_options_position);
    optText.setOrigin(SFG::Rect<float>(optText.getLocalBounds()).center());
    optText.on_click = std::bind(&MenuGameState::resolutionClicked, this);
    this->m_texts.push_back(optText);

    optText.setString("Back");
    optText.setPosition(m_options_position + SFG::Vector2f(0.f, 50.f));
    optText.on_click = std::bind(&MenuGameState::backToMainClicked, this);
    optText.setOrigin(SFG::Rect<float>(optText.getLocalBounds()).center());
    this->m_texts.push_back(optText);

}

int MenuGameState::init(SFG::Window& win)
{
	this->UI()->setTarget(&win.getSFMLWindow());
    this->m_view.setSize(win.getSFMLWindow().getSize().x, win.getSFMLWindow().getSize().y);
    this->m_view.setCenter(0.f, 0.f);
    win.getSFMLWindow().setView(m_view);
    return 0;
}

int MenuGameState::processEvents(SFG::Window& window, std::vector<sf::Event>& events)
{
    for (size_t i = 0; i < events.size(); i++)
    {
        if (events[i].type == sf::Event::MouseMoved)
        {
            //Map things correctly
            sf::Vector2f logical_mouse_position = window.getSFMLWindow().mapPixelToCoords(sf::Vector2i(events[i].mouseMove.x, events[i].mouseMove.y), window.getSFMLWindow().getView());
            for (auto& text : m_texts)
            {
                //Check if mouse is hovering
                if (text.getGlobalBounds().contains(logical_mouse_position))
                {
                    text.setColor(sf::Color::Green);
                    //Check if animation not already running
                    if (!text.hoverAnim.isValid())
                    {
                        //Create the animation
                        SFG::Pointer<CMTAnimation> anim(new CMTAnimation);
                        anim->m_target = &text;
                        anim->m_func = std::bind(&MenuGameState::hoverAnimation, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
                        //Establish reference in the targetted object
                        text.hoverAnim = anim;

                        this->m_animations.push_back(anim);
                    }
                }
                else
                {
                    //if previously set, get the animation and set to auto-end
                    if (text.hoverAnim.isValid())
                    {
                        text.hoverAnim->m_done = true;
                        text.hoverAnim.reset(nullptr);
                    }
                    text.setRotation(0.f);
                    text.setColor(sf::Color::White);
                }
            }

        }
        else if (events[i].type == sf::Event::MouseButtonPressed)
        {
            //Map things correctly
            sf::Vector2f logical_mouse_position = window.getSFMLWindow().mapPixelToCoords(sf::Vector2i(events[i].mouseButton.x, events[i].mouseButton.y), window.getSFMLWindow().getView());
            for (auto& text : m_texts)
            {
                //Check if mouse is hovering
                if (text.getGlobalBounds().contains(logical_mouse_position))
                {
                    text.mouse_held = true;
                    //Create the animation
                    SFG::Pointer<CMTAnimation> anim(new CMTAnimation);
                    anim->m_target = &text;
                    anim->m_func = std::bind(&MenuGameState::clickAnimation, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
                    this->m_animations.push_back(anim);
                    //text.setRotation(10.f);
                    text.setColor(sf::Color::Green);
                }
                else
                {
                    text.mouse_held = false;
                    text.setRotation(0.f);
                    text.setColor(sf::Color::White);
                }
            }

        }
        else if (events[i].type == sf::Event::MouseButtonReleased)
        {
            //Map things correctly
            sf::Vector2f logical_mouse_position = window.getSFMLWindow().mapPixelToCoords(sf::Vector2i(events[i].mouseButton.x, events[i].mouseButton.y), window.getSFMLWindow().getView());
            for (auto& text : m_texts)
            {
                //Check if mouse is hovering
                if (text.getGlobalBounds().contains(logical_mouse_position))
                {
                    if (text.mouse_held == true)
                    {
                        //Register as click
                        text.setRotation(0.f);
                        text.setColor(sf::Color::Red);

                        text.on_click();
                        text.mouse_held = false;
                    }
                }
                else
                {
                    text.setRotation(0.f);
                    text.setColor(sf::Color::White);
                }
            }

        }
        else if(events[i].type == sf::Event::Resized)
        {
            float oldw = m_view.getSize().x;
            float oldh = m_view.getSize().y;


            m_view.setCenter(0.f, 0.f);

            m_view.setSize(events[i].size.width, events[i].size.height);

            window.getSFMLWindow().setView(m_view);
            //SFG::Util::printLog(SFG::Util::Development, __FILE__, __LINE__, "In MGS resized: resized (%f|%f) changed center to (%f|%f)", events[i].size.width, events[i].size.height, m_view.getCenter().x, m_view.getCenter().y);
        }
    }
    window.getSFMLWindow().setView(m_view);
    return 0;
}

void MenuGameState::draw(sf::RenderTarget* t)
{
    assert(t != nullptr);

    //Set view
    t->setView(m_view);
    //Draw items
    for (auto text : m_texts)
    {
        t->draw(text);
    }

}

void MenuGameState::optionsClicked(void)
{
    //Go to options screen
    SFG::Pointer<ViewAnimation> anim(new ViewAnimation());
    anim->m_target = &this->m_view;
    anim->m_func = std::bind(&MenuGameState::panToOptions, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    this->m_animations.push_back(anim);
}

void MenuGameState::backToMainClicked(void)
{
    //Go to main screen
    SFG::Pointer<ViewAnimation> anim(new ViewAnimation());
    anim->m_target = &this->m_view;
    anim->m_func = std::bind(&MenuGameState::panToMain, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    this->m_animations.push_back(anim);
}

int MenuGameState::update(double dt)
{
    for (size_t i = 0; i < m_animations.size(); i++)
    {
        auto& a = m_animations[i];
        if (!a->m_done)
        {
            if (a->update(dt))
            {
                m_animations.erase(m_animations.begin() + i);
            }
        }
    }
    return 0;
}

void MenuGameState::generalPan(sf::View* target, double time, bool* done, const SFG::Vector2f& to)
{
    assert(target != nullptr);
    assert(done != nullptr);

    float dist = 2.f*float(time);

    //Get the direction vector
    SFG::Vector2f vector;
    vector.x = to.x - m_view.getCenter().x;
    vector.y = to.y - m_view.getCenter().y;

    //printf("%f|%f\n", vector.x, vector.y);

    auto tr = vector.trimmed((dist <= vector.length()) ? dist : vector.length());

    //printf("tr.length() is %f\n", tr.length());
    if (tr.length() <= 1.f) *done = true;

    target->move(tr);
}

void MenuGameState::clickAnimation(ClickableMenuText* target, double time, bool* done) const
{
    //300 ms animation time
    double rotation = time*10. / 300.;
    //set the animation
    if (rotation >= 10.)
    {
        rotation = 10.;
        *done = true; //Mark as finished
    }
    target->setRotation(float(rotation));
}

void MenuGameState::hoverAnimation(ClickableMenuText* target, double time, bool* done) const
{
    target->hover_time = float(time);
    target->setRotation(sin(target->hover_time / 100.f)*5.f);
}

SFG::Pointer<Loadable> Module::request(const sf::String& source)
{
    for (size_t i = 0; i < this->m_data.size(); i++)
    {
        if (source == this->m_data[i]->m_source)
        {
            return this->m_data[i];
        }
    }
    printf("[Warning] Object not available as requested by identifier \"%s\" in %s:%d\n", source.toAnsiString().c_str(), __FILE__, __LINE__);
    return nullptr;
}

SFG::Pointer<Loadable> Module::requestByName(const sf::String& name)
{
    for (size_t i = 0; i < this->m_data.size(); i++)
    {
        if (name == this->m_data[i]->m_name)
        {
            return this->m_data[i];
        }
    }
    printf("[Warning] Object not available as requested by identifier \"%s\" in %s:%d\n", name.toAnsiString().c_str(), __FILE__, __LINE__);
    return nullptr;
}

int Module::unload()
{
    if (m_data.size() == 0) return 1;
    for (auto l : m_data) {
        if (l.isValid()) {
            l.release();
        }
    }
    m_data.clear();
    return 0;
}
