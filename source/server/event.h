#pragma once

#include <string>
#include <memory>

#include "messages.pb.h"
#include "settings.h"
#include "game_elements.h"

class Event;
using ShrdEvent = std::shared_ptr<Event>;
using SharedMsg = std::shared_ptr<ProtoMessage::GameMessage>;

enum class EventPriority : unsigned short
{
    kNone = 0

    /* Events with these priority are handled the faster the high EventPriority.*/
    , kLow
    , kMedium
    , kHigh

    /* Events with thise priority are handled first. */
    , kError
    , kEndGame
};

enum class EventType : unsigned short
{
    kNone = 0
    
    /* UserHandler handles these events */
    , kFirstConnect
    , kSuccessCheckLogPas
    , kFailureCheckLogPas
    , kAvatarsDataReady    

    /* NetworkHandler handles these events */
    , kUserLogPasServerRequest
    , kAvatarListIsReady
    , kStartGameServerResponseSucsses 
    , kExitGameResponse
    
    /* DatabaseHandler handles these events */
    , kUserLogPasClientResponse
    , kUsersAvatarListClientRequest
    , kGetAvatarsData
    , kAddItemInInventory    
    , kSaveGame

    /* GameHandler handles these events */
    , kExitGameRequest

    /* Few handlers handle these events */
    , kStartGameClientRequest /*DatabaseHandler + GameHandler*/

    /* EventDispatcher routing these events */
    , kBroadcastMessage /* to the NetworkHandler */
    , kDisconnectSession /* to the session */

    , kEnd
};




class Event 
{
public:

    Event(EventPriority, EventType, const std::string&);
    virtual ~Event() = default;

    EventPriority GetPriority() const;
    EventType GetType() const;

    /*!
    * @brief event_routing_key_ == client's session_id_ for most events 
    *   except those handled by the DatabaseHandler, because in the MVP-version 
    *   the DatabaseHandler is one for all clients. 
    */
    const std::string& GetRoutingKey() const;

private:
    const EventPriority priority_;
    const EventType type_;
    const std::string& event_routing_key_;
};



/*!
* @brief Add these events: ClientSession.
*       Handles: UserHandler.
*       Does nothing useful in the MVP-version
*/
class FirstConnect : public Event
{
public:
    FirstConnect(const std::string&);
    ~FirstConnect() = default;
};

/*!
* @brief Add these events: UserHandler.
*       Handles:NetworkHandler* 
*/
class LoginRequest : public Event
{
public:
    LoginRequest(const std::string&);
    ~LoginRequest() = default;

};


/*!
* @brief Added: NetworkHandler.
*       Handles: DatabaseHandler
*/
class LogPasClientResponse : public Event
{
public:
    LogPasClientResponse(const SharedMsg
        , const std::string& session_id);
    ~LogPasClientResponse() = default;

    const SharedMsg GetEventMessage() const;

    /*!
    * @brief Used by DatabaseHandler to add this event in the common queue. 
    */
    const std::string& GetSessionId();

private:
    const SharedMsg message_;
    const std::string& session_id_;
}; 


/*!
* @brief Add these events: DatabaseHandler.
*       Handles: NetworkHandler in MVP-version, UserHandler
*/
class SuccessCheckLogPas : public Event
{
public:
    SuccessCheckLogPas(const std::string& session_id
        , std::shared_ptr<const std::string> uuid);
    ~SuccessCheckLogPas() = default;

    std::shared_ptr<const std::string> GetUuid();

private:
    std::shared_ptr<const std::string> uuid_;
};

/*!
* @brief Add these events: DatabaseHandler.
*       Handles: NetworkHandler in MVP-version
*/
class FailureCheckLogPas : public Event
{
public:
    FailureCheckLogPas(const std::string&);
    ~FailureCheckLogPas() = default;
};

/*!
* @brief Added: NetworkHandler.
*       Handles: DatabaseHandler
*/
class AvatarListClientRequest : public Event
{
public:
    AvatarListClientRequest(const std::string& session_id, const std::string& uuid);
    ~AvatarListClientRequest() = default;

    /*!
    * @brief Used by DatabaseHandler to add this event in the common queue.
    */
    const std::string& GetSessionId();
    const std::string& GetUuid();

private:
    const std::string& session_id_;
    const std::string& uuid_;
};

