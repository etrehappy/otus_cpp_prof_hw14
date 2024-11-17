#pragma once

#include <memory>
#include <mutex>
#include <SQLiteCpp/SQLiteCpp.h>

#include "game_elements.h"

class DatabaseHandler
{
public:
	DatabaseHandler();
	~DatabaseHandler() =  default;

	std::shared_ptr<Location> SetLocation(const uint32_t, std::mutex&);

private:
	void OpenDb();
	std::map<NpcName, Npc> GetNpcListData(const uint32_t);

	std::shared_ptr <SQLite::Database> db_read_;

};

