#include "stdafx.h"
#include "custom_UI.h"

namespace SFG
{

sf::FloatRect changeRect(const sf::FloatRect& fr, double factor) {
    sf::FloatRect rect;
    rect.left = fr.left + fr.width * float(factor / 2.0);
    rect.top = fr.top + fr.height * float(factor / 2.0);
    rect.width = fr.width * float(1.0 - factor / 2.0);
    rect.height = fr.height * float(1.0 - factor / 2.0);

    return rect;
}

//UILabel

void UILabel::draw(sf::RenderTarget& target, float scale)
{
    //Set correct scale
    if ((unsigned int)(m_font_size * scale) != m_text.getCharacterSize()) {
        m_text.setCharacterSize((unsigned int)(m_font_size * scale));
        m_flags |= Flags::TextChanged;
    }
    //Bounding rect is m_size; align m_text inside that according to
    if (m_flags & Flags::TextChanged) {
        updateTextBreaks();
        m_flags &= ~Flags::TextChanged;
    }
    
    //Adjust alignment
    switch (m_x_align) {
    case Align::Low:
        this->m_text.setPosition(this->m_size.left, this->m_text.getPosition().y);
        break;
    case Align::Center:
        this->m_text.setPosition(this->m_size.left + CENTER(m_size.width, m_text.getGlobalBounds().width),
                                 this->m_text.getPosition().y);
        break;
    case Align::High:
        this->m_text.setPosition(this->m_size.left + this->m_size.width,
                                 this->m_text.getPosition().y);
        break;
    default:
        printf("Error: Unknown value at %s:%d\n", __FILE__, __LINE__);
        break;
    }
    switch (m_y_align) {
    case Align::Low:
        this->m_text.setPosition(m_text.getGlobalBounds().left, this->m_size.top);
        break;
    case Align::Center:
        this->m_text.setPosition(m_text.getGlobalBounds().left,
                                 this->m_size.top + CENTER(m_size.height, m_text.getGlobalBounds().height));
        break;
    case Align::High:
        this->m_text.setPosition(m_text.getGlobalBounds().left,
                                 this->m_size.top + this->m_size.height);
        break;
    default:
        printf("Error: Unknown value at %s:%d\n", __FILE__, __LINE__);
        break;
    }
    
    m_sprite.setPosition(m_text.getPosition());
    target.draw(m_sprite);
    
    target.draw(m_text);
}

int UILabel::updateTextBreaks()
{
    float currentWidth = 0.0f;
    size_t last_whitespace = 0;
    int chars_in_this_line = 0;
    sf::String modified = this->m_string;
    std::string test = this->m_string;
    for (size_t i = 0; i < modified.getSize(); i++) {
        //this->setBounds(sf::FloatRect(this->bounds().left, this->bounds().top, this->m_text.getGlobalBounds().width,this->m_text.getGlobalBounds().height));
        if (modified[i] == ' ') last_whitespace = i;
        else if (modified[i] == '\n') {
            //Reset
            chars_in_this_line = 0;
            currentWidth = 0.f;
            last_whitespace = 0;
            continue;
        }

        float glyphwidth = this->m_font.getGlyph(modified[i], this->m_text.getCharacterSize(), false).advance;

        float kerning = 0.f;
        if (i > 0)
            kerning = this->m_font.getKerning(modified[i - 1], modified[i], this->m_text.getCharacterSize());

        if (currentWidth + glyphwidth + kerning >= this->bounds().width
                && chars_in_this_line > 1) {
            //We need to break.
            if (last_whitespace > 0) {
                //Whitespace available, break at it. This won't increase the string size
                modified[last_whitespace] = '\n';
                currentWidth = 0.f;
                chars_in_this_line = 0;

                i = last_whitespace;

                last_whitespace = 0;
                //std::string test = modified;
                //printf("Testing...: %s\n", test.c_str());
                continue;
            }
            else {
                //No whitespace, but we have to break anyways. This will increase the string size
                modified.insert(i, "\n");
                chars_in_this_line = 0;
                last_whitespace = 0;
                currentWidth = 0.f;

                //std::string test = modified;
                //printf("Testing...: %s\n", test.c_str());

                continue;
            }

            //i++; //Go next (needs to be here as both break methods are an item that won't bloat the width by themselves.
        }
        chars_in_this_line++;
        currentWidth += glyphwidth + kerning;


    }
    //reset the text
    this->m_text.setString(modified);
    return 0;
}

void UILabel::setText(const sf::String& text, int size, const std::string& font)
{
    this->m_string = text;
    m_font.loadFromFile(font);
    this->m_text.setFont(m_font);
    this->m_text.setCharacterSize(size);
    m_font_size = size;

    setText(text);
}

void UILabel::setBounds(const sf::FloatRect& fr)
{
    this->m_size = fr;

    this->m_bounds = m_size;

    //To make the text break correctly:
    this->m_flags |= Flags::TextChanged;
}

void UILabel::setSize(float w, float h)
{
    this->m_size.width = w;
    this->m_size.height = h;

    this->m_bounds = m_size;

    //To make the text break correctly:
    this->m_flags |= Flags::TextChanged;
}

void UILabel::setText(const sf::String& text)
{
    this->m_text.setString(text);
    this->m_size.width = m_text.getGlobalBounds().width;
    this->m_size.height = m_text.getGlobalBounds().height;

    this->m_flags |= Flags::TextChanged;
}

void UILabel::setPosition(float x, float y)
{
    this->m_size.left = x;
    this->m_size.top = y;

    m_bounds = this->m_size;

    this->m_text.setPosition(x, y);
}
//UIGrid

UIGrid::~UIGrid()
{
    for (auto c : m_components) {
        if (c != NULL) {
            delete c;
        }
    }
}

void UIGrid::setGridSize(float w, float h)
{
    this->m_position.width = w;
    this->m_position.height = h;

    //Adjust cell sizes accordingly
    float cell_width = w / (float)this->m_cols;
    float cell_height = h / (float)this->m_rows;

    this->m_colsizes.assign(m_cols, cell_width);
    this->m_rowsizes.assign(m_rows, cell_height);
}

void UIGrid::setSize(int x, int y)
{
    this->m_cols = x;
    this->m_rows = y;

    m_colsizes.resize(x, 50.f);
    m_rowsizes.resize(y, 50.f);

    this->m_components.resize(x*y, NULL);
}

void UIGrid::update(sf::Vector2f mousePos, float dt)
{
    for (auto c : m_components) {
        if (c != nullptr) {
            /*if (c->bounds().contains(mousePos)) {
            	if (c->hovered == false) {
            		//Call hover
            		c->on_hoverEnter();
            	}
            }
            else {
            	if (c->hovered == true) {
            		c->on_hoverLeave();
            	}
            }*/
            c->update(dt);
        }
    }
}

void UIGrid::draw(sf::RenderTarget& target, float scale, float top_offset)
{
    int i = 1;
    for (auto c : this->m_components)
    {
        if (c == nullptr) {
            i++;
            continue;
        }
        int x = (m_cols == 0) ? i % this->m_cols : 0;
        int y = i / this->m_rows;

        float left_offset = 0.0f;
        for (int t = 0; t < x; t++) {
            left_offset += this->m_colsizes[t];
        }

        for (int t = 0; t < y; t++) {
            top_offset += this->m_rowsizes[t];
        }

        //Comment for the out-commented values: In order to not shift the drawn elements, we need to draw it relative to the window's origin and not relative to the Matrixes origin
        c->setBounds(sf::FloatRect(
                         /*this->m_position.left*/ + left_offset,
                         /*this->m_position.top*/ + top_offset,
                         this->m_colsizes[x], this->m_rowsizes[y]));

        c->draw(target, scale);
        i++;
    }
}

//UIComboBox
UIComboBox::UIComboBox()
{
    m_expanded = false;
    this->setFunction(Function::mbDown, [=](void* data) {
        //#TODO: Handle shit here
        if (this->m_expanded) {
            this->m_expanded = false;
        }
        else {
            this->m_expanded = true;
        }
        return 1;
    });
    this->setFunction(Function::mouseMoved, [=](void* data) {
        sf::Vector2f* mpos = (sf::Vector2f*)data;
        float y = 0.f;
        for (size_t i = this->m_scroll_index; i < this->m_elements.size(); i++) {
            if (m_elements[i]->bounds().contains(*mpos)) {
                //found it
                m_selected_index = i;
                return 1;
            }
            y += m_elements[i]->bounds().height;
            if (y >= this->m_max_expanding_size.height) {
                //We are going out of range...
                break;
            }
        }
        return 0;
    });
}

void UIComboBox::draw(sf::RenderTarget& target, float scale)
{
    this->setBounds(changeRect(this->bounds(), scale));
    if (!m_expanded) {
        //Only draw the selected element and the drop-down arrow
        if (m_scroll_index >= m_scroll_index) {
            //Error: Index out of range
            return;
        }
        auto e = m_elements[m_scroll_index];
        e->setBounds(changeRect(this->bounds(), 0.9)); //Set the bounds
        e->draw(target, scale);

        return;
    }
    else {
        float y = 0.f;
        for (size_t i = 0; i < m_elements.size(); i++) {
            if (y + this->bounds().height > this->m_max_expanding_size.height) {
                //We need to start a scroll area
                break;
            }
            sf::FloatRect fr;
            fr.top = this->bounds().top + y;
            fr.left = this->bounds().left;
            fr.width = this->bounds().width;
            fr.height = m_elements[i]->bounds().height;
            m_elements[i]->setBounds(fr);
            //Draw background
            sf::RectangleShape rs;
            rs.setFillColor(m_background_color);
            rs.setSize(sf::Vector2f(fr.width, fr.height));
            rs.setPosition(fr.left, fr.top);
            target.draw(rs);
            //Draw element
            m_elements[i]->draw(target, scale);

            //Increase height counter.
            y += fr.height;
        }
    }
}

//UITitleBar

void UITitleBar::draw(sf::RenderTarget& target, float scale) 
{
	m_rect.setScale(1.f, scale);

	float current_x = 0.f;
	//Draw elements to texture
	for(size_t i = 0; i < m_elements.size(); i++)
	{
		auto& c = m_elements[i];
		c->setPosition(current_x, 0.f);
		c->setSize(0.f, this->m_bounds.height);
		c->draw(m_tex, scale);
		current_x += c->bounds().width;
		if(current_x > float(m_tex.getSize().x))
		{
			SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__, "Buttons in titlebar are wider than the bar itself");
		}
	}
	m_button_sprite.setTexture(m_tex.getTexture());
	
		
	target.draw(m_rect);
	target.draw(m_button_sprite);
}

