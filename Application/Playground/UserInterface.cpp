// UserInterface.cpp

#include "UserInterface.h"
#include "Project.h"
#include "ChangeItem.h"
#include "Release.h"
#include "Requester.h"
#include "ChangeRequest.h"
#include <iostream>
#include <string>
#include <vector>
#include <functional>
using namespace std;

void UserInterface::showMenu(vector<string>& menuOptions) {
    menuOptions = {
        "Create Project",
        "Create Requester",
        "Create Release",
        "Create Change Request or Change Item",
        "View or Update Change Item",
        "Produce Report",
        "Exit"
    };

    for (size_t i = 0; i < menuOptions.size(); i++) {
        cout << i + 1 << ". " << menuOptions[i] << endl;
    }
}

void UserInterface::handleUserInput(sqlite3* db) {
    vector<string> menuOptions;
    vector<function<void()>> actions = {
        bind(&UserInterface::createProject, this, db),
        bind(&UserInterface::createRequester, this, db),
        bind(&UserInterface::createRelease, this, db),
        bind(&UserInterface::createChangeRequestOrItem, this, db),
        bind(&UserInterface::viewOrUpdateChangeItem, this, db),
        bind(&UserInterface::produceReport, this, db),
        []() { cout << "Exiting..." << endl; }
    };

    int choice;
    do {
        showMenu(menuOptions);
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice >= 1 && choice <= static_cast<int>(actions.size())) {
            actions[choice - 1]();
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != static_cast<int>(actions.size()));
}

void UserInterface::createProject(sqlite3* db) {
    string name, description, startDate, endDate;
    cout << "Enter project name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter project description: ";
    getline(cin, description);
    cout << "Enter project start date (YYYY-MM-DD): ";
    getline(cin, startDate);
    cout << "Enter project end date (YYYY-MM-DD): ";
    getline(cin, endDate);

    Project::addProject(db, name, description, startDate, endDate);
    cout << "Project created successfully!" << endl;
}

void UserInterface::createRequester(sqlite3* db) {
    string name, email, phone, requesterType;
    cout << "Enter requester name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter requester email: ";
    getline(cin, email);
    cout << "Enter requester phone: ";
    getline(cin, phone);
    cout << "Enter requester type: ";
    getline(cin, requesterType);

    Requester::addRequester(db, name, email, phone, requesterType);
    cout << "Requester created successfully!" << endl;
}

void UserInterface::createRelease(sqlite3* db) {
    int projectId;
    string releaseDate;
    cout << "Enter project ID: ";
    cin >> projectId;
    cout << "Enter release date (YYYY-MM-DD): ";
    cin.ignore();
    getline(cin, releaseDate);

    Release::addRelease(db, releaseDate, projectId);
    cout << "Release created successfully!" << endl;
}

void UserInterface::createChangeRequestOrItem(sqlite3* db) {
    int choice;
    cout << "Enter 1 to create Change Request, 2 to create Change Item: ";
    cin >> choice;

    if (choice == 1) {
        int requesterId, changeId, releaseId;
        string requestDate;
        cout << "Enter requester ID: ";
        cin >> requesterId;
        cout << "Enter change ID: ";
        cin >> changeId;
        cout << "Enter release ID: ";
        cin >> releaseId;
        cout << "Enter request date (YYYY-MM-DD): ";
        cin.ignore();
        getline(cin, requestDate);

        ChangeRequest::addChangeRequest(db, requestDate, requesterId, changeId, releaseId);
        cout << "Change Request created successfully!" << endl;
    } else if (choice == 2) {
        string title, description, priority, status, createdDate;
        int projectId, releaseId;
        cout << "Enter title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter description: ";
        getline(cin, description);
        cout << "Enter priority: ";
        getline(cin, priority);
        cout << "Enter status: ";
        getline(cin, status);
        cout << "Enter created date (YYYY-MM-DD): ";
        getline(cin, createdDate);
        cout << "Enter project ID: ";
        cin >> projectId;
        cout << "Enter release ID: ";
        cin >> releaseId;

        ChangeItem::addChangeItem(db, title, description, priority, status, createdDate, projectId, releaseId);
        cout << "Change Item created successfully!" << endl;
    } else {
        cout << "Invalid choice." << endl;
    }
}

void UserInterface::viewOrUpdateChangeItem(sqlite3* db) {
    ChangeItem::getChangeItems(db);
}

void UserInterface::produceReport(sqlite3* db) {
    cout << "Producing report..." << endl;
    Project::getProjects(db);
}
