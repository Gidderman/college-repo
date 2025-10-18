//=============================================================================================================
//PROJECT TWO
//GIDDEON CARD
//=============================================================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <iomanip>
#include <filesystem>

//=============================================================================================================
//COURSE STRUCTURE
//=============================================================================================================
struct Course {
    std::string courseId;
    std::string name;
    std::vector<std::string> prereqs;

    //default constructor
    Course() {
        courseId = "###";
        name = "default";
    }
};

//=============================================================================================================
//NODE STUCTURE
//=============================================================================================================
struct Node {
    struct::Course course;
    Node* left;
    Node* right;
    
    //default constructor
    Node() {
        course = Course();
        left = nullptr;
        right = nullptr;
    }

    //used to create a node with a course passed in
    Node(Course course) {
        this->course = course;
        left = nullptr;
        right = nullptr;
    }
};

//=============================================================================================================
//BINARY SEARCH TREE CODE
//=============================================================================================================

class BinarySearchTree {
    public:
        BinarySearchTree();
        ~BinarySearchTree();
        void destroyTree(Node* node);
        void insert(Course course);
        Course search(std::string courseId);
        void printAll();
        Node* findMin(Node* node);
        Node* removeNode(Node* node, std::string courseId);
        
    private:
        void inOrder(Node* node);
        Node* root;
};

//Constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

//Destructor
BinarySearchTree::~BinarySearchTree() {
    destroyTree(root);
}

//Recursive function used in the destructor
void BinarySearchTree::destroyTree(Node* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

//Used to add nodes to the Binary Search Tree
void BinarySearchTree::insert(Course course) {
    //if this is the first entry in the binary search tree
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        //pointer to navigate the BST
        Node* pCurr = root;

        //cycle through the nodes until a proper place has been found
        while (pCurr != nullptr) {
            //course id is smaller than current course id
            if (course.courseId < pCurr->course.courseId) {
                //assign the node
                if (pCurr->left == nullptr) {
                    pCurr->left = new Node(course);
                    pCurr = nullptr;
                }
                else {
                    pCurr = pCurr->left;
                }
            }
            //course id is larger than current course id
            else {
                //assign the node
                if (pCurr->right == nullptr) {
                    pCurr->right = new Node(course);
                    pCurr = nullptr;
                }
                else {
                    pCurr = pCurr->right;
                }
            }
        }
    }

}

//Used to find a course
Course BinarySearchTree::search(std::string courseId) {
    if (root != nullptr) {
        //used to navigate the BST
        Node* pCurr = root;

        //initial check to determine if the course being searched for is the root.
        if (pCurr->course.courseId == courseId) {
            return pCurr->course;
        }

        //search loop
        while (pCurr->course.courseId != courseId) {
            //navigates down the left of the tree
            if (courseId < pCurr->course.courseId) {
                if (pCurr->left == nullptr) {
                    break;
                }
                pCurr = pCurr->left;
            }
            //navigates dow the right of the tree
            else {
                if (pCurr->right == nullptr) {
                    break;
                }
                pCurr = pCurr->right;
            }

            //returns the searched for course if found
            if (pCurr->course.courseId == courseId) {
                return pCurr->course;
            }
        }
    }
    //if course is not found, returns a blank course
    Course blankCourse = Course();
    return blankCourse;
}

//recursive function used to print all courses from least to most
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        
        //Printing function for current node below, to include cycling through prerequisites
        std::cout << std::left 
                  << std::setw(9) << node->course.courseId 
                  << std::setw(3) << " | " 
                  << std::setw(34) << node->course.name 
                  << std::setw(3) << " | ";
        
        if (node->course.prereqs.size() == 0) {
            std::cout << "None" << std:: endl;
        }
        else {
            for (int i = 0; i < node->course.prereqs.size(); i++) {
                if (i != node->course.prereqs.size() - 1) {
                    std::cout << node->course.prereqs.at(i) << ", ";
                }
                else {
                    std::cout << node->course.prereqs.at(i) << std::endl;
                }
            }
        }
    
        inOrder(node->right);
    }
}

//Used to print all courses
void BinarySearchTree::printAll() {
    //header for clear printing format
    std::cout << "COURSE ID | COURSE NAME                        | PREREQUISITE COURSES" << std::endl;
    std::cout << std::setw(69) << std::setfill('=') << "" << std::endl << std::setfill(' ');

    inOrder(root);
}

