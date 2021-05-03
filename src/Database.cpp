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
(stripe_id INTEGER NOT NULL,\n\
 iteration INTEGER NOT NULL,\n\
 params BLOB,\n\
 x      REAL NOT NULL,\n\
 y      REAL NOT NULL,\n\
 PRIMARY KEY (stripe_id, iteration))\n\
WITHOUT ROWID;";

}

int Database::save_particle
    (int stripe_id, int iteration,
     const std::optional<std::vector<unsigned char>>& bytes,
     double x, double y)
{
    db << "INSERT INTO particles VALUES (?, ?, ?, ?, ?);"
       << stripe_id << iteration << bytes << x << y;

    int id;
    db << "SELECT LAST_INSERT_ROWID();" >> id;

    return id;
}


void Database::clear_above(double xstar)
{
    db << "DELETE FROM particles WHERE x > ?;" << xstar;
}

} // namespace
