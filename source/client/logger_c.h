#pragma once

#include <filesystem>
#include <exception>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#define MVP_LOG_EXEPTION(e) logger_c::ErrorExeption(e, __func__, __LINE__);

namespace logger_c
{
    void InitLogging();
    void Connect();

    void ErrorExeption(const std::exception&, const std::string&, int line);
    void ErrorReadSome(const std::string&);
    void ErrorMessageCallbackNotSet(const std::string&);
    void ErrorParsingMessage();
    void ErrorSerializeToString();
    void ErrorSend(const std::string&);
    void ErrorLocatinReadDb();

}//logger_c


namespace logger_c
{

#ifdef _WIN32    
    #pragma warning(push) 
    #pragma warning(disable : 4100) 
#else
    #pragma GCC diagnostic push 
    #pragma GCC diagnostic ignored "-Wunused-variable"
#endif 

    static const char* kInfoConnect = "Клиент подключен к серверу";
    
    static const char* kErrorFatal = "Неизвестная ошибка";
    static const char* kErrorExeption = "Исключение в (функция::строка) ";
    static const char* kErrorReadSome = "Ошибка чтения при подключении к серверу: ";
    static const char* kErrorMessageCallbackNotSet = "Не установлен callback для обработки сообщений в функции: ";
    static const char* kErrorParsingMessage = "Не удалось преобразовать ProtoMessage в string";
    static const char* kErrorSerializeToString = "Ошибка сериализации при отправке";
    static const char* kErrorSend = "Ошибка при отправке. Сообщение не отправлено: ";
    static const char* kErrorLocatinReadDb = "Ошибка при чтении локации.";

#ifdef _WIN32    
    #pragma warning(pop)
#else
    #pragma GCC diagnostic pop
#endif

}//logger_c
