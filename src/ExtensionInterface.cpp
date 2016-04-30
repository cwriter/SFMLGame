#include "stdafx.h"
#include "ExtensionInterface.h"

dynVar* dynVar::append(dynVar* toAppend) {
	//Append can be the following:
	//string.append(string)
	//string.append(int)
	//int.append(int) (adds a digit to the number, for example int(1).append(2) is int(12) and not 3)
	//Other appends are not defined (yet)
	if (this->type == Type::STRING) {
		if (toAppend->type == Type::STRING) {
			//Append string to string
			std::string t;
			try {
				t = toAppend->STRFunc_toString();
			}
			catch (dynVarException& e) {
				puts(e.what());
			}
			STRFunc_append(t);
		}
		else if (toAppend->type == Type::INT) {
			std::string t;
			try {
				t = toAppend->INTFunc_toString();
			}
			catch (dynVarException& e) {
				puts(e.what());
			}
			STRFunc_append(t);
		}
	}
	else if (this->type == Type::INT) {
		if (toAppend->type == Type::INT) {
			int t = 0;
			try {
				t = toAppend->INTFunc_getInt();
			}
			catch (dynVarException& e) {
				puts(e.what());
			}

			INTFunc_append(t);
		}
	}
	return this;
}

dynVar* ScriptEngine::parseDeclaration(const sf::String& exp, const sf::String& name, int& curly_bracket_count, std::vector<dynVar*>& args, size_t& i, size_t& ranger_1, size_t& ranger_2, dynVar* ret)
{
	//Declaration
	//sf::String type(exp.begin(), exp.begin() + exp.find(L' '));
	sf::String type = exp.substring(0, exp.find(' '));
	dynVar* var = new dynVar();
	if (var == NULL) {
		//Allocation error
		throw;	//#TODO: add better code here
	}
	if (type == L"string") {
		var->type = dynVar::Type::STRING;
		var->operator=(std::string(""));
	}
	else if (type == L"int") {
		var->type = dynVar::Type::INT;
	}
	//Insert other types

	else if (type == L"return") {
		//Not really a type. But it's fun anyways :)
		//We'll only handle variables from the start
		size_t sign_os = std::min(exp.find("="), exp.find(";"));
		if (sign_os == exp.InvalidPos) {
			//This is not good...
		}
		//sf::String varname(exp.begin() + exp.find(L' ') + 1, exp.begin() + sign_os);	//#TODO: Test this...
		sf::String varname = exp.substring(exp.find(L' ') + 1, sign_os - (exp.find(L' ') + 1));	//#TODO: Test this...

		if (resolveName(varname, name, curly_bracket_count, &ret) != 0) {
			printf("[Warning] Return expression could not be resolved.\n");
		}
		//Delete the allocated var
		delete var;
		return ret; //Return
	}

	size_t sign_os = std::min(exp.find("="), exp.find(";"));
	if (sign_os == exp.InvalidPos) {
		//This is not good...
	}
	sf::String varname = exp.substring(exp.find(L' ') + 1, sign_os - 1 - exp.find(L' '));				//#TODO: Test this...
																					//Remove spaces (if not done yet)
	SFML_ERASE_WHITESPACE(varname);
	addVariable(name, curly_bracket_count, varname, var);
	if (exp[sign_os] == L'=') {
		//Data inc
		//sf::String value(exp.begin() + sign_os + 1, exp.begin() + exp.find(L';'));
		sf::String value = exp.substring(sign_os + 1, exp.find(";") - (sign_os + 1 ));

		//Convert value string to desired type
		if (var->type == dynVar::Type::STRING) {
			dynVar* tmp = NULL;
			if (resolveName(value, name, curly_bracket_count, &tmp) != 0) {
				//Error
				printf("[Warning] Identifier \"%s\" could not be resolved. Variable %s has been set to %s.\n", value.toAnsiString().c_str(), varname.toAnsiString().c_str(), value.toAnsiString().c_str());
			}
			(*var) = tmp->toString(); //Should copy
			var->makeStandalone(); //Does deepcopy
		}
	}
	else {
		//That was all.
	}
	ranger_1 = i + 1;
	ranger_2 = ranger_1 + 1;

	return NULL;
}

