#include "stdafx.h"
#include "UI.h"
#include <SFGUI/Widgets.hpp>

namespace SFG
{

//UIManager
#ifndef SFG_USE_CUSTOM_UI
sfg::SFGUI UIManager::sfgui;

int UIManager::load(const sf::String& path, const StringManager& strman)
{
	auto button = sfg::Button::Create( "Hello" );
	auto checkbox = sfg::CheckButton::Create("Test checkbox");
	auto checkbox2 = sfg::CheckButton::Create("Test checkbox 2");
	auto window = sfg::Window::Create();
	auto grid = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	grid->Pack(button);
	grid->Pack(checkbox, false, false);
	grid->Pack(checkbox2, false, false);
	window->SetTitle( "Hello World example" );
	
	window->Add(grid);
	
	m_sfg_desktop.Add(window);
	m_windows.push_back(window);
	
	return -1;
}

void UIManager::draw(sf::RenderTexture& tex)
{
	auto tmp = tex.getView();
	tex.setView(tex.getDefaultView());
	sfgui.Display(tex);
	tex.setView(tmp);
}

int UIManager::processEvents(std::vector< sf::Event >& events)
{
	//sfgui doesn't disable events though :(
	for(auto& e : events)
	{
		m_sfg_desktop.HandleEvent(e);
		if(e.type == sf::Event::MouseButtonPressed || e.type == sf::Event::MouseButtonReleased)
		{
			for(auto& w : m_windows)
			{
				if(w->GetClientRect().contains((sf::Vector2f(e.mouseButton.x, e.mouseButton.y))))
				{
					e.type = sf::Event::EventType::Count;
				}
			}
		}
		if(e.type == sf::Event::Resized)
		{
			
		}
	}
	
	return 0;
}
	
#else
size_t UIManager::selectElementByMousePos(sf::Vector2f* out)
{
    if (m_target == nullptr)
	{
		SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__,
			"m_target is nullptr");
		return 0;
	}
	if(this->m_windows.size() == 0) return 0;
    /*sf::Vector2f mpos = m_target->mapPixelToCoords(
                            sf::Mouse::getPosition(*m_target),
                            m_target->getDefaultView());*/
	sf::Vector2f mpos(sf::Mouse::getPosition(*m_target).x, sf::Mouse::getPosition(*m_target).y);
    if (out != nullptr) *out = mpos;
    //for (auto w : m_windows) {
	//Reverse loop
	size_t i = m_windows.size();
	do{
		i--;
		//printf("Loop: Window is %d\n", i);
		UIWindow* w = m_windows[i];
        if (w == nullptr) continue;
        sf::FloatRect fr = w->m_position;
        fr.height += w->titlebarheight;
        if (fr.contains(mpos))
        {
			//printf("\tContaining mouse!\n");
            return i;
        }
    } while(i > 0);
    return m_windows.size();
}

int UIManager::processEvents(std::vector<sf::Event>& events)
{
    sf::Vector2f mpos;
    size_t index = selectElementByMousePos(&mpos);
    size_t tindex = 0;


    //Handle general events
    for (auto& e : events) {
        switch (e.type) {
        case sf::Event::Resized:
            //printf("Resize event called\n");
            for (auto w : m_windows) {
                w->changed = true;
                w->correctPositions();
            }
            break;
		default:
			break;
        }
    }



    //Specific events
    for (auto& e : events) {


        switch (e.type) {
        case sf::Event::MouseButtonPressed:
            if (index >= m_windows.size())	//It actually should never be bigger than m_windows.size(). But you never know...
                continue;
            //Call the window-specific handler
            if (this->m_windows[index]->on_mbDown(mpos, e.mouseButton.button) == 0) {
                //Unused, don't delete from list
            }
            else {
                //Used, please "delete"
                e.type = sf::Event::Count;
                m_windows[index]->changed = true;
            }
            break;

        case sf::Event::MouseButtonReleased:
            //Call the window-specific handler
            for (auto w : m_windows)
            {
                w->on_mbUp(mpos, e.mouseButton.button);
            }
            if (index >= m_windows.size())	//It actually should never be bigger than m_windows.size(). But you never know...
                continue;
            if (this->m_windows[index]->on_mbUp(mpos, e.mouseButton.button) == 0) {
                //Unused, don't delete from list
            }
            else {
                //Used, please "delete"
                e.type = sf::Event::Count;
                m_windows[index]->changed = true;
            }
            break;

        case sf::Event::MouseMoved:
            if (index >= m_windows.size())	//It actually should never be bigger than m_windows.size(). But you never know...
                continue;
            //Call the window-specific handler
            if (this->m_windows[index]->on_hover(mpos) == 0) {
                //Unused, don't delete from list
            }
            else {
                //Used, please "delete"
                e.type = sf::Event::Count;
                m_windows[index]->changed = true;
            }
            //Set all other elements to "unhovered"

            for (auto w : this->m_windows) {
                if (tindex != index) w->hoverAll(false);
                tindex++;
            }
            break;
			
		default:
			break;

        }



    }
    return 0;
}

void UIManager::draw(sf::RenderTarget& target)
{
/*
#ifdef _DEBUG
    SFG::Util::printLog(SFG::Util::Warning, __FILE__, __LINE__, "[Warning] Using non-scaling aware function");
#endif
*/
	priv_draw(target, m_manager_scale);

}

