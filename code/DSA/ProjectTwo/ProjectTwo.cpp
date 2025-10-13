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
        void inOrder(Node* node);
        void printAll();
        Node* findMin(Node* node);
        Node* removeNode(Node* node, std::string courseId);
    
    private:
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

        if (pCurr->course.courseId == courseId) {
            return pCurr->course;
        }

        while (pCurr->course.courseId != courseId) {
            if (courseId < pCurr->course.courseId) {
                if (pCurr->left == nullptr) {
                    break;
                }
                pCurr = pCurr->left;
            }
            else {
                if (pCurr->right == nullptr) {
                    break;
                }
                pCurr = pCurr->right;
            }

            if (pCurr->course.courseId == courseId) {
                return pCurr->course;
            }
        }
    }
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
    std::cout << "COURSE ID | COURSE NAME                        | PREREQUISITE COURSES" << std::endl;
    std::cout << std::setw(69) << std::setfill('=') << "" << std::endl << std::setfill(' ');

    inOrder(root);
}

Node* BinarySearchTree::findMin(Node* node) {
    node = node->right;

    while (node->left != nullptr) {
        node = node->left;
    }

    return node;
}

//Used to remove a node
Node* BinarySearchTree::removeNode(Node* node, std::string courseId) {
    if (node == nullptr) {
        return nullptr;
    }

    if (courseId < node->course.courseId) {
        node->left = removeNode(node->left, courseId);
    }
    else if (courseId > node->course.courseId) {
        node->right = removeNode(node->right, courseId);
    }
    else {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
        }
        else if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        else if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }
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
    std::ifstream inFile;

    inFile.open(fileName);

    if (!inFile.is_open()) {
        std::cout << "Failed to open file, exiting program." << std::endl;
        exit(1);
    } 

    int streamTracker = 0;

    while(!inFile.eof()) {
        if (inFile.good()) {
            inFile.seekg(streamTracker);
            
            //used for getting substrings
            std::string tempVal;

            getline(inFile, tempVal, ',');

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
                else {
                    std::cout << "File format error: Line does not start with a course code" << std::endl;
                    std::cout << "Exiting program..." << std::endl;
                    exit(1);
                }
            }

            //first entry is verified to be a course code at this point
            //return to the beginning of the line
            inFile.seekg(streamTracker);

            std::cout << "LINE 301: Stream position prior to getting line: " << streamTracker << std::endl;

            //get the entire line
            getline(inFile, tempVal, '\n');

            std::cout << "LINE 306: Temp value (line): " << tempVal << std::endl;

            //assign tracker to current stream position
            streamTracker = inFile.tellg();

            std::cout << "LINE 311: Stream position after getting line: " << streamTracker << std::endl;

            std::vector<std::string> tempVect;

            int stringTracker = 0;
            for (int i = 0; i < tempVal.size(); i++) {
                if (tempVal.at(i) == ',' || i == tempVal.size() - 1) {
                    std::cout << "319: Line size: " << tempVal.size() << std::endl;
                    std::cout << "320: Beginning of substring: " << stringTracker << std::endl;
                    std::cout << "321: End of substring: " << i << std::endl;
                    
                    std::string subString = tempVal.substr(stringTracker, i - stringTracker);


                    for (int i = 0; i < subString.size(); i++) {
                        if (subString.at(i) == ',') {
                            subString.erase(std::remove(subString.begin(), subString.end(), ','), subString.end());
                        }
                    }
                    
                    if(subString.size() > 0) {
                        tempVect.push_back(subString);
                        std::cout << "LINE 329: Adding substring value to vector: " << subString << std::endl;
                    }

                    stringTracker = i;
                }
            }

            if (tempVect.size() < 2) {
                std::cout << "File format error: Line does not have two entries." << std::endl;
                std::cout << "Exiting program..." << std::endl;
                exit(1);
            }

            Course course = Course();
            course.courseId = tempVect.at(0);
            course.name = tempVect.at(1);

            if (tempVect.size() > 2) {
                for (int i = 2; i < tempVect.size(); i++) {
                    course.prereqs.push_back(tempVect.at(i));
                }
            }

            bst.insert(course);
        }
        else {
            std::cout << "File error: " << inFile.rdstate() << std::endl;
            std::cout << "Exiting program..." << std::endl;
            exit(1);
        }
    }

    inFile.close();
}


//=============================================================================================================
//MAIN MENU CODE
//=============================================================================================================

int main() {
    BinarySearchTree bst = BinarySearchTree();
    int userInput = 0;

    while(true) {
        std::cout << "Welcome, please make a selection: " << std::endl;
        std::cout << "1. Load Courses" << std::endl;
        std::cout << "2. Print All Courses" << std::endl;
        std::cout << "3. Find a Course" << std::endl;
        std::cout << "9. Quit" << std::endl;
        
        
        std::cin >> userInput;
        Course searchedCourse;

        switch (userInput) {
            //load all courses
            case 1:
                std::cout << "Loading file..." << std::endl;
                parseFile(bst, "CS 300 ABCU_Advising_Program_Input.csv");
                break;

            //print all courses
            case 2:
                bst.printAll();
                break;

            //find a course
            case 3:
                searchedCourse = bst.search("MATH201");

                std::cout << "Course: " << searchedCourse.name << " ID: " << searchedCourse.courseId << " Prerequisite Courses: ";
        
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
                break;

            //exit
            case 9:
                std::cout << "Goodbye!" << std::endl;
                exit(0);
        }
    }

    return 0;
}