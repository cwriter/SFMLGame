#pragma once
#include "stdafx.h"

namespace SFG
{
	
class InputAction
{
public:
	
	virtual ~InputAction(){}
	
	inline void setKey(const sf::Keyboard::Key& k) { m_key = k; }
	
	inline const sf::Keyboard::Key& getKey() const { return m_key; }
	
	inline void setFunc(const std::function<void(double)>& func) { this->m_func = func; }
	
	inline virtual void update(double dt) { if(m_pressed) m_func(dt); }
	
	inline virtual void handle(bool pressed) { m_pressed = pressed; }
	
protected:
	bool m_pressed;
	std::function<void(double)> m_func;
	
private:
	sf::Keyboard::Key m_key;
	
};

class StateInputAction
	: public InputAction
{
public:
	StateInputAction(const sf::Keyboard::Key& k, const std::function<void(double)>& func)
	{
		setKey(k);
		setFunc(func);
	}
private:
	
};

class EdgeInputAction
	: public InputAction
{
public:
	EdgeInputAction(const sf::Keyboard::Key& k, bool posedge, const std::function<void(double)>& func)
	{
		setKey(k);
		setFunc(func);
		setType(posedge);
	}
	
	inline void setType(bool posedge) { m_posedge = posedge; }
	
	inline virtual void update(double dt) override { if(m_pressed) { m_func(dt); m_pressed = false; } }
	
	inline virtual void handle(bool pressed) override { if(m_posedge == pressed) m_pressed = true; }
	
private:
	bool m_posedge;
};
	
class InputActionHandler
{
public:
	void update(double dt)
	{
		for(size_t i = 0; i < m_actions.size(); i++)
		{
			m_actions[i]->update(dt);
		}
	}
	
	void handle(const sf::Keyboard::Key& key, bool pressed)
	{
		for(size_t i = 0; i < m_actions.size(); i++)
		{
			if(m_actions[i]->getKey() == key)
				m_actions[i]->handle(pressed);
		}
	}
	
	void add(const SFG::Pointer<InputAction>& ia)
	{
		m_actions.push_back(ia);
	}
private:
	std::vector<SFG::Pointer<InputAction>> m_actions;
};
	
	
class KeyMapper
{
public:
    KeyMapper();
    ~KeyMapper();

    ///<summary>
    ///Maps key inputs to logical actions. This just converts the saved numbers to other numbers.
    ///</summary>
    ///<param name="code">The code to translate</param>
    ///<param name="ignoreUnknown">If set, keys that could not be resolved will be returned as Unknown and thus will have no effect (default). If set to false,
    ///the input code will be returned if it could not be resolved</param>
    ///<returns>The converted value if a conversion is available, sf::Keyboard::Key::Unknown if 'ignoreUnknown' is true (default),
    ///the input code otherwise
    ///</returns>
    sf::Keyboard::Key keyMapper(sf::Keyboard::Key code, bool ignoreUnknown = true) const;

private:
    std::map<sf::Keyboard::Key, sf::Keyboard::Key> m_keyMap; //Provides a keymap
};

};