/*!
* @brief Added: DatabaseHandler.
*       Handles: NetworkHandler.
*/
class AvatarListIsReady : public Event
{
public:
    using PairIntStr = std::pair<int32_t, std::string>;

    AvatarListIsReady(const std::shared_ptr<std::vector<PairIntStr>>, const std::string&);
    ~AvatarListIsReady() = default;

    const std::shared_ptr<std::vector<PairIntStr>> GetAvatars();
        
private:
    const std::shared_ptr<std::vector<PairIntStr>> avatars_;
};

/*!
* @brief Added: NetworkHandler.
*       Handles: GameHandler
*/
class StartGameClientRequest : public Event
{
public:
    StartGameClientRequest(const std::string& session_id);
    ~StartGameClientRequest() = default;
};

/*!
* @brief Add these events: DatabaseHandler.
*       Handles: GameHandler
*/
class AvatarDataReady : public Event
{
public:
    AvatarDataReady(const std::string&, std::shared_ptr<Avatar>);
    ~AvatarDataReady() = default;

    std::shared_ptr<Avatar> MoveAvatar();
private:
    const std::shared_ptr<Avatar> avatar_;
};

/*!
* @brief Add these events: NetworkHandler.
*       Handles: DatabaseHandler
*/
class GetAvatarsData : public Event
{
public:
    GetAvatarsData(const std::string& session_id, const uint32_t id_selected_avatar);
    ~GetAvatarsData() = default;

    uint32_t GetIdSelectedAvatar();
    const std::string& GetSessionId();

private:
    const uint32_t avatar_id_;
    const std::string& session_id_;
};

/*!
* @brief Added: GameHandler.
*       Handles: NetworkHandler
*/
class StartGameServerResponseSucsses : public Event
{
public:
    StartGameServerResponseSucsses(const std::string& session_id, const std::shared_ptr<Avatar>);
    ~StartGameServerResponseSucsses() = default;

    const std::shared_ptr<const Avatar> GetConstAvatar() const;

private:
    const std::shared_ptr<const Avatar> avatar_;
};

/*!
* @brief Added: NetworkHandler.
*       Handles: DatabaseHandler
*/
class AddItemInInventory : public Event
{
public:
    AddItemInInventory(ItemType, uint32_t avatar_id);
    ~AddItemInInventory() = default;

    ItemType GetItem();
    uint32_t GetAvatarId();
private:
    ItemType item_;
    uint32_t avatar_id_;
};

/*!
* @brief Added: sender's NetworkHandler.
*       Handles: all client's NetworkHandler
*/
class BroadcastMessage : public Event
{
public:
    /*!
    * @brief EventDispatcher uses the session_id for filtering
    */
    BroadcastMessage(const SharedMsg, 
        const std::string& session_id);
    ~BroadcastMessage() = default;

    const SharedMsg GetEventMessage() const;
  
private:
    const SharedMsg message_;
};

/*!
* @brief Added: NetworkHandler.
*       Handles: GameHandler
*/
class ExitGameRequest : public Event
{
public:
    ExitGameRequest(const SharedMsg,  const std::string& session_id);
    ~ExitGameRequest() = default;

    const SharedMsg GetEventMessage() const;

private:
    const SharedMsg message_;
};

/*!
* @brief Added: GameHandler.
*       Handles: DatabaseHandler
*/
class SaveGame : public Event
{
public:
    SaveGame(const std::string& session_id, const std::shared_ptr<Avatar>
        , bool exit_game = false);
    ~SaveGame() = default;

    const std::string& GetSessionId();
    const std::shared_ptr<const Avatar> GetConstAvatar() const;
    bool IsExitGame();

private:
    const std::string& session_id_;
    const std::shared_ptr<const Avatar> avatar_;
    bool exit_game_;
};

/*!
* @brief Added: DatabaseHandler.
*       Handles: NetworkHandler
*/
class ExitGameResponse : public Event
{
public:
    ExitGameResponse(const std::string& session_id);
    ~ExitGameResponse() = default;
};

/*!
* @brief Added: DatabaseHandler.
*       Handles: NetworkHandler
*/
class DisconnectSession : public Event
{
public:
    DisconnectSession(const std::string session_id);
    ~DisconnectSession() = default;

private:
    const std::string session_id_;
};