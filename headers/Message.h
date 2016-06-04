#pragma once
#include "stdafx.h"

///<summary>
///Abstract base class for any action parameter wrapping
///</summary>
class MsgPackage
{
public:
    void* getValue(size_t index)
    {
        assert(index < m_data.size());
        return m_data[index];
    }
    void addValue(void* val)
    {
        m_data.push_back(val);
    }
private:
    std::vector<void*> m_data;
};

class MsgObjectAction
{
public:
    MsgObjectAction(std::function<int(int, const SFG::Pointer<MsgPackage>&)> func)
        : function(func)
    {

    }
    int call(int ID, const SFG::Pointer<MsgPackage>& pkg)
    {
        return function(ID, pkg);
    }

    std::function<int(int, const SFG::Pointer<MsgPackage>&)> function;
private:

};


class MsgObject
{
public:
    virtual ~MsgObject()
    {

    }

    ///<summary>
    ///Same as notify, but extensible without recompiling the engine.
    ///</summary>
    virtual int dynamicNotify(int msg, const SFG::Pointer<MsgPackage>& pkg);

    virtual int notify(int msg) {
        printf("[Warning] MsgObject::notify called, but this function hasn't been overridden by the child class: %s:%d\n", __FILE__, __LINE__);
        std::string o;
        SFG::Util::getStackTrace(o);
        puts(o.c_str());
#ifdef _WIN32
        //DebugBreak();
#endif
        return -1;
    }

    template <typename T> int notify(int msg, T* sender) {
        printf("[Warning] MsgObject::notify(int, T*) called, but this function hasn't been overridden by the child class: %s:%d\n", __FILE__, __LINE__);
        return -1;
    }

    template <typename T, typename S> int notify(int msg, T* sender, S* Object) {
        printf("[Warning] MsgObject::notify(int, T*, S*) called, but this function hasn't been overridden by the child class: %s:%d\n", __FILE__, __LINE__);
        return -1;
    }

    virtual void removeListener(MsgObject* l);

    virtual void addListener(MsgObject* l);

    ///<summary>
    ///Adds a listener for a specific MsgObjectAction.
    ///</summary>
    ///<param name="l">The message object that shall listen to this object</param>
    ///<param name="moa">The MsgObjectAction to link with that an event</param>
    virtual void addListener(MsgObject* l, const MsgObjectAction& moa);

    virtual void notifyAll(int msg);

    virtual void dynNotifyAll(int msg, const SFG::Pointer<MsgPackage>& pkg);

    template<typename T> void notifyAll(int msg, T* sender) {
        for (auto m : m_listeners)
        {
            m->notify(msg, sender);
        }
    }

    template<typename T, typename S> void notifyAll(int msg, T* sender, S* object) {
        for (auto m : m_listeners)
        {
            m->notify(msg, sender, object);
        }
    }

    std::vector<MsgObject*> m_listeners;
    std::map<MsgObject*, std::vector<MsgObjectAction>> m_actions;
};
