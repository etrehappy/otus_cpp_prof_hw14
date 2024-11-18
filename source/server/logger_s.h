#pragma once

#include <exception>
#include <filesystem>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "event.h"
#include "settings.h"

#define MVP_LOG_EXEPTION(e) logger_s::ErrorExeption(e, __func__, __LINE__);

namespace logger_s
{
    void InitLogging();

    void ServerStart();
    void ClientConnect(const std::string&);
    void AuthenticateAttempt(const std::string&);
    void ClientDisconnect(const std::string&);

    void ErrorFatal();
    void ErrorExeption(const std::exception&, const std::string&, int);
    void ErrorEmptyQueue();
    void ErrorHandlerNotFound(EventType);
    void ErrorSerrialisation(EventType);
    void ErrorDynamicPointerCast(EventType);
    void ErrorSqlExecuteStep(EventType);
    void ErrorDatabaseCreateDir(const std::string&);

#ifdef _WIN32    
    #pragma warning(push) 
    #pragma warning(disable : 4100) 
#else
    #pragma GCC diagnostic push 
    #pragma GCC diagnostic ignored "-Wunused-variable"
#endif 

    static const char* kInfoServerStart = "Сервер запущен и ожидает подключения...";
    static const char* kInfoClientConnect = "Клиент подключен: ";
    static const char* kInfoAuthenticateAttempt = "Попытка аутентификации пользователя: ";
      
    /*GameHandler, NetworkHandler uses:*/
    static const char* kErrExeptEmptyVariable = "не получены данные";

    /*Logger uses:*/
    static const char* kErrorFatal = "Неизвестная ошибка";
    static const char* kErrorExeption = "Исключение в ";
    static const char* kErrorEmptyQueue = "Попытка извлечь событие из пустой очереди";
    static const char* kErrorHandlerNotFound = "Не найден обработчик события # ";
    static const char* kErrorSerrialisation = "Не удалось сериализовать событие # ";
    static const char* kErrorDynamicPointerCast = "Ошибка преобразования shared_ptr";
    static const char* kErrorSqlExecuteStep = "Ошибка при чтении данных из таблицы по событию ";
    static const char* kErrorDatabaseCreateDir = "Ошибка при создании директории: ";

    static const char* kClientDisconnect = "Клиент отключен: ";

#ifdef _WIN32    
    #pragma warning(pop)
#else
    #pragma GCC diagnostic pop
#endif

} /*logger_s*/