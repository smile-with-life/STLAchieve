#pragma once
#include "file_appender.h"

namespace quicklog {
template<typename Mutex>
inline File_Appender<Mutex>::File_Appender(std::string filePath)
    : Base_Appender<Mutex>()
    , m_file(filePath)   
{
    try
    {     
        m_file.open();
    }
    catch (log_exception& exception)
    {
        std::cout << exception.what() << std::endl;
    } 
    catch (...)
    {
        throw;
    }
}

template<typename Mutex>
inline std::string File_Appender<Mutex>::getFileName()
{
    return m_file.fileName();
}

template<typename Mutex>
inline std::string File_Appender<Mutex>::getFilePath()
{
    return m_file.filePath();
}

template<typename Mutex>
inline void File_Appender<Mutex>::print(const log_msg& logMessage)
{
    std::string content;
    this->m_format->format(logMessage, content);
    m_file.write(content);
}
template<typename Mutex>
inline void File_Appender<Mutex>::print(const std::string& content)
{
    m_file.write(content);
}
template <typename Mutex>
inline void File_Appender<Mutex>::native_flush()
{
    m_file.flush();
}


}//namespace quicklog