//small function used to find the minimum node, called in the remove function
Node* BinarySearchTree::findMin(Node* node) {
    node = node->right;

    while (node->left != nullptr) {
        node = node->left;
    }

    return node;
}

//Used to remove a node
Node* BinarySearchTree::removeNode(Node* node, std::string courseId) {
    //if the passed in node is null, do nothing.
    if (node == nullptr) {
        return nullptr;
    }

    //recursively calls the remove node function one to the left if node is located to the left
    if (courseId < node->course.courseId) {
        node->left = removeNode(node->left, courseId);
    }
    //recursively calls the remove node function one to the right if the node is located to the right
    else if (courseId > node->course.courseId) {
        node->right = removeNode(node->right, courseId);
    }
    //the node has been found
    else {
        //if the node is a leaf
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
        }
        //if the node has one node to its right
        else if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        //if the node has one node to its left
        else if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }
        //the node has both a left and a right node
        else {
            Node* successor = findMin(node->right);
            node->course = successor->course;
            node->right = removeNode(node->right, successor->course.courseId);
        }
    }

    return node;
}

//=============================================================================================================
//FILE PARSER CODE
//=============================================================================================================
void parseFile(BinarySearchTree& bst, std::string fileName) {
    //check to ensure the file exists, and if not prompt the user to re-enter the file path
    while(!std::filesystem::exists(fileName)) {
        std::cout << "File does not exist. Please enter a new file name. Enter \'q\' to quit." << std::endl;

        // get user input.
        getline(std::cin, fileName);

        if(fileName == "q") {
            return;
        }
    }
    
    //declare the file variable
    std::ifstream inFile;

    //opens the file using the passed in name to locate the file
    inFile.open(fileName);

    //checks to verify the file has opened, and if not, to exit the program
    if (!inFile.is_open()) {
        std::cout << "Failed to open file, exiting program." << std::endl;
        exit(1);
    } 

    //variable used to track where we are reading in the file
    int streamTracker = 0;

    //main loop for parsing the file, goes until the file ends
    while(!inFile.eof()) {
        //checks that the stream has no error flags
        if (inFile.good()) {
            //sets the stream to the desired position
            inFile.seekg(streamTracker);
            
            //used for getting substrings
            std::string tempVal;

            //get the line up to the first comma and writes it to the tempVal variable
            getline(inFile, tempVal, ',');

            //if we haven't just taken a newline
            if (tempVal != "\n") {
                //check size of tempVal
                if (tempVal.size() == 7) {
                    //should all be letters
                    std::string tempValBegin = tempVal.substr(0, 4);
    
                    //should all be numbers
                    std::string tempValEnd = tempVal.substr(4, 3);
    
                    //verifies the first 4 digits are all letters
                    for (int i = 0; i < tempValBegin.size(); i++) {
                        if (!isalpha(tempValBegin.at(i))) {
                            std::cout << "File format error: Line does not start with a course code" << std::endl;
                            std::cout << "Exiting program..." << std::endl;
                            exit(1);        
                        }
                    }
    
                    //verifies the last three digits are numbers
                    for (int i = 0; i < tempValEnd.size(); i++) {
                        if (!isdigit(tempValEnd.at(i))) {
                            std::cout << "File format error: Line does not start with a course code" << std::endl;
                            std::cout << "Exiting program..." << std::endl;
                            exit(1);
                        }
                    }
    
                }
                //There are error flags related to the stream
                else {
                    std::cout << "File format error: Line does not start with a course code" << std::endl;
                    std::cout << "Exiting program..." << std::endl;
                    exit(1);
                }
            }

            //first entry is verified to be a course code at this point
            //return to the beginning of the line
            inFile.seekg(streamTracker);

            //get the entire line
            getline(inFile, tempVal, '\n');

            //assign tracker to current stream position
            streamTracker = inFile.tellg();

            //establishes a temporary vector for storing the line
            std::vector<std::string> tempVect;
            
            //tracks location within the string pulled from the file
            int stringTracker = 0;

            //iterates over the line that was extracted
            for (int i = 0; i < tempVal.size(); i++) {
                //if we have arrived at a comma for partitioning or the end of the string
                if (tempVal.at(i) == ',' || i == tempVal.size() - 1) {
                    //subtrack the data iterated over and store it in a substring variable
                    std::string subString = tempVal.substr(stringTracker, i - stringTracker);

                    //removes any undesired comma's from the substring
                    for (int i = 0; i < subString.size(); i++) {
                        if (subString.at(i) == ',') {
                            subString.erase(std::remove(subString.begin(), subString.end(), ','), subString.end());
                        }
                    }
                    
                    //if the substring has content, add it to the temp vector
                    if(subString.size() > 0) {
                        tempVect.push_back(subString);
                    }

                    //set the string tracking variable to the current location
                    stringTracker = i;
                }
            }

            //error handling for if the file format is not correct
            if (tempVect.size() < 2) {
                std::cout << "File format error: Line does not have two entries." << std::endl;
                std::cout << "Exiting program..." << std::endl;
                exit(1);
            }

            //initialize the a course variable and assign course ID and name
            Course course = Course();
            course.courseId = tempVect.at(0);
            course.name = tempVect.at(1);

            //add the prerequisite courses, if there are any
            if (tempVect.size() > 2) {
                for (int i = 2; i < tempVect.size(); i++) {
                    course.prereqs.push_back(tempVect.at(i));
                }
            }

            //insert the course into the BST
            bst.insert(course);
        }
        //There is an error with the file.
        else {
            std::cout << "File error: " << inFile.rdstate() << std::endl;
            std::cout << "Exiting program..." << std::endl;
            exit(1);
        }
    }

    //close the file
    inFile.close();
}


