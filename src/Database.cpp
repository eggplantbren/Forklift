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
    db << "DROP INDEX IF EXISTS stripe_id_idx;";
    db << "DROP TABLE IF EXISTS particles;";
    db << "DROP TABLE IF EXISTS constants;";
}

void Database::create_tables()
{
    db << "\n\
CREATE TABLE IF NOT EXISTS constants\n\
(id            INTEGER NOT NULL PRIMARY KEY,\n\
 num_particles INTEGER NOT NULL,\n\
 mcmc_steps    INTEGER NOT NULL);";

    db << "INSERT OR IGNORE INTO constants VALUES (1, ?, ?);"
       << Constants::num_particles << Constants::mcmc_steps;


    db << "\n\
CREATE TABLE IF NOT EXISTS particles\n\
(stripe_id INTEGER NOT NULL,\n\
 iteration INTEGER NOT NULL,\n\
 params BLOB,\n\
 x      REAL NOT NULL,\n\
 y      REAL NOT NULL,\n\
 xtb    REAL NOT NULL,\n\
 ytb    REAL NOT NULL,\n\
 PRIMARY KEY (stripe_id, iteration))\n\
WITHOUT ROWID;";

    db << "CREATE INDEX IF NOT EXISTS stripe_id_idx ON particles (stripe_id);";
}

int Database::save_particle
    (int stripe_id, int iteration,
     const std::optional<std::vector<unsigned char>>& bytes,
     const Double& x, const Double& y)
{
    db << "INSERT INTO particles VALUES (?, ?, ?, ?, ?, ?, ?);"
       << stripe_id << iteration << bytes << x.get_value() << y.get_value()
       << x.get_tiebreaker() << y.get_tiebreaker();

    int id;
    db << "SELECT LAST_INSERT_ROWID();" >> id;

    return id;
}


void Database::clear_above(const Double& xstar)
{
    db << "DELETE FROM particles WHERE (x, xtb) >= (?, ?);"
       << xstar.get_value() << xstar.get_tiebreaker();
}

} // namespace
