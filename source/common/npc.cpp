#include "npc.h"

Npc::Npc(const uint32_t id, const std::string text_name, const uint32_t race_id
        , const std::string descr, const uint32_t health
        , const uint32_t location_id, const uint32_t x, const uint32_t y)
    : id_{id}, string_name_{std::move(text_name)}, race_id_{race_id}
    , description_{std::move(descr)}, health_{health}, location_id_{location_id}
    , position_{x, y}, name_{static_cast<NpcName>(id_)}
{

}

const std::string& Npc::GetTextName() const
{
    return string_name_;
}

const std::string& Npc::GetDescription() const
{
    return description_;
}

uint32_t Npc::GetLocationId() const
{
    return location_id_;
}

std::pair<uint32_t, uint32_t> Npc::GetPosition() const
{
    return position_;
}


NpcName Npc::GetEnumName() const
{
    return name_;
}
