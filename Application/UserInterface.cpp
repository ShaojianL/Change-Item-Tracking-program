#include "UserInterface.h"
#include "Database.h"
#include <sqlite3.h>
#include "Project.h"
#include "Requester.h"
#include "Release.h"
#include "ChangeRequest.h"
#include "ChangeItem.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <functional>
#include <limits>
#include <map>
using namespace std;

void UserInterface::showMenu(vector<string>& mainMenu) {
    cout << "Main Menu:" << endl;

    for (size_t i = 0; i < mainMenu.size(); i++) {
        cout << i + 1 << ". " << mainMenu[i] << endl;
    }
}

void UserInterface::printDatabase(sqlite3* db) {
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

int getIntegerResponse(string InputPrompt) {
    cout << InputPrompt;
    int userInput;
    while(true) {
        cin >> userInput;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid integer: ";
        } else {
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');  // Clear the input buffer
            return userInput;
        }
    }
}

string getStringResponse(const string& InputPrompt) {
    cout << InputPrompt;
    string userInput;
    while (true) {
        getline(cin, userInput);
        if (userInput.empty()) {
            cout << "Invalid input. Please enter a valid string: ";
        } else {
            return userInput;
        }
    }
}

string getDescription(string InputPrompt) {
    string description;
    cout << InputPrompt;
    while(true) {
        getline(cin, description);

        if(description.length() <= 30) {
            return description;
        } else {
            cout <<"Description exceeds 30 characters. Please enter a shorter description: ";
        }
    }
}

string getDate(string InputPrompt) {
    int year, month, day;
    char dash1, dash2;
    string date;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    cout << InputPrompt;
    while(true) {
        getline(cin, date);
        int checker = 0;
        istringstream iss(date);
        if (!(iss >> year >> dash1 >> month >> dash2 >> day)) {
            cout <<"Date is invalid. Please enter a valid date: ";
            continue;
        }
        iss >> year >> dash1 >> month >> dash2 >> day;
        if(year < 1 || year > 9999) {
            checker++;
        }
        if(month < 1 || month > 12) {
            checker++;
        }
        if(month == 2) {
            bool leapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            if(leapYear) {
                daysInMonth[1] = 29;
            }
        }
        if(day < 1 || day > daysInMonth[month - 1]) {
            checker++;
        }
        if(checker > 0) {
            cout <<"Date is invalid. Please enter a valid date: ";
        } else {
            ostringstream finalDate;
            finalDate << setw(4) << setfill('0') << year << "-"
            << setw(2) << setfill('0') << month << "-"
            << setw(2) << setfill('0') << day;
            return finalDate.str();
        }
    } 
}

string getEmail(string InputPrompt) {
    string email;
    cout << InputPrompt;
    while(true) {
        int checker = 0;
        getline(cin, email);

        std::string::size_type atPos = email.find("@");
        if(atPos == string::npos) {
            checker++;
        }
        if(atPos == 0) {
            checker++;
        }
        string halfEmail = email.substr(atPos + 2);
        std::string::size_type periodPos = halfEmail.find(".");
        if(periodPos == string::npos) {
            checker++;
        }
        if(checker > 0) {
            cout << "Email is invalid. Please enter valid email: ";
        } else {
            return email;
        }
    }
}

