#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <sqlite3.h>
#include <vector>
#include <string>
#include <optional>
#include "Project.h"
#include "Requester.h"
#include "Release.h"
#include "ChangeRequest.h"
#include "ChangeItem.h"
#include "Database.h"

/**
 * The UserInterface module is responsible for handling the interactions with the user. 
 * It provides a text-based interface to allow the user to perform various operations on the database, 
 * such as creating projects, requesters, releases, change items, and change requests, 
 * as well as viewing and updating change items and generating reports.
 * 1. Displaying Menus: Show the main menu and sub-menus to the user.
 * 2. Handling User Input: Process the user input and call the appropriate functions to perform the desired operations.
 * 3. Creating: Allow the user to create a new project, release, change item etc, by providing the necessary information.
 * 4. generating reports: Allow the user to generate reports based on the data in the database.
 *  
 * Example flow
 * 
 * When a user selects an option to create new project:
 *  1. The UserInterface module calls Project::addProject() with the provided project details
 *  2. Project::addProject() constructs the SQL statement `INSERT` statement and calls
 *     Database::executeSQL() to insert the new project record into the database.
 * 
 * When a user selects an option to generate a report:
 *  1. The UserInterface module calls Project::getProjects() constructs the SQL `SELECT` statement and calls
 *     Database::querySQL() to retrieve the project records from the database.
 * 
 */
class UserInterface {
public:
    void showMenu(std::vector<std::string>& menuOptions);
    void handleUserInput(sqlite3* db);
    void printDatabase(sqlite3* db);

private:
    void createMenu(sqlite3* db);
    void updateMenu(sqlite3* db);
    void reportMenu(sqlite3* db);

    void createProject(sqlite3* db);
    void createRequester(sqlite3* db);
    void createRelease(sqlite3* db);
    void createChangeRequest(sqlite3* db);

    void updateChangeItem(sqlite3* db);

    void changeItemsForFutureCompletion(sqlite3* db);
    void requestersToBeContacted(sqlite3* db);
};

#endif // USERINTERFACE_H
