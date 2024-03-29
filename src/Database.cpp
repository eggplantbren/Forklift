#include "Database.h"
#include <iostream>

namespace Forklift
{

std::mutex Database::write_mutex;

Database::Database()
:db(filename)
{
    std::cout << "Initialising database..." << std::flush;
    set_pragmas();

    begin();
    drop_tables();
    create_tables();
    commit();
    std::cout << "done." << std::endl;
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
(id                   INTEGER NOT NULL PRIMARY KEY,\n\
 num_particles_floor  INTEGER NOT NULL,\n\
 num_particles_stripe INTEGER NOT NULL,\n\
 mcmc_steps           INTEGER NOT NULL,\n\
 rng_seed             INTEGER NOT NULL,\n\
 num_threads          INTEGER NOT NULL);";

    db << "INSERT OR IGNORE INTO constants VALUES (1, ?, ?, ?, ?, ?);"
       << Config::num_particles_floor << Config::num_particles_stripe
       << Config::mcmc_steps << Config::rng_seed << Config::num_threads;

    db << "\n\
CREATE TABLE IF NOT EXISTS particles\n\
(stripe_id INTEGER NOT NULL,\n\
 iteration INTEGER NOT NULL,\n\
 params    BLOB,\n\
 x         REAL NOT NULL,\n\
 y         REAL NOT NULL,\n\
 xtb       REAL NOT NULL,\n\
 ytb       REAL NOT NULL,\n\
 include   BOOLEAN NOT NULL,\n\
 PRIMARY KEY (stripe_id, iteration))\n\
WITHOUT ROWID;";

    db << "CREATE INDEX IF NOT EXISTS x_idx ON particles (x, xtb);";
    db << "CREATE INDEX IF NOT EXISTS x_idx ON particles (x, xtb);";
    db << "CREATE INDEX IF NOT EXISTS include_stripe_idx ON particles (include, stripe_id, iteration, x, y);";
}

int Database::save_particle
    (int stripe_id, int iteration,
     const std::optional<std::vector<unsigned char>>& bytes,
     const Double& x, const Double& y)
{

    write_mutex.lock();
    db << "INSERT INTO particles VALUES (?, ?, ?, ?, ?, ?, ?, ?);"
       << stripe_id << iteration
       << bytes << x.get_value() << y.get_value()
       << x.get_tiebreaker() << y.get_tiebreaker()
       << true;

    int id;
    db << "SELECT LAST_INSERT_ROWID();" >> id;
    write_mutex.unlock();

    return id;
}


void Database::clear_above(const Double& xstar)
{
    db << "UPDATE particles SET include = 0 WHERE (x, xtb) >= (?, ?);"
       << xstar.get_value() << xstar.get_tiebreaker();
}

} // namespace
