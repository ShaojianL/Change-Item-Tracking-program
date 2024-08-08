// ChangeRequest.h
#ifndef CHANGEREQUEST_H
#define CHANGEREQUEST_H

#include <string>
#include <sqlite3.h>
#include "Database.h"
/**
 * ChangeRequest module is designed to encapsulate the operations related to their respective entities. 
 * They parse the provided standard strings and other inputs, and then call the necessary database operations to perform CRUD on records. 
 * This approach helps in organizing the code and keeping the database operations encapsulated within these modules.
 * 
 * The modules Project, Release, Requester, ChangeItem, and ChangeRequest are primarily responsible for constructing SQL statements based on the input data they receive. 
 * They then pass these SQL statements to the Database module, which handles the execution of these statements.
 * This is why the Database.h header file is included in the ChangeRequest.h header file.
 */
class ChangeRequest {
public:
    static void addChangeRequest(sqlite3* db, const std::string& requestDate, int requesterId, int changeId, int releaseId);
    static void updateChangeRequest(sqlite3* db, int changeRequestId, const std::string& requestDate);
    static void deleteChangeRequest(sqlite3* db, int changeRequestId);
    static void getChangeRequests(sqlite3* db);
};

#endif // CHANGEREQUEST_H
