#pragma once
#include "stdafx.h"
#include "XML.h"
#include "StringManager.h"
#include "Window.h"

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

class UIComponent
{
public:
    enum Function {
        mbDown = 0,
        mbUp,
        hoverEnter,
        hoverLeave,
        mouseMoved,


        FunctionListSize
    };
    UIComponent() {
        this->m_functions.resize(Function::FunctionListSize, [](void* data) {
            return 0;
        });
        hovered = false;
    }
    virtual ~UIComponent() {}

    void setFunction(int type, std::function<int(void*)> func) {
        m_functions[type] = func;
    }

    virtual int on_mbDown() {
        return launchFunction(Function::mbDown);
    }
    virtual int on_mbUp() {
        return launchFunction(Function::mbUp);
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

    sf::String m_string;
    sf::Text m_text;
    sf::Font m_font;
    int m_font_size;
};

class UIButton : public UILabel
{
public:
    UIButton() {
        this->setFunction(Function::mbDown, [=](void* data) {
            //Set animation

            return 0;
        });
        m_text.setColor(sf::Color::Black);
        m_button_rect.setFillColor(sf::Color::Magenta);
        this->setAlign(Align::Center, Align::Center);
    }
    //~UIButton();

    virtual void draw(sf::RenderTarget& target, float scale) override
    {
        //This has a quite bad performance as it draws the text twice... Haven't found a way to optimize this, though
        UILabel::draw(target, scale);
        m_button_rect.setPosition(m_text.getGlobalBounds().left - m_text.getGlobalBounds().width / 2, m_text.getGlobalBounds().top - m_text.getGlobalBounds().height / 2);
        m_button_rect.setSize(sf::Vector2f(m_text.getGlobalBounds().width * 2, m_text.getGlobalBounds().height * 2));
        target.draw(m_button_rect);
        UILabel::draw(target, scale);
    }

private:
    sf::RectangleShape m_button_rect;
};

class UITitleBar : public UIComponent
{
public:
    UITitleBar()
    {
        m_rect.setFillColor(sf::Color(50, 50, 250));
    }

    virtual void setPosition(const sf::Vector2f& pos) {
        this->m_rect.setPosition(pos);
        this->m_bounds = this->m_rect.getGlobalBounds();
    }

    virtual void setSize(const sf::Vector2f& size) {
        this->m_rect.setSize(size);
        this->m_bounds = this->m_rect.getGlobalBounds();
    }

    virtual void draw(sf::RenderTarget& target, float scale) override {
        m_rect.setScale(1.f, scale);
        target.draw(m_rect);
    }

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
};

class UIWindow
{
public:
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

    virtual void dragMoveHandler(sf::Vector2f mpos);

    virtual void resizeMoveHandler(sf::Vector2f mpos);

    virtual int update(sf::Vector2f& mpos, float dt) {
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

    int load(XMLReader& xml, sf::String& name, const StringManager& strman);

    sf::FloatRect relativePos;
    sf::FloatRect m_position;

    bool changed;

    float titlebarheight;
private:



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
        m_target = NULL;
    }
    ~UIManager() {
        for (auto p : this->m_tmp_textures)
        {
            if (p != NULL) {
                delete p;
            }
        }
        for (auto w : this->m_windows) {
            if (w != NULL) {
                delete w;
            }
        }
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
    int load(const sf::String& path, const StringManager& strman)
    {
        sf::FileInputStream file;
        if (!file.open(path)) return -1;
        sf::Int64 fsize = file.getSize();
        if (fsize == -1) return -2;

        char* data = (char*)malloc(size_t(fsize));
        if (data == NULL) return -2;

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

    size_t selectElementByMousePos(sf::Vector2f* out);

    int processEvents(std::vector<sf::Event>& events);

    void draw(sf::RenderTarget& target);
    void draw(SFG::Window& target);

    void update(float dt) {
        for (auto w : m_windows) {
            sf::Vector2f mpos;
            mpos = m_target->mapPixelToCoords(sf::Mouse::getPosition(*m_target), m_target->getView());
            w->update(mpos, dt);
        }
    }

    inline void addWindow(UIWindow* w) {
        this->m_windows.push_back(w);
        this->m_tmp_textures.push_back(NULL);
    }

    sf::FloatRect m_positions;

private:

    void drawWindowToTexture(sf::RenderTarget& target, size_t i, float scale);



    sf::RenderWindow* m_target;
    std::vector<UIWindow*> m_windows;
    std::vector<sf::RenderTexture*> m_tmp_textures;
};

};
