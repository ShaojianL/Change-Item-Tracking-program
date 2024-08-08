// main.cpp
// Revision History:
// Revision 1: 2024-07-17
#include "Database.h"
#include "UserInterface.h"
#include <sqlite3.h>
#include <iostream>
#include <fstream>

/**
 * Check if the database file exists
 */

bool databaseExists(const std::string& dbFileName) {
    std::ifstream file(dbFileName);
    return file.good();
}

int main() {
    // Database file name
    std::string dbFileName = "test_change_tracking.db";

    bool dbExists = databaseExists(dbFileName);

    // Initialize the database
    sqlite3* db = Database::initializeDatabase(dbFileName);
    if (db == nullptr) {
        std::cerr << "Failed to initialize the database." << std::endl;
        return -1;
    }

    // Create tables
    if (!dbExists) {
        Database::createTables(db);
    }

    // User Interface
    UserInterface ui;
    ui.handleUserInput(db);

    // Close the database connection
    sqlite3_close(db);

    // Goodbye message
    std::cout << "Goodbye!" << std::endl;
    return 0;
}
