#include "stdafx.h"
#include "DLLWrapper.h"

namespace SFG
{

	DLLWrapper::DLLWrapper()
	{
	}

	DLLWrapper::~DLLWrapper()
	{
	}

	ImportedClassBuilder* DLLWrapper::getClass(const std::string& name)
	{
		//Resolve to "Class_<name>"

		dllcreatorfunc d = (dllcreatorfunc)m_library.resolve(std::string("Class_" + name).c_str());
		SFG::ImportedClassBuilder* ret = d();
		ret->setDLLRef(&m_library);

		return ret;
	}
	
	ImportedClassBuilder::ImportedClassBuilder(const std::string& name, std::map<std::string, std::function<void*(void*)>*>* funcs, std::map<std::string, size_t>* vars, std::atomic<int32_t>* dllflags)
	{
		this->m_name = name;
		this->v_func = *funcs;

		std::vector<std::string> to_erase;
		//Clarify variable names
		this->v_vars = *vars;
		for (auto f : v_vars)
		{
			if (f.first.find("<") == f.first.npos)
			{
				//Missing specifier, add accordingly
				v_vars[name + "<" + f.first] = f.second;
				to_erase.push_back(f.first);
			}
		}
		for (auto s : to_erase)	v_vars.erase(s);
		to_erase.clear();
	}

	ImportedClassBuilder* ImportedClassBuilder::inherit(ImportedClassBuilder* ICB)
	{
		//What to do? -> Copy the contents of ICB to the end of the old class.

		ImportedClassBuilder* new_ICB = new ImportedClassBuilder(this->m_name + "<" + ICB->getName(), functions(), variables(), this->getDLLRef()->getDLLFlags()); 
		//new name: base1<base2 (as base2 got pushed upon base1)

		for (auto f : *ICB->functions())
		{
			//Automatically set. If already present, then override, if not, add.
			//!!! EXCEPTIONS: BIG THREE !!! 
			if (f.first == "cnstrct" || f.first == "destrct")
			{
				//Warning: Constructors are special
				new_ICB->v_func[f.first + std::to_string(getFreeIndex(f.first, &this->v_func))] = f.second;
			}
			else
			{
				new_ICB->v_func[f.first] = f.second;
			}
		}
		for (auto v : *ICB->variables())
		{
			(*new_ICB->variables())[v.first + "<" + ICB->getName()] = v.second; //Don't override old variables, but add a suitable prefix
		}
		
		return 0;
	}

	void* ImportedClassBuilder::call(const std::string& funcname, void* param)
	{
		std::atomic<int32_t>* flags = m_dllref->getDLLFlags();

		//If execution is disabled, pause the call until the disable bit has been removed
		while (*flags & DLLWrapper::Flags::ExecutionDisabled)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}

		//We can't handle rearranged memory layouts or added or removed variable entities when hotpatching


		return (*v_func[funcname])(param);
	}

	size_t ImportedClassBuilder::varsum(const std::string& cls)
	{
		//WARNING: UNTESTED
		size_t ret = 0;

		//Parse name
		size_t pos = this->m_name.find('<');
		while ((pos = this->m_name.find('<', pos)) != m_name.npos)
		{
			size_t endpos = this->m_name.find('<', pos);
			std::string tmp(m_name.begin() + pos + 1, m_name.begin() + ((endpos == m_name.npos) ? m_name.length() : endpos));

			for (auto v : *this->variables())
			{
				if (v.first.find(tmp) == 0)
				{
					//Variable name begins with class name, so add the size
					ret += v.second;
				}
			}

			if (tmp == cls)
			{
				break; //We have seen enough
			}
		}




		return ret;
	}

	ImportedClass::ImportedClass(void* args, const std::string& name, std::map<std::string, std::function<void*(void*)>*>* funcs, std::map<std::string, size_t>* vars, SFG::DLL* dllref)
	{
		//Set the name
		this->m_name = name;
		//Set the (v)irtual (f)unction (p)ointers
		this->v_func = funcs;

		//Set the variables (allocate memory)
		if (this->getVartableSize() > 0)
		{
			this->m_data = (char*)calloc(this->getVartableSize(), 1); //Allocate and set to 0
			if (m_data == nullptr) {
				printf("[Critical] Failed to allocate memory (%Iu bytes) for class in %s:%d\n", this->getVartableSize(), __FILE__, __LINE__);
			}
			//Done
		}
		//execute all higher-level constructors first
		for (size_t i = ImportedClassBuilder::getFreeIndex("cnstrct", v_func); i > 0; i--)
		{
			(*(*this->v_func)["cnstrct" + std::to_string(i)])(this->m_data + this->getVartableSize(i)); //Higher constructor, pass 'this'
		}
		//Call the very specific constructor. This returns void*, but that should be NULL
		(*(*this->v_func)["cnstrct"])(this->m_data); //Base constructor
		
		m_dllref = dllref;
	}

	ImportedClass::~ImportedClass() 
	{
		//Call the very specific destructor
		(*(*this->v_func)["destrct"])(m_data);
		//Now execute all higher-level constructors (highest to lowest to let the higher class clean itself up before handing the data down.
		for (size_t i = ImportedClassBuilder::getFreeIndex("destrct", v_func); i > 0; i--)
		{
			(*(*this->v_func)["destrct" + std::to_string(i)])(this->m_data + this->getVartableSize(i)); //Higher destructor, we give it its memory to cleanup
		}

		//Then, free our memory (we trust the destructor to have called the destructor of the objects in m_data)
		if (m_data != nullptr) free(m_data);
	}

	size_t ImportedClass::getVartableSize(const std::string& cls)
	{
		assert(this->v_vars != nullptr); //Should not happen
										 //WARNING: UNTESTED
		size_t ret = 0;

		//Parse name
		size_t pos = this->m_name.find('<');
		while ((pos = this->m_name.find('<', pos)) != m_name.npos)
		{
			size_t endpos = this->m_name.find('<', pos);
			std::string tmp(m_name.begin() + pos + 1, m_name.begin() + ((endpos == m_name.npos) ? m_name.length() : endpos));

			for (auto v : *this->v_vars)
			{
				if (v.first.find(tmp) == 0)
				{
					//Variable name begins with class name, so add the size
					ret += v.second;
				}
			}

			if (tmp == cls)
			{
				break; //We have seen enough
			}
		}
		return ret;
	}

	size_t ImportedClass::getVartableSize(size_t depth)
	{
		assert(this->v_vars != nullptr); //Should not happen
										 //WARNING: UNTESTED
		size_t ret = 0;
		size_t count = 0;
		//Parse name
		size_t pos = this->m_name.find('<');
		while ((pos = this->m_name.find('<', pos)) != m_name.npos)
		{
			size_t endpos = this->m_name.find('<', pos);
			std::string tmp(m_name.begin() + pos + 1, m_name.begin() + ((endpos == m_name.npos) ? m_name.length() : endpos));

			for (auto v : *this->v_vars)
			{
				if (v.first.find(tmp) == 0)
				{
					//Variable name begins with class name, so add the size
					ret += v.second;
				}
			}

			if (depth == count)
			{
				break; //We have seen enough
			}
			count++;
		}
		return ret;
	}

	/*void* ImportedClass::cast(ImportedClassBuilder* ICB, const std::string& ID)
	{
		//Call function by ID as defined in ICB or get variable. 
		
		return nullptr;
	}*/
};