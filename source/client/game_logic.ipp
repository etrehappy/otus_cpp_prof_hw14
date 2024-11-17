#include "game_logic.h"
#include "logger_c.h"
#include "texts.h"

using ProtoMsg = ProtoMessage::GameMessage;
using C_ProtoPtr = std::unique_ptr<const ProtoMsg>;
using ProtoPtr = std::unique_ptr<ProtoMsg>;
using ProtoCommandType = ProtoMessage::CommandType;
using ChatPairPtr = std::unique_ptr<std::pair<CommandList, ChatMessage>>;
using GoPairPtr = std::unique_ptr<std::pair<CommandList, GoCommand>>;

template<>
C_ProtoPtr GameLogic::PrepareMessage<ProtoCommandType::kLoginPasResponse>
    (const std::string& login, const std::string& password)
{
try
{	
    ProtoPtr message = std::make_unique<ProtoMsg>();
    message->set_command(ProtoCommandType::kLoginPasResponse);
    message->mutable_auth_data()->mutable_login_response()->set_login(login);
    message->mutable_auth_data()->mutable_login_response()->set_password(password);

    return message;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)	
}
    return nullptr;
}

template<>
C_ProtoPtr GameLogic::PrepareMessage<ProtoCommandType::kUsersAvatarListClientRequest>()
{
try
{
    ProtoPtr message = std::make_unique<ProtoMsg>();
    message->set_command(ProtoCommandType::kUsersAvatarListClientRequest);

    return message;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)	
}
    return nullptr;
}

template<>
C_ProtoPtr GameLogic::PrepareMessage<ProtoCommandType::kStartGameClientRequest>
    (const uint32_t& avatar_id)
{
try
{
    ProtoPtr message = std::make_unique<ProtoMsg>();
    message->set_command(ProtoCommandType::kStartGameClientRequest);
    message->mutable_start_game()->set_id_selected_avatar(avatar_id);

    return message;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
    return nullptr;
}

template<>
C_ProtoPtr GameLogic::PrepareMessage<ProtoCommandType::kExitGameClientRequest>()
{
try
{
    ProtoPtr message = std::make_unique<ProtoMsg>();
    message->set_command(ProtoCommandType::kExitGameClientRequest);

    auto message_avatar = message->mutable_exit_game()->mutable_avatar();

    message_avatar->set_level(current_avatar_->GetLevel());
    message_avatar->set_experience(current_avatar_->GetExperience());
    message_avatar->set_health(current_avatar_->GetHealth());
    message_avatar->set_mana(current_avatar_->GetMana());
    message_avatar->set_strength(current_avatar_->GetStrength());
    message_avatar->set_dexterity(current_avatar_->GetDexterity());
    message_avatar->set_intelligence(current_avatar_->GetIntelligence());
    message_avatar->set_location_id(current_avatar_->GetLocationId());
    message_avatar->set_pos_x(current_avatar_->GetPosition().first);
    message_avatar->set_pos_y(current_avatar_->GetPosition().second);

    return message;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
return nullptr;
}

template<>
C_ProtoPtr GameLogic::PrepareMessage<ProtoCommandType::kSendToCommonChat>
    (const std::string& text_message)
{
try
{
    ProtoPtr message = std::make_unique<ProtoMsg>();

    message->set_command(ProtoMessage::CommandType::kSendToCommonChat);
    message->mutable_chat_data()->set_text_message(text_message);

    return message;
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
    return nullptr;
}



template<>
void GameLogic::HandleCommand<CommandList::kMenu>()
{
try
{
    auto if_input_error = [this]() {
        view_.ShowTextWithNewLine(text_v::kMenuNumber);
        };
    const uint16_t offset = 1;
    const uint16_t max_number = static_cast<uint16_t>(EMainMenu::kEnd) - offset;

    view_.ShowMenu(main_menu_);
    view_.ShowTextWithNewLine(text_v::kMenuNumber);

    EMainMenu user_input = static_cast<EMainMenu>(
        data_provider_.GetInt(if_input_error, max_number));

    switch (user_input)
    {
    case EMainMenu::kSaveAndExit:            
        NotifyNewMessage(std::move(
            PrepareMessage<ProtoCommandType::kExitGameClientRequest>()));
        stop_get_command_ = true;
        break;
    case EMainMenu::kEnd:
        break;
    default:
        break;
    }
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}

template<>
void GameLogic::HandleCommand<CommandList::kMap>()
{
try
{
    current_location_->UpdateSymbolsOnMap(*current_avatar_);
    view_.ShowMap(*current_location_, avatar_and_location_);
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}

template<>
void GameLogic::HandleCommand<CommandList::kCurrentLocation>()
{
try
{
    view_.ShowLocDescription(*current_location_, avatar_and_location_);
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}