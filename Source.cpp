// ProjectTwo.cpp

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Struct Course
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Node for the Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// Function Prototypes
Node* insertCourse(Node* node, Course course);
Course* findCourse(Node* node, const string& courseNumber);
void printCoursesInOrder(Node* node);
Node* loadFromFile(const string& filename);
void searchCourse(Node* root);
string toUpperCase(string str);

// Fn showMenu
int main() {
    Node* root = nullptr;
    bool dataLoaded = false;
    int choice = 0;
    string filename;

    cout << "Welcome to the course planner.\n";

    while (true) {
        cout << "\n1. Load course data\n";
        cout << "2. Print all courses\n";
        cout << "3. Print course info\n";
        cout << "9. Exit\n";
        cout << "What would you like to do? ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter the filename to load: ";
            cin.ignore();
            getline(cin, filename);
            root = loadFromFile(filename);
            if (root != nullptr) {
                dataLoaded = true;
                cout << "Course data loaded\n";
            }
            else {
                cout << "Failed to load data\n";
            }
        }
        else if (choice == 2) {
            if (!dataLoaded) {
                cout << "Please load course data first\n";
            }
            else {
                printCoursesInOrder(root);
            }
        }
        else if (choice == 3) {
            if (!dataLoaded) {
                cout << "Please load course data first\n";
            }
            else {
                searchCourse(root);
            }
        }
        else if (choice == 9) {
            cout << "Goodbye\n";
            break;
        }
        else {
            cout << choice << " is not a valid option.\n";
        }
    }

    return 0;
}

// Fn insertCourse
Node* insertCourse(Node* node, Course course) {
    if (node == nullptr) {
        return new Node(course);
    }
    if (course.courseNumber < node->course.courseNumber) {
        node->left = insertCourse(node->left, course);
    }
    else {
        node->right = insertCourse(node->right, course);
    }
    return node;
}

// Fn findCourse
Course* findCourse(Node* node, const string& courseNumber) {
    if (node == nullptr) return nullptr;

    string nodeCourseNumber = toUpperCase(node->course.courseNumber);
    string searchCourseNumber = toUpperCase(courseNumber);

    if (searchCourseNumber == nodeCourseNumber) {
        return &node->course;
    }
    else if (searchCourseNumber < nodeCourseNumber) {
        return findCourse(node->left, courseNumber);
    }
    else {
        return findCourse(node->right, courseNumber);
    }
}

// Fn printCoursesInOrder
void printCoursesInOrder(Node* node) {
    if (node == nullptr) return;
    printCoursesInOrder(node->left);
    cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
    printCoursesInOrder(node->right);
}

// Updated Fn loadFromFile
Node* loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Could not open file: " << filename << endl;
        return nullptr;
    }

    vector<Course> courseList;
    vector<string> courseNumbers;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Course course;

        getline(ss, course.courseNumber, ',');
        getline(ss, course.courseTitle, ',');

        if (course.courseNumber.empty() || course.courseTitle.empty()) {
            cout << "Invalid course data: " << line << endl;
            continue;
        }

        while (getline(ss, token, ',')) {
            if (!token.empty()) {
                course.prerequisites.push_back(token);
            }
        }

        courseNumbers.push_back(course.courseNumber);
        courseList.push_back(course);
    }

    file.close();

    for (auto& course : courseList) {
        for (auto& prereq : course.prerequisites) {
            if (find(courseNumbers.begin(), courseNumbers.end(), prereq) == courseNumbers.end()) {
                cout << "Missing prerequisite: " << prereq << " for course " << course.courseNumber << endl;
                return nullptr;
            }
        }
    }

    Node* root = nullptr;
    for (auto& course : courseList) {
        root = insertCourse(root, course);
    }

    return root;
}

// Fn searchCourse
void searchCourse(Node* root) {
    while (true) {
        cout << "Enter course number or 'back' to return: ";
        string input;
        cin >> input;

        if (toUpperCase(input) == "BACK") {
            break;
        }

        Course* course = findCourse(root, input);

        if (course == nullptr) {
            cout << "Course not found.\n";
        }
        else {
            cout << course->courseNumber << ", " << course->courseTitle << endl;
            if (!course->prerequisites.empty()) {
                cout << "Prerequisites: ";
                for (size_t i = 0; i < course->prerequisites.size(); ++i) {
                    cout << course->prerequisites[i];
                    if (i != course->prerequisites.size() - 1)
                        cout << ", ";
                }
                cout << endl;
            }
            else {
                cout << "No prerequisites\n";
            }
        }
    }
}

// Helper function to convert string to uppercase
string toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}
