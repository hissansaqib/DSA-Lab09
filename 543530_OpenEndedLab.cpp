/*
Name: Muhammad Hissan Saqib
Reg No: 543530
Section: BSCS-15-D

Design Choices:
1. BST for Student Records:
   - Roll numbers are unique and need frequent lookups.
   - Searching and adding both take O(log n) time on average. 
   - In worst case (if roll numbers arrive already sorted), the tree skews and takes O(n).
   - Inorder traversal naturally prints the list in ascending order without extra sorting.

2. Queue for Help Requests:
   - A queue ensures fairness using First-In-First-Out (FIFO).
   - Students getting help are served in the exact order they raised requests.
   - Both enqueue and dequeue take O(1) constant time.
*/

#include <iostream>
#include <string>

using namespace std;

// Node for storing student info in BST
struct StudentNode {
    int rollNo;
    string name;
    StudentNode* left;
    StudentNode* right;

    StudentNode(int r, string n) {
        rollNo = r;
        name = n;
        left = nullptr;
        right = nullptr;
    }
};

class StudentBST {
private:
    StudentNode* root;

    StudentNode* insert(StudentNode* curr, int rollNo, string name, bool& duplicateFound) {
        if (curr == nullptr) {
            return new StudentNode(rollNo, name);
        }

        if (rollNo == curr->rollNo) {
            duplicateFound = true;
            return curr;
        } else if (rollNo < curr->rollNo) {
            curr->left = insert(curr->left, rollNo, name, duplicateFound);
        } else {
            curr->right = insert(curr->right, rollNo, name, duplicateFound);
        }
        return curr;
    }

    StudentNode* search(StudentNode* curr, int rollNo) {
        if (curr == nullptr || curr->rollNo == rollNo) {
            return curr;
        }
        if (rollNo < curr->rollNo) {
            return search(curr->left, rollNo);
        }
        return search(curr->right, rollNo);
    }

    void printInOrder(StudentNode* curr) {
        if (curr == nullptr) return;
        printInOrder(curr->left);
        cout << curr->rollNo << "\t" << curr->name << "\n";
        printInOrder(curr->right);
    }

    void clearTree(StudentNode* curr) {
        if (curr == nullptr) return;
        clearTree(curr->left);
        clearTree(curr->right);
        delete curr;
    }

public:
    StudentBST() {
        root = nullptr;
    }

    ~StudentBST() {
        clearTree(root);
    }

    bool addStudent(int rollNo, string name) {
        bool duplicateFound = false;
        root = insert(root, rollNo, name, duplicateFound);
        return !duplicateFound;
    }

    StudentNode* findStudent(int rollNo) {
        return search(root, rollNo);
    }

    void displayAll() {
        if (root == nullptr) {
            cout << "No records found.\n";
            return;
        }
        cout << "\nRoll No\tName\n";
        cout << "----------------------\n";
        printInOrder(root);
        cout << "----------------------\n";
    }

    bool isEmpty() {
        return root == nullptr;
    }
};

// Linked-list queue for FIFO help requests
struct QueueNode {
    int rollNo;
    QueueNode* next;

    QueueNode(int r) {
        rollNo = r;
        next = nullptr;
    }
};

class RequestQueue {
private:
    QueueNode* front;
    QueueNode* rear;

public:
    RequestQueue() {
        front = nullptr;
        rear = nullptr;
    }

    ~RequestQueue() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(int rollNo) {
        QueueNode* temp = new QueueNode(rollNo);
        if (rear == nullptr) {
            front = rear = temp;
            return;
        }
        rear->next = temp;
        rear = temp;
    }

    int pop() {
        if (isEmpty()) return -1;

        QueueNode* temp = front;
        int roll = temp->rollNo;
        front = front->next;

        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        return roll;
    }

    bool isEmpty() {
        return front == nullptr;
    }

    void display() {
        if (isEmpty()) {
            cout << "Help queue is empty.\n";
            return;
        }
        cout << "\nWaiting Requests Queue:\n";
        QueueNode* temp = front;
        int count = 1;
        while (temp != nullptr) {
            cout << count++ << ". Roll No: " << temp->rollNo << "\n";
            temp = temp->next;
        }
    }
};

int main() {
    StudentBST bst;
    RequestQueue queue;
    int choice = 0;

    while (choice != 7) {
        cout << "\n--- Student Help Desk ---\n";
        cout << "1. Add Student\n";
        cout << "2. Search Student\n";
        cout << "3. Display All Students\n";
        cout << "4. Add Help Request\n";
        cout << "5. Serve Help Request\n";
        cout << "6. View Pending Requests\n";
        cout << "7. Exit\n";
        cout << "Choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input. Enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            int roll;
            string name;
            cout << "Enter Roll No: ";
            cin >> roll;
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, name);

            if (bst.addStudent(roll, name)) {
                cout << "Student added.\n";
            } else {
                cout << "Error: Roll number already exists.\n";
            }
        } 
        else if (choice == 2) {
            if (bst.isEmpty()) {
                cout << "Database is empty.\n";
            } else {
                int roll;
                cout << "Enter Roll No to search: ";
                cin >> roll;
                StudentNode* res = bst.findStudent(roll);
                if (res != nullptr) {
                    cout << "Found: " << res->name << " (Roll No: " << res->rollNo << ")\n";
                } else {
                    cout << "Student not found.\n";
                }
            }
        } 
        else if (choice == 3) {
            bst.displayAll();
        } 
        else if (choice == 4) {
            int roll;
            cout << "Enter Roll No for help request: ";
            cin >> roll;

            // Student must exist in BST before adding ticket
            StudentNode* res = bst.findStudent(roll);
            if (res == nullptr) {
                cout << "Error: Student is not registered.\n";
            } else {
                queue.push(roll);
                cout << "Request added for " << res->name << ".\n";
            }
        } 
        else if (choice == 5) {
            if (queue.isEmpty()) {
                cout << "No pending requests.\n";
            } else {
                int served = queue.pop();
                StudentNode* res = bst.findStudent(served);
                if (res != nullptr) {
                    cout << "Serving: " << res->name << " (Roll: " << served << ")\n";
                } else {
                    cout << "Serving Roll: " << served << "\n";
                }
            }
        } 
        else if (choice == 6) {
            queue.display();
        } 
        else if (choice == 7) {
            cout << "Exiting program...\n";
        } 
        else {
            cout << "Invalid choice, try again.\n";
        }
    }

    return 0;
}