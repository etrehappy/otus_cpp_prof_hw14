#include <algorithm>

#include "game_logic.h"
#include "game_logic.ipp"
#include "logger_c.h"
#include "texts.h"

/*
game_logic.ipp
using ProtoMsg = ProtoMessage::GameMessage;
using C_ProtoPtr = std::unique_ptr<const ProtoMsg>;
using ProtoPtr = std::unique_ptr<ProtoMsg>;
using ProtoCommandType = ProtoMessage::CommandType;
using ChatPairPtr = std::unique_ptr<std::pair<CommandList, ChatMessage>>;
*/

GameLogic::GameLogic(View& v, DatabaseHandler& m, IInputDataProvider& input, IExit& exit)
    : uuid_{}, current_avatar_{nullptr}, current_location_{nullptr}, view_{v}
    , model_{m}, data_provider_{input}, exit_{exit}, avatar_and_location_ {}, net_notify_{}
    , game_thread_{}, parser_{std::make_shared<Parser>()}, stop_get_command_{false}
    , main_menu_{}
{
}

GameLogic::~GameLogic()
{
    if (game_thread_.joinable())
    {
        game_thread_.join();
    }    
}

void GameLogic::RegisterSingleObserver(std::function<void(C_ProtoPtr)> callback)
{
    net_notify_ = callback;

    if (!net_notify_)
    {
        logger_c::ErrorMessageCallbackNotSet(__func__);
    }
}

void GameLogic::SelectFunction(const ProtoMsg& message)
{
    switch (message.command())
    {
    case ProtoCommandType::kLoginPasRequest:
        SendLogPasResponse();
        break;

    case ProtoCommandType::kAuthSuccess:
        AuthSuccess();
        break;

    case ProtoCommandType::kAuthFailure:
        AuthFailure();
        break;

    case ProtoCommandType::kUsersAvatarListServerResponse:
        HandlerChooseAvatar(message);
        break;

    case ProtoCommandType::kStartGameServerResponse:
        HandlerStartGame(message);
        break;

    case ProtoCommandType::kGetCommonMessage:
        HandlerCommonMessage(message);
        break;

    default:
        break;
    }
}


////////////////////////////////////////////////////////////////////////////////
// Member functions below are executed in the thread Network::Listen 


void GameLogic::SendLogPasResponse() 
{
try
{
    std::pair<const std::string, const std::string> credentials = std::move(GetCredentials());
    const std::string& login = credentials.first;
    const std::string& password = credentials.second;

    if (login.empty() || password.empty())
    {
        view_.ShowTextWithNewLine(text_v::kErrorLogPas);
        return;
    }

    NotifyNewMessage(std::move(
        PrepareMessage<ProtoCommandType::kLoginPasResponse>(login, password)));
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}

}/* SendLogPasResponse */
 
std::pair<const std::string, const std::string> GameLogic::GetCredentials()
{  
    std::pair<std::string, std::string> p;

try
{
    view_.ShowTextWithNewLine(text_v::kLoginReq);
    auto if_login_input_error = [this]() {view_.ShowTextWithNewLine(text_v::kLoginReq);};
    p.first = std::move(data_provider_.GetString(if_login_input_error));

    view_.ShowTextWithoutNewLine(text_v::kPasReq);
    auto if_pas_input_error = [this]() {view_.ShowTextWithoutNewLine(text_v::kPasReq); };
    p.second = std::move(data_provider_.GetString(if_pas_input_error));
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
    return p;

}/* GetCredentials */

void GameLogic::AuthSuccess()
{
try
{
    view_.ShowTextWithNewLine(text_v::kAuthSuccess);

    NotifyNewMessage(std::move(
        PrepareMessage<ProtoCommandType::kUsersAvatarListClientRequest>()));

}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}

void GameLogic::AuthFailure()
{
    view_.ShowTextWithNewLine(text_v::kAuthFailure);

    SendLogPasResponse();
}

void GameLogic::HandlerChooseAvatar(const ProtoMsg& message)
{
    const uint32_t avatar_id = GetChoisedAvatarId(message);
    if (!avatar_id)
    {
        view_.ShowErrorChoiseAvatar();
        CreateAvatar();
        NotifyNewMessage(std::move(
            PrepareMessage<ProtoCommandType::kUsersAvatarListClientRequest>()));
        return;
    }

    NotifyNewMessage(std::move(
        PrepareMessage<ProtoCommandType::kStartGameClientRequest>(avatar_id)));
    
    return;
}

uint32_t GameLogic::GetChoisedAvatarId(const ProtoMsg& message)
{
    const int error{0};
try
{
    if (!message.has_avatar_list_data())
    {        
        return error;
    }

    view_.ShowAvatarList(message);

    view_.ShowTextWithNewLine(text_v::kAvatarNumber);

    auto if_input_error = [this]() {
        view_.ShowTextWithNewLine(text_v::kAvatarNumber);
    };
        
    auto max_number = static_cast<uint16_t>(message.avatar_list_data().avatar_list().size());
    auto choised_avatar_number = data_provider_.GetInt(if_input_error, max_number);

    return FindAvatarId(choised_avatar_number, message);
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)    
    return error;
}
}/* GetChoisedAvatar */

