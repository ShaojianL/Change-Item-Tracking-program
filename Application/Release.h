// Release.h
#ifndef RELEASE_H
#define RELEASE_H

#include <string>
#include <sqlite3.h>
#include "Database.h"
/**
 * Release module is designed to encapsulate the operations related to their respective entities. 
 * They parse the provided standard strings and other inputs, and then call the necessary database operations to perform CRUD on records. 
 * This approach helps in organizing the code and keeping the database operations encapsulated within these modules.
 * 
 * The modules Project, Release, Requester, ChangeItem, and ChangeRequest are primarily responsible for constructing SQL statements based on the input data they receive. 
 * They then pass these SQL statements to the Database module, which handles the execution of these statements.
 * 
 * This is why the Database.h header file is included in the Release.h header file.
 */
class Release {
public:
    static void addRelease(sqlite3* db, int projectId, const std::string& releaseNumber, const std::string& releaseDate);
    static void updateRelease(sqlite3* db, int releaseId, const std::string& releaseNumber, const std::string& releaseDate);
    static void deleteRelease(sqlite3* db, int releaseId);
    static void getReleases(sqlite3* db);
    static void getReleases(sqlite3* db, int projectId); // Overloaded function
};


#endif // RELEASE_H
