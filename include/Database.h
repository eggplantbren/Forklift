#ifndef Forklift_Database_h
#define Forklift_Database_h

#include "Config.h"
#include "Double.h"
#include <mutex>
#include <optional>
#include <sqlite_modern_cpp/hdr/sqlite_modern_cpp.h>
#include <vector>

namespace Forklift
{

class Database
{
    private:
        static constexpr auto filename = "output/forklift.db";
        static std::mutex write_mutex; // Only used in save_particle

        sqlite::database db;

        void begin();
        void commit();
        void create_tables();
        void drop_tables();
        void set_pragmas();

    public:
        Database();

        // Save a particle. Returns the rowid
        int save_particle
            (int stripe_id, int iteration,
             const std::optional<std::vector<unsigned char>>& bytes,
             const Double& x, const Double& y);

        // Delete some particles
        void clear_above(const Double& xstar);
};


} // namespace


#endif