uint32_t GameLogic::FindAvatarId(uint16_t choised_avatar_number
    , const ProtoMsg& message)
{
    int it_avatar_list_number = 1;

    auto it_avatar = message.avatar_list_data().avatar_list().cbegin();
    for (; it_avatar != message.avatar_list_data().avatar_list().cend(); ++it_avatar)
    {
        if (choised_avatar_number == it_avatar_list_number)
        {
            break;
        }
        ++it_avatar_list_number;
    }  

    return it_avatar->id();

}/* GetMatchedAvatarId */

void GameLogic::CreateAvatar()
{
    return;
}


void GameLogic::SetAvatar(const ProtoMsg& message)
{
try {
    
    std::lock_guard guard{avatar_and_location_};

    if (current_avatar_ != nullptr)
    {
        current_avatar_.reset();
    }

    auto message_avatar = message.start_game().avatar();

    current_avatar_ = std::make_shared<Avatar>(message_avatar.id(), uuid_);

    current_avatar_->SetAllNotConstParametrs(message_avatar.name()
        , message_avatar.race()
        , message_avatar.class_()
        , message_avatar.level()
        , message_avatar.experience()
        , message_avatar.health()
        , message_avatar.mana()
        , message_avatar.strength()
        , message_avatar.dexterity()
        , message_avatar.intelligence()
        , message_avatar.location_id()
        , message_avatar.pos_x()
        , message_avatar.pos_y()
    );
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
} /* SetAvatar */

void GameLogic::HandlerStartGame(const ProtoMsg& message)
{
try
{
    SetAvatar(message);

    current_location_ = std::move(model_.SetLocation(
        current_avatar_->GetLocationId(), avatar_and_location_));

    if (!current_avatar_ || !current_location_)
    {
        view_.ShowErrorStartGame();
        NotifyNewMessage(std::move(
            PrepareMessage<ProtoCommandType::kUsersAvatarListClientRequest>()));
        return;
    }

    current_location_->InitPositions(*current_avatar_);

    view_.ShowStartGameMessage(*current_avatar_, *current_location_, avatar_and_location_);

    game_thread_ = std::thread(&GameLogic::StartGame, this);

}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
} 

    return;

}/* HandlerStartGame */

void GameLogic::HandlerCommonMessage(const ProtoMsg& message)
{
try
{
    const std::string& sender_name = message.chat_data().sender_avatar_name();
    const std::string& sender_text = message.chat_data().text_message();

    view_.ShowCommonMessage(sender_name, sender_text);
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}


// Member functions above are executed in the thread Network::Listen 
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Member functions below are executed in the thread GameLogic::StartGame

void GameLogic::StartGame()
{
    std::string command_str{};
    auto if_input_error = [this]() {view_.ShowTextWithNewLine(text_v::kBracket); };
    CommandVariant variant{CommandList::kNone};

    while (!stop_get_command_)
    {        
    try
    {
        view_.ShowTextWithNewLine(text_v::kBracket);

        /*Get a command frome an user*/
        command_str = std::move(data_provider_.GetString(if_input_error));

        variant = std::move(parser_->GetUserCommand(command_str));

        if (std::holds_alternative<CommandList>(variant))
        {
            HandleCommandVariant(std::get<CommandList>(variant));
        }
        else if (std::holds_alternative<ChatPairPtr>(variant))
        {
            HandleCommandVariant(std::get<ChatPairPtr>(variant));
        }
        else if (std::holds_alternative<GoPairPtr>(variant))
        {
            HandleCommandVariant(std::get<GoPairPtr>(variant));
        }
    }/* try */
    catch (std::exception& e)
    {
        MVP_LOG_EXEPTION(e)

    } /* catch */

    } /* while */ 
    
} /* StartGame */


/* thread StartGame */
void GameLogic::HandleCommandVariant(CommandList& command)
{
    switch (command)
    {
    case CommandList::kMenu:
        HandleCommand<CommandList::kMenu>();
        break;
    case CommandList::kSpeak:
        //HandleCommand<CommandList::kSpeak>();
        break;
    case CommandList::kMap:
        HandleCommand<CommandList::kMap>();
        break;
    case CommandList::kCurrentLocation:
        HandleCommand<CommandList::kCurrentLocation>();
        break;
    case CommandList::kGo:
    case CommandList::kChat:
        break;
    case CommandList::kNone:
        view_.ShowTextWithNewLine(text_v::kBracket);
        break;
    case CommandList::kEnd:
        break;
    default:
        break;
    }
}

void GameLogic::HandleCommandVariant(ChatPairPtr& pair_ptr)
{
try
{
    const std::string& recipient = pair_ptr->second.recipient;
    const std::string& text = pair_ptr->second.text;

    if (recipient == command_str::kToEveryone)
    {
        NotifyNewMessage(std::move(
            PrepareMessage<ProtoCommandType::kSendToCommonChat>(text)));
    }
    else
    {
        /* TODO: */
    }

}/* try */
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}

}/* ChatPairPtr */

