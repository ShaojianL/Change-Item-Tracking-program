// Database.cpp
// Revision History: 
// Version 1: 2024-07-17
#include "Database.h"
#include <iostream>

/////////////////////////////////////////////////////////////////////////////////
// Function to initialize the database
sqlite3* Database::initializeDatabase(const std::string& dbFileName) {
    sqlite3* db;
    if (sqlite3_open(dbFileName.c_str(), &db)) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }
    return db;
}

/////////////////////////////////////////////////////////////////////////////////
// Function to create tables for each object
void Database::createTables(sqlite3* db) {
    std::string sql = R"(
    CREATE TABLE IF NOT EXISTS Project (
        ProjectID INTEGER PRIMARY KEY AUTOINCREMENT,
        ProjectName TEXT NOT NULL,
        Description TEXT,
        StartDate DATE NOT NULL,
        EndDate DATE NOT NULL
    );

    CREATE TABLE IF NOT EXISTS ChangeItem (
        ChangeID INTEGER PRIMARY KEY AUTOINCREMENT,
        Title TEXT NOT NULL,
        Description TEXT NOT NULL,
        Priority INTEGER,
        Status TEXT NOT NULL,
        CreatedDate DATE NOT NULL,
        ResolvedDate DATE NOT NULL,
        AnticipatedRelease TEXT,
        ProjectID INTEGER,
        ReleaseID INTEGER,
        FOREIGN KEY (ProjectID) REFERENCES Project(ProjectID),
        FOREIGN KEY (ReleaseID) REFERENCES Release(ReleaseID)
    );

    CREATE TABLE IF NOT EXISTS Release (
        ReleaseID INTEGER PRIMARY KEY AUTOINCREMENT,
        ProjectID INTEGER,
        ReleaseNumber TEXT,
        ReleaseDate DATE NOT NULL,
        FOREIGN KEY (ProjectID) REFERENCES Project(ProjectID),
        UNIQUE(ProjectID, ReleaseNumber)
    );

    CREATE TABLE IF NOT EXISTS Requester ( 
        RequesterID INTEGER PRIMARY KEY AUTOINCREMENT,
        Name TEXT NOT NULL,
        Email TEXT NOT NULL,
        Phone TEXT NOT NULL,
        RequesterType TEXT NOT NULL
    );

    CREATE TABLE IF NOT EXISTS ChangeRequest (
        ChangeRequestID INTEGER PRIMARY KEY AUTOINCREMENT,
        RequestDate DATE NOT NULL,
        RequesterID INTEGER,
        ChangeID INTEGER,
        ReleaseID INTEGER,
        FOREIGN KEY (RequesterID) REFERENCES Requester(RequesterID),
        FOREIGN KEY (ChangeID) REFERENCES ChangeItem(ChangeID),
        FOREIGN KEY (ReleaseID) REFERENCES Release(ReleaseID)
    );
    )";

    executeSQL(db, sql);
}

/**
 * Function to execute an SQL statement
 * @param db The database connection
 * @param sql A const reference to a string containing the SQL statement
 * The function executes an SQL statement using the SQLite3 C API
 */

void Database::executeSQL(sqlite3* db, const std::string& sql) {
    char* errMsg = nullptr;
    // if errors in executing the SQL statement, print the error message
    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}


/**
 * Function to query the database using an SQL statement
 * @param db The database connection
 * @param sql A const reference to a string containing the SQL statement
 * @return A pointer to an `sqlite3_stmt` variable
 
 */
sqlite3_stmt* Database::querySQL(sqlite3* db, const std::string& sql) {

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }
    return stmt;
}
