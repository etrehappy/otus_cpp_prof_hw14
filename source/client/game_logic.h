#pragma once

#include <memory>
#include <mutex>
#include <thread>

#include "db_handler.h"
#include "game_elements.h"
#include "i_inputdata.h"
#include "menus.h"
#include "messages.pb.h"
#include "parser.h"
#include "view.h"

/*!
*	@brief This is a part of Controller. It job is to manage dfferent events in
*		the game (like an authorisation, an move, etc.).
*/
class GameLogic
{
public:
    using ProtoMsg = ProtoMessage::GameMessage;
    using C_ProtoPtr = std::unique_ptr<const ProtoMsg>;
    using ProtoPtr = std::unique_ptr<ProtoMsg>;
    using ProtoCommandType = ProtoMessage::CommandType;
    using ChatPairPtr = std::unique_ptr<std::pair<CommandList, ChatMessage>>;
    using GoPairPtr = std::unique_ptr<std::pair<CommandList, GoCommand>>;

    GameLogic(View&, DatabaseHandler&, IInputDataProvider&, IExit&);
    ~GameLogic();

    /*!
    *	@brief NetworkHandler observes a new message for a server
    */
    void RegisterSingleObserver(std::function<void(C_ProtoPtr)>);
    void SelectFunction(const ProtoMsg&);

private:
       
/* 1. Authentication */

    void SendLogPasResponse();
    std::pair<const std::string, const std::string> GetCredentials();
    void AuthSuccess();
    void AuthFailure();
    
/* 2. Сhoose an avatar */

    void HandlerChooseAvatar(const ProtoMsg&);
    uint32_t GetChoisedAvatarId(const ProtoMsg&);
    uint32_t FindAvatarId(uint16_t, const ProtoMsg&);

    /*!
    *	@brief MVP-version hasn't a realisation of it.
    */
    void CreateAvatar();

/* 3. Start a game */

    void SetAvatar(const ProtoMsg&);
    void HandlerStartGame(const ProtoMsg&);
    void StartGame();     
    void HandleCommandVariant(CommandList&);
    void HandleCommandVariant(ChatPairPtr&);  
    void HandleCommandVariant(GoPairPtr&);

    template <CommandList>
    void HandleCommand();

    void HandlerCommonMessage(const ProtoMsg&);

    /*!
    *	@brief If steps > 0 - move to east, if < 0 - move to west.
    */
    void MoveX(int32_t steps);
    /*!
    *	@brief If steps > 0 - move to south, if < 0 - move to north.
    */
    void MoveY(int32_t steps);

/* Everything else */

    void StopGame();
    void NotifyNewMessage(C_ProtoPtr );

    template<ProtoMessage::CommandType T, typename... Args>
    C_ProtoPtr PrepareMessage(Args&&... args);

    std::string uuid_;
    std::shared_ptr<Avatar> current_avatar_;
    std::shared_ptr<Location> current_location_;
    View& view_;
    DatabaseHandler& model_;
    IInputDataProvider& data_provider_;
    IExit& exit_;
    std::mutex avatar_and_location_{};
    std::function<void(C_ProtoPtr)> net_notify_;
    std::thread game_thread_;
    const std::shared_ptr<Parser> parser_;
    bool stop_get_command_;
    MainMenu main_menu_;
};