void GameLogic::HandleCommandVariant(GoPairPtr& pair_ptr)
{
    const GoCommand command = pair_ptr->second;
    
    switch (command.direction)
    {
    case DirectionsList::kNorth:
        MoveY( -command.steps);
        break;
    case DirectionsList::kSouth:
        MoveY(command.steps);
        break;
    case DirectionsList::kWest:
        MoveX(-command.steps);
        break;
    case DirectionsList::kEast:
        MoveX(command.steps);
        break;
    default:
        break;
    }
}

void GameLogic::MoveX(int32_t steps)
{
try
{
    const int32_t avatar_pos_x = current_avatar_->GetPosition().first;
    const int32_t avatar_pos_y = current_avatar_->GetPosition().second;
    const int32_t min_number = 1;
    const int32_t offset = 1;

    const auto& map = current_location_->GetMap();
    int32_t new_x_pos = std::max(min_number, avatar_pos_x + steps);
    const char current_symbol = map[avatar_pos_y][new_x_pos];
    const bool is_out_of_bounds = new_x_pos >= map[0].size() - offset;

    if (is_out_of_bounds)
    {
        view_.ShowTextWithNewLine(text_v::kOutOfBounds);
    }
    else if (current_symbol == map_symbols::kDoor)
    {
        view_.ShowTextWithNewLine(text_v::kDoorClosed);
    }
    else if (current_symbol == map_symbols::kWall)
    {
        view_.ShowTextWithNewLine(text_v::kWallInFront);
    }
    else if (current_symbol == map_symbols::kEmpty)
    {
        current_location_->ClearPosOnMap(avatar_pos_x, avatar_pos_y);
        current_location_->SetNewPosOnMap(new_x_pos, avatar_pos_y, map_symbols::kPlayer);
        current_avatar_->SetPosX(new_x_pos);
    }
    else
    {
        view_.ShowTextWithNewLine(text_v::kObstacle);
    }
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}

void GameLogic::MoveY(int32_t steps)
{
try
{
    const int32_t avatar_pos_x = current_avatar_->GetPosition().first;
    const int32_t avatar_pos_y = current_avatar_->GetPosition().second;
    const int32_t min_number = 1;
    const int32_t offset = 1;

    const auto& map = current_location_->GetMap();
    int32_t new_y_pos = std::max(min_number, avatar_pos_y + steps);
    const char current_symbol = map[new_y_pos][avatar_pos_x];
    const bool is_out_of_bounds = new_y_pos >= map[0].size() - offset;

    if (is_out_of_bounds)
    {
        view_.ShowTextWithNewLine(text_v::kOutOfBounds);
    }
    else if (current_symbol == map_symbols::kDoor)
    {
        view_.ShowTextWithNewLine(text_v::kDoorClosed);
    }
    else if (current_symbol == map_symbols::kWall)
    {
        view_.ShowTextWithNewLine(text_v::kWallInFront);
    }
    else if (current_symbol == map_symbols::kEmpty)
    {
        current_location_->ClearPosOnMap(avatar_pos_x, avatar_pos_y);
        current_location_->SetNewPosOnMap(avatar_pos_x, new_y_pos, map_symbols::kPlayer);
        current_avatar_->SetPosY(new_y_pos);
    }
    else
    {
        view_.ShowTextWithNewLine(text_v::kObstacle);
    }
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}
////////////////////////////////////////////////////////////////////////////////


void GameLogic::NotifyNewMessage(C_ProtoPtr message_to_server)
{
try
{
    if (!message_to_server)
    {
        view_.ShowErrorSendMessageToServer();
        StopGame();
        return;
    }

    net_notify_(std::move(message_to_server));
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}

void GameLogic::StopGame()
{
    try
    {
        stop_get_command_ = true;

        exit_.StopGame();
    }
    catch (std::exception& e)
    {
        MVP_LOG_EXEPTION(e)
    }
}