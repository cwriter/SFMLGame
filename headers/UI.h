#pragma once
#include "stdafx.h"
#include "XML.h"
#include "StringManager.h"
#include "Window.h"
#include "Pointer.h"
#include <functional>

namespace SFG
{

class Window;

#define _CRTDBG_MAP_ALLOC

#define CENTER(x, y) ((x - y) / 2.0f)
#define WINDOW_MIN_WIDTH 100
#define WINDOW_MIN_HEIGHT 100

sf::FloatRect changeRect(const sf::FloatRect& fr, double factor);

class UIComponent;
class UILabel;

class UIGrid
{
public:
    UIGrid(int x, int y)
    {
        setSize(x, y);

    }
    UIGrid()
    {
        setSize(0, 0);
    }
    ~UIGrid();

    void addComponent(UIComponent* comp, int x, int y) {
        this->m_components[y*this->m_cols + x] = comp;
    }

    void setGridSize(float w, float h);

    void setColWidth(int col, float width) {
        this->m_colsizes[col] = width;
    }

    void setRowHeight(int row, float height) {
        this->m_rowsizes[row] = height;
    }

    void update(sf::Vector2f mousePos, float dt);

    void setSize(int x, int y);

    void draw(sf::RenderTarget& target, float scale, float top_offset);

    void setPosition(float x, float y) {
        this->m_position.left = x;
        this->m_position.top = y;
    }

    void setPosition(sf::Vector2f& vec) {
        this->m_position.left = vec.x;
        this->m_position.top = vec.y;
    }

    inline std::vector<UIComponent*>& getComponentVector() {
        return this->m_components;
    }
private:

    sf::FloatRect m_position;
    //float x, y, w, h;

    std::vector<float> m_colsizes;
    std::vector<float> m_rowsizes;

    int m_cols;
    int m_rows;
    std::vector<UIComponent*> m_components;
};

class MouseButtonData
{
public:
	int btn;
	sf::Vector2f pos;
};

class ActionStruct
{
public:
	sf::String name;
	std::function<void(void)> func;
private:
	
};

class UIComponent
{
public:
    enum Function {
        mbDown = 0,
        mbUp,
        hoverEnter,
        hoverLeave,
        mouseMoved,
		fillContextMenu,


        FunctionListSize
    };
    UIComponent() {
        this->m_functions.resize(Function::FunctionListSize, [](void* data) {
            return 0;
        });
        hovered = false;
    }
    virtual ~UIComponent() {}
    
    
    ///<summary>
    ///Returns a vector of text and linked action to be filled into a context menu
    ///</summary>
    SFG::Pointer<std::vector<ActionStruct>> getContextMenu()
    {
		SFG::Pointer<std::vector<ActionStruct>> ptr(new std::vector<ActionStruct>());
		
		launchFunction(Function::fillContextMenu, ptr.getElement());
		
		return ptr;
	}

    void setFunction(int type, std::function<int(void*)> func) {
        m_functions[type] = func;
    }
    
    ///<summary>
    ///Sets the size of a component. If a parameter is 0, the component shall
    ///take the size it finds fitting.
    ///</summary>
    virtual void setSize(float x, float y)
	{
		
	}

    virtual int on_mbDown(int btn, const sf::Vector2f& pos) {
		MouseButtonData mbd;
		mbd.btn = btn;
		mbd.pos = pos;
        return launchFunction(Function::mbDown, &mbd);
    }
    
    virtual int on_mbUp(int btn, const sf::Vector2f& pos) {
		MouseButtonData mbd;
		mbd.btn = btn;
		mbd.pos = pos;
        return launchFunction(Function::mbUp, &mbd);
    }
    
    virtual int on_hoverEnter() {
        hovered = true;
        return launchFunction(Function::hoverEnter, NULL);
    }
    
    virtual int on_hoverLeave() {
        hovered = false;
        return launchFunction(Function::hoverLeave, NULL);
    }
    
    virtual int on_mouseMoved(const sf::Vector2f* mpos) {
        return launchFunction(Function::mouseMoved, (void*)mpos);
    }

    virtual void move(float x, float y) {
        this->m_bounds.left += x;
        this->m_bounds.top += y;
    }

    virtual sf::Vector2f getPosition() {
        return sf::Vector2f();
    }

    virtual void draw(sf::RenderTarget& target, float scale) {

    }

    virtual void setPosition(float x, float y) {
        this->m_bounds.left = x;
        this->m_bounds.top = y;
    }

    virtual void update(float dt) {

    }

    virtual sf::FloatRect& bounds() {
        return m_bounds;
    }

    virtual void setBounds(const sf::FloatRect& fr) {
        this->m_bounds = fr;
    }

    sf::String m_name;
    bool hovered;

protected:
    sf::FloatRect m_bounds;
private:
    std::vector<std::function<int(void*)>> m_functions;

