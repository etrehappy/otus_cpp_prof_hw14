#pragma once

#include <map>
#include <string>
#include <vector>

#include "avatar.h"
#include "npc.h"

namespace map_symbols
{
#ifdef _WIN32    
    #pragma warning(push) 
    #pragma warning(disable : 4100) 
#else
    #pragma GCC diagnostic push 
    #pragma GCC diagnostic ignored "-Wunused-variable"
#endif 

    static const char kWall{'#'};
    static const char kPlayer{'@'};
    static const char kEmpty{' '};
    static const char kDoor{'/'};

    static const char kNpcLora{'L'};
    static const char kNpcAdrik{'A'};

#ifdef _WIN32    
    #pragma warning(pop)
#else
    #pragma GCC diagnostic pop
#endif
}/* map_symbols */

/*!
*	@brief LocationName = id from database.
*/
enum class LocationName
{

    /* Таверна */
    kTavern = 1

    , kEnd
};

class Location
{
public:

    Location(int id, std::string s_name, std::string description
        , int x, int y, bool is_safe_zone
        , std::map<NpcName, Npc>&& npc_container);

    ~Location() =  default;

    int GetId() const;
    const std::string& GetName() const;
    const std::string& GetDescription() const;
    bool IsSafeZone() const;
    void InitPositions(const Avatar& current_avatar);
    void UpdateSymbolsOnMap(const Avatar& current_avatar);
    const std::vector<std::string>& GetMap() const;
    void SetNewPosOnMap(uint32_t x, uint32_t y, const char map_symbol);
    void ClearPosOnMap(uint32_t x, uint32_t y);

private:
    void CreateMap();

    template<LocationName L>
    void SetNpcStandartPos();

    template<LocationName L>
    void AddDoorOnMap();

    int id_;
    std::string string_name_;
    std::string description_;
    int width_, height_;
    bool is_safe_zone_;
    std::map<char, std::pair<uint32_t, uint32_t>> positions_;
    LocationName name_;
    std::map<NpcName, Npc> npc_container_;

    /* location_map[height][width] */
    std::vector<std::string> location_map_;
};
