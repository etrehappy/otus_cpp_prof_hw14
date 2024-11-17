#include "logger_c.h"
#include "settings.h"

void logger_c::InitLogging()
{
    namespace bl = boost::log;
    namespace blk = boost::log::keywords;

    std::filesystem::create_directories(settings::kLoggsDir);
    const int rotation_size = 10 * 1024 * 1024;

    const auto time_based_rotation = bl::sinks::file::rotation_at_time_point(0, 0, 0);
    const auto text_format = "[%TimeStamp%] [%Severity%] %Message%";    

    /* INFO */
    boost::log::add_file_log(
        blk::file_name = settings::kLoggsInfoFile
        , blk::rotation_size = rotation_size
        , blk::time_based_rotation = time_based_rotation
        , blk::format = text_format
        , blk::open_mode = std::ios::trunc
        , blk::filter = bl::trivial::severity <= bl::trivial::info
    );

    /* WARNING+ */
    boost::log::add_file_log(
        blk::file_name = settings::kLoggsWarningPlusFile
        , blk::rotation_size = rotation_size
        , blk::time_based_rotation = time_based_rotation
        , blk::format = text_format
        , blk::open_mode = std::ios::trunc
        , blk::filter = bl::trivial::severity >= bl::trivial::warning
    );

    bl::add_common_attributes();
}

void logger_c::Connect()
{
    BOOST_LOG_TRIVIAL(info) << logger_c::kInfoConnect << std::endl;
    boost::log::core::get()->flush();
}

void logger_c::ErrorExeption(const std::exception& e, const std::string& s, int l)
{
    BOOST_LOG_TRIVIAL(error) << logger_c::kErrorExeption << s << "::" << l << " " << e.what() << std::endl;
    boost::log::core::get()->flush();
}

void logger_c::ErrorReadSome(const std::string& error)
{
    BOOST_LOG_TRIVIAL(error) << logger_c::kErrorReadSome << error << std::endl;
    boost::log::core::get()->flush();
}

void logger_c::ErrorMessageCallbackNotSet(const std::string& function)
{
    BOOST_LOG_TRIVIAL(error) << logger_c::kErrorMessageCallbackNotSet << function << std::endl;
    boost::log::core::get()->flush();
}

void logger_c::ErrorParsingMessage()
{
    BOOST_LOG_TRIVIAL(error) << logger_c::kErrorParsingMessage << std::endl;
    boost::log::core::get()->flush();
}

void logger_c::ErrorSerializeToString()
{
    BOOST_LOG_TRIVIAL(error) << logger_c::kErrorSerializeToString << std::endl;
    boost::log::core::get()->flush();
}

void logger_c::ErrorSend(const std::string& error_text)
{
    BOOST_LOG_TRIVIAL(error) << logger_c::kErrorSend << error_text << std::endl;
    boost::log::core::get()->flush();
}

void logger_c::ErrorLocatinReadDb()
{
    BOOST_LOG_TRIVIAL(error) << logger_c::kErrorLocatinReadDb << std::endl;
    boost::log::core::get()->flush();
}
