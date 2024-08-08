// Project.cpp
// Revision History: 
// Version 1: 2024-07-17
#include "Project.h"
#include "Database.h"
#include <iostream>
#include <iomanip>
#include "PrintUtils.h"

// void Project::addProject(sqlite3* db, const std::string& name, const std::string& description, const std::string& startDate, const std::string& endDate) {
//     std::string sql = "INSERT INTO Project (ProjectName, Description, StartDate, EndDate) VALUES ('" + name + "', '" + description + "', '" + startDate + "', '" + endDate + "');";
//     Database::executeSQL(db, sql);
// }
// Use prepared statement for addProject
void Project::addProject(sqlite3* db, const std::string& name, const std::string& description, const std::string& startDate, const std::string& endDate) {
    const char* sql = "INSERT INTO Project (ProjectName, Description, StartDate, EndDate) VALUES (?, ?, ?, ?)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, startDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, endDate.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}
void Project::updateProject(sqlite3* db, int projectId, const std::string& name, const std::string& description, const std::string& startDate, const std::string& endDate) {
    std::string sql = "UPDATE Project SET ProjectName = '" + name + "', Description = '" + description + "', StartDate = '" + startDate + "', EndDate = '" + endDate + "' WHERE ProjectID = " + std::to_string(projectId);
    Database::executeSQL(db, sql);
}

void Project::getProjects(sqlite3* db) {
    std::string sql = "SELECT * FROM Project";
    sqlite3_stmt* stmt = Database::querySQL(db, sql);   

    // Print header
    printProjectHeaders();

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        const unsigned char* description = sqlite3_column_text(stmt, 2);
        const unsigned char* startDate = sqlite3_column_text(stmt, 3);
        const unsigned char* endDate = sqlite3_column_text(stmt, 4);

        std::cout << std::left
                  << std::setw(7) << id
                  << std::setw(20) << (name ? reinterpret_cast<const char*>(name) : "")
                  << std::setw(30) << (description ? reinterpret_cast<const char*>(description) : "")
                  << std::setw(15) << (startDate ? reinterpret_cast<const char*>(startDate) : "")
                  << std::setw(15) << (endDate ? reinterpret_cast<const char*>(endDate) : "")
                  << std::endl;
    }

    sqlite3_finalize(stmt);
}
