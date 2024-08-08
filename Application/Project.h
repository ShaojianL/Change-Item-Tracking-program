// Project.h
#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <sqlite3.h>
#include "Database.h"
/**
 * Project module is designed to encapsulate the operations related to their respective entities. 
 * They parse the provided standard strings and other inputs, and then call the necessary database operations to perform CRUD on records. 
 * This approach helps in organizing the code and keeping the database operations encapsulated within these modules.
 * 
 * The modules Project, Release, Requester, ChangeItem, and ChangeRequest are primarily responsible for constructing SQL statements based on the input data they receive. 
 * They then pass these SQL statements to the Database module, which handles the execution of these statements.
 * 
 * This is why the Database.h header file is included in the Project.h header file.
 */
class Project {
public:
    static void addProject(sqlite3* db, 
                            const std::string& name, 
                            const std::string& description, 
                            const std::string& startDate, 
                            const std::string& endDate);

    static void updateProject(sqlite3* db, 
                                int projectId, 
                                const std::string& name, 
                                const std::string& description, 
                                const std::string& startDate, 
                                const std::string& endDate);

    static void deleteProject(sqlite3* db, int projectId);
    
    static void getProjects(sqlite3* db);
};

#endif // PROJECT_H
