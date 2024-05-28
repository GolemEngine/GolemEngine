#include "Debug/log.h"

#include <iostream>
#include <cstdarg>

std::fstream Log::m_file;

Log::~Log()
{
    m_file.close();
}

void Log::OpenFile(std::filesystem::path const& _filename)
{
    m_file.open(_filename, std::fstream::out);

    if (m_file.is_open())
    {
        std::cout << "Open file: " << _filename << std::endl;
    }
    else 
        std::cout << "Cannot open the file: " << _filename << std::endl;
}

void Log::Print(const char* _format, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, _format);
        
    vsprintf_s(buffer, sizeof(buffer), _format, args);
    std::cout << buffer << std::endl;

    if (m_file.is_open())
    {
        m_file << buffer << std::endl;
        m_file.flush();
    }
    
    va_end(args);
}