int ScriptEngine::parseClassInstruction(const sf::String& exp, const sf::String& name, int& curly_bracket_count, std::vector<dynVar*>& args, size_t& i, size_t& ranger_1, size_t& ranger_2, dynVar* ret)
{
	//Class and function
	size_t instance_i = exp.find(L'.');
	if (instance_i == exp.InvalidPos) {
		//Error: not found
	}
	//sf::String instance(exp.begin(), exp.begin() + instance_i);
	sf::String instance = exp.substring(0, instance_i);

	size_t funcname_i = exp.find(L'(', instance_i);
	if (funcname_i == exp.InvalidPos) {
		//Error: not found
	}

	//sf::String funcname(exp.begin() + instance_i + 1, exp.begin() + funcname_i);
	sf::String funcname = exp.substring(instance_i + 1, funcname_i);

	//Got function name and instance
	//First, lookup instance
	//#TODO
	dynVar* dV = NULL;
	if (resolveName(instance, name, curly_bracket_count, &dV) != 0) {
		//Error
	}
	//Get the type
	std::string type = dV->getType();
	//(Maybe lookup in class register/database)

	//Get the args
	size_t del1 = funcname_i + 1;
	size_t del2 = del1 + 1;

	std::vector<dynVar*> cmf_args;
	cmf_args.push_back(dV); //Always add this-pointer

	while ((del2 = std::min(exp.find(",", del1), exp.find(")", del1))) != exp.InvalidPos)
	{
		dynVar* dref = NULL;
		//Resolve the name
		//sf::String varname(exp.begin() + del1, exp.begin() + del2);
		sf::String varname = exp.substring(del1, del2 - del1);
		//Remove spaces
		SFML_ERASE_WHITESPACE(varname);
		//Resolve
		if (resolveName(varname, name, curly_bracket_count, &dref) != 0) {
			//Error
		}
		cmf_args.push_back(dref); //Add element to argument list
		del1 = del2 + 1;
	}
	if (dV->type == dynVar::Type::STRING) {
		if (funcname == L"append") {
			if (cmf_args.size() >= 2)
				dV->STRFunc_append(cmf_args[1]->toString());
			else {
				//Error: Not enough args
			}
		}
	}
	else if (dV->type == dynVar::Type::CLASS) {
		//For now, we look it up in the default function table and execute from there
		this->Invoke(type + "::" + funcname, cmf_args);
	}

	i = m_datastring.find(L';', i);
	ranger_1 = i + 1;
	ranger_2 = ranger_1 + 1;
	return 0;
}

dynVar* ScriptEngine::handleExpression(const sf::String& exp, const sf::String& name, int& curly_bracket_count, std::vector<dynVar*>& args, size_t& i, size_t& ranger_1, size_t& ranger_2, dynVar* ret)
{

	size_t d1 = exp.find(L' ');
	size_t d2 = exp.find(L'.');
	size_t d3 = exp.find(L'(');
	size_t d4 = exp.find(L'[');

	size_t dtype = std::min({ d1, d2, d3, d4 });

	if (dtype == d1) {
		//Declaration
		dynVar* tmp = parseDeclaration(exp, name, curly_bracket_count, args, i, ranger_1, ranger_2, ret);
		if (tmp != NULL) return tmp;
	}
	else if (dtype == d2) {
		//Class and function
		parseClassInstruction(exp, name, curly_bracket_count, args, i, ranger_1, ranger_2, ret);
	}
	else if (dtype == d3) {
		//Function (or control instance)
		//sf::String func(exp.begin(), exp.begin() + d3);
		sf::String func = exp.substring(0, d3);
		//First, identify control "functions"
		if (func == L"for") {
			size_t first = exp.find(L';', d3);
			if (first == exp.InvalidPos) {
				//Print error
				return ret; //Need to set ret
			}
			//Init
			//sf::String initexp(exp.begin() + d3 + 1, exp.begin() + first);
			sf::String initexp = exp.substring(d3 + 1, first - (d3 + 1));
			dynVar* tmp = parseDeclaration(initexp, name, curly_bracket_count, args, i, ranger_1, ranger_2, ret);
			if (tmp != NULL) return tmp;
			//#TODO: condition and every loop operation
			size_t second = exp.find(L';', first);
			if (second == exp.InvalidPos) {
				//Print error
				return ret; //Need to set ret
			}
			//sf::String condition(exp.begin() + first + 1, exp.begin() + second);
			sf::String condition = exp.substring(first + 1, second - (first + 1));
			//Loop is starting here

		}
		else if (func == L"goto") {
			size_t first = exp.find(L')', d3);
			if (first == exp.InvalidPos) {
				//Error
			}
			//sf::String jp(exp.begin() + d3 + 1, exp.begin() + first);
			sf::String jp = exp.substring(d3 + 1,first - (d3 + 1));
			auto it = m_jumppoints.find(jp);
			if(it == m_jumppoints.end()){
				//Error
			}
			else {
				i = it->second;
			}
		}
		else if (func == L"if") {
			size_t first = exp.find(L')', d3);
			if (first == exp.InvalidPos) {
				//Error
			}
			//sf::String condition(exp.begin() + d3, exp.begin() + first);
			sf::String condition = exp.substring(d3 + 1, first - (d3 + 1));
		}
		else if (func == L"while") {

		}
		else if (func == L"else") {

		}
		else if (func == L"") {

		}
	}
	else if (dtype == d4) {
		//Array index (not yet implemented)
	}

	return ret;
}

