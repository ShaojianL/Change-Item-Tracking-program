// Database.h
#ifndef DATABASE_H
#define DATABASE_H

#include<sqlite3.h>
#include<string>
/**
 * The Database module provides utility functions for initializing the database, creating tables, executing SQL statements, and querying the database. 
 * The other modules (Project, Release, Requester, ChangeItem, ChangeRequest) use these utility functions to interact with the database.
 * 
 * The modules Project, Release, Requester, ChangeItem, and ChangeRequest are primarily responsible for constructing SQL statements based on the input data they receive. 
 * They then pass these SQL statements to the Database module, which handles the execution of these statements.
 */
class Database {
    public:
        /**
         * Intialize the database and return a pointer to the database
         */
        static sqlite3* initializeDatabase (const std::string& databaseName);
        /**
         * Create the tables in the database
         */
        static void createTables(sqlite3* db);
        /**
         * Execute an SQL statement
         * The function executes an SQL statement using the SQLite3 C API
         */
        static void executeSQL(sqlite3* db, const std::string& sql);
        static sqlite3_stmt* querySQL(sqlite3* db, const std::string& sql);
};
#endif // DATABASE_H