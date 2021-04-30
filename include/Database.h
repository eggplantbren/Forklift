#ifndef Forklift_Database_h
#define Forklift_Database_h

#include <optional>
#include <sqlite_modern_cpp/hdr/sqlite_modern_cpp.h>
#include <vector>

namespace Forklift
{

class Database
{
    private:
        static constexpr auto filename = "output/forklift.db";
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
             double x, double y);
};


} // namespace


#endif
