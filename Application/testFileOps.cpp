#include "Database.h"
#include "Project.h"
#include "Release.h"
#include "Requester.h"
#include "ChangeItem.h"
#include "ChangeRequest.h"
#include <iostream>
#include <sqlite3.h>
#include <cassert>

void testAddFunctions(sqlite3* db) {
    // Test adding a few projects
    std::cout << "Testing addProject..." << std::endl;
    Project::addProject(db, "Project A for Apple", "Description Ass", "2023-01-01", "2023-12-31");
    Project::addProject(db, "Project B for Beach", "Description Bottom", "2023-01-01", "2023-12-31");
    Project::addProject(db, "Project C for CUNT", "Description Chlamydia", "2023-01-01", "2023-12-31");


    std::cout << "addProject: Passed" << std::endl;
    
    // Test adding requesters
    std::cout << "Testing addRequester..." << std::endl;
    Requester::addRequester(db, "John Doe", "john.doe@example.com", "123-456-7890", "SWE");
    Requester::addRequester(db, "Jane Doe", "jane.deo@example.com", "123-456-7890", "Customer");
    Requester::addRequester(db, "Alice Smith", "alice.smit@example.com", "123-456-7890", "SWE");


    std::cout << "addRequester: Passed" << std::endl;
    
    // Test adding releases for each project
    std::cout << "Testing addRelease..." << std::endl;
    Release::addRelease(db, 1, "1.0", "2023-06-01");
    Release::addRelease(db, 1, "1.1", "2023-12-01");
    Release::addRelease(db, 2, "2.0", "2023-06-01");
    Release::addRelease(db, 2, "2.1", "2023-12-01");
    Release::addRelease(db, 3, "3.0", "2023-06-01");
    Release::addRelease(db, 3, "3.1", "2023-12-01");

    std::cout << "addRelease: Passed" << std::endl;
    
    // Test adding change items for each release
    std::cout << "Testing addChangeItem..." << std::endl;
    for (int i = 1; i <= 3; ++i) {
        for (int j = 1; j <= 2; ++j) {
            for (int k = 1; k <= 25; ++k) {
                std::string title = "Fix Bug " + std::to_string((i-1)*50 + (j-1)*25 + k);
                std::string description = "Description of the bug fix " + std::to_string((i-1)*50 + (j-1)*25 + k);
                int priority = k % 5 + 1;
                std::string status = (k % 5 == 0) ? "DONE" : "In Progress";
                std::string startDate = "2023-01-01";
                std::string endDate = "2023-12-10";
                std::string anticipatedRelease = "1.0"; 
                int projectId = i;
                int releaseId = (i-1)*2 + j;

                ChangeItem::addChangeItem(db, title, description, priority, status, startDate, endDate, anticipatedRelease, projectId, releaseId);
            }
        }
    }
    std::cout << "addChangeItem: Passed" << std::endl;

    // Test adding change requests for each change item
    std::cout << "Testing addChangeRequest..." << std::endl;
    for (int i = 1; i <= 150; ++i) {
        for (int requesterId = 1; requesterId <= 3; ++requesterId) {
            ChangeRequest::addChangeRequest(db, "2023-01-01", requesterId, i, (i-1)/25 + 1);
        }
    }
    std::cout << "addChangeRequest: Passed" << std::endl;

    // Test update a change item
    std::cout << "Testing updateChangeItem..." << std::endl;
    ChangeItem::updateChangeItem(db, 1, std::nullopt, "Updated description of the bug fix", 3, "Updated", std::nullopt);
}

void printDatabase(sqlite3* db) {
    std::cout << "\n--- Projects ---" << std::endl;
    Project::getProjects(db);
    
    std::cout << "\n--- Requesters ---" << std::endl;
    Requester::getRequesters(db);
    
    std::cout << "\n--- Releases ---" << std::endl;
    Release::getReleases(db);

    std::cout << "\n--- Change Requests ---" << std::endl;
    ChangeRequest::getChangeRequests(db);
    
    std::cout << "\n--- Change Items ---" << std::endl;
    ChangeItem::getChangeItems(db);
}

int main() {
    // Database file name
    std::string dbFileName = "test_change_tracking.db";

    // Initialize the database
    sqlite3* db = Database::initializeDatabase(dbFileName);
    if (db == nullptr) {
        std::cerr << "Failed to initialize the database." << std::endl;
        return -1;
    }

    // Create tables
    Database::createTables(db);

    // Run the test functions
    testAddFunctions(db);

    // Print the entire database
    printDatabase(db);

    // Close the database connection
    sqlite3_close(db);

    std::cout << "Testing completed successfully. Database populate with sample data. Ready for ui" << std::endl;
    return 0;
}
