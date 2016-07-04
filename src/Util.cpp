#include "stdafx.h"
#include "Util.h"


namespace SFG
{
Util::Util()
{
}


Util::~Util()
{
}

void Util::printLog(LogMessageType t, const char* file, int line, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);

    std::string message = std::string(format) + " in " + file + ":" + std::to_string(line) + "\n";
    switch (t)
    {
    case LogMessageType::Error:
        vprintf(std::string(std::string("[Error] ") + message).c_str(), ap);
        break;
    case LogMessageType::Warning:
        vprintf(std::string(std::string("[Warning] ") + message).c_str(), ap);
        break;
    case LogMessageType::Development:
        vprintf(std::string(std::string("[Warning/Development] ") + message).c_str(), ap);
        break;
    case LogMessageType::Information:
        vprintf(std::string(std::string("[Information] ") + message).c_str(), ap);
        break;
    default:

        break;
    }
    va_end(ap);
}

//https://msdn.microsoft.com/en-us/library/windows/desktop/bb204633(v=vs.85).aspx
void Util::getStackTrace(std::string& stream)
{
#ifdef _WIN32
    stream = "===========Call stack===========\n";
    typedef USHORT(WINAPI *CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);
    CaptureStackBackTraceType func = (CaptureStackBackTraceType)(GetProcAddress(LoadLibraryA("kernel32.dll"), "RtlCaptureStackBackTrace"));

    if (func == NULL)
    {
        stream += "[Error] Failed to resolve Call Stack print function\n";
        return;
    }

    //Set to get the full decorated function name
    DWORD options = SymGetOptions();
    DWORD newOptions = options & ~SYMOPT_UNDNAME;
    newOptions = newOptions | SYMOPT_PUBLICS_ONLY;
    SymSetOptions(newOptions);

    // Quote from Microsoft Documentation:
    // ## Windows Server 2003 and Windows XP:
    // ## The sum of the FramesToSkip and FramesToCapture parameters must be less than 63.
    const int kMaxCallers = 62;

    void         * callers_stack[kMaxCallers];
    unsigned short frames;
    SYMBOL_INFO  * symbol;
    HANDLE         process;
    process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);
    frames = (func)(0, kMaxCallers, callers_stack, NULL);
    symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    //printf("(%016I64LX):\n", sample_address);
    //out << "(" << sample_address << "): " << std::endl;
    const unsigned short  MAX_CALLERS_SHOWN = 6;
    frames = frames < MAX_CALLERS_SHOWN ? frames : MAX_CALLERS_SHOWN;
    for (unsigned int i = 0; i < frames; i++)
    {
        SymFromAddr(process, (DWORD64)(callers_stack[i]), 0, symbol);
        //printf("*** %d: %016I64LX %s - 0x%0X\n", i, callers_stack[i], symbol->Name, symbol->Address);

        char undecoratedName[MAX_SYM_NAME];
        //UnDecorateSymbolName(symbol->Name, undecoratedName, MAX_SYM_NAME, UNDNAME_COMPLETE);
        stream += std::string(symbol->Name, symbol->NameLen) + "\n";
        //stream += std::string(undecoratedName) + "\n";

    }

    free(symbol);
    stream += "=======End of Call stack========\n";
#else
    stream = "===========Call stack===========\n";
    void *array[10];
    size_t size = 0;

    size = backtrace(array, 10);
    char** symbols = backtrace_symbols(array, size);

    if(symbols == nullptr)
    {
        printLog(LogMessageType::Error, __FILE__, __LINE__, "Failed to get symbols.");
        return;
    }

    for(size_t i = 0; i < size; i++)
    {
        size_t dms = 0;
        int status = 0;
        //char* name = abi::__cxa_demangle(symbols[i], tmp, &dms, &status);

        //We have to create the subsequence first
        std::string tmp(symbols[i]);
		size_t tmp_i = tmp.find_first_of('+');
		size_t end_index = tmp_i != std::string::npos ? tmp_i : tmp.length() - 1;
        std::string symbol(tmp.begin() + tmp.find_first_of('(') + 1, tmp.begin() + end_index);

        char* name = abi::__cxa_demangle(symbol.c_str(), nullptr, nullptr, &status);
        if(status != 0)
        {
            //printLog(LogMessageType::Error, __FILE__, __LINE__, "Failed to demangle name \"%s\" with code %d", symbol.c_str(), status);
        }
        if(name == nullptr)
        {
            stream += std::string(symbols[i]) + "\n";
        }
        else
        {
            stream += std::string(name) + "\n";
            free(name);
        }
    }
    stream += "=======End of Call stack========\n";
#endif
}

};
