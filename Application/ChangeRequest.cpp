// ChangeRequest.cpp
// Revision History: 
// Version 1: 2024-07-17
#include "ChangeRequest.h"
#include "Database.h"
#include <iostream>
#include "PrintUtils.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to retrieve and print all change requests from the database
#include "ChangeRequest.h"
#include "Database.h"
#include <iostream>
#include <iomanip>

void ChangeRequest::getChangeRequests(sqlite3* db) {
    std::string sql = "SELECT * FROM ChangeRequest";
    sqlite3_stmt* stmt = Database::querySQL(db, sql);

    if (!stmt) {
        std::cerr << "Failed to execute query for ChangeRequest table." << std::endl;
        return;
    }

    // Print header
    printChangeRequestHeaders();

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int changeRequestId = sqlite3_column_int(stmt, 0);
        const unsigned char* requestDate = sqlite3_column_text(stmt, 1);
        int requesterId = sqlite3_column_int(stmt, 2);
        int changeId = sqlite3_column_int(stmt, 3);
        int releaseId = sqlite3_column_int(stmt, 4);

        std::cout << std::left
                  << std::setw(20) << changeRequestId
                  << std::setw(20) << (requestDate ? reinterpret_cast<const char*>(requestDate) : "")
                  << std::setw(15) << requesterId
                  << std::setw(15) << changeId
                  << std::setw(15) << releaseId
                  << std::endl;
    }

    sqlite3_finalize(stmt);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to add a new change request to the database
void ChangeRequest::addChangeRequest(sqlite3* db, const std::string& requestDate, int requesterId, int changeId, int releaseId) {
    // SQL query to insert a new record into the ChangeRequest table
    std::string sql = "INSERT INTO ChangeRequest (RequestDate, RequesterID, ChangeID, ReleaseID) VALUES ('" + requestDate + "', " + std::to_string(requesterId) + ", " + std::to_string(changeId) + ", " + std::to_string(releaseId) + ")";
    // Execute the SQL statement
    Database::executeSQL(db, sql);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to update the request date of an existing change request
void ChangeRequest::updateChangeRequest(sqlite3* db, int changeRequestId, const std::string& requestDate) {
    // SQL query to update the RequestDate column for a specific ChangeRequestID
    std::string sql = "UPDATE ChangeRequest SET RequestDate = '" + requestDate + "' WHERE ChangeRequestID = " + std::to_string(changeRequestId);
    // Execute the SQL statement
    Database::executeSQL(db, sql);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to delete a change request from the database
void ChangeRequest::deleteChangeRequest(sqlite3* db, int changeRequestId) {
    // SQL query to delete a record from the ChangeRequest table by ChangeRequestID
    std::string sql = "DELETE FROM ChangeRequest WHERE ChangeRequestID = " + std::to_string(changeRequestId);
    // Execute the SQL statement
    Database::executeSQL(db, sql);
}