void UIManager::draw(SFG::Window& target)
{
	priv_draw(target, target.scale());
}

void UIManager::priv_draw(sf::RenderTarget& target, float scale)
{
	
	const sf::View& prevview = target.getView();
	target.setView(target.getDefaultView());
	
    size_t i = 0;
    for (auto w : m_windows) {
        //#TODO: Get the size of the view and align objects accordingly

        if (w->changed)
        {
            w->correctPositions();
            drawWindowToTexture(target, i, scale);

            m_tmp_textures[i]->display();
        }

        sf::Sprite tmp;
        tmp.setTexture(this->m_tmp_textures[i]->getTexture());
        tmp.setPosition(w->m_position.left, w->m_position.top);
        target.draw(tmp);

        i++;
    }
    
    target.setView(prevview);
}


void UIManager::drawWindowToTexture(sf::RenderTarget& target, size_t i, float scale)
{

    /*sf::Vector2f size = target.getView().getSize();
	sf::Vector2f center = target.getView().getCenter();*/
	sf::Vector2f size = target.getDefaultView().getSize();
	sf::Vector2f center = target.getDefaultView().getCenter();
	sf::Vector2f top_left(center.x - size.x / 2.f, center.y - size.y / 2.f);
	
	m_windows[i]->lastViewRect = sf::FloatRect(top_left.x, top_left.y, size.x, size.y);
	//printf("View rect (%f|%f|%f|%f)\n", top_left.x, top_left.y, size.x, size.y);
	
    m_windows[i]->m_position.left = top_left.x + size.x * m_windows[i]->relativePos.left / 100.f;
    m_windows[i]->m_position.width = size.x * m_windows[i]->relativePos.width / 100.f * scale;
    m_windows[i]->m_position.top = top_left.y + size.y * m_windows[i]->relativePos.top / 100.f;
    m_windows[i]->m_position.height = size.y * m_windows[i]->relativePos.height / 100.f * scale;


    if (m_tmp_textures[i] == nullptr) {
        m_tmp_textures[i] = new sf::RenderTexture();
        if (m_tmp_textures[i] == nullptr) {
            perror("Allocation error");
        }
    }
    else if (!m_windows[i]->changed) {
        //Nothing changed, don't redraw
        return;
    }

    unsigned int wtex = (unsigned int)(m_windows[i]->m_position.width);
    unsigned int htex = (unsigned int)(m_windows[i]->m_position.height
                                       + m_windows[i]->titlebarheight);

    m_tmp_textures[i]->create(wtex, htex);
    m_tmp_textures[i]->clear();

    sf::FloatRect poscpy = m_windows[i]->m_position;
    m_windows[i]->m_position.left = 0.0f;
    m_windows[i]->m_position.top = 0.0f;

	m_windows[i]->draw(*m_tmp_textures[i], size, scale);

    m_windows[i]->m_position = poscpy;

    m_windows[i]->changed = false;	//Mark all changes as accepted and redrawn
}

int UIManager::load(const sf::String& path, const StringManager& strman)
{
    sf::FileInputStream file;
    if (!file.open(path)) return -1;
    sf::Int64 fsize = file.getSize();
    if (fsize == -1) return -2;

    char* data = (char*)malloc(size_t(fsize));
    if (data == nullptr) return -2;

    if (file.read(data, fsize) != fsize)
    {
        free(data);
        return -3;
    }
    sf::String src = sf::String::fromUtf8(data, data + fsize);
    XMLReader reader;
    //reader.setSource(std::string(data, size_t(fsize))); //#TODO/CRITICAL: CHECK IF WORKING!
    reader.setSource(src); //#TODO/CRITICAL: CHECK IF WORKING!
    //Free the file read data
    free(data);
    int ret = reader.parse();
    if (ret != 0)
    {
        printf("[Error] Failed to parse structure in %s:%d with Code %d\n", __FILE__, __LINE__, ret);
        return -2;
    }
    //get the general group handle
    auto h = reader.getXMLGroupHandle(L"xml/module[UI]/manager/");
    if (h == nullptr)
    {
        printf("[Error] Failed to load %s in %s:%d\n", path.toAnsiString().c_str(), __FILE__, __LINE__);
        return -4;
    }
    //Get a reader for this
    XMLReader sub(*h);
    //Reparse (should not be necessary)
    ret = sub.parse();
    if (ret != 0)
    {
        printf("[Error] Failed to parse structure in %s:%d with Code %d\n", __FILE__, __LINE__, ret);
        return -3;
    }
    for (size_t i = 0; true; i++)
    {
        UIWindow* window = new UIWindow();
        if (window == nullptr)
        {
            printf("[Error] Failed to allocate memory in %s:%d\n", __FILE__, __LINE__);
            return -1;
        }
        auto _tmp = sf::String(std::to_wstring(i));
        if (window->load(sub, _tmp, strman) == -1)
        {
            //If we read all windows, just break
            delete window; //Cleanup the tmp instance
            break;
        }
        else
        {
            //Otherwise, push the object on the vector
            addWindow(window);
        }
    }


    return 0;
}
#endif
};
