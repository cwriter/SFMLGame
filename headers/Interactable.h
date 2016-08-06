#pragma once
#include "stdafx.h"
#include <SFGUI/Desktop.hpp>
#include <functional>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>


class mouseRequest 
{
public:
	sf::Vector2f pos;
	sf::Mouse::Button button;
};


class delayedActionTask
{
public:
	delayedActionTask(){
		active = true;
	}
	
	~delayedActionTask(){
		deactivate();
	}
	enum TaskType
	{
		mouseRequest = 0,
		takeoverRequest
		
	};
	
	bool isActive() const{
		return active;
	}
	
	void deactivate(){
		if(!active) return;
		active = false;
		switch(m_task_type)
		{
		case mouseRequest:
			delete ((::mouseRequest*)m_request);
			break;
		default:
			assert(false);
			free(m_request);
			break;
		}
	}
	
	TaskType m_task_type;
	
	void* m_request;
	
private:
	std::atomic<bool> active;
	
};


namespace SFG 
{
	class Interactable
	{
	public:
		//virtual ~Interactable();
		
		inline void setClick(const std::function<int(const sf::Vector2f&, const sf::Mouse::Button& button)>& func) {
			m_onclick = func;
		}
		
		inline void setRightClick(const std::function<int(const sf::Vector2f&, const sf::Mouse::Button& button)>& func) {
			m_onrightclick = func;
		}
		
		inline void setHover(const std::function<int(const sf::Vector2f&)>& func) {
			m_onhover = func;
		}
		
		inline int onClick(const sf::Vector2f& pos, const sf::Mouse::Button& button){
			assert(m_onclick);
			return m_onclick(pos, button);
		}
		
		inline int onRightClick(const sf::Vector2f& pos, const sf::Mouse::Button& button){
			assert(m_onrightclick);
			return m_onrightclick(pos, button);
		}
		
		inline int onHover(const sf::Vector2f& pos) {
			assert(m_onhover);
			return m_onhover(pos);
		}
		
		inline void setGuiDesktop(sfg::Desktop* d) {
			m_sfg_desktop = d;
		}
		
		inline sfg::Desktop* desktop() const {
			assert(m_sfg_desktop != nullptr);
			return m_sfg_desktop;
		}
		
		virtual sf::FloatRect getLogicBoundingRect() const {
			
			return sf::FloatRect();
		}
		
		virtual void addDelayedActionTask(const SFG::Pointer<delayedActionTask>& task)
		{
			this->m_requests.push_back(task);
		}
		
		virtual void clearDelayedActionTasks() {
			m_requests.clear();
		}
	protected:
		std::vector<SFG::Pointer<delayedActionTask>> m_requests;
		
	private:
		sfg::Desktop* m_sfg_desktop;
		
		std::function<int(const sf::Vector2f&, const sf::Mouse::Button& button)> m_onclick;
		std::function<int(const sf::Vector2f&, const sf::Mouse::Button& button)> m_onrightclick;
		
		std::function<int(const sf::Vector2f&)> m_onhover;
		
	};
};