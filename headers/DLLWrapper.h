#pragma once
#include "stdafx.h"
#include "DLL.h"



namespace SFG
{

class ImportedClassBuilder;
class ImportedClass;

typedef ImportedClassBuilder* (*dllcreatorfunc)();

class DLLWrapper
{
public:
    DLLWrapper();
    ~DLLWrapper();

    enum Flags
    {
        None = 0,				//Default state
        ExecutionDisabled = 1	//No Function is allowed to be executed
    };

    ///<summary>
    ///Loads a library
    ///</summary>
    ///<param name="path">The path pointing to the DLL</param>
    bool loadLibrary(const std::string& path)
    {
        return m_library.load(path.c_str());
    }

    ///<summary>
    ///Returns a builder for a class type.
    ///</summary>
    ///<param name="name">Name of the class to retrieve</param>
    ImportedClassBuilder* getClass(const std::string& name);


private:
    SFG::DLL m_library;
};

///<summary>
///Base class of every instance of a ImportedClass or derivates.
///</summary>
class ImportedClass
{
public:
    ImportedClass(	void* args,
                    const std::string& name,
                    std::map<std::string,
                    std::function<void*(void*)>*>* funcs,
                    std::map<std::string,
                    size_t>* vars,
                    SFG::DLL* dllref);

    ~ImportedClass();


    //void* cast(ImportedClassBuilder* ICB, const std::string& ID);

    ///<summary>
    ///Gets the size of the variable table
    ///</summary>
    ///<param name="cls">The class to consider</param>
    size_t getVartableSize(const std::string& cls = "");

    ///<summary>
    ///Gets the size of the variable table
    ///</summary>
    ///<param name="depth">The depth to search. For example, if the class hierarchy is "cls-bas1-bas2-bas3"
    ///and depth is 2, only cls and bas1 will be considered</param>
    size_t getVartableSize(size_t depth);
private:
    std::string m_name;
    std::map<std::string, std::function<void*(void*)>*>* v_func; //Provides function table
    char* m_data;												 //Provides memory for member variables
    std::map<std::string, size_t>* v_vars;						 //Provides variable table

    SFG::DLL* m_dllref;											//A reference to the dll that provides the class builder
};
///<summary>
///Loads a class declaration from a specific DLL
///</summary>
class ImportedClassBuilder
{
public:
    ImportedClassBuilder(	const std::string& name,
                            std::map<std::string,
                            std::function<void*(void*)>*>* funcs,
                            std::map<std::string, size_t>* vars,
                            std::atomic<int32_t>* dllflags);

    ~ImportedClassBuilder() {

    }

    ImportedClass* createInstance(void* args)
    {
        return new ImportedClass(args, this->m_name, &v_func, &v_vars, this->getDLLRef());
    }

    ///<summary>
    ///Find free index for a name (to append to 'base')
    ///</summary>
    template<typename T> static size_t getFreeIndex(const std::string& base, std::map<std::string, T>* map)
    {
        size_t i = 1;
        for (i = 1; map->find(base + std::to_string(i)) != map->end(); i++);
        return i;
    }

    ///<summary>
    ///Calculates the size of the vartable of a given class
    ///</summary>
    ///<param name="cls">The name of the class to get the vartable size from</param>
    size_t varsum(const std::string& cls);

    ImportedClassBuilder* inherit(ImportedClassBuilder* ICB);

    ///<summary>
    ///Calls function 'funcname' with parameter 'param'
    ///</summary>
    void* call(const std::string& funcname, void* param);

    std::map<std::string, std::function<void*(void*)>*>* functions() {
        return &this->v_func;
    }

    std::map<std::string, size_t>* variables() {
        return &this->v_vars;
    }

    std::string& getName() {
        return this->m_name;
    }

    inline void setDLLRef(SFG::DLL* dll) {
        this->m_dllref = dll;
    }

    inline SFG::DLL* getDLLRef() const {
        return this->m_dllref;
    }

private:
    std::string m_name;											//Defined as "child{<base}*"
    std::map<std::string, std::function<void*(void*)>*> v_func; //Provides function table
    std::map<std::string, size_t> v_vars;						//Contains names and size of variables

    SFG::DLL* m_dllref;											//A reference to the dll that provides the class builder
};

}
