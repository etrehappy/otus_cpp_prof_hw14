#pragma once

#include <memory>
#include <sqlite3.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include "game_elements.h"
#include "logger_s.h"
#include "settings.h"

class Database
{
public:
    Database();
    ~Database() = default;

    void Create() const;
    std::shared_ptr<SQLite::Database> GetToReadAndWrite() const;	

private:	
    void CreateTables(SQLite::Database&) const;

    /*!
    * @brief For MVP-version only
    */
    void SetTestData(SQLite::Database&) const;
    void OpenToReadAndWrite();

    std::shared_ptr<SQLite::Database> db_read_write_;
};