    int launchFunction(int type, void* data = NULL) {
        return this->m_functions[type](data);
    }

};

class UIComboBox : public UIComponent
{
public:
    UIComboBox();
    ~UIComboBox() {};

    void draw(sf::RenderTarget& target, float scale) override;
private:
    sf::Color m_background_color;
    sf::FloatRect m_max_expanding_size;
    size_t m_scroll_index; //The topmost item in the current scrollview (can, but doesn't have to be the first item in the vector
    size_t m_selected_index;
    bool m_expanded;
    std::vector<UIComponent*> m_elements;
};



class UILabel : public UIComponent
{
public:
    UILabel() {
        m_x_align = Align::Low;
        m_y_align = Align::Low;
        m_flags = 0;
    }

    enum Flags {
        None = 0,
        TextChanged = 1
    };

    enum Align {
        Low = 0,
        Center,
        High
    };



    ///<summary>
    ///Updates the text breaks (nl/'\n') when necessary to fit into the provided rect
    ///</summary>
    int updateTextBreaks();

    virtual void draw(sf::RenderTarget& target, float scale) override;

    void setText(const sf::String& text, int size, const std::string& font);
	
	void setImage(const sf::Texture& tex)
	{
		this->m_sprite.setTexture(tex);
	}

    void setBounds(const sf::FloatRect& fr) override;

    void setSize(float w, float h);

    void setText(const sf::String& text);

    void setAlign(int horizontal, int vertical) {
        m_x_align = horizontal;
        m_y_align = vertical;
    }

    void setPosition(sf::Vector2f& pos) {
        this->m_size.left = pos.x;
        this->m_size.top = pos.y;
    }


    virtual void setPosition(float x, float y) override;

    virtual sf::Vector2f getPosition() override {
        return sf::Vector2f(this->m_text.getGlobalBounds().left,
                            this->m_text.getGlobalBounds().top);
    }
protected:
    int m_flags;

    int m_x_align;
    int m_y_align;

    sf::FloatRect m_size;

	sf::Sprite m_sprite;
	
    sf::String m_string;
    sf::Text m_text;
    sf::Font m_font;
    int m_font_size;
};

class UIButton : public UILabel
{
public:
    UIButton();
    //~UIButton();

    virtual void draw(sf::RenderTarget& target, float scale) override;
	
	void setAction(std::function<void(void)> func)
	{
		m_action_on_click = func;
	}

private:
	std::function<void(void)> m_action_on_click;
    sf::RectangleShape m_button_rect;
};

class UITitleBar : public UIComponent
{
public:
    UITitleBar()
    {
        m_rect.setFillColor(sf::Color(50, 50, 250));
		m_tex.create(10,10);
    }

    virtual void setPosition(const sf::Vector2f& pos) {
        this->m_rect.setPosition(pos);
        this->m_bounds = this->m_rect.getGlobalBounds();
		this->m_button_sprite.setPosition(pos);
    }

    virtual void setSize(const sf::Vector2f& size) {
        this->m_rect.setSize(size);
		m_tex.create(uint(size.x), uint(size.y));
		m_tex.clear(sf::Color(0,0,0,0)); //Fill with transparent color
		
        this->m_bounds = this->m_rect.getGlobalBounds();
    }

    virtual void draw(sf::RenderTarget& target, float scale) override;

    virtual void move(float x, float y) override {
        this->m_rect.move(x, y);
        this->m_bounds = this->m_rect.getGlobalBounds();
    }

    virtual sf::FloatRect& bounds() override {
        return (m_bounds = this->m_rect.getGlobalBounds());
    }

    virtual sf::Vector2f getPosition() override {
        return sf::Vector2f(this->m_rect.getGlobalBounds().left,
                            this->m_rect.getGlobalBounds().top);
    }

private:
    sf::RectangleShape m_rect;
	std::vector<SFG::Pointer<UIComponent>> m_elements;
	sf::Sprite m_button_sprite;
	sf::RenderTexture m_tex;
};

class UIWindow
{
public:
	
	enum WindowFlags
	{
		Default = 0,
		NoResize = 1,
		NoTitlebar = 2,
		NoContextMenu = 4,
		CloseButton = 8
	};
	
    UIWindow()
    {
        this->m_grid = NULL;
        init();
        //this->m_titlebarlabel.setAlign(UILabel::Align::Center, UILabel::Align::Center);
        this->m_titlebarlabel.setAlign(UILabel::Align::Center, UILabel::Align::Low);
        this->m_windowframe.setFillColor(sf::Color(40, 150, 200));

        this->m_resizeknob.setSize(sf::Vector2f(20.f, 20.f));
        this->titlebarheight = 20.f;
        this->changed = true;
		
		this->m_window_flags = 0;
    }

    ~UIWindow() {
        if (this->m_grid != NULL) {
            delete this->m_grid;
        }
    }

