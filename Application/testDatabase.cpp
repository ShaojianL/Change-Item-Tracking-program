// TestDatabase.cpp

/**
 * This is a test for the Database class.
 * g++ -o testDatabase Database.cpp TestDatabase.cpp -lsqlite3
./testDatabase

 */
#include "Database.h"
#include <iostream>

/**
 * Helper function to print the database contents.
 */
void printDatabase(sqlite3* db) {
    std::string sql = "SELECT name FROM sqlite_master WHERE type='table';";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string tableName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        std::cout << "Table: " << tableName << std::endl;

        std::string query = "SELECT * FROM " + tableName;
        sqlite3_stmt* tableStmt;
        sqlite3_prepare_v2(db, query.c_str(), -1, &tableStmt, nullptr);

        int cols = sqlite3_column_count(tableStmt);
        while (sqlite3_step(tableStmt) == SQLITE_ROW) {
            for (int col = 0; col < cols; col++) {
                std::cout << sqlite3_column_name(tableStmt, col) << ": " << sqlite3_column_text(tableStmt, col) << " ";
            }
            std::cout << std::endl;
        }
        sqlite3_finalize(tableStmt);
    }

    sqlite3_finalize(stmt);
}

/**
 * For each test functions, create a new database file, run the test, and then remove the database file. 
 *
 */
bool testInitializeDatabase() {
    sqlite3* db = Database::initializeDatabase("test.db");
    if (db) {
        sqlite3_close(db);
        std::remove("test.db");
        return true;
    }
    return false;
}

/**
 * initialize a database, create tables, and check if the table exists.
 
 */
bool testCreateTables() {
    sqlite3* db = Database::initializeDatabase("test.db");
    if (!db) return false;

    Database::createTables(db);

    std::string sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='Project';";
    sqlite3_stmt* stmt = Database::querySQL(db, sql);

    bool tableExists = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    printDatabase(db);
    sqlite3_close(db);
    std::remove("test.db");

    return tableExists;
}


bool testExecuteSQL() {
    sqlite3* db = Database::initializeDatabase("test.db");
    if (!db) return false;

    std::string sql = "CREATE TABLE TestTable (ID INTEGER PRIMARY KEY, Name TEXT);";
    Database::executeSQL(db, sql);

    sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='TestTable';";
    sqlite3_stmt* stmt = Database::querySQL(db, sql);

    bool tableExists = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    printDatabase(db);
    sqlite3_close(db);
    std::remove("test.db");

    return tableExists;
}

bool testQuerySQL() {
    sqlite3* db = Database::initializeDatabase("test.db");
    if (!db) return false;

    std::string sql = "CREATE TABLE TestTable (ID INTEGER PRIMARY KEY, Name TEXT);";
    Database::executeSQL(db, sql);

    sql = "INSERT INTO TestTable (Name) VALUES ('TestName');";
    Database::executeSQL(db, sql);

    sql = "SELECT Name FROM TestTable WHERE ID = 1;";
    sqlite3_stmt* stmt = Database::querySQL(db, sql);

    bool querySuccess = (sqlite3_step(stmt) == SQLITE_ROW && std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))) == "TestName");
    sqlite3_finalize(stmt);
    printDatabase(db);
    sqlite3_close(db);
    std::remove("test.db");

    return querySuccess;
}

int main() {
    if (testInitializeDatabase()) {
        std::cout << "Test Initialize Database: PASSED" << std::endl;
    } else {
        std::cout << "Test Initialize Database: FAILED" << std::endl;
    }

    if (testCreateTables()) {
        std::cout << "Test Create Tables: PASSED" << std::endl;
    } else {
        std::cout << "Test Create Tables: FAILED" << std::endl;
    }

    if (testExecuteSQL()) {
        std::cout << "Test Execute SQL: PASSED" << std::endl;
    } else {
        std::cout << "Test Execute SQL: FAILED" << std::endl;
    }

    if (testQuerySQL()) {
        std::cout << "Test Query SQL: PASSED" << std::endl;
    } else {
        std::cout << "Test Query SQL: FAILED" << std::endl;
    }

    return 0;
}
