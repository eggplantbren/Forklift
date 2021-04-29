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

int Database::save_particle
    (const std::optional<std::vector<unsigned char>>& bytes, double f, double g)
{
    db << "INSERT INTO particles (params, f, g) VALUES (?, ?, ?);"
       << bytes << f << g;

    int id;
    db << "SELECT LAST_INSERT_ROWID();" >> id;

    return id;
}

} // namespace