//UIButton

UIButton::UIButton()
{
    this->setFunction(Function::mbDown, [=](void* data) {
        //Set animation
		//TODO: Make animation
		
		
		this->m_action_on_click();

        return 0;
    });
    m_text.setColor(sf::Color::Black);
    m_button_rect.setFillColor(sf::Color::Magenta);
    this->setAlign(Align::Center, Align::Center);
}

void UIButton::draw(sf::RenderTarget& target, float scale)
{
    //This has a quite bad performance as it draws the text twice... Haven't found a way to optimize this, though
    UILabel::draw(target, scale);
    m_button_rect.setPosition(m_text.getGlobalBounds().left - m_text.getGlobalBounds().width / 2, m_text.getGlobalBounds().top - m_text.getGlobalBounds().height / 2);
    //m_button_rect.setSize(sf::Vector2f(m_text.getGlobalBounds().width * 2, m_text.getGlobalBounds().height * 2));
	m_button_rect.setSize(sf::Vector2f(this->bounds().width, this->bounds().height));
    target.draw(m_button_rect);
    UILabel::draw(target, scale);
}


//UIWindow

int UIWindow::load(const XMLReader& xml, const sf::String& name, const StringManager& strman)
{
	size_t windex = 0;
    //Load a specific window style and default values from an xml file
    bool found = false;
	
	//xml.getXMLGroupHandle("")->dump();
	
	xml.for_all("object", [&](const XMLGroup* g){

		SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
			"Found window object"
		);
		//Careful: This will overwrite the old xml instance. This is DESIRED.
        XMLReader xml(*g);
		
        m_name = xml.getValue("name.");
        if (m_name == L"__xml_failure") {
            SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__,
								"Failed to get name for UI component");
			return;
        }
        //When the name is defined and the name doesn't match
        if (!name.isEmpty() && m_name != name)
        {
			if(std::isdigit(name[0]) && name == std::to_string(windex))
			{
				found = true;
			}
			else
			{
				windex++;
				return;
			}
        }
        else
		{

			
		}

        m_title = xml.getValue("title/");
        if (m_name == L"__xml_failure") {
            printf("Title not set. Defaulting to \"title\".\n");
            m_title = L"title";
        }
        this->m_titlebarlabel.setText(m_title, 18, "Fonts/arial.ttf");
		bool r = false;
        this->relativePos = xml.asFloatRect("rect/", r);
		if (r == false) {
			//Handle
		}

        //Get grid and elements next
        int cols = xml.asInt("grid/cols.", r);
        if (r == false) {
            //Handle
        }
        int rows = xml.asInt("grid/rows.", r);
        if (r == false) {
            //Handle
        }
        if (this->m_grid != nullptr) {
            delete m_grid;
            m_grid = nullptr;
        }
        if (this->m_grid == nullptr) {
            this->m_grid = new UIGrid(cols, rows);
            if (this->m_grid == nullptr) {
                //Error
                perror("ALLOCATION ERROR");
            }
        }

        //Cycle through elements
        xml.for_all("grid/element", [&](const XMLGroup* g){
			XMLReader xml(*g);
            sf::String type = xml.getValue("type.");
			
			bool b = false;
			int textsize = xml.asInt("textsize.", b);
			if(!b)
			{
				SFG::Util::printLog(SFG::Util::Warning, __FILE__, __LINE__,
					"Failed to get the textsize. Defaulting to 12");
				textsize = 12;
			}

            UIComponent* component = nullptr;


            if (type == L"label") {
                component = new UILabel();
                if (component == nullptr) {
                    perror("ALLOCATION ERROR");
                }
                else {
                    //Load and set the text
                    sf::String textid = xml.getValue("");
                    //The path has been checked before, so we don't have to check for "__xml_failure" again.
                    //textid.erase(std::remove_if(textid.begin(), textid.end(), isspace), textid.end());
                    SFML_ERASE_WHITESPACE(textid);
					SFG::Util::printLog(SFG::Util::LogMessageType::Information, __FILE__, __LINE__, "A label is created from string \"%s\"", textid.toAnsiString().c_str());
                    ((UILabel*)component)->setText(strman.getString(textid, lang("**-**")), textsize, "Fonts/arial.ttf");
                }
            }
            else if (type == L"button") {
                //Load and set button
                component = new UIButton();
                if (component == nullptr) {
                    perror("ALLOCATION ERROR");
                }
                else {
                    //Load and set the text
                    sf::String textid = xml.getValue("");
                    //The path has been checked before, so we don't have to check for "__xml_failure" again.
                    //textid.erase(std::remove_if(textid.begin(), textid.end(), isspace), textid.end());
                    SFML_ERASE_WHITESPACE(textid);
					SFG::Util::printLog(SFG::Util::LogMessageType::Information, __FILE__, __LINE__, "A label is created from string \"%s\"", textid.toAnsiString().c_str());
                    ((UIButton*)component)->setText(strman.getString(textid, lang("**-**")), textsize, "Fonts/arial.ttf");
                }
            }
            if (component == nullptr) {
                printf("UI component in window %s could not be loaded.\n", this->m_name.toAnsiString().c_str());
            }
            else {
                //Get name
                sf::String name = xml.getValue("name.");
                component->m_name = name;

                sf::String pos = xml.getValue("position.");
                std::vector<sf::String> vpos;
                split(vpos, pos);

                if (component == nullptr) {
                    printf("WARNING: Adding NULL component to grid\n");
                }
                this->m_grid->addComponent(component, std::stoi(vpos[0].toAnsiString()), std::stoi(vpos[1].toAnsiString()));
            }

		});
		found = true;
		correctPositions();
		windex++;
    });
	if(found)
		return 0;
	else
		return -1;
}

