#include "Database.h"

namespace Forklift
{

Database::Database()
:db(filename)
{
    set_pragmas();

    begin();
    drop_tables();
    create_tables();
    commit();
}

void Database::set_pragmas()
{
    db << "PRAGMA FOREIGN_KEYS = ON;";
    db << "PRAGMA JOURNAL_MODE = WAL;";
    db << "PRAGMA SYNCHRONOUS = 0;";
}

void Database::begin()
{
    db << "BEGIN;";
}

void Database::commit()
{
    db << "COMMIT;";
}

void Database::drop_tables()
{
    db << "DROP TABLE IF EXISTS particles;";
}

void Database::create_tables()
{
    db << "\n\
CREATE TABLE IF NOT EXISTS particles\n\
(id     INTEGER NOT NULL PRIMARY KEY,\n\
 params BLOB,\n\
 f      REAL NOT NULL,\n\
 g      REAL NOT NULL);";

}

} // namespace
