#pragma once
#include "stdafx.h"
#include "DLL.h"
#define SCRIPT_STRING_MAXLENGTH 100000

#ifndef _WIN32
#define __FUNCTION__ "not supported"
#endif // _WIN32

typedef char* (*DefTable)(const char*);

class dynVarException : public std::exception {
public:
    dynVarException(const std::string& except) {
        this->m_string = except;
    }



    const char* what() const noexcept override {
        return m_string.c_str();
    }

private:
    std::string m_string;
    int m_type;
};
#ifdef _WIN32
#define CHECK_DYNVAR_TYPE(x,y) if (this->type != x) { throw dynVarException("[Error] " #y " expected type to be " #x "."); return NULL;}
#else
#define CHECK_DYNVAR_TYPE(x,y)
#endif // _WIN32
/*
Hints for Type::CLASS:
If this option is defined, the following rules apply:
+ The GEN_* functions are available
+ The script interpreter has to lookup member functions as follows:
<ClassName>::<functionName>
+ Member functions need to be defined as follows:
<ClassName>::<function>(<Parameters>)
+ Member function calls need to include the "this"-Parameter as the _FIRST_ Argument
data contains the following:
+ 0-Terminated string of class type (this identifies the member lookup)
+ class data (variable length and content)
	<szString>: Class type
	<size_t>: Begin of function indexes
	<undef> : Member Variables


*/
class dynVar {
public:

    enum Type {
        CHAR = 0,
        INT,
        PTR,
        FLOAT,
        STRING,
        CLASS
    };

    dynVar();
    ~dynVar();
    dynVar(dynVar& c);
    dynVar(int i);

    dynVar(const std::string& str);

    dynVar(dynVar&& c);

    dynVar* makeStandalone();

    std::string getType();

    size_t STRFunc_strlen();

    std::string CLSFunc_getType() {
        if (data.bigdata == NULL) {
            throw dynVarException("[Error] Corrupted class can't give type.\n");
            return "class";
        }
        else {
            return std::string((char*)data.bigdata);
        }
    }

    dynVar* STRFunc_append(const std::string& toApp);

    dynVar* INTFunc_append(int i);

    std::string toString() {
        switch (this->type) {
        case Type::STRING:
            return this->STRFunc_toString();
        case Type::INT:
            return this->INTFunc_toString();

        default:
            //Oops
            return "Error";
        }
    }

    std::string INTFunc_toString();

    std::string STRFunc_toString();

    int INTFunc_getInt();

    //Some type-dependent functions
    dynVar* append(dynVar* toAppend);
    dynVar* operator=(const std::string& str);
    dynVar* operator=(int i);
    dynVar* operator=(char* dat);

    int type;

    union {
        void* bigdata;
        int _int;
        float _float;
    } data;


    size_t* m_reference_count;



private:
    void _p_init();

    void GENFunc_private_resize(size_t newsize);

    size_t STRFunc_private_getAppendPoint();

};

class ScriptEngine
{
public:
    ScriptEngine() {
        highest_scope = 0;
        m_jumpback = 0;
    };
    ~ScriptEngine();

    int load(const sf::String& path);

    dynVar* Invoke(const sf::String& name, std::vector<dynVar*>& args);

    int parseClassInstruction(const sf::String& exp, const sf::String& name, int& curly_bracket_count, std::vector<dynVar*>& args, size_t& i, size_t& ranger_1, size_t& ranger_2, dynVar* ret);

    dynVar* parseDeclaration(const sf::String& exp, const sf::String& name, int& curly_bracket_count, std::vector<dynVar*>& args, size_t& i, size_t& ranger_1, size_t& ranger_2, dynVar* ret);

    dynVar* handleExpression(const sf::String& exp, const sf::String& name, int& curly_bracket_count, std::vector<dynVar*>& args, size_t& i, size_t& ranger_1, size_t& ranger_2, dynVar* ret);

    //bool parseCondition(const sf::String& exp, const sf::String& name, int& curly_bracket_count, std::vector<dynVar*>& args, size_t& i, size_t& ranger_1, size_t& ranger_2, dynVar* ret);

    int resolveName(const sf::String& value, const sf::String& function, int scope, dynVar** var);

    void addVariable(const sf::String& function, int SL, const sf::String& varname, dynVar* var) {
        highest_scope = std::max(SL, highest_scope);
        this->m_vartable[function + L"$" + varname + L"#" + std::to_wstring(SL)] = var;
    }

    int cleanScope(int SL);

    bool checkClass(dynVar* dV, const sf::String& classname) {
        if (dV->type != dynVar::Type::CLASS || dV->data.bigdata == NULL) return false;
        if (strcmp((char*)dV->data.bigdata, classname.toAnsiString().c_str()) == 0) {
            //It's ok
            return true;
        }
        else
        {
            //It's not ok
            return false;
        }
        return false;
    }

private:
    std::map<sf::String, sf::String> m_string_litterals;
    std::map<sf::String, dynVar*> m_vartable;
    std::map<sf::String, size_t> m_jumppoints;

    int highest_scope;
    size_t m_jumpback;
    sf::String m_datastring;
    std::map<sf::String, size_t> m_function_starts;
};

class ExtensionInterface
{
public:
    ExtensionInterface();
    ~ExtensionInterface();

    int load(const sf::String& path) {
        //Change the path to the correct value...
        //m_hDLL = LoadLibraryW(std::wstring(path.toWideString() + L"/natcode.dll").c_str());
        bool ret = m_hDLL.load(sf::String(path + "natcode.dll").toAnsiString().c_str());
        if (ret == false) {
            printf("Error in %s:%d: Failed to load DLL %s\n", __FILE__, __LINE__, path.toAnsiString().c_str());
            return -1;
        }
        //Load definition table (this is just a function actually).
        m_defTable = (DefTable)m_hDLL.resolve("DefinitionTable");
        if (m_defTable == NULL) {
            printf("Error in %s:%d: Failed to load definition table\n", __FILE__, __LINE__);
            return -2;
        }
        //Native code definitions are loaded, now load the scriptcode


        return 0;
    }
    void unload() {
        m_hDLL.close();
    }


    ///<summary>
    ///This function invokes the DLL's function table with the according call.<br/>
    ///Please note that errors, warnings and infos need to be handled in the DLL function.
    ///</summary>
    ///<param name="call">
    ///The call the DLL will handle. You are allowed to use every format you can think of execpt <br/>
    ///formats that incorporate '\0'-bytes and / or text that uses characters outside the ANSI definition.
    ///</param>
    dynVar Invoke(const std::string& call) {
        dynVar ret;

        ret = m_defTable(call.c_str());

        return ret;
    }

private:

    SFG::DLL m_hDLL;

    DefTable m_defTable;
};