int UIWindow::on_mbDown(const sf::Vector2f& mpos, int button)
{
	SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
		"MBDown @ (%f|%f), window bounds (%f|%f|%f|%f)", 
		mpos.x, mpos.y, m_position.left, m_position.top,
		m_position.width, m_position.height);
    //Handle window-specific elements
	if(!(m_window_flags & WindowFlags::NoTitlebar))
	{
		SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
			"Titlebar bounds (%f|%f|%f|%f), mpos (%f|%f)", 
			m_titlebar.bounds().left, m_titlebar.bounds().top,
			m_titlebar.bounds().width, m_titlebar.bounds().height,
			mpos.x, mpos.y);
		if (this->m_titlebar.bounds().contains(mpos)) {
			return this->m_titlebar.on_mbDown(button, mpos);
		}
	}
	if(!(m_window_flags & WindowFlags::NoResize))
	{
		if (this->m_resizeknob.bounds().contains(mpos)) {
			return this->m_resizeknob.on_mbDown(button, mpos);
		}
	}
    
	//Get the position inside the grid
    if (this->m_grid == nullptr)
        return 0;	//Grid not available

    //Handle general elements
    for (auto c : this->m_grid->getComponentVector()) {
        if (c == nullptr) continue;
        if (c->bounds().contains(mpos)) {
            //Found the object the cursor is hovering over
            return c->on_mbDown(button, mpos);
        }
    }
    return 0;
}

