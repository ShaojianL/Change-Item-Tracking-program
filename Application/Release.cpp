#include "Release.h"
#include "Database.h"
#include <iostream>
#include <iomanip>
#include "PrintUtils.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to add a new release to the database
void Release::addRelease(sqlite3* db, int projectId, const std::string& releaseNumber, const std::string& releaseDate) {
    // SQL query to insert a new record into the Release table
    std::string sql = "INSERT INTO Release (ProjectID, ReleaseNumber, ReleaseDate) VALUES (" + std::to_string(projectId) + ", " + releaseNumber + ", '" + releaseDate + "')";
    // std::cout << "SQL for addRelease: " << sql << std::endl; // Debug output 
    // Execute the SQL statement
    Database::executeSQL(db, sql);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to update the release date and release number of an existing release
void Release::updateRelease(sqlite3* db, int releaseId, const std::string& releaseNumber, const std::string& releaseDate) {
    // SQL query to update the ReleaseDate and ReleaseNumber column for a specific ReleaseID
    std::string sql = "UPDATE Release SET ReleaseNumber = " + releaseNumber + ", ReleaseDate = '" + releaseDate + "' WHERE ReleaseID = " + std::to_string(releaseId);
    // Execute the SQL statement
    Database::executeSQL(db, sql);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to delete a release from the database
void Release::deleteRelease(sqlite3* db, int releaseId) {
    // SQL query to delete a record from the Release table by ReleaseID
    std::string sql = "DELETE FROM Release WHERE ReleaseID = " + std::to_string(releaseId);
    // Execute the SQL statement
    Database::executeSQL(db, sql);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to retrieve and print all releases from the database
void Release::getReleases(sqlite3* db) {
    // SQL query to select ReleaseID, ReleaseNumber, ReleaseDate, and ProjectID from the Release table
    std::string sql = "SELECT ReleaseID, ReleaseNumber, ReleaseDate, ProjectID FROM Release";
    // Prepare the SQL statement
    sqlite3_stmt* stmt = Database::querySQL(db, sql);
    // Print header
    printReleaseHeaders();
    // Loop through each row in the result 
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Retrieve column values for the current row
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* releaseNumber = sqlite3_column_text(stmt, 1);
        const unsigned char* releaseDate = sqlite3_column_text(stmt, 2);
        int projectId = sqlite3_column_int(stmt, 3);

        // Print the retrieved values
        // use setw() to format the output
        std::cout << std::left << std::setw(10) << id << std::setw(15) << releaseNumber << std::setw(15) << releaseDate << std::setw(10) << projectId << std::endl;

    }

    // Finalize the statement to release resources
    sqlite3_finalize(stmt);
}

void Release::getReleases(sqlite3* db, int projectId) {
    std::string sql = "SELECT ReleaseID, ReleaseNumber, ReleaseDate FROM Release WHERE ProjectID = ?";
    sqlite3_stmt* stmt = Database::querySQL(db, sql);

    if (stmt == nullptr) {
        std::cerr << "Failed to prepare statement." << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, projectId);

    std::cout << "Releases for project ID " << projectId << ":\n";
    std::cout << std::left
              << std::setw(10) << "ID"
              << std::setw(20) << "Release Number"
              << std::setw(20) << "Release Date"
              << "\n";
    std::cout << std::string(50, '-') << "\n";

    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* releaseNumber = sqlite3_column_text(stmt, 1);
        const unsigned char* releaseDate = sqlite3_column_text(stmt, 2);
        std::cout << std::left
            << std::setw(10) << id
            << std::setw(20) << (releaseNumber ? reinterpret_cast<const char*>(releaseNumber) : "")
            << std::setw(20) << (releaseDate ? reinterpret_cast<const char*>(releaseDate) : "")
            << "\n";
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}