dynVar* ScriptEngine::Invoke(const sf::String& name, std::vector<dynVar*>& args)
{
	//#TODO: Add a cleanup functiom to remove local vars after this function returns
	dynVar* ret = NULL;

	//Find function first
	auto it = m_function_starts.find(name);
	if (it == m_function_starts.end())
	{
		//failed to find
		printf("[Error] Failed to find function %s in function table in %s:%d\n", name.toAnsiString().c_str(), __FILE__, __LINE__);
		return ret;
	}
	//Check if it is a class member function
	size_t class_identifier = name.find(L"::");
	if (class_identifier != name.InvalidPos) {
		//Congratulations, it's a class!
		//It needs to have at least 1 argument
		if (args.size() < 1) {
			//Error
			printf("[Error] Class member function (%s) was called, but no acceptable this-pointer was passed.\n", name.toAnsiString().c_str());
			return ret;
		}
		//sf::String classname(name.begin(), name.begin() + class_identifier);
		sf::String classname = name.substring(0, class_identifier);
		//Check if first arg is the "this"-pointer
		if (!checkClass(args[0], classname)) {
			printf("[Error] Class member function (%s) was called, but no acceptable this-pointer was passed. (first argument is of type \"%s\" instead of \"%s\")\n", name.toAnsiString().c_str(), classname.toAnsiString().c_str(), args[0]->getType().c_str());
			return ret;
		}
	}

	//std::map<sf::String, dynVar*> tmp_param_types;
	size_t begin = it->second;

	size_t last = begin;
	for (last; last < m_datastring.getSize(); last++) {
		if (m_datastring[last] == L'(') {
			last++;
			break;
		}
	}

	//Parse the arguments and fail (if necessary)
	size_t current_arg = 0;
	size_t function_body_begin = 0;
	for (size_t i = begin; i < m_datastring.getSize(); i++)
	{
		if (m_datastring[i] == L')')
		{
			//End of argument list
			if (current_arg == args.size())
			{
				//Everything is perfect
				for (function_body_begin = i; function_body_begin < m_datastring.getSize(); function_body_begin++) {
					if (m_datastring[function_body_begin] == L'{') {
						break;
					}
				}
				function_body_begin++;
				break;
			}
			else if (current_arg < args.size())
			{
				printf("[Error] Given argument count does exceed expected count (%zd passed, %zd expected) in function %s\n", args.size(), current_arg, name.toAnsiString().c_str());
				return ret;
			}
		}


		if (current_arg >= args.size()) {
			printf("[Error] Given argument count not sufficient (%zd passed, %zd or more expected) in function %s\n", args.size(), current_arg + 1, name.toAnsiString().c_str());
			break;
		}

		if (m_datastring[i] == L',')
		{
			last = i + 1;
			continue;
		}
		if (m_datastring[i] == L' ')
		{
			//sf::String str_type(m_datastring.begin() + last, m_datastring.begin() + i);
			sf::String str_type = m_datastring.substring(last, i - last);
			if (str_type == L"int")
			{
				if (args[current_arg]->type != dynVar::Type::INT) {
					printf("[Error] Wrong argument type in function %s: %s expected, type with ID %d given\n", name.toAnsiString().c_str(), str_type.toAnsiString().c_str(), args[current_arg]->type);
					return ret;
				}
			}
			else if (str_type == L"string")
			{
				if (args[current_arg]->type != dynVar::Type::STRING) {
					printf("[Error] Wrong argument type in function %s: %s expected, type with ID %d given\n", name.toAnsiString().c_str(), str_type.toAnsiString().c_str(), args[current_arg]->type);
					return ret;
				}
			}
			else if (str_type == L"float")
			{
				if (args[current_arg]->type != dynVar::Type::FLOAT) {
					printf("[Error] Wrong argument type in function %s: %s expected, type with ID %d given\n", name.toAnsiString().c_str(), str_type.toAnsiString().c_str(), args[current_arg]->type);
					return ret;
				}
			}
			else
			{
				printf("[Error] Undefined argument type in function %s: %s is not defined\n", name.toAnsiString().c_str(), str_type.toAnsiString().c_str());
				return ret;
			}
			//Get the name and add it
			last = i + 1;
			for (i++; i < m_datastring.getSize(); i++)
			{
				if (m_datastring[i] == L',' || m_datastring[i] == L')' || m_datastring[i] == L' ')
				{
					//sf::String argname(m_datastring.begin() + last, m_datastring.begin() + i);
					sf::String argname = m_datastring.substring(last, i - last);
					//m_vartable[argname] = args[current_arg];
					addVariable(name, 0, argname, args[current_arg]);
					i--;
					break;
				}
			}
			current_arg++;
		}

	}

	//Now execute the function itself
	//tmp_param_type is the variable map
	//Definition:	<text>; is an expression.
	//				Most declarations are "<Type> <name>;"
	//				Most functions calls are "<func>(<parameters>);
	//				Most member function calls are "<var>.<func>(<parameters>);
	//				Special: for-loop.
	int curly_bracket_count = 1;
	size_t ranger_1 = function_body_begin;
	size_t ranger_2 = function_body_begin;
	for (size_t i = function_body_begin; i < m_datastring.getSize(); i++) {
		if (m_datastring[i] == L'{') {
			curly_bracket_count++;
			//We don't really handle scopes atm
			ranger_1 = i + 1;
			ranger_2 = ranger_1 + 1;
		}
		else if (m_datastring[i] == L'}') {
			curly_bracket_count--;

			//Clean scope
			cleanScope(curly_bracket_count);
			ranger_1 = i + 1;
			ranger_2 = ranger_1 + 1;
			if (curly_bracket_count == 0) {
				//End of function reached, we need to return
				printf("[Warning] End of script function \"%s\" reached, but nothing was returned.\n", name.toAnsiString().c_str());
				return ret;
			}
		}
		else if (m_datastring[i] == ';') {
			//End of expression
			//sf::String exp(m_datastring.begin() + ranger_1, m_datastring.begin() + ranger_2 + 1);
			sf::String exp = m_datastring.substring(ranger_1, ranger_2 + 1 - ranger_1);

			dynVar* tret = NULL;
			tret = this->handleExpression(exp, name, curly_bracket_count, args, i, ranger_1, ranger_2, ret);
			if (tret != NULL) return tret;
		}
		else {
			ranger_2++;
		}
	}

	return ret;
}

