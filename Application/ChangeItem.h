#ifndef CHANGEITEM_H
#define CHANGEITEM_H

#include <string>
#include <sqlite3.h>
#include <sstream>
#include <optional>

class ChangeItem {
public:
    static void addChangeItem(sqlite3* db, 
                              const std::string& title, 
                              const std::string& description, 
                              int priority, 
                              const std::string& status, 
                              const std::string& createdDate, 
                              const std::string& resolvedDate, 
                              const std::string& anticipatedRelease,
                              int projectId, 
                              int releaseId);
    
    static void updateChangeItem(sqlite3* db, int changeId, 
                                 const std::optional<std::string>& title = std::nullopt, 
                                 const std::optional<std::string>& description = std::nullopt, 
                                 const std::optional<int>& priority = std::nullopt, 
                                 const std::optional<std::string>& status = std::nullopt, 
                                 const std::optional<std::string>& resolvedDate = std::nullopt,
                                 const std::optional<std::string>& anticipatedRelease = std::nullopt);
    
    static void deleteChangeItem(sqlite3* db, int changeId);
    
    static void getChangeItems(sqlite3* db);
    static void getChangeItems(sqlite3* db, int projectId);
    static void getChangeItems(sqlite3* db, int projectId, int releaseId); 

    static void getChangeItem(sqlite3* db, int changeItemId);
    static void getInProgressChangeItems(sqlite3* db, int projectId);
    static void getDoneChangeItems(sqlite3* db, int projectId, int releaseId);
    static void getRequestersForChangeItem(sqlite3* db, int changeItemId);
};

#endif // CHANGEITEM_H