int UIWindow::on_mbUp(const sf::Vector2f& mpos, int button)
{
    //Reset everything first
    this->m_resize_track = 0;
    this->m_track = 0;

    //Handle window-specific elements
	if(!(m_window_flags & WindowFlags::NoTitlebar))
	{
		if (this->m_titlebar.bounds().contains(mpos)) {
			return this->m_titlebar.on_mbUp(button, mpos);
		}
	}
	if(!(m_window_flags & WindowFlags::NoResize))
	{
		if (this->m_resizeknob.bounds().contains(mpos)) {
			return this->m_resizeknob.on_mbUp(button, mpos);
		}
	}

    //Get the position inside the grid
    if (this->m_grid == NULL)
        return 0;	//Grid not available


    if (!this->m_position.contains(mpos)) return 0;

    for (auto c : this->m_grid->getComponentVector()) {
        if (c == NULL) continue;
        if (c->bounds().contains(mpos)) {
            //Found the object the cursor is hovering over
            return c->on_mbUp(button, mpos);
        }
    }
    return 0;
}

int UIWindow::on_hover(const sf::Vector2f& mpos)
{
    //Get the position inside the grid
    if (this->m_grid == NULL)
        return 0;	//Grid not available

    //Handle window-specific elements
    if (this->m_titlebar.bounds().contains(mpos)) {
        if (!m_titlebar.hovered)
            return this->m_titlebar.on_hoverEnter();
    }
    if (this->m_resizeknob.bounds().contains(mpos)) {
        if (!m_resizeknob.hovered)
            return this->m_resizeknob.on_hoverEnter();
    }


    for (auto c : this->m_grid->getComponentVector()) {
        if (c == NULL) continue;
        if (c->bounds().contains(mpos)) {
            //Found the object the cursor is hovering over
            if (!c->hovered)
                return c->on_hoverEnter();
            else {
                return c->on_mouseMoved(&mpos);
            }
        }
    }
    return 0;
}

