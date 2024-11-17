#pragma once

#include <string>

/*!
*	@brief NpcName = id from database.
*/
enum NpcName
{
    /* Npc from the Tavern (Таверна) */
    kLora = 1
    , kAdrik = 2

    , kEnd
};

class Npc
{
public:

    Npc(const uint32_t id, const std::string text_name, const uint32_t race_id
        , const std::string descr, const uint32_t health, const uint32_t location_id
        , const uint32_t x, const uint32_t y);

    ~Npc() = default;

    const std::string& GetTextName() const;
    const std::string& GetDescription() const;
    uint32_t GetLocationId() const;
    std::pair<uint32_t, uint32_t> GetPosition() const;
    NpcName GetEnumName() const;

private:
    const uint32_t id_;
    const std::string string_name_;
    const uint32_t race_id_;
    const std::string description_;
    uint32_t health_;
    const uint32_t location_id_;
    const std::pair<const uint32_t, const uint32_t> position_;
    const NpcName name_;
};