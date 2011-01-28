include_directories(sqlite3)
build_sub_module("vendor" LuaSQLite/lsqlite3.c sqlite3/sqlite3.c)