#include "tests_main.h"
#include <locale>


int main(int argc, char** argv) {
    
    std::locale::global(std::locale("en_US.utf8"));   

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}