    virtual int hoverAll(bool f) {
        if (m_grid == NULL) return -1;
        for (auto c : this->m_grid->getComponentVector()) {
            if (c == NULL) continue;
            if (f && !c->hovered) {
                c->on_hoverEnter();
                changed = true;
            }
            if (!f && c->hovered) {
                c->on_hoverLeave();
                changed = true;
            }
        }
        return 0;
    }

    int setWindowFlags(uint32_t flags)
	{
		this->m_window_flags |= flags;
		return 0;
	}
    
    uint32_t getWindowFlags() const
    {
		return this->m_window_flags;
	}
    
    virtual int on_mbDown(const sf::Vector2f& mpos, int button);

    virtual int on_mbUp(const sf::Vector2f& mpos, int button);

    virtual int on_hover(const sf::Vector2f& mpos);

    virtual int processEvents(std::vector<sf::Event>& events) {

        return 0;
    }

    virtual void draw(sf::RenderTarget& target, const sf::Vector2f& size, float scale);

    virtual void setGrid(UIGrid* g) {
        this->m_grid = g;
    }

    virtual void dragMoveHandler(const sf::Vector2f& mpos);

    virtual void resizeMoveHandler(const sf::Vector2f& mpos);

    virtual int update(const sf::Vector2f& mpos, float dt) {
        if (m_track > 0)
            dragMoveHandler(mpos);
        if (m_resize_track > 0)
            resizeMoveHandler(mpos);
        if(m_grid)
            m_grid->update(mpos, dt);

        return 0;
    }

    virtual int correctPositions() {
        this->m_titlebar.setPosition(sf::Vector2f(m_position.left, m_position.top));
        this->m_titlebarlabel.setPosition(m_position.left, m_position.top);
        this->m_windowframe.setPosition(m_position.left, m_position.top + m_titlebar.bounds().height);
        this->m_resizeknob.setPosition(sf::Vector2f(
                                           m_position.left + m_position.width - m_resizeknob.bounds().width,
                                           m_position.top + m_titlebar.bounds().height + m_position.height - m_resizeknob.bounds().height));
        return 0;
    }

    int load(const XMLReader& xml, const sf::String& name, const StringManager& strman);

	sf::FloatRect lastViewRect;
	
    sf::FloatRect relativePos;
    sf::FloatRect m_position;

    bool changed;

    float titlebarheight;
private:

	uint32_t m_window_flags;

    void init();

    unsigned char m_track;
    unsigned char m_resize_track;
	
	
	
    sf::Vector2f m_last_track_pos;
    sf::Vector2f m_last_resize_track_pos;
    //sf::RenderWindow* m_target;
    UITitleBar m_titlebar;
    sf::RectangleShape m_windowframe;
    UITitleBar m_resizeknob;
    UILabel m_titlebarlabel;
    sf::String m_name;
    sf::String m_title;

    float m_scale; //Pls use for resize functions only

    int m_type;
    UIGrid* m_grid;
};

class UIManager
{
public:
    UIManager() {
        m_target = nullptr;
		m_manager_scale = 1.f;
		
		m_context_window.setWindowFlags(
			UIWindow::WindowFlags::NoTitlebar | 
			UIWindow::WindowFlags::NoResize
		);
		
    }
    ~UIManager() {
        for (auto p : this->m_tmp_textures)
        {
            if (p != nullptr) {
                delete p;
            }
        }
        for (auto w : this->m_windows) {
            if (w != nullptr) {
                delete w;
            }
        }
    }

    void setScale(float s){
		m_manager_scale = s;
	}
	
	float getScale() const {
		return m_manager_scale;
	}
    
    void setTarget(sf::RenderWindow* w) {
        m_target = w;
    }

    sf::RenderWindow* getTarget() {
        return m_target;
    }

    ///<summary>
    ///Loads up all windows / things that should be open already
    ///</summary>
    ///<param name="path">The path to the file containing the UI Manager data</param>
    int load(const sf::String& path, const StringManager& strman);
	
	
    size_t selectElementByMousePos(sf::Vector2f* out);

    int processEvents(std::vector<sf::Event>& events);

    void draw(sf::RenderTarget& target);
    void draw(SFG::Window& target);

    void update(float dt) {
		assert(m_target != nullptr);
        for (auto w : m_windows) {
            sf::Vector2f mpos;
            mpos = m_target->mapPixelToCoords(sf::Mouse::getPosition(*m_target), m_target->getView());
            w->update(mpos, dt);
        }
    }

    inline void addWindow(UIWindow* w) {
        this->m_windows.push_back(w);
        this->m_tmp_textures.push_back(nullptr);
    }

    sf::FloatRect m_positions;

private:

    void drawWindowToTexture(sf::RenderTarget& target, size_t i, float scale);


	float m_manager_scale;
    sf::RenderWindow* m_target;
	
	//The global context menu
	SFG::UIWindow m_context_window;
	
    std::vector<UIWindow*> m_windows;
    std::vector<sf::RenderTexture*> m_tmp_textures;
};

};
