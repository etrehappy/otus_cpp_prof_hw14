#include "location.h"
#include "../client/logger_c.h"

template<>
void Location::SetNpcStandartPos<LocationName::kTavern>()
{
try
{
    const auto npc_Lora_pos = npc_container_.find(NpcName::kLora)->second.GetPosition();
    positions_[map_symbols::kNpcLora] =
    {npc_Lora_pos.first, npc_Lora_pos.second};

    const auto npc_Adrik_pos = npc_container_.find(NpcName::kAdrik)->second.GetPosition();

    positions_[map_symbols::kNpcAdrik] =
    {npc_Adrik_pos.first, npc_Adrik_pos.second};
}
catch (std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}
}/* SetNpcStandartPos */

template<>
void Location::AddDoorOnMap<LocationName::kTavern>()
{
try
{
    location_map_[1][width_ - 2] = map_symbols::kDoor;
}
catch (const std::exception& e)
{
    MVP_LOG_EXEPTION(e)
}    
}/* AddDoorOnMap */