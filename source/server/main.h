#pragma once

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp> 
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <stdexcept>

#include "database.h"
#include "db_handler.h"
#include "event_dispatcher.h"
#include "event_queue.h"
#include "logger_s.h"
#include "network_handler.h"
#include "session.h"
#include "user_handler.h"


/* This could be useful in the future.

User Handler: 
- check login status true/false in "UserHandler::HandleClassedEvent(std::shared_ptr<SuccessCheckLogPas> event)"
- 

Network Handler:
- error event for client in "NetworkHandler::HandleClassedEvent(std::shared_ptr<StartGameServerResponseSucsses> event)"

Game Handler:
- error event for client in "GameHandler::HandleClassedEvent(std::shared_ptr<StartGameClientRequest> event)"

Database:
- rules to check database's tables in Database::Create()

DatabaseHandler:
- Send error instead FailureCheckLogPas in "DatabaseHandler::HandleClassedEvent(std::shared_ptr<LogPasClientResponse> event)"

main:
- Correct server shutdown
*/