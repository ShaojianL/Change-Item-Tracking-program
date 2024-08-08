#include "PrintUtils.h"

void printChangeItemHeaders() {
    std::cout << std::left
              << std::setw(5) << "Index"
              << std::setw(12) << "Change ID"
              << std::setw(20) << "Title"
              << std::setw(30) << "Description"
              << std::setw(10) << "Priority"
              << std::setw(15) << "Status"
              << std::setw(15) << "Created Date"
              << std::setw(15) << "Resolved Date"
              << std::setw(20) << "Anticipated"
              << std::setw(12) << "Project ID"
              << std::setw(12) << "Release ID"
              << std::setw(15) << "Release"
              << std::endl;
    std::cout << std::left
              << std::setw(5) << ""
              << std::setw(12) << ""
              << std::setw(20) << ""
              << std::setw(30) << ""
              << std::setw(10) << ""
              << std::setw(15) << ""
              << std::setw(15) << ""
              << std::setw(15) << ""
              << std::setw(20) << "Release"
              << std::setw(12) << ""
              << std::setw(12) << ""
              << std::setw(15) << "Number"
              << std::endl;
    std::cout << std::string(175, '-') << std::endl; // Divider line
}

void printChangeRequestHeaders() {
    std::cout << std::left
              << std::setw(5) << "Index"
              << std::setw(12) << "Request ID"
              << std::setw(20) << "Requester ID"
              << std::setw(30) << "Change ID"
              << std::setw(20) << "Release ID"
              << std::setw(20) << "Request Date"
              << std::endl;
    std::cout << std::string(110, '-') << std::endl; // Divider line
}

void printProjectHeaders() {
    std::cout << std::left
              << std::setw(5) << "Index"
              << std::setw(12) << "Project ID"
              << std::setw(30) << "Name"
              << std::setw(50) << "Description"
              << std::setw(15) << "Start Date"
              << std::setw(15) << "End Date"
              << std::endl;
    std::cout << std::string(130, '-') << std::endl; // Divider line
}

void printReleaseHeaders() {
    std::cout << std::left
              << std::setw(5) << "Index"
              << std::setw(12) << "Release ID"
              << std::setw(20) << "Project ID"
              << std::setw(20) << "Release"
              << std::setw(20) << "Release"
              << std::endl;
    std::cout << std::left
            << std::setw(5) << ""
            << std::setw(12) << ""
            << std::setw(20) << ""
            << std::setw(20) << "Number"
            << std::setw(20) << "Date"
            << std::endl;
    std::cout << std::string(95, '-') << std::endl; // Divider line
}

void printRequesterHeaders() {
    std::cout << std::left
              << std::setw(5) << "Index"
              << std::setw(12) << "Requester"
              << std::setw(30) << "Name"
              << std::setw(30) << "Email"
              << std::setw(15) << "Phone"
              << std::setw(20) << "Requester"
              << std::endl;
    std::cout << std::left
              << std::setw(5) << ""
              << std::setw(12) << "ID"
              << std::setw(30) << "Name"
              << std::setw(30) << "Email"
              << std::setw(15) << "Phone"
              << std::setw(20) << "Type"
              << std::endl;
    std::cout << std::string(120, '-') << std::endl; // Divider line
}
