#include "ChangeItem.h"
#include "Database.h"
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <iomanip>
#include "PrintUtils.h"

struct ChangeItemStruct {
    int changeId;
    std::string title;
    std::string description;
    int priority;
    std::string status;
    std::string createdDate;
    std::string resolvedDate;
    std::string anticipatedRelease;
    int projectId;
    int releaseId;
    std::string releaseNumber; // Add this line
};

int getIntegerResponseTwo(std::string InputPrompt) {
    std::cout << InputPrompt;
    int userInput;
    while(true) {
        std::cin >> userInput;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid integer: ";
        } else {
            return userInput;
        }
    }
}

std::string convertToString(const unsigned char* text) {
    return reinterpret_cast<const char*>(text);
}

void ChangeItem::addChangeItem(sqlite3* db, const std::string& title, const std::string& description, int priority, const std::string& status, const std::string& createdDate, const std::string& resolvedDate, const std::string& anticipatedRelease, int projectId, int releaseId) {
    const char* sql = "INSERT INTO ChangeItem (Title, Description, Priority, Status, CreatedDate, ResolvedDate, AnticipatedRelease, ProjectID, ReleaseID) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, priority);
    sqlite3_bind_text(stmt, 4, status.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, createdDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, resolvedDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, anticipatedRelease.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 8, projectId);
    sqlite3_bind_int(stmt, 9, releaseId);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void ChangeItem::updateChangeItem(sqlite3* db, int changeId, const std::optional<std::string>& title, const std::optional<std::string>& description, const std::optional<int>& priority, const std::optional<std::string>& status, const std::optional<std::string>& resolvedDate, const std::optional<std::string>& anticipatedRelease) {
    std::ostringstream sql;
    sql << "UPDATE ChangeItem SET ";
    bool first = true;

    if (title.has_value()) {
        if (!first) sql << ", ";
        sql << "Title = ?";
        first = false;
    }
    if (description.has_value()) {
        if (!first) sql << ", ";
        sql << "Description = ?";
        first = false;
    }
    if (priority.has_value()) {
        if (!first) sql << ", ";
        sql << "Priority = ?";
        first = false;
    }
    if (status.has_value()) {
        if (!first) sql << ", ";
        sql << "Status = ?";
        first = false;
    }
    if (resolvedDate.has_value()) {
        if (!first) sql << ", ";
        sql << "ResolvedDate = ?";
        first = false;
    }
    if (anticipatedRelease.has_value()) {
        if (!first) sql << ", ";
        sql << "AnticipatedRelease = ?";
        first = false;
    }

    sql << " WHERE ChangeID = ?";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    int index = 1;

    if (title.has_value()) {
        sqlite3_bind_text(stmt, index++, title->c_str(), -1, SQLITE_STATIC);
    }
    if (description.has_value()) {
        sqlite3_bind_text(stmt, index++, description->c_str(), -1, SQLITE_STATIC);
    }
    if (priority.has_value()) {
        sqlite3_bind_int(stmt, index++, *priority);
    }
    if (status.has_value()) {
        sqlite3_bind_text(stmt, index++, status->c_str(), -1, SQLITE_STATIC);
    }
    if (resolvedDate.has_value()) {
        sqlite3_bind_text(stmt, index++, resolvedDate->c_str(), -1, SQLITE_STATIC);
    }
    if (anticipatedRelease.has_value()) {
        sqlite3_bind_text(stmt, index++, anticipatedRelease->c_str(), -1, SQLITE_STATIC);
    }

    sqlite3_bind_int(stmt, index, changeId);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

void ChangeItem::deleteChangeItem(sqlite3* db, int changeId) {
    std::string sql = "DELETE FROM ChangeItem WHERE ChangeID = " + std::to_string(changeId);
    Database::executeSQL(db, sql);
}

void ChangeItem::getChangeItems(sqlite3* db) {
    std::string sql = "SELECT ci.ChangeID, ci.Title, ci.Description, ci.Priority, ci.Status, ci.CreatedDate, ci.ResolvedDate, ci.AnticipatedRelease, ci.ProjectID, ci.ReleaseID, r.ReleaseNumber "
                      "FROM ChangeItem ci "
                      "LEFT JOIN Release r ON ci.ReleaseID = r.ReleaseID "
                      "ORDER BY ci.CreatedDate DESC";

    sqlite3_stmt* stmt = Database::querySQL(db, sql);

    if (!stmt) {
        std::cerr << "Failed to execute query for ChangeItem table." << std::endl;
        return;
    }

    std::vector<ChangeItemStruct> changeItemList;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ChangeItemStruct item;
        item.changeId = sqlite3_column_int(stmt, 0);
        item.title = convertToString(sqlite3_column_text(stmt, 1));
        item.description = convertToString(sqlite3_column_text(stmt, 2));
        item.priority = sqlite3_column_int(stmt, 3);
        item.status = convertToString(sqlite3_column_text(stmt, 4));
        item.createdDate = convertToString(sqlite3_column_text(stmt, 5));
        item.resolvedDate = convertToString(sqlite3_column_text(stmt, 6));
        item.anticipatedRelease = convertToString(sqlite3_column_text(stmt, 7));
        item.projectId = sqlite3_column_int(stmt, 8);
        item.releaseId = sqlite3_column_int(stmt, 9);
        item.releaseNumber = convertToString(sqlite3_column_text(stmt, 10));

        changeItemList.push_back(item);
    }

    int currentPage = 0;
    int changeItemListSize = changeItemList.size();

    while (true) {
        int startIndex = currentPage * 20;
        int endIndex = std::min(startIndex + 20, changeItemListSize);

        // Need a header for the output
        if (startIndex == 0) {
            printChangeItemHeaders();
        }

        for (int i = startIndex; i < endIndex; i++) {
            const auto& item = changeItemList[i];

            std::cout << std::left
                      << std::setw(5) << (i + 1)
                      << std::setw(12) << item.changeId
                      << std::setw(20) << item.title
                      << std::setw(30) << item.description
                      << std::setw(10) << item.priority
                      << std::setw(15) << item.status
                      << std::setw(15) << item.createdDate
                      << std::setw(15) << item.resolvedDate
                      << std::setw(20) << item.anticipatedRelease
                      << std::setw(12) << item.projectId
                      << std::setw(12) << item.releaseId
                      << std::setw(15) << item.releaseNumber
                      << std::endl;
        }

        if (endIndex < changeItemListSize) {
            std::cout << "1. Next Page" << std::endl;
            std::cout << "2. Exit" << std::endl;
        } else {
            std::cout << "1. Exit" << std::endl;
        }

        int choice = getIntegerResponseTwo("Enter your choice: ");

        if (choice == 1) {
            if (endIndex < changeItemListSize) {
                currentPage++;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    sqlite3_finalize(stmt);
}





void ChangeItem::getChangeItems(sqlite3* db, int projectId) {
    std::string sql = "SELECT ci.ChangeID, ci.Title, ci.Description, ci.Priority, ci.Status, ci.CreatedDate, ci.ResolvedDate, ci.AnticipatedRelease, ci.ProjectID, ci.ReleaseID, r.ReleaseNumber "
                      "FROM ChangeItem ci "
                      "LEFT JOIN Release r ON ci.ReleaseID = r.ReleaseID "
                      "WHERE ci.ProjectID = " + std::to_string(projectId) + 
                      " ORDER BY ci.CreatedDate DESC";

    sqlite3_stmt* stmt = Database::querySQL(db, sql);

    if (!stmt) {
        std::cerr << "Failed to execute query for ChangeItem table." << std::endl;
        return;
    }

    std::vector<ChangeItemStruct> changeItemList;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ChangeItemStruct item;
        item.changeId = sqlite3_column_int(stmt, 0);
        item.title = convertToString(sqlite3_column_text(stmt, 1));
        item.description = convertToString(sqlite3_column_text(stmt, 2));
        item.priority = sqlite3_column_int(stmt, 3);
        item.status = convertToString(sqlite3_column_text(stmt, 4));
        item.createdDate = convertToString(sqlite3_column_text(stmt, 5));
        item.resolvedDate = convertToString(sqlite3_column_text(stmt, 6));
        item.anticipatedRelease = convertToString(sqlite3_column_text(stmt, 7));
        item.projectId = sqlite3_column_int(stmt, 8);
        item.releaseId = sqlite3_column_int(stmt, 9);
        item.releaseNumber = convertToString(sqlite3_column_text(stmt, 10));

        changeItemList.push_back(item);
    }

    int currentPage = 0;
    int changeItemListSize = changeItemList.size();

    while (true) {
        int startIndex = currentPage * 20;
        int endIndex = std::min(startIndex + 20, changeItemListSize);

        for (int i = startIndex; i < endIndex; i++) {
            const auto& item = changeItemList[i];
            if(item.projectId != projectId) {
                continue;
            }
            // Need a header for the output
            if (i == 0) {
                printChangeItemHeaders();
            }
            // use setw to format the output, like in the previous function
            std::cout << std::left
            << std::setw(5) << (i + 1)
            << std::setw(12) << item.changeId
            << std::setw(20) << item.title
            << std::setw(30) << item.description
            << std::setw(10) << item.priority
            << std::setw(15) << item.status
            << std::setw(15) << item.createdDate
            << std::setw(15) << item.resolvedDate
            << std::setw(20) << item.anticipatedRelease
            << std::setw(12) << item.projectId
            << std::setw(12) << item.releaseId
            << std::setw(15) << item.releaseNumber
            << std::endl;

        }

        if (endIndex < changeItemListSize) {
            std::cout << "1. Next Page" << std::endl;
            std::cout << "2. Exit" << std::endl;
        } else {
            std::cout << "1. Exit" << std::endl;
        }

        int choice = getIntegerResponseTwo("Enter your choice: ");

        if (choice > 2) {
            std::cout << "Invalid choice. Returning to main menu" << std::endl;
            break;
        }
        if (choice == 1) {
            if (endIndex < changeItemListSize) {
                currentPage++;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    sqlite3_finalize(stmt);
}


void ChangeItem::getChangeItems(sqlite3* db, int projectId, int releaseId) {
    std::string sql = "SELECT ci.ChangeID, ci.Title, ci.Description, ci.Priority, ci.Status, ci.CreatedDate, ci.ResolvedDate, ci.AnticipatedRelease, ci.ProjectID, ci.ReleaseID, r.ReleaseNumber "
                      "FROM ChangeItem ci "
                      "LEFT JOIN Release r ON ci.ReleaseID = r.ReleaseID "
                      "WHERE ci.ProjectID = " + std::to_string(projectId) + " AND ci.ReleaseID = " + std::to_string(releaseId) + 
                      " ORDER BY ci.CreatedDate DESC";

    sqlite3_stmt* stmt = Database::querySQL(db, sql);
    if (!stmt) {
        std::cerr << "Failed to query change items." << std::endl;
        return;
    }

    std::vector<ChangeItemStruct> changeItemList;


    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ChangeItemStruct item;
        item.changeId = sqlite3_column_int(stmt, 0);
        item.title = convertToString(sqlite3_column_text(stmt, 1));
        item.description = convertToString(sqlite3_column_text(stmt, 2));
        item.priority = sqlite3_column_int(stmt, 3);
        item.status = convertToString(sqlite3_column_text(stmt, 4));
        item.createdDate = convertToString(sqlite3_column_text(stmt, 5));
        item.resolvedDate = convertToString(sqlite3_column_text(stmt, 6));
        item.anticipatedRelease = convertToString(sqlite3_column_text(stmt, 7));
        item.projectId = sqlite3_column_int(stmt, 8);
        item.releaseId = sqlite3_column_int(stmt, 9);
        item.releaseNumber = convertToString(sqlite3_column_text(stmt, 10));

        changeItemList.push_back(item);
    }

    int currentPage = 0;
    int changeItemListSize = changeItemList.size();

    while (true) {
        int startIndex = currentPage * 20;
        int endIndex = std::min(startIndex + 20, changeItemListSize);

        for (int i = startIndex; i < endIndex; i++) {
            const auto& item = changeItemList[i];

            // Need a header for the output
            if (i == 0) {
                printChangeItemHeaders();
            }
        }

        if (endIndex < changeItemListSize) {
            std::cout << "1. Next Page" << std::endl;
            std::cout << "2. Exit" << std::endl;
        } else {
            std::cout << "1. Exit" << std::endl;
        }

        int choice = getIntegerResponseTwo("Enter your choice: ");

        if (choice == 1) {
            if (endIndex < changeItemListSize) {
                currentPage++;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    sqlite3_finalize(stmt);
}

void ChangeItem::getInProgressChangeItems(sqlite3* db, int projectId) {
    std::string sql = "SELECT ci.ChangeID, ci.Title, ci.Description, ci.Priority, ci.Status, ci.CreatedDate, ci.ResolvedDate, ci.AnticipatedRelease, ci.ProjectID, ci.ReleaseID, r.ReleaseNumber "
                      "FROM ChangeItem ci "
                      "LEFT JOIN Release r ON ci.ReleaseID = r.ReleaseID "
                      "WHERE ci.ProjectID = " + std::to_string(projectId) + " AND ci.Status != 'Done' AND ci.Status != 'Cancelled' "
                      "ORDER BY ci.CreatedDate DESC";

    sqlite3_stmt* stmt = Database::querySQL(db, sql);

    if (!stmt) {
        std::cerr << "Failed to execute query for ChangeItem table." << std::endl;
        return;
    }

    std::vector<ChangeItemStruct> changeItemList;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ChangeItemStruct item;
        item.changeId = sqlite3_column_int(stmt, 0);
        item.title = convertToString(sqlite3_column_text(stmt, 1));
        item.description = convertToString(sqlite3_column_text(stmt, 2));
        item.priority = sqlite3_column_int(stmt, 3);
        item.status = convertToString(sqlite3_column_text(stmt, 4));
        item.createdDate = convertToString(sqlite3_column_text(stmt, 5));
        item.resolvedDate = convertToString(sqlite3_column_text(stmt, 6));
        item.anticipatedRelease = convertToString(sqlite3_column_text(stmt, 7));
        item.projectId = sqlite3_column_int(stmt, 8);
        item.releaseId = sqlite3_column_int(stmt, 9);
        item.releaseNumber = convertToString(sqlite3_column_text(stmt, 10));

        changeItemList.push_back(item);
    }

    int currentPage = 0;
    int changeItemListSize = changeItemList.size();

    while (true) {
        int startIndex = currentPage * 20;
        int endIndex = std::min(startIndex + 20, changeItemListSize);

        for (int i = startIndex; i < endIndex; i++) {
            const auto& item = changeItemList[i];

            // Need a header for the output
            if (i == 0) {
                printChangeItemHeaders();
            }
            // use setw to format the output
            std::cout << std::left
                      << std::setw(5) << (i + 1)
                      << std::setw(12) << item.changeId
                      << std::setw(20) << item.title
                      << std::setw(30) << item.description
                      << std::setw(15) << item.priority
                      << std::setw(15) << item.status
                      << std::setw(15) << item.createdDate
                      << std::setw(15) << item.resolvedDate
                      << std::setw(15) << item.anticipatedRelease
                      << std::setw(7) << item.projectId
                      << std::setw(7) << item.releaseId
                      << std::setw(15) << item.releaseNumber << std::endl;
        }

        if (endIndex < changeItemListSize) {
            std::cout << "1. Next Page" << std::endl;
            std::cout << "2. Exit" << std::endl;
        } else {
            std::cout << "1. Exit" << std::endl;
        }

        int choice = getIntegerResponseTwo("Enter your choice: ");

        if (choice == 1) {
            if (endIndex < changeItemListSize) {
                currentPage++;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    sqlite3_finalize(stmt);
}


void ChangeItem::getDoneChangeItems(sqlite3* db, int projectId, int releaseId) {
    std::string sql = "SELECT ci.ChangeID, ci.Title, ci.Description, ci.Priority, ci.Status, ci.CreatedDate, ci.ResolvedDate, ci.AnticipatedRelease, r.ReleaseNumber "
                      "FROM ChangeItem ci "
                      "LEFT JOIN Release r ON ci.ReleaseID = r.ReleaseID "
                      "WHERE ci.ProjectID = " + std::to_string(projectId) + 
                      " AND ci.ReleaseID = " + std::to_string(releaseId) + 
                      " AND ci.Status = 'DONE' "
                      "ORDER BY ci.CreatedDate DESC";

    sqlite3_stmt* stmt = Database::querySQL(db, sql);
    if (!stmt) {
        std::cerr << "Failed to query change items." << std::endl;
        return;
    }

    std::vector<ChangeItemStruct> changeItemList;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ChangeItemStruct item;
        item.changeId = sqlite3_column_int(stmt, 0);
        item.title = convertToString(sqlite3_column_text(stmt, 1));
        item.description = convertToString(sqlite3_column_text(stmt, 2));
        item.priority = sqlite3_column_int(stmt, 3);
        item.status = convertToString(sqlite3_column_text(stmt, 4));
        item.createdDate = convertToString(sqlite3_column_text(stmt, 5));
        item.resolvedDate = convertToString(sqlite3_column_text(stmt, 6));
        item.anticipatedRelease = convertToString(sqlite3_column_text(stmt, 7));
        item.releaseNumber = convertToString(sqlite3_column_text(stmt, 8));

        changeItemList.push_back(item);
    }

    std::cout << "\n--- Change Items with Status 'DONE' ---" << std::endl;
    for (const auto& item : changeItemList) {

        printChangeItemHeaders();

        // use setw to format the output
        std::cout << std::left
                  << std::setw(7) << item.changeId
                  << std::setw(20) << item.title
                  << std::setw(30) << item.description
                  << std::setw(15) << item.priority
                  << std::setw(15) << item.status
                  << std::setw(15) << item.createdDate
                  << std::setw(15) << item.resolvedDate
                  << std::setw(15) << item.anticipatedRelease
                  << std::setw(15) << item.releaseNumber << std::endl;
    }

    sqlite3_finalize(stmt);
}

void ChangeItem::getRequestersForChangeItem(sqlite3* db, int changeItemId) {
    std::string sql = "SELECT r.RequesterID, r.Name, r.Email, r.Phone, r.RequesterType "
                      "FROM Requester r "
                      "INNER JOIN ChangeRequest cr ON r.RequesterID = cr.RequesterID "
                      "WHERE cr.ChangeID = " + std::to_string(changeItemId);

    sqlite3_stmt* stmt = Database::querySQL(db, sql);
    if (!stmt) {
        std::cerr << "Failed to query requesters for change item." << std::endl;
        return;
    }

    std::cout << "\n Requesters for Change Item ID " << changeItemId << ": " << std::endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int requesterId = sqlite3_column_int(stmt, 0);
        std::string name = convertToString(sqlite3_column_text(stmt, 1));
        std::string email = convertToString(sqlite3_column_text(stmt, 2));
        std::string phone = convertToString(sqlite3_column_text(stmt, 3));
        std::string requesterType = convertToString(sqlite3_column_text(stmt, 4));

        // need a header for output
        std::cout << std::left
                  << std::setw(7) << "Requester ID"
                  << std::setw(20) << "Name"
                  << std::setw(30) << "Email"
                  << std::setw(15) << "Phone"
                  << std::setw(15) << "Requester Type" << std::endl;
        // use setw to format the output
        std::cout << std::left
                  << std::setw(7) << requesterId
                  << std::setw(20) << name
                  << std::setw(30) << email
                  << std::setw(15) << phone
                  << std::setw(15) << requesterType << std::endl;
    }

    sqlite3_finalize(stmt);
}

void ChangeItem::getChangeItem(sqlite3* db, int changeItemId) {
    std::string sql = "SELECT ci.ChangeID, ci.Title, ci.Description, ci.Priority, ci.Status, ci.CreatedDate, ci.ResolvedDate, ci.AnticipatedRelease, r.ReleaseNumber "
                      "FROM ChangeItem ci "
                      "LEFT JOIN Release r ON ci.ReleaseID = r.ReleaseID "
                      "WHERE ci.ChangeID = " + std::to_string(changeItemId);

    sqlite3_stmt* stmt = Database::querySQL(db, sql);
    if (!stmt) {
        std::cerr << "Failed to query change item." << std::endl;
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int changeId = sqlite3_column_int(stmt, 0);
        std::string title = convertToString(sqlite3_column_text(stmt, 1));
        std::string description = convertToString(sqlite3_column_text(stmt, 2));
        int priority = sqlite3_column_int(stmt, 3);
        std::string status = convertToString(sqlite3_column_text(stmt, 4));
        std::string createdDate = convertToString(sqlite3_column_text(stmt, 5));
        std::string resolvedDate = convertToString(sqlite3_column_text(stmt, 6));
        std::string anticipatedRelease = convertToString(sqlite3_column_text(stmt, 7));
        std::string releaseNumber = convertToString(sqlite3_column_text(stmt, 8));

        // need a header for output
        std::cout << std::left
                  << std::setw(7) << "Change ID"
                  << std::setw(20) << "Title"
                  << std::setw(30) << "Description"
                  << std::setw(15) << "Priority"
                  << std::setw(15) << "Status"
                  << std::setw(15) << "Created Date"
                  << std::setw(15) << "Resolved Date"
                  << std::setw(15) << "Anticipated Release"
                  << std::setw(15) << "Release Number" << std::endl;
        // use setw to format the output
        std::cout << std::left
                  << std::setw(7) << changeId
                  << std::setw(20) << title
                  << std::setw(30) << description
                  << std::setw(15) << priority
                  << std::setw(15) << status
                  << std::setw(15) << createdDate
                  << std::setw(15) << resolvedDate
                  << std::setw(15) << anticipatedRelease
                  << std::setw(15) << releaseNumber << std::endl;
    } else {
        std::cerr << "Change item not found." << std::endl;
    }

    sqlite3_finalize(stmt);
}

