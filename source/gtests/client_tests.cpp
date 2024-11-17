#include "tests_main.h"

#include "../client/db_handler.h"
#include "game_logic.h"
#include "location.h"
#include "controller.h"
#include "view.h"

class MockViewForGameLogic : public View 
{
public:
    MOCK_METHOD(void, ShowTextWithNewLine, (const std::string&), ());
};

class MockView : public View {
public:
    MOCK_METHOD(void, ShowErrorInput, (), ());
    MOCK_METHOD(void, ShowTextWithNewLine, (const std::string&), ());
};

TEST(ControllerTest, GetString_ValidInput) {
    MockView mock_view;
    DatabaseHandler db_handler;
    Controller controller(mock_view, db_handler);

    // Установка входных данных
    std::istringstream input("valid_input\n");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_CALL(mock_view, ShowErrorInput()).Times(0);

    std::string result = controller.GetString([]{});
    EXPECT_EQ(result, "valid_input");
}

TEST(ControllerTest, GetInt_ValidInput) 
{
    MockView mock_view;
    DatabaseHandler db_handler;
    Controller controller(mock_view, db_handler);

    std::istringstream input("42\n");
    std::cin.rdbuf(input.rdbuf());

    uint16_t max_number = 100;
    uint16_t result = controller.GetInt([]{}, max_number);

    EXPECT_EQ(result, 42);
}

TEST(DatabaseHandlerTest, SetLocation_ValidId) {
    DatabaseHandler db_handler;
    std::mutex mutex;
    auto location = db_handler.SetLocation(1, mutex);

    ASSERT_NE(location, nullptr);
    EXPECT_EQ(location->GetId(), 1);
}

//TEST(GameLogicTest, SendLogPasResponse_EmptyCredentials) 
//{
//    MockViewForGameLogic mock_view;
//    DatabaseHandler db_handler;
//    MockInputDataProvider mock_input;
//    MockExit mock_exit;
//
//    GameLogic game_logic(mock_view, db_handler, mock_input, mock_exit);
//
//    EXPECT_CALL(mock_view, ShowTextWithNewLine(::testing::StrEq("Ошибка обработки учетных данных.")));
//
//    game_logic.SendLogPasResponse();
//}