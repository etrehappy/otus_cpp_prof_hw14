#pragma once

#include "avatar.h"
#include "location.h"
#include "npc.h"
#include "settings.h"

enum class ItemType : uint32_t
{
    kNone = 0
    , kDoorKey = 1
    , kEnd
};