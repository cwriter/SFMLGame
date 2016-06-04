#pragma once
#include "stdafx.h"


///<summary>
///Wraps one Context Option (=Action)
///<summary>
class ContextOption
{
public:
    ContextOption()
    {

    }
    ~ContextOption()
    {
    }

    ///<summary>
    ///Trigger this option
    ///</summary>
    int trigger()
    {

        return 0;
    }

private:
    std::string m_name;
};

class ContextOptionList
{
public:
    ContextOptionList()
    {

    }
    ~ContextOptionList()
    {

    }

    void addOption(SFG::Pointer<ContextOption> opt)
    {
        this->m_option_list.push_back(opt);
    }


private:
    std::vector<SFG::Pointer<ContextOption>> m_option_list;
};