void UIWindow::dragMoveHandler(const sf::Vector2f& mpos)
{
	//printf("mpos: (%f|%f)\nm_last_track_pos: (%f|%f)\n", mpos.x, mpos.y, m_last_track_pos.x, m_last_track_pos.y);
	
    if (m_track == 1) {
		/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
			"m_track == 1");*/
        /*this->m_titlebar.move(mpos.x - m_last_track_pos.x,
        	mpos.y - m_last_track_pos.y);*/

        this->m_position.left += mpos.x - m_last_track_pos.x;
        this->m_position.top += mpos.y - m_last_track_pos.y;

		relativePos.left = (m_position.left - lastViewRect.left) * 100.f / lastViewRect.width;
		relativePos.top = (m_position.top - lastViewRect.top) * 100.f / lastViewRect.height;

        this->m_last_track_pos = mpos;

        //Update coords
        this->correctPositions();
    }
    else if (m_track == 2) {
		/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
			"m_track == 1");*/
        this->m_last_track_pos = mpos;
        m_track = 1;
    }
}

void UIWindow::resizeMoveHandler(const sf::Vector2f& mpos)
{
    if (m_resize_track == 1) {

        changed = true;

        this->m_position.width += mpos.x - m_last_resize_track_pos.x;
        this->m_position.height += mpos.y - m_last_resize_track_pos.y;

        this->m_last_resize_track_pos = mpos;

        this->relativePos.width = m_position.width * 100.f / lastViewRect.width;
        this->relativePos.height = m_position.height * 100.f / lastViewRect.height;

        this->m_last_resize_track_pos = mpos;
		
		if(this->m_grid != nullptr)
		{
			m_grid->setGridSize(m_position.width, m_position.height);
		}

    }
    else if (m_resize_track == 2) {
        this->m_last_resize_track_pos = mpos;
        m_resize_track = 1;
    }
}

