#include "stdafx.h"
#include "GameConsole.h"

namespace SFG 
{
	template<>
	sf::String Var< bool >::toString() const
	{
		std::ostringstream str;
		str << std::boolalpha << get();
		return str.str();
	}
	
	void GameConsole::addChar(sf::Uint32 c)
	{
		changed();
		//printf("Entered '%c' (aka %d)\n", c, c);
		switch(c)
		{
		case sf::Keyboard::Return:
			sendCommand();
			break;
		case '\r':
			sendCommand();
			break;
		case '\b':
			m_input = m_input.substring(0, m_input.getSize() - 1);
			break;
		case sf::Keyboard::BackSpace:
			m_input = m_input.substring(0, m_input.getSize() - 1);
			break;
		default:
			m_input += c;
		}
	}
	
	void GameConsole::print(const sf::String& tr)
	{
		m_output += tr + "\n";
		changed();
	}
	
	sf::String CommandTranslator::translate(const sf::String& cmd) const
	{
		//Commands shall be in the following format:
		//1. For Variables
		//	{set|get} {varname}
		//2. For functions
		//	{run} {funcname} {param}*
		
		size_t i = 0;
		for(i = 0; i < cmd.getSize(); i++)
			if(!isspace(cmd[i])) break;;

		size_t lastpos = 0;
		int op = 0;
		while(i < cmd.getSize())
		{
			if(cmd.substring(i, 3) == "get")
			{
				op = 1;
			}
			else if(cmd.substring(i, 3) == "set")
			{
				op = 2;
			}
			else if(cmd.substring(i, 3) == "run")
			{
				op = 3;
			}
			
			i = i + 4;
			
			for(; i < cmd.getSize(); i++)
				if(!isspace(cmd[i])) break;
			
			lastpos = i;
			
			for(; i < cmd.getSize(); i++)
				if(isspace(cmd[i])) break;
				
				
			sf::String str = cmd.substring(lastpos, i - lastpos);
			
			printf("Text is \"%s\"\n", str.toAnsiString().c_str());
				
			++i;
				
			std::vector<sf::String> params;
				
			while(i < cmd.getSize())
			{
				for(; i < cmd.getSize(); i++)
					if(!isspace(cmd[i])) break;
					
				lastpos = i;
					
				for(; i < cmd.getSize(); i++)
					if(isspace(cmd[i])) break;
					
				sf::String param = cmd.substring(lastpos, i - lastpos);
				
				printf("Param is \"%s\"\n", param.toAnsiString().c_str());
				params.push_back(param);
			}
			
			if(op < 3)	//Var operation
			{
				auto it = m_vars.begin();
				if((it = m_vars.find(str)) != m_vars.end())	
				{
					
					return it->second(op, params.size() > 0 ? params[0] : sf::String());
				}
			}
			else
			{
				//Run operation
				auto it = m_funcs.find(str);
				if(it != m_funcs.end())
				{
					it->second(params);
					return sf::String();
				}
			}
			i++;
		}
		return sf::String();
	}


	
};