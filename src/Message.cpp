#include "stdafx.h"
#include "Message.h"



int MsgObject::dynamicNotify(int msg, const SFG::Pointer<MsgPackage>& pkg)
{
    //return action->call(msg, pkg);
    //return -1;
    for (auto a : m_actions[this])
    {
        a.call(msg, pkg);
    }

    return 0;
}

void MsgObject::removeListener(MsgObject* l)
{
    for (size_t i = 0; i < m_listeners.size(); i++)
    {
        if (m_listeners[i] == l)
        {
            m_listeners.erase(m_listeners.begin() + i);
            return;
        }
    }
    printf("[Warning] Object wasn't found and could not be removed in %s:%d\n", __FILE__, __LINE__);
}

void MsgObject::addListener(MsgObject* l)
{
    for (size_t i = 0; i < m_listeners.size(); i++)
    {
        if (m_listeners[i] == l)
        {
            printf("[Warning] Object was already in the list and was not added in %s:%d\n", __FILE__, __LINE__);
            return;
        }
    }
    m_listeners.push_back(l);
}

void MsgObject::addListener(MsgObject* l, const MsgObjectAction& moa)
{
    for (size_t i = 0; i < m_listeners.size(); i++)
    {
        if (m_listeners[i] == l)
        {
            printf("[Warning] Object was already in the list and was not added in %s:%d\n", __FILE__, __LINE__);
            return;
        }
    }
    m_listeners.push_back(l);
    m_actions[l].push_back(moa);
}

void MsgObject::notifyAll(int msg)
{
    for (auto m : m_listeners)
    {
        m->notify(msg);
    }
}

void MsgObject::dynNotifyAll(int msg, const SFG::Pointer<MsgPackage>& pkg)
{
    for (auto m : m_listeners)
    {
        //m->dynamicNotify(msg, pkg);
        for (auto a : m_actions[m])
        {
            a.call(msg, pkg);
        }
    }
}
