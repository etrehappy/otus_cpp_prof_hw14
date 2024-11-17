#include "tests_main.h"
#include "event_queue.h"
#include "event.h"
#include "event_dispatcher.h"
#include "event_handler.h"


// Тестовая очередь событий
class EventQueueTest : public ::testing::Test {
protected:
    EventQueue event_queue_;
};

// Проверка добавления события
TEST_F(EventQueueTest, AddEventIncreasesQueueSize) {
    auto event = std::make_shared<Event>(EventPriority::kMedium, EventType::kFirstConnect, "test_key");
    ASSERT_TRUE(event_queue_.IsEmpty()) << "Очередь должна быть пустой перед добавлением.";

    event_queue_.AddEvent(event);
    ASSERT_FALSE(event_queue_.IsEmpty()) << "Очередь не должна быть пустой после добавления события.";
}

// Проверка извлечения события
TEST_F(EventQueueTest, ExtractTopReturnsCorrectEvent) {
    auto event1 = std::make_shared<Event>(EventPriority::kLow, EventType::kFailureCheckLogPas, "test_key1");
    auto event2 = std::make_shared<Event>(EventPriority::kHigh, EventType::kSuccessCheckLogPas, "test_key2");

    event_queue_.AddEvent(event1);
    event_queue_.AddEvent(event2);

    auto top_event = event_queue_.ExtractTop();
    ASSERT_EQ(top_event->GetType(), EventType::kSuccessCheckLogPas)
        << "Извлеченное событие должно быть с более высоким приоритетом.";
}


// Вспомогательный класс для тестирования обработчиков событий
class TestEventHandler : public EventHandler {
public:
    mutable bool was_called = false;

    boost::asio::awaitable<void> ForwardEventToHandler(ShrdEvent event) override {
        was_called = true;
        co_return;
    }

    void RegisterInDispatcher(EventDispatcher& d) override {}
};

// Тестовая структура для диспетчера событий
class EventDispatcherTest : public ::testing::Test {
protected:
    EventDispatcher dispatcher_;
    EventQueue event_queue_;
};

// Проверка регистрации обработчика
TEST_F(EventDispatcherTest, RegisterHandlerAddsHandlerToDispatcher) {
    auto handler = std::make_shared<TestEventHandler>();
    dispatcher_.RegisterHandler(EventType::kFirstConnect, handler, "test_key");

    auto event = std::make_shared<Event>(EventPriority::kMedium, EventType::kFirstConnect, "test_key");
    event_queue_.AddEvent(event);

    ASSERT_NO_THROW(dispatcher_.Dispatch(event_queue_))
        << "Диспетчер должен корректно обрабатывать зарегистрированные события.";
}