//=============================================================================================================
//MAIN MENU CODE
//=============================================================================================================

int main() {
    //initialize the binary search tree
    BinarySearchTree bst = BinarySearchTree();
    //initialize a variable for user input
    int userInput = 0;

    //main operating loop
    while(true) {
        //print the menu options
        std::cout << "Welcome to the course planner." << std::endl << std::endl;
        std::cout << "1. Load Data Structure" << std::endl;
        std::cout << "2. Print Course List" << std::endl;
        std::cout << "3. Print Course" << std::endl;
        std::cout << "9. Exit" << std::endl;
        
        //accepts user input and filters for valid integer input.
        while(!(std::cin >> userInput)) {
            //clear flags and ignores any other input
            std::cin.clear();
            std::cin.ignore(256, '\n');

            std::cout << "Invalid input, please enter an *INTEGER*." << std::endl << std::endl;
        }

        //initialize variables for course searching
        Course searchedCourse;
        std::string search = "";

        //execute user input
        switch (userInput) {
            //load all courses
            case 1:
                //prompts the user
                std::cout << "Please enter the file path to load from." << std::endl;

                //ignore any left over newlines and accept user input
                std::cin.ignore(1);
                getline(std::cin, search);

                //parse the file
                parseFile(bst, search);
                std::cout << "Loading file..." << std::endl;

                break;

            //print all courses
            case 2:
                bst.printAll();
                break;

            //find a course
            case 3:
                //prompt the user
                std::cout << "What course would do you want to know about?" << std::endl;

                //accept user input
                std::cin >> search;

                //capatilizes any lowercase letters
                for (int i = 0; i < search.size(); i++) {
                    if (std::isalpha(search.at(i))) {
                        search.at(i) = toupper(search.at(i));
                    }
                }
                
                //searches the courses
                searchedCourse = bst.search(search);

                //course is not found, allow user to retry their entry
                while (searchedCourse.courseId == "###") {
                    std::cout << "Course not found, please try again. Enter \'q\' to exit search." << std::endl;

                    std::cin >> search;
                    
                    if (search == "q") {
                        break;
                    }

                    searchedCourse = bst.search(search);
                }

                //if the searched for course is not a default value, print the course information
                if (searchedCourse.courseId != "###") {
                    std::cout << "Course: " << searchedCourse.name << std::endl << "ID: " << searchedCourse.courseId 
                              << std::endl << "Prerequisite Courses: ";
            
                    if (searchedCourse.prereqs.size() == 0) {
                        std::cout << "None" << std:: endl;
                    }
                    else {
                        for (int i = 0; i < searchedCourse.prereqs.size(); i++) {
                            if (i != searchedCourse.prereqs.size() - 1) {
                                std::cout << searchedCourse.prereqs.at(i) << ", ";
                            }
                            else {
                                std::cout << searchedCourse.prereqs.at(i) << std::endl;
                            }
                        }
                    }
                }
                break;


            //exit
            case 9:
                std::cout << "Goodbye!" << std::endl;
                exit(0);
            
            //number that isn't an option
            default:
                std::cout << userInput << " is not a valid option." << std::endl << std::endl;
                break;
        }
    }

    return 0;
}