void UIWindow::draw(sf::RenderTarget& target, const sf::Vector2f& size, float scale)
{
    this->m_scale = scale;
    //sf::Vector2f size = target.getDefaultView().getSize();
    m_position.left = size.x * relativePos.left / 100.f;
    m_position.width = size.x * relativePos.width / 100.f * scale;
    m_position.top = size.y * relativePos.top / 100.f;
    m_position.height = size.y * relativePos.height / 100.f * scale;

    sf::FloatRect dfr = m_position;
    dfr.left = 0.f;
    dfr.top = 0.f;

    //printf("Titlebar stats: %f:%f and %f wide\n", m_position.left, m_position.top, m_position.width);

	if(!(m_window_flags & WindowFlags::NoTitlebar))
	{
		this->m_titlebar.setPosition(sf::Vector2f(dfr.left, dfr.top));
		this->m_titlebar.setSize(sf::Vector2f(m_position.width, this->titlebarheight));
		this->m_titlebar.draw(target, scale);
		this->m_titlebar.setPosition(sf::Vector2f(m_position.left, m_position.top));

		this->m_titlebarlabel.setPosition(dfr.left, dfr.top);
		this->m_titlebarlabel.setSize(this->m_titlebar.bounds().width, this->m_titlebar.bounds().height);
		this->m_titlebarlabel.draw(target, scale);
		this->m_titlebarlabel.setPosition(m_position.left, m_position.top);
	}

    this->m_windowframe.setSize(sf::Vector2f(m_position.width, m_position.height));
    this->m_windowframe.setPosition(dfr.left, dfr.top + m_titlebar.bounds().height);
    //this->m_windowframe.setScale(scale, scale);
    target.draw(m_windowframe);
    this->m_windowframe.setPosition(m_position.left, m_position.top + m_titlebar.bounds().height);

	if(!(m_window_flags & WindowFlags::NoResize))
	{
		this->m_resizeknob.setPosition(sf::Vector2f(
										dfr.left + dfr.width - m_resizeknob.bounds().width,
										dfr.top + m_titlebar.bounds().height + m_position.height - m_resizeknob.bounds().height));
		this->m_resizeknob.setSize(sf::Vector2f(titlebarheight, titlebarheight));
		this->m_resizeknob.draw(target, scale);
		this->m_resizeknob.setPosition(sf::Vector2f(
										m_position.left + m_position.width - m_resizeknob.bounds().width,
										m_position.top + m_titlebar.bounds().height + m_position.height - m_resizeknob.bounds().height));
	}
    //Draw Grid and it's children
    if (m_grid != nullptr) {
        m_grid->setPosition(m_windowframe.getGlobalBounds().left,
                            m_windowframe.getGlobalBounds().top);
        m_grid->setGridSize(m_windowframe.getGlobalBounds().width,
                            m_windowframe.getGlobalBounds().height);
        this->m_grid->draw(target, scale, this->titlebarheight * scale);
    }
}

void UIWindow::init()
{
    this->m_titlebar.setFunction(UIComponent::Function::mbDown,
    [=](void* data) {
		SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
			"Titlebar MBDown");
		MouseButtonData* mbd = (MouseButtonData*)data;
		if(mbd->btn == sf::Mouse::Button::Left)
			m_track = 2;
        //sf::Vector2f coords = m_target->mapPixelToCoords(sf::Mouse::getPosition(*m_target), m_target->getDefaultView());

        //m_last_track_pos = coords;
        return 1;
    });

    this->m_titlebar.setFunction(UIComponent::Function::mbUp,
    [=](void* data) {
		MouseButtonData* mbd = (MouseButtonData*)data;
		if(mbd->btn == sf::Mouse::Button::Left)
		{
			m_track = 0;
			m_resize_track = 0;
		}
		else if(mbd->btn == sf::Mouse::Button::Right)
		{
			//Display context menu
		}
        return 1;
    });
	
	this->m_titlebar.setFunction(UIComponent::Function::fillContextMenu,
	[=](void* data) {
		
		std::vector<ActionStruct>* vec = (std::vector<ActionStruct>*)data;
		
		ActionStruct as;
		as.name = "Minimize";
		
		vec->push_back(as);
		
		return 1;
	});

    this->m_resizeknob.setFunction(UIComponent::Function::mbDown,
    [=](void* data) {
        this->m_resize_track = 2;

        return 1;
    });

    this->m_resizeknob.setFunction(UIComponent::Function::mbUp,
    [=](void* data) {
        this->m_resize_track = 0;
        this->m_track = 0;
        return 1;
    });
	
	this->changed = true;
}

}