ScriptEngine::~ScriptEngine() {
	for (auto it = m_vartable.begin(); it != m_vartable.end(); it++) {
		delete it->second;
	}
}

int ScriptEngine::resolveName(const sf::String& value, const sf::String& function, int scope, dynVar** var) {
	//Check for string litteral
	if (this->m_string_litterals.find(value) != m_string_litterals.end()) {
		*var = new dynVar(); //Need to delete...
		this->addVariable(function, scope, L"const_tmp", *var);
		(*var)->operator=(this->m_string_litterals[value]);
		return 0;
	}
	else {
		//Find all variables of that name in this function
		for (int i = this->highest_scope; i >= 0; i--) {
			sf::String searchstr = function + L"$" + value + L"#" + std::to_wstring(i);
			//Check if it is a copy instruction from another variable
			if (m_vartable.find(searchstr) != m_vartable.end()) {
				//Found it
				//var->operator=(m_vartable[searchstr]->toString()); //Why is that?
				(*var) = (m_vartable[searchstr]);
				return 0;
			}
		}
		//Well, this is becoming awkward. Let's just throw a warning and take value as the data;
		*var = new dynVar(); //Need to delete...
		this->addVariable(function,scope, value,*var); //Manage
		(*var)->operator=(value);
		return -1;
	}
	return -2;
}

