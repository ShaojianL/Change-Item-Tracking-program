// Requester.h
#ifndef REQUESTER_H
#define REQUESTER_H

#include <string>
#include <sqlite3.h>
#include "Database.h"
/**
 * Requester module is designed to encapsulate the operations related to their respective entities. 
 * They parse the provided standard strings and other inputs, and then call the necessary database operations to perform CRUD on records. 
 * This approach helps in organizing the code and keeping the database operations encapsulated within these modules.
 * 
 * The modules Project, Release, Requester, ChangeItem, and ChangeRequest are primarily responsible for constructing SQL statements based on the input data they receive. 
 * They then pass these SQL statements to the Database module, which handles the execution of these statements.
 * 
 * This is why the Database.h header file is included in the Requester.h header file.
 */
class Requester {
public:
    static void addRequester(sqlite3* db, const std::string& name, const std::string& email, const std::string& phone, const std::string& requesterType);
    static void updateRequester(sqlite3* db, int requesterId, const std::string& name, const std::string& email, const std::string& phone, const std::string& requesterType);
    static void deleteRequester(sqlite3* db, int requesterId);
    static void getRequesters(sqlite3* db);
};

#endif // REQUESTER_H
