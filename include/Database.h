#ifndef Forklift_Database_h
#define Forklift_Database_h

#include <sqlite_modern_cpp/hdr/sqlite_modern_cpp.h>

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

};


} // namespace


#endif