dynVar::dynVar(dynVar& c) {
	//This does NOT create a deep copy (yet)
	this->m_reference_count = c.m_reference_count;
	(*m_reference_count)++;

	this->data = c.data;
	this->type = c.type;
}

dynVar::dynVar(dynVar&& c) {
	//Set the reference count
	this->m_reference_count = c.m_reference_count;
	//Do NOT increase reference count, but remove data from c
	if (*c.m_reference_count == 0) c.m_reference_count = NULL;
	this->data = c.data;
	c.data.bigdata = NULL;
	this->type = c.type;
}

dynVar::dynVar(int i) {
	_p_init();
	operator=(i);
}

dynVar::dynVar() {
	_p_init();
}

std::string dynVar::INTFunc_toString() {
	CHECK_DYNVAR_TYPE(Type::INT, __FUNCTION__);
	return std::to_string(data._int);
}

int dynVar::INTFunc_getInt() {
	CHECK_DYNVAR_TYPE(Type::INT, __FUNCTION__);
	return data._int;
}

std::string dynVar::STRFunc_toString() {
	CHECK_DYNVAR_TYPE(Type::STRING, __FUNCTION__);
	return std::string((char*)this->data.bigdata);
}

dynVar::dynVar(const std::string& str) {
	_p_init();
	operator=(str);
}

dynVar::~dynVar() {
	if (m_reference_count != NULL) {
		(*m_reference_count)--;

		if (*m_reference_count == 0) {
			delete m_reference_count;

			if (data.bigdata != NULL && (type == Type::STRING || type == Type::CLASS)) {
				free(data.bigdata);//Delete the memory allocated by the DLL
			}
		}
	}
#ifdef _DEBUG
	else {
		assert(data.bigdata == NULL);
	}
#endif
}

dynVar* dynVar::makeStandalone() {
	if (this->m_reference_count == NULL || (this->data.bigdata == NULL && (this->type == Type::STRING || this->type == Type::CLASS))) {
		throw dynVarException("[Error] Trying to make uninitialized variable standalone.\n");
		return this;
	}

	if (*this->m_reference_count == 1) {
		//It's already standalone
		return this;
	}
	//decrease m_reference_count, we're gone!
	(*m_reference_count)--;
	m_reference_count = new size_t;
	if (m_reference_count == NULL) {
		//Another allocation error...
		throw dynVarException("[Error] Failed to allocate memory in " __FUNCTION__ ".\n");
		return this;
	}
	*m_reference_count = 1;
	if (this->type == Type::STRING) {
		//Relocate the string
		size_t stringsize = strlen((char*)this->data.bigdata) + 1;
		char* tmp = (char*)malloc(stringsize);
		if (tmp == NULL) {
			//Allocation error yet again
			throw dynVarException("[Error] Failed to allocate memory in " __FUNCTION__ ".\n");
			return this;
		}
#ifdef _WIN32
		strcpy_s(tmp, stringsize, (char*)this->data.bigdata); //Copy to tmp
#else
		strcpy(tmp, (char*)this->data.bigdata); //Copy to tmp
#endif
		this->data.bigdata = tmp; //Assign
	}
	else if (this->type == Type::INT) {
		//No allocation needed
	}
	return this;
}

std::string dynVar::getType() {
	if (this->type == Type::STRING) {
		return "string";
	}
	else if (this->type == Type::INT) {
		return "int";
	}
	else if (this->type == Type::FLOAT) {
		return "float";
	}
	else if (this->type == Type::CLASS) {
		return CLSFunc_getType();
	}
	else {
		return "undefined";
	}
}

