#include "logger_s.h"

void logger_s::InitLogging() 
{
    std::filesystem::create_directories(settings::kLoggsDir);
    const int rotation_size = 10 * 1024 * 1024;

    const auto time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0);
    const auto text_format = "[%TimeStamp%] [%Severity%] %Message%";

    /* for info level and higher*/
    boost::log::add_file_log(
        boost::log::keywords::file_name = settings::kLoggsInfoFile,
        boost::log::keywords::rotation_size = rotation_size,
        boost::log::keywords::time_based_rotation = time_based_rotation,
        boost::log::keywords::format = text_format,
        boost::log::keywords::open_mode = std::ios::trunc,
        boost::log::keywords::filter = boost::log::trivial::severity <= boost::log::trivial::info
    );

    /* for info warning and higher*/
    boost::log::add_file_log(
        boost::log::keywords::file_name = settings::kLoggsWarningPlusFile,
        boost::log::keywords::rotation_size = rotation_size,
        boost::log::keywords::time_based_rotation = time_based_rotation,
        boost::log::keywords::format = text_format,
        boost::log::keywords::open_mode = std::ios::trunc,
        boost::log::keywords::filter = boost::log::trivial::severity >= boost::log::trivial::warning
    );

    boost::log::add_common_attributes();
}

void logger_s::ServerStart()
{
    BOOST_LOG_TRIVIAL(info) << kInfoServerStart << std::endl;
    boost::log::core::get()->flush();
}

void logger_s::ClientConnect(const std::string& id)
{
    BOOST_LOG_TRIVIAL(info) << logger_s::kInfoClientConnect << id;
    boost::log::core::get()->flush();
}

void logger_s::ClientDisconnect(const std::string& id)
{
    BOOST_LOG_TRIVIAL(info) << logger_s::kClientDisconnect << id;
    boost::log::core::get()->flush();
}

void logger_s::ErrorFatal()
{
    BOOST_LOG_TRIVIAL(fatal) << logger_s::kErrorFatal << std::endl;
    boost::log::core::get()->flush();
}

void logger_s::ErrorExeption(const std::exception& e, const std::string& s, int l)
{
    BOOST_LOG_TRIVIAL(error) << logger_s::kErrorExeption << s << "::" << l << " " << e.what() << std::endl;
    boost::log::core::get()->flush();
}

void logger_s::ErrorEmptyQueue()
{
    BOOST_LOG_TRIVIAL(error) << logger_s::kErrorEmptyQueue << std::endl;
    boost::log::core::get()->flush();
}

void logger_s::ErrorHandlerNotFound(EventType t)
{
    BOOST_LOG_TRIVIAL(error) << logger_s::kErrorHandlerNotFound << static_cast<unsigned short >(t) <<std::endl;
    boost::log::core::get()->flush();
}

void logger_s::ErrorSerrialisation(EventType t)
{
    BOOST_LOG_TRIVIAL(error) << logger_s::kErrorSerrialisation << static_cast<unsigned short>(t) << std::endl;
    boost::log::core::get()->flush();
}

void logger_s::AuthenticateAttempt(const std::string& login)
{
    BOOST_LOG_TRIVIAL(info) << logger_s::kInfoAuthenticateAttempt << login;
    boost::log::core::get()->flush();
}

void logger_s::ErrorDynamicPointerCast(EventType t)
{
    BOOST_LOG_TRIVIAL(error) << logger_s::kErrorDynamicPointerCast << static_cast<unsigned short>(t) << std::endl;
    boost::log::core::get()->flush();
}

void logger_s::ErrorSqlExecuteStep(EventType t)
{
    BOOST_LOG_TRIVIAL(error) << logger_s::kErrorSqlExecuteStep << static_cast<unsigned short>(t) << std::endl;
    boost::log::core::get()->flush();
}

void logger_s::ErrorDatabaseCreateDir(const std::string& path)
{
    BOOST_LOG_TRIVIAL(error) << logger_s::kErrorDatabaseCreateDir << path << std::endl;
    boost::log::core::get()->flush();
}
