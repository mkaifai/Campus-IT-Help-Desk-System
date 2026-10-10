#include <iostream>
#include <string>
#include <limits>
using namespace std;

/*
Project: Campus IT Help Desk System
Priority 1 = Critical (network/server/lab completely down)
Priority 2 = Important (student cannot use required software)
Priority 3 = Normal (printing, password help, minor issue)
A smaller number means a higher priority.
*/

struct HelpRequestNode {
    int ticketID;
    string studentName;
    string problem;
    int priority;
    HelpRequestNode* next;
};

class HelpDeskPriorityQueue {
private:
    HelpRequestNode* front;
    HelpRequestNode* rear;
    int count;

public:
    HelpDeskPriorityQueue() {
        front = NULL;
        rear = NULL;
        count = 0;
    }

    ~HelpDeskPriorityQueue() {
        while (front != NULL) {
            HelpRequestNode* temp = front;
            front = front->next;
            delete temp;
        }
        rear = NULL;
        count = 0;
    }

    void Add(HelpRequestNode* request) {
        request->next = NULL;

        if (front == NULL) {
            front = request;
            rear = request;
        } else if (request->priority < front->priority) {
            request->next = front;
            front = request;
        } else {
            HelpRequestNode* current = front;
            // Keep requests with the same priority in arrival order.
            while (current->next != NULL &&
                   current->next->priority <= request->priority) {
                current = current->next;
            }
            request->next = current->next;
            current->next = request;

            if (request->next == NULL) {
                rear = request;
            }
        }
        count++;
    }

    HelpRequestNode* Remove() {
        if (front == NULL) return NULL;

        HelpRequestNode* request = front;
        front = front->next;
        if (front == NULL) rear = NULL;

        request->next = NULL;
        count--;
        return request;
    }

    bool IsEmpty() {
        return front == NULL;
    }

    bool IsNotEmpty() {
        return front != NULL;
    }

    void DisplayAll() {
        if (IsEmpty()) {
            cout << "There are no pending IT complaints.\n";
            return;
        }

        HelpRequestNode* current = front;
        cout << "\n--- Pending IT Help Requests ---\n";
        while (current != NULL) {
            cout << "Ticket ID: " << current->ticketID << "\n";
            cout << "Student Name: " << current->studentName << "\n";
            cout << "Problem: " << current->problem << "\n";
            cout << "Priority: " << current->priority;
            if (current->priority == 1) cout << " (Critical)";
            else if (current->priority == 2) cout << " (Important)";
            else cout << " (Normal)";
            cout << "\n-----------------------------\n";
            current = current->next;
        }
        cout << "Total pending complaints: " << count << "\n";
    }
};

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInput();
            return value;
        }
        cout << "Invalid input. Please enter a number.\n";
        clearInput();
    }
}

HelpRequestNode* CreateHelpRequest() {
    HelpRequestNode* request = new HelpRequestNode;

    request->ticketID = readInt("Enter Ticket ID: ");

    cout << "Enter Student Name: ";
    getline(cin, request->studentName);

    cout << "Describe the IT problem: ";
    getline(cin, request->problem);

    while (true) {
        request->priority = readInt(
            "Priority (1=Critical, 2=Important, 3=Normal): ");
        if (request->priority >= 1 && request->priority <= 3) break;
        cout << "Priority must be 1, 2, or 3. Try again.\n";
    }

    request->next = NULL;
    return request;
}

int main() {
    HelpDeskPriorityQueue helpDesk;
    int choice;

    do {
        cout << "\n===== Campus IT Help Desk =====\n";
        cout << "1. Add IT Complaint\n";
        cout << "2. Handle Highest Priority Complaint\n";
        cout << "3. Display All Complaints\n";
        cout << "4. Check Queue Status\n";
        cout << "5. Exit\n";

        choice = readInt("Enter your choice: ");

        switch (choice) {
        case 1: {
            HelpRequestNode* request = CreateHelpRequest();
            helpDesk.Add(request);
            cout << "Complaint registered successfully.\n";
            break;
        }
        case 2: {
            HelpRequestNode* handled = helpDesk.Remove();
            if (handled == NULL) {
                cout << "Queue is empty. No complaint to handle.\n";
            } else {
                cout << "\n--- Complaint Being Handled ---\n";
                cout << "Ticket ID: " << handled->ticketID << "\n";
                cout << "Student: " << handled->studentName << "\n";
                cout << "Problem: " << handled->problem << "\n";
                cout << "Priority: " << handled->priority << "\n";
                delete handled;
            }
            break;
        }
        case 3:
            helpDesk.DisplayAll();
            break;
        case 4:
            if (helpDesk.IsEmpty()) cout << "Queue status: EMPTY\n";
            else cout << "Queue status: NOT EMPTY\n";
            break;
        case 5:
            cout << "Exiting Campus IT Help Desk.\n";
            break;
        default:
            cout << "Invalid choice. Please select 1 to 5.\n";
        }
    } while (choice != 5);

    return 0;
}
