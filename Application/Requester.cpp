// Requester.cpp
#include "Requester.h"
#include "Database.h"
#include <iostream>
#include <iomanip>
#include "PrintUtils.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to add a new requester to the database
void Requester::addRequester (sqlite3* db, const std::string& name, const std::string& email, const std::string& phone, const std::string& requesterType) {
    // SQL query to insert a new record into the requester table
    std::string sql = "INSERT INTO requester (name, email, phone, RequesterType) VALUES ('" + name + "', '" + email + "', '" + phone + "', '" + requesterType + "');";
    // Execute the SQL statement
    Database::executeSQL(db, sql);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to update an existing requester in the database
void Requester::updateRequester (sqlite3* db, int requesterId, const std::string& name, const std::string& email, const std::string& phone, const std::string& requesterType) {
    // SQL query to update the requester record with the given requester_id
    std::string sql = "UPDATE requester SET name = '" + name + "', email = '" + email + "', phone = '" + phone + "', RequesterType = '" + requesterType + "' WHERE requester_id = " + std::to_string(requesterId);
    // Execute the SQL statement
    Database::executeSQL(db, sql);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to delete a requester from the database
void Requester::deleteRequester (sqlite3* db, int requesterId) {
    // SQL query to delete the requester record with the given requester_id
    std::string sql = "DELETE FROM requester WHERE requester_id = " + std::to_string(requesterId);
    // Execute the SQL statement
    Database::executeSQL(db, sql);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to retrieve and print all requesters from the database
void Requester::getRequesters (sqlite3* db) {
    // SQL query to select all columns from the requester table
    std::string sql = "SELECT * FROM requester";
    // Prepare the SQL statement
    sqlite3_stmt* stmt = Database::querySQL(db, sql);

    // Print header
    printRequesterHeaders();

    // Loop through each row in the result set
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Retrieve column values for the current row
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        const unsigned char* email = sqlite3_column_text(stmt, 2);
        const unsigned char* phone = sqlite3_column_text(stmt, 3);
        const unsigned char* requesterType = sqlite3_column_text(stmt, 4);

        // use setw() to format the output
        std::cout << std::left 
        << std::setw(5) << id 
        << std::setw(15) << name 
        << std::setw(30) << email 
        << std::setw(15) << phone 
        << std::setw(10) << requesterType 
        << std::endl;
        
    }

    // Finalize the statement to release resources
    sqlite3_finalize(stmt);
}
