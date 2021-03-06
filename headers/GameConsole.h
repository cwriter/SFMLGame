#pragma once
#include "stdafx.h"
#include <type_traits>
#include <sstream>

namespace SFG
{
	template<typename T>
	class Var
	{
	public:
		
		template<typename U = T>
		operator typename std::enable_if<std::is_fundamental<U>::value, U >::type () const
		{
			return var;
		}
		
		template<typename U = T>
		operator typename std::enable_if<!std::is_fundamental<U>::value, U&>::type () const
		{
			return var;
		}
		
		template<typename U = T>
		typename std::enable_if<std::is_fundamental<U>::value, U>::type
		get() const
		{
			return var;
		}
		
		template<typename U = T>
		typename std::enable_if<!std::is_fundamental<U>::value, const U&>::type
		get() const
		{
			return var;
		}
		
		template<typename U = T>
		typename std::enable_if<std::is_fundamental<U>::value, void>::type
		set(U v)
		{
			if(onValueChange) onValueChange(v);
			var = v;
		}
		
		template<typename U = T>
		typename std::enable_if<!std::is_fundamental<U>::value, void>::type
		set(const U& v)
		{
			if(onValueChange) onValueChange(v);
			var = v;
		}
		
		template<typename U = T>
		typename std::enable_if<std::is_fundamental<U>::value, U>::type
		operator=(U v)
		{
			set(v);
			return get();
		}
		
		template<typename U = T>
		typename std::enable_if<!std::is_fundamental<U>::value, const U&>::type
		operator=(const U& v)
		{
			set(v);
			return get();
		}
		
		bool operator==(const T& cmp) const
		{
			return var == cmp;
		}
		
		/*bool operator==(T cmp) const
		{
			return var == cmp;
		}*/
		
		sf::String toString() const
		{
			std::ostringstream str;
			str << get();
			return str.str();
		}
		
		std::function<void(const T&)> onValueChange;
		
	private:
		T var;
	};
	
	template<>
	sf::String Var<bool>::toString() const;
	
	
	
	class CommandTranslator
	{
	public:
		sf::String translate(const sf::String& cmd) const;
		
		void setRun(const sf::String& name,
					std::function<void(const std::vector<sf::String>&)> func)
		{
			m_funcs[name] = func;
		}
		
		template<typename T>
		void setVar(const sf::String& name, SFG::Var<T>* var)
		{
			assert(var != nullptr);
			m_vars[name] = [var](int op, const sf::String& val){
				switch(op)
				{
					case 1:
						return var->toString();
						break;
					case 2:
						T v;
						if(std::is_same<bool, T>::value) std::istringstream(val.toAnsiString()) >> std::boolalpha >> v;
						else std::istringstream(val.toAnsiString()) >> v;
						std::cout << "v is " << v << std::endl;
						var->set(v);
						break; 
					default:
						return sf::String();
						break;
				};
				return sf::String();
			};
		}
		
		SFG::Pointer<std::vector<sf::String>> autocomplete(const sf::String& current, size_t* startpoint = nullptr) const
		{
			sf::String searchstr = current;
			
			size_t begin = 0;
			if(searchstr.getSize() > 1)
			{
				begin = searchstr.getSize() -1;
				while(!isspace(searchstr[begin]))
				{
					begin--;
					if(begin == 0) break;
				}
				searchstr = searchstr.substring(begin + 1, current.getSize() - (begin+1));
			}
			
			
			SFG::Pointer<std::vector<sf::String>> ptr(new std::vector<sf::String>());
			
			for(const auto& it : m_vars)
			{
				if(it.first.find(searchstr) != sf::String::InvalidPos || 
					searchstr.getSize() == 0)
				{
					if(startpoint != nullptr)
						*startpoint = searchstr.getSize();
					ptr->push_back(it.first);
				}
			}
			
			return ptr;
		}
		
	private:
		std::map<sf::String, std::function<void(const std::vector<sf::String>&)>> m_funcs;
		std::map<sf::String, std::function<sf::String(int, const sf::String&)>> m_vars;
		
	};
	
	class GameConsole
	{
	public:
		GameConsole()
		{
			m_active = false;
			m_has_changed = true;
			
			m_output = "GameConsole started...\n";
		}
		
		virtual ~GameConsole()
		{
			
		}
		
		int init()
		{
			if(!m_font.loadFromFile("Fonts/arial.ttf"))
			{
				Util::printLog(Util::Error, __FILE__, __LINE__,
					"Failed to load font"
				);
				return -1;
			}
			m_text.setFont(m_font);
			m_text.setCharacterSize(12);
			return 0;
		}
		
		inline void setActive(bool active = true)
		{
			m_active = active;
		}
		
		inline bool isActive() const
		{
			return m_active;
		}
		
		void draw(sf::RenderTarget& t)
		{
			if(!isActive()) return;
			auto tmp = t.getView();
			sf::View adjusted = t.getDefaultView();
			adjusted.setSize(t.getSize().x, t.getSize().y);
			adjusted.setCenter(t.getSize().x / 2.f, t.getSize().y / 2.f);
			t.setView(adjusted);
			t.draw(m_text);
			t.setView(tmp);
		}
		
		void addChar(sf::Uint32 c);
		
		inline void changed()
		{
			m_has_changed = true;
		}
		
		inline bool hasChanged() const
		{
			return m_has_changed;
		}
		
		int update(float dt)
		{
			if(!isActive()) return 1;
			
			//Don't calculate anything else if nothing has changed
			if(!hasChanged()) return 2;
			m_text.setString(m_output + ">" + m_input);
			
			return 0;
		}
		
		const sf::String& getCommand() const
		{
			return m_command;
		}
		void print(const sf::String& tr);
		
		void clearCommand()
		{
			m_command.clear();
		}
		
		const sf::String& getCurrentInput() const
		{
			return m_input;
		}
		void setInput(const sf::String& str)
		{
			m_input = str;
		}
		
		void fire()
		{
			sendCommand();
		}

	protected:
		void sendCommand()
		{
			m_output += m_input + "\n";
			m_command = m_input;
			
			m_input.clear();
		}
	private:
		bool m_active;
		bool m_has_changed;
		
		sf::String m_output;
		sf::String m_input;
		
		sf::String m_command;
		
		sf::Font m_font;
		sf::Text m_text;
	};
	
};