bool isNumber(const string& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

string getPhoneNumber(string InputPrompt) {
    string part1, part2, part3;
    char dash1, dash2;
    string phoneNumber;

    cout << InputPrompt;
    while(true) {
        getline(cin, phoneNumber);
        istringstream iss(phoneNumber);
        if(phoneNumber.length() == 12) {
            dash1 = phoneNumber[3];
            dash2 = phoneNumber[7];
            if(!(dash1 == '-' && dash2 == '-')) {
                cout << "Phone number is invalid. Please enter a valid phone number: ";
                continue;
            }
        } else {
            cout << "Phone number is invalid. Please enter a valid phone number: ";
            continue;
        }
        if (!getline(iss, part1, '-') || !getline(iss, part2, '-') || !getline(iss, part3)) {
            cout << "Phone number is invalid. Please enter a valid phone number: ";
            continue;
        }

        if (dash1 == '-' && dash2 == '-' && part1.length() == 3 && part2.length() == 3 && part3.length() == 4 &&
            isNumber(part1) && isNumber(part2) && isNumber(part3)) {
            break;  // Valid phone number, exit the loop
        } else {
            cout << "Phone number is invalid. Please enter a valid phone number: ";
        }
    }
    return phoneNumber;
}

void UserInterface::handleUserInput(sqlite3* db) {
    vector<string> mainMenu = {
        "Create",
        "Update",
        "Reports",
        "Exit"
    };

    vector<function<void()>> actions = {
        bind(&UserInterface::createMenu, this, db),
        bind(&UserInterface::updateMenu, this, db),
        bind(&UserInterface::reportMenu, this, db),
        []() { cout << "Exiting..." << endl; }
    };

    int choice;
    do {
        showMenu(mainMenu);
        cout << "Enter your choice: ";
        if(cin >> choice) {
            if (choice >= 1 && choice <= static_cast<int>(actions.size())) {
                actions[choice - 1]();
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number" << endl;
        }
    } while (choice != static_cast<int>(actions.size()));
}

void UserInterface::createMenu(sqlite3* db) {
    vector<string> createMenu = {
        "Create Project",
        "Create Requester",
        "Create Release",
        "Create Change Request",
        "Return to Main Menu"
    };

    vector<function<void()>> actions = {
        bind(&UserInterface::createProject, this, db),
        bind(&UserInterface::createRequester, this, db),
        bind(&UserInterface::createRelease, this, db),
        bind(&UserInterface::createChangeRequest, this, db),
        []() {}
    };

    int choice;
    do {
        showMenu(createMenu);
        choice = getIntegerResponse("Enter your choice: ");
        if (choice >= 1 && choice <= static_cast<int>(actions.size())) {
            actions[choice - 1]();
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != static_cast<int>(actions.size()) && choice != 5);
}

void UserInterface::updateMenu(sqlite3* db) {
    vector<string> updateMenu = {
        "Update Change Item",
        "Return to Main Menu"
    };

    vector<function<void()>> actions = {
        bind(&UserInterface::updateChangeItem, this, db),
        []() {}
    };

    int choice;
    do {
        showMenu(updateMenu);
        choice = getIntegerResponse("Enter your choice: ");
        if (choice >= 1 && choice <= static_cast<int>(actions.size())) {
            actions[choice - 1]();
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != static_cast<int>(actions.size()) && choice != 2);
}

void UserInterface::reportMenu(sqlite3* db) {
    vector<string> reportMenu = {
        "Change Items for Future Completion",
        "Requesters to be Contacted",
        "Return to Main Menu"
    };

    vector<function<void()>> actions = {
        bind(&UserInterface::changeItemsForFutureCompletion, this, db),
        bind(&UserInterface::requestersToBeContacted, this, db),
        []() {}
    };

    int choice;
    do {
        showMenu(reportMenu);
        choice = getIntegerResponse("Enter your choice: ");
        if (choice >= 1 && choice <= static_cast<int>(actions.size())) {
            actions[choice - 1]();
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != static_cast<int>(actions.size()) && choice != 3);
}

void UserInterface::createProject(sqlite3* db) {
    string name, description, startDate, endDate;
    cin.ignore();
    name = getStringResponse("Enter project name: ");
    description = getDescription("Enter project description: ");
    startDate = getDate("Enter project start date (YYYY-MM-DD): ");
    endDate = getDate("Enter project estimated end date (YYYY-MM-DD): ");
    Project::addProject(db, name, description, startDate, endDate);

    int projectId = sqlite3_last_insert_rowid(db);
    string releaseNumber = "0.0";
    string releaseDate = startDate;

    Release::addRelease(db, projectId, releaseNumber, releaseDate);
    
    cout << "Project created successfully with name " << name << "!" << endl;
    cout << "Pre-release " << releaseNumber << " created successfully with release date " << releaseDate << "!" << endl;
}

void UserInterface::createRequester(sqlite3* db) {
    string name, email, phone, requesterType;
    cin.ignore();
    name = getStringResponse("Enter requester name: ");
    email = getEmail("Enter requester email: ");
    phone = getPhoneNumber("Enter requester phone number (xxx-xxx-xxxx): ");
    cout << "Enter requester type (Customer, SWE, Marketing, Sales): ";
    getline(cin, requesterType);
    Requester::addRequester(db, name, email, phone, requesterType);
    cout << "Requester created successfully with name " << name << "!" << endl;
}

void UserInterface::createRelease(sqlite3* db) {
    int selectedProjectId;
    string releaseNumber;
    string releaseDate;

    Project::getProjects(db);

    selectedProjectId = getIntegerResponse("Enter project ID: ");
    
    Release::getReleases(db, selectedProjectId);
    cin.ignore();
    releaseNumber = getStringResponse("Enter a release number: ");
    releaseDate = getDate("Enter release date (YYYY-MM-DD): ");

    Release::addRelease(db, selectedProjectId, releaseNumber, releaseDate);
    cout << "Release created successfully for project ID " << selectedProjectId << " with release number " << releaseNumber << "!" << endl;
}

void UserInterface::createChangeRequest(sqlite3* db) {
    Project::getProjects(db);
    int selectedProjectId = getIntegerResponse("Enter project ID: ");

    Release::getReleases(db, selectedProjectId);
    int selectedReleaseId = getIntegerResponse("Enter release ID: ");

    string title, description, status, createdDate, endDate;
    int priority; 
    std::string anticipatedRelease;

    cin.ignore();
    title = getStringResponse("Enter change item title: ");
    description = getDescription("Enter change item description: ");
    priority = getIntegerResponse("Enter priority (1-5): ");
    while (true) {
        if (priority < 1 || priority > 5) {
            priority = getIntegerResponse("Enter valid priority (1-5): ");
        } else {
            break;
        }
    }
    status = getStringResponse("Enter status: ");
    createdDate = getDate("Enter created date (YYYY-MM-DD): ");
    endDate = getDate("Enter end date (YYYY-MM-DD): ");
    anticipatedRelease = getStringResponse("Enter anticipated release: ");

    ChangeItem::addChangeItem(db, title, description, priority, status, createdDate, endDate, anticipatedRelease, selectedProjectId, selectedReleaseId);
    int newChangeItemId = sqlite3_last_insert_rowid(db);
    cout << "Change item, " << title << ", created successfully!" << endl;

    vector<int> requesterIds;
    string addRequesterChoice;

    do {
        cout << "Do you want to add a new requester or assign an existing one? (new/existing): ";
        getline(cin, addRequesterChoice);

        if (addRequesterChoice == "new") {
            createRequester(db);
            int newRequesterId = sqlite3_last_insert_rowid(db);
            requesterIds.push_back(newRequesterId);
        } else if (addRequesterChoice == "existing") {
            Requester::getRequesters(db);
            int selectedRequesterId = getIntegerResponse("Enter requester ID: ");
            requesterIds.push_back(selectedRequesterId);
        } else {
            cout << "Invalid choice. Please enter 'new' or 'existing'." << endl;
        }

        cout << "Do you want to add another requester? (yes/no): ";
        getline(cin, addRequesterChoice);
    } while (addRequesterChoice == "yes");

    string requestDate = getDate("Enter request date (YYYY-MM-DD): ");

    for (int requesterId : requesterIds) {
        ChangeRequest::addChangeRequest(db, requestDate, requesterId, newChangeItemId, selectedReleaseId);
    }
    
    cout << "Change request created successfully!" << endl;
}

void UserInterface::updateChangeItem(sqlite3* db) {
    int selectedProjectId = getIntegerResponse("Enter project ID: ");
    ChangeItem::getChangeItems(db, selectedProjectId);

    int changeItemId;
    std::optional<std::string> title = std::nullopt;
    std::optional<std::string> description = std::nullopt;
    std::optional<std::string> status = std::nullopt;
    std::optional<std::string> resolvedDate = std::nullopt;
    std::optional<int> priority = std::nullopt;
    std::string input;

    changeItemId = getIntegerResponse("Enter change item ID: ");

    std::cout << "Do you want to update the title? (y/n): ";
    std::getline(std::cin, input);
    if (input == "y") {
        std::string newTitle;
        newTitle = getStringResponse("Enter new title: ");
        title = newTitle;
    }

    std::cout << "Do you want to update the description? (y/n): ";
    std::getline(std::cin, input);
    if (input == "y") {
        std::string newDescription;
        newDescription = getDescription("Enter new description: ");
        description = newDescription;
    }

    std::cout << "Do you want to update the priority? (y/n): ";
    std::getline(std::cin, input);
    if (input == "y") {
        int newPriority;
        newPriority = getIntegerResponse("Enter new priority: ");
        while(true) {
            if(newPriority < 1 || newPriority > 5) {
                newPriority = getIntegerResponse("Enter valid priority (1-5): ");
            } else {
                break;
            }
        }
        priority = newPriority;
    }

    cout << "Do you want to update the status? (y/n): ";
    getline(std::cin, input);
    if (input == "y") {
        std::string newStatus;
        newStatus = getStringResponse("Enter new status: ");
        status = newStatus;
    }

    std::cout << "Do you want to update the resolved date? (y/n): ";
    std::getline(std::cin, input);
    if (input == "y") {
        std::string newResolvedDate;
        newResolvedDate = getDate("Enter new resolved date: ");
        resolvedDate = newResolvedDate;
    }

    ChangeItem::updateChangeItem(db, changeItemId, title, description, priority, status, resolvedDate);

    std::cout << "Change item, " << changeItemId << ", updated successfully!" << std::endl;
}

void UserInterface::changeItemsForFutureCompletion(sqlite3* db) {
    Project::getProjects(db);
    int selectedProjectId = getIntegerResponse("Enter project ID: ");
    ChangeItem::getInProgressChangeItems(db, selectedProjectId);

    cout << "Press enter to return to the main menu." << endl;
    cin.ignore();
    cin.get();
}

void UserInterface::requestersToBeContacted(sqlite3* db) {
    std::cout << "\n--- Projects ---" << std::endl;
    Project::getProjects(db);

    int selectedProjectId = getIntegerResponse("Enter project ID: ");

    std::cout << "\n--- Releases ---" << std::endl;
    Release::getReleases(db, selectedProjectId);

    int selectedReleaseId = getIntegerResponse("Enter release ID: ");

    ChangeItem::getDoneChangeItems(db, selectedProjectId, selectedReleaseId);

    int selectedChangeItemId = getIntegerResponse("Enter change item ID: ");

    std::cout << "\n--- Requester to be Contacted for Change Item " << selectedChangeItemId << " ---" << std::endl;
    ChangeItem::getChangeItem(db, selectedChangeItemId);

    ChangeItem::getRequestersForChangeItem(db, selectedChangeItemId);

    std::cout << "Press enter to return to the main menu." << std::endl;
    std::cin.ignore();
    std::cin.get();
}