dynVar* dynVar::STRFunc_append(const std::string& toApp) {
	size_t end = 0;
	try {
		end = STRFunc_private_getAppendPoint();
	}
	catch (dynVarException& e) {
		puts(e.what());
		return this;
	}
	//Resize data
	try {
		GENFunc_private_resize(end + toApp.length() + 1);
	}
	catch (dynVarException& e) {
		puts(e.what());
		return this;
	}

	//copy to append
#ifdef _WIN32
	memcpy_s((char*)this->data.bigdata + end, toApp.length() + 1, toApp.c_str(), toApp.length());
#else
	memcpy((char*)this->data.bigdata + end, toApp.c_str(), toApp.length());
#endif // _WIN32
	((char*)this->data.bigdata)[end + toApp.length()] = '\0';

	//Done
	return this;
}

size_t dynVar::STRFunc_private_getAppendPoint() {
	CHECK_DYNVAR_TYPE(Type::STRING, __FUNCTION__);
	size_t end = STRFunc_strlen();
	if (end == std::string::npos)
	{
		//That's an issue
		throw dynVarException("[Error] STRFunc_append called on function with invalid string.");
		return 0;
	}
	return end;
}

void dynVar::GENFunc_private_resize(size_t newsize) {
	//Check if it is a member that requires big data
	if (this->type != Type::CLASS && this->type != Type::STRING) {
		//Does not use big data, throw
		throw dynVarException("[Error] " __FUNCTION__ " called on incompatible dynVar type.");
		return;
	}
	void* newdat = realloc(this->data.bigdata, newsize);
	if (newdat == NULL) {
		throw dynVarException("[Error] STRFunc_append failed to allocate memory for resize.");
		return;
	}
	else {
		this->data.bigdata = newdat;
	}
}

void dynVar::_p_init() {
	data.bigdata = NULL;
	m_reference_count = new size_t;
	if (m_reference_count == NULL) {
		//We don't have memory.
		printf("[Error] Failed to allocate memory in %s.\n", __FUNCTION__);
		return;
	}
	*m_reference_count = 1;
}

dynVar* dynVar::operator=(char* dat) {
	//The dll function will encode the result as <int><void*>
	this->type = *((int*)dat);
	this->data.bigdata = *((void**)(dat + sizeof(int)));


	free(dat);	//Free the memory given by the dll
	return this;
}

dynVar* dynVar::operator=(int i) {
	this->type = dynVar::Type::INT;
	if (this->data.bigdata != NULL) {
		free(data.bigdata);
	}
	this->data._int = i;
	return this;
}

dynVar* dynVar::operator=(const std::string& str)
{
	if ((this->type == Type::STRING || this->type == Type::CLASS) && this->data.bigdata != NULL) {
		free(data.bigdata);
	}
	this->type = dynVar::Type::STRING;
	this->data.bigdata = malloc(sizeof(char)*str.length() + 1);
	if (this->data.bigdata == NULL) {
		printf("[Error] Failed to allocate memory for dynVar assignment (string)\n");
		return this;
	}
#ifdef _WIN32
	memcpy_s(this->data.bigdata, sizeof(char)*str.length() + 1, str.data(), sizeof(char)*str.length());
#else
	memcpy(this->data.bigdata, str.data(), sizeof(char)*str.length());
#endif // _WIN32
	((char*)this->data.bigdata)[str.length()] = '\0';
	return this;
}

size_t dynVar::STRFunc_strlen() {
	CHECK_DYNVAR_TYPE(Type::STRING, __FUNCTION__);
	assert(data.bigdata != NULL);
	for (size_t i = 0; i < SCRIPT_STRING_MAXLENGTH; i++) {
		if (((char*)data.bigdata)[i] == '\0') {
			return i;
		}
	}

	return std::string::npos;
}

dynVar* dynVar::INTFunc_append(int i) {
	//We treat all numbers as decimals
	int offset = 10;
	while (i >= offset) {
		offset *= 10;
	}
	data._int *= offset;	//Shift all digits to the right
	data._int += i;	//Add the i value

	return this;
}

