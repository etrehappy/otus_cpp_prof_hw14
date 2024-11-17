﻿template<>
boost::asio::awaitable<void> GameHandler::HandleClassedEvent(std::shared_ptr<AvatarDataReady> event)
{
try
{
    current_avatar_ = std::move(event->MoveAvatar());
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
    co_return;
}/* AvatarDataReady */

template<>
boost::asio::awaitable<void> GameHandler::HandleClassedEvent(std::shared_ptr<StartGameClientRequest> event) 
{
try
{
    auto executor = co_await boost::asio::this_coro::executor;
    size_t attempt_counter{0};
    size_t delay = 500;

    while (!current_avatar_)
    {
        boost::asio::steady_timer timer(executor, std::chrono::milliseconds(delay));
        co_await timer.async_wait(boost::asio::use_awaitable);

        if (++attempt_counter > 10)
        {
            throw std::invalid_argument{logger_s::kErrExeptEmptyVariable};
            co_return;
        }
    }

    event_queue_.AddEvent(
        std::make_shared<StartGameServerResponseSucsses>(event->GetRoutingKey() 
        , current_avatar_)
    );        
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}    
    co_return;
}/* StartGameClientRequest */


template<>
boost::asio::awaitable<void> GameHandler::HandleClassedEvent(std::shared_ptr<ExitGameRequest> event)
{
try
{
    const auto& proto_message_avatar = (*event->GetEventMessage()).exit_game().avatar();

    current_avatar_->SetFewParametrs(proto_message_avatar.level()
        , proto_message_avatar.experience()
        , proto_message_avatar.health()
        , proto_message_avatar.mana()
        , proto_message_avatar.strength()
        , proto_message_avatar.dexterity()
        , proto_message_avatar.intelligence()
        , proto_message_avatar.location_id()
        , proto_message_avatar.pos_x()
        , proto_message_avatar.pos_y()
    );

    const bool exit_game = true;
    event_queue_.AddEvent(std::make_shared<SaveGame>(event->GetRoutingKey(),
        current_avatar_, exit_game
    ));

}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e);
}

    co_return;
}/* ExitGameRequest */