#include "main.h"

int main() 
{
#ifdef _WIN32    
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::locale::global(std::locale("ru_RU.UTF-8"));
#else
    setlocale(LC_ALL, "ru_RU.UTF-8");
#endif
    
try
{
    logger_c::InitLogging();

    View view{};
    DatabaseHandler model{};
    Controller controller{view, model};

    controller.Start();
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
    google::protobuf::ShutdownProtobufLibrary();
    return 0;

}/* main */