int ScriptEngine::load(const sf::String& path)
{
	sf::String str;
	if (basicLoadFile(path, str) < 0) {
		return -1;
	}


	//Remove all string constants and replace them with placeholders first
	size_t strconst_begin = 0;
	size_t rplmt_ID = 0;
	for (size_t i = 0; i < str.getSize(); i++) {
		if (str[i] == L'\\') {
			//The next char will be escaped anyways, so just skip it.
			i++;
			continue;
		}
		if (str[i] == L'\"') {
			if (strconst_begin == 0) {
				//New string started
				strconst_begin = i;
			}
			else {
				//String ends
				//sf::String str_lit(str.begin() + strconst_begin + 1, str.begin() + i);
				sf::String str_lit = str.substring(strconst_begin + 1, i - (strconst_begin + 1));
				//str.erase(str.begin() + strconst_begin, str.begin() + i + 1);
				str.erase(strconst_begin, i + 1 - strconst_begin);
				//Insert a dynamic replacement
				sf::String rplmt(L"CONST_STR_ID_" + std::to_wstring(rplmt_ID));
				rplmt_ID++;
				this->m_string_litterals[rplmt] = str_lit;
				str.insert(strconst_begin, rplmt);
				strconst_begin = 0;
			}

		}
	}

	sf::String trimmed;
	//trimmed.reserve(str.size());
	unsigned int skip_to_next = 0;
	for (size_t i = 0; i < str.getSize(); i++) {
		//if (isspace(str[i]))
		//	continue;

		if (str[i] == L'\"') {
			skip_to_next = L'\"';
		}

		if (i > 0) {
			//Chop if previous char was a space or any special sign like ,.-!() and so on
			if (isspace(str[i]) &&
				(isspace(str[i - 1]) || !isalnum(str[i - 1]))) {
				continue;
			}
			if (str[i] == L'(' &&
				str[i - 1] == L' ') {
				//trimmed.pop_back();
				trimmed.erase(trimmed.getSize() - 1);
			}
		}
		else {
			//If the space is the first char, delete too
			if(isspace(str[i]))
				continue;
		}

		//trimmed.push_back(str[i]);
		trimmed += str[i];
	}
	//trimmed now has a minimal size



	//Find all functions
	size_t last = 0;
	while ((last = trimmed.find(L"func ", last)) != sf::String::InvalidPos)
	{
		//find every function
		for (size_t i = last + wcslen(L"func "); i < trimmed.getSize(); i++)
		{
			if (trimmed[i] == L'(')
			{
				//sf::String str(trimmed.begin() + last + lstrlenW(L"func "), trimmed.begin() + i);
				sf::String str = trimmed.substring(last + strlen("func "), i - (last + strlen("func ")));
				m_function_starts[str] = i;
				break;
			}

		}
		last++;
	}
	last = 0;
	//Find all jumppoints
	while ((last = trimmed.find(L"jmppnt ", last)) != sf::String::InvalidPos)
	{
		//find every function
		for (size_t i = last + wcslen(L"jmppnt "); i < trimmed.getSize(); i++)
		{
			if (trimmed[i] == L':')
			{
				//sf::String str(trimmed.begin() + last + lstrlenW(L"jmppnt "), trimmed.begin() + i);
				sf::String str = trimmed.substring(last + strlen("jmppnt "), i - (last + strlen("jmppnt ")));
				m_jumppoints[str] = i + 1;
				break;
			}

		}
		last++;
	}

	m_datastring = trimmed;
	return 0;
}

int ScriptEngine::cleanScope(int SL)
{
	int count = 0;
	for (std::map<sf::String, dynVar*>::iterator it = m_vartable.begin(); it != m_vartable.end();)
	{
		//sf::String checker(it->first.begin() + it->first.rfind(L'#') + 1, it->first.end());
		size_t pos = 0;
		//printf("In string \"%s\", length %d\n", it->first.toAnsiString().c_str(), it->first.getSize());
		while (it->first.find("#", pos + 1) != it->first.InvalidPos)
		{
			pos = it->first.find("#", pos + 1);
		}
		sf::String checker = it->first.substring(pos + 1);
		int l = std::stoi(checker.toAnsiString());
		if (l > SL) {
			//Out of scope, remove
			delete it->second;
			m_vartable.erase(it);
			it = m_vartable.begin();
			count++;
		}
		else
			++it;
	}
	return count;
}

ExtensionInterface::ExtensionInterface()
{
}

ExtensionInterface::~ExtensionInterface()
{
}
