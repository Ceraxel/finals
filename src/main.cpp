#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <iomanip>

using namespace std;

struct Student {
    string id_number;
    string full_name;
    string birthday;
    string address;
    string gender;
    string degree_prog;
    string year_level;
};

class Node {
public:
    Student data;
    Node* next;
    Node* prev;
};

enum class Mode {DISPLAY, DELETE};

class Records {
public:
    Records();
    void add_record(Student student);
    void delete_record();
    void display_specific(); 
    void display_records(const Node* record); 

    void ask_input();
    bool is_id_duplicate(Student student);
    bool is_id_invalid(Student student);
    Node* search_record(Mode search_mode);
    void print(const Student* student); 
    void export_to_txt(); 

    Node* get_head_node() const {
        Node* tmp = head;
        return tmp;
    }

private:
    Node* head;
    Node* tail; 
};


// Forward declaration of functions
int menu_input();
void clear();
void pause();
void clear_err();


// Main
int main() {
    Records student_list; // Creates an instance of Records class

    while (true) {
        system("clear");
        int choice = menu_input();
        system("clear");
        switch (choice) {
            case 1: student_list.ask_input();        break;
            case 2: student_list.display_specific(); break;
            case 3: student_list.display_records(student_list.get_head_node());      break;
            case 4: student_list.delete_record();    break;
            case 5: return 0;
            default:
                continue;;
        }
        system("read");
    }
    return 0;
}

//   Function definitions
int menu_input() {
    while (true) {
        clear();
        int choice;
        cout << "----------------------------------------------------------" << '\n';
        cout << "                Student Information System                " << '\n';
        cout << "----------------------------------------------------------" << '\n';
        cout << "\t[ 1 ] - Add New Record" << '\n';
        cout << "\t[ 2 ] - Search Record" << '\n';
        cout << "\t[ 3 ] - Display All Records" << '\n';
        cout << "\t[ 4 ] - Delete Record" << '\n';
        cout << "\t[ 5 ] - Exit" << '\n';
        cout << "----------------------------------------------------------" << '\n';

        cout << "Choice: ";
        cin >> choice;

        // Returns the input if it is a number and within the range of 1 to 5
        if (cin && !(choice < 1 || choice > 5))
            return choice;

        // Otherwise, clear the error and continue the loop
        clear_err();
    }
}

// Constructor for the record class
// Populate the list by reading data from records.txt and adding it to records
Records::Records() { 
    head = nullptr;
    tail = nullptr;

    ifstream inf { "records.txt" };
    string place_holder;
    while (inf.peek() != EOF) {
        Student student;
        getline(inf, student.id_number);
        getline(inf, student.full_name);
        getline(inf, student.birthday);
        getline(inf, student.address);
        getline(inf, student.gender);
        getline(inf, student.degree_prog);
        getline(inf, student.year_level);
        add_record(student);
        // Capture new line
        getline(inf, place_holder);
    }
}

void Records::ask_input() {
    Student student;

    while (true) {
        clear();
        cout << "----------------------------------------------------------" << '\n';
        cout << "                         Add Record                       " << '\n';
        cout << "----------------------------------------------------------" << '\n';
        cout << "Student ID No.: ";
        cin >> student.id_number;
        cout << "Full name (LN, MI, FN): ";
        getline(cin >> ws, student.full_name);
        cout << "Birthday (MM/DD/YYYY): ";
        getline(cin >> ws, student.birthday);
        cout << "Address: ";
        getline(cin >> ws, student.address);
        cout << "Gender: ";
        getline(cin >> ws, student.gender);
        cout << "Degree Program (e.g. BSCSSE): ";
        getline(cin >> ws, student.degree_prog);
        cout << "Year Level: ";
        getline(cin >> ws, student.year_level);

        // Check if the student id is a duplicate
        if (is_id_duplicate(student)) {
            cout << "\nERROR: Student ID must be unique" << endl;
            pause();
            continue;
        }

        // Check if the student id is valid
        if (is_id_invalid(student)) {
            cout << "\nERROR: Student ID must be numeric" << endl;
            pause();
            continue;
        }

        break;
    }

    add_record(student);
    pause();
}

void Records::add_record(Student student) {
    Node* node = new Node;
    node->data = student;
    node->next = nullptr;

    if (head == nullptr) {
        head = node;
        tail = node;
    } else {
        node->prev = tail;
        tail->next = node;
        tail = tail->next;
    }
    export_to_txt();
}

Node* Records::search_record(Mode search_mode) {
    int search_by;
    string input;
    while (true) {
        cout << "----------------------------------------------------------" << '\n';
        cout << "                       Search by\n";
        cout << "----------------------------------------------------------" << '\n';
        cout << "\t[ 1 ] - Student ID\n";
        cout << "\t[ 2 ] - Name\n";
        cout << "----------------------------------------------------------" << '\n';
        cout << "Choice: ";

        cin >> search_by;

        if (cin && (search_by == 1 || search_by == 2))
            break;

        clear_err();
    }
    cout << "Search Key: ";
    getline(cin >> ws, input);

    clear();

    Node* traverse = get_head_node();
    Node* result_head = nullptr;
    Node* result = nullptr;

    string target;
    while (traverse != nullptr) {
        target = (search_by == 1) ? traverse->data.id_number : traverse->data.full_name;

        if (target != input && search_mode == Mode::DELETE) {
            traverse = traverse->next;
            continue;
        }

        if (search_mode == Mode::DELETE) {
            result_head = traverse;
            break;
        }

        // If the current record does not contain the substring, proceed to next iteration
        if (target.find(input) == string::npos){
            traverse = traverse->next;
            continue;
        }


        // If it does contain the substring, create a new node and copy the data of the current node to it
        Node* tmp = new Node();
        tmp->data = traverse->data;
        tmp->next = nullptr;

        if (result == nullptr) {
            result = tmp;
            result_head = result;
        } else {
            result->next = tmp;
            result = result->next;
        }

        traverse = traverse->next;
    }

    if (result != nullptr)
        result->next = nullptr;

    return result_head;
}

void Records::delete_record() {
    Node* record = search_record(Mode::DELETE);
    if (record == nullptr)
        return;

    Node* tmp = record;

    if (record == head) {
        head = record->next;
        delete tmp;
    } else if (record == tail) {
        tmp = record->prev;
        tmp->next = nullptr;
        delete record;
    } else {
        tmp = record->prev;
        tmp = record->next;
        delete record;
    }

    export_to_txt();
}

void Records::display_records(const Node* record) {
    cout << setfill('-') << setw(157) << "-" << '\n' << setfill(' ');
    cout << left << "| ";
    cout << setw(13) << "Student ID"     << "| ";
    cout << setw(40) << "Full Name"      << "| ";
    cout << setw(15) << "Birthday"       << "| ";
    cout << setw(40) << "Address"        << "| ";
    cout << setw(8)  << "Gender"         << "| ";
    cout << setw(15) << "Degree Program" << "| ";
    cout << setw(10) << "Year Level "    << "|\n";
    cout << setfill('-') << setw(157) << "-" << '\n' << setfill(' ');

    if (head == nullptr)
        std::cout << setw(78) << "|" << setw(78) << "No Records" << "|\n";

    while (record != nullptr) {
        print(&record->data);
        record = record->next;
    }
    cout << setfill('-') << setw(157) << "-" << '\n' << setfill(' ');
}

void Records::display_specific() {
    Node* result = search_record(Mode::DISPLAY);
    cout << flush;
    if (result == nullptr) {
        cout << "Record not found";
        return;
    }
    display_records(result);
}

void Records::print(const Student* student) {
    cout << left << "| ";
    cout << setw(13) << student->id_number   << "| ";
    cout << setw(40) << student->full_name   << "| ";
    cout << setw(15) << student->birthday    << "| ";
    cout << setw(40) << student->address     << "| ";
    cout << setw(8)  << student->gender      << "| ";
    cout << setw(15) << student->degree_prog << "| ";
    cout << setw(10) << student->year_level  << " |\n";
}

void Records::export_to_txt() {
    ofstream outf { "records.txt" };
    Node* tmp = head;
    while(tmp != nullptr) {
        outf << tmp->data.id_number << '\n';
        outf << tmp->data.full_name << '\n';
        outf << tmp->data.birthday << '\n';
        outf << tmp->data.address << '\n';
        outf << tmp->data.gender << '\n';
        outf << tmp->data.degree_prog << '\n';
        outf << tmp->data.year_level << '\n';
        outf << '\n';
        tmp = tmp->next;
    }
    outf.close();
}

bool Records::is_id_duplicate(Student student) {
    Node* tmp = head;
    while (tmp != nullptr) {
        if (student.id_number == tmp->data.id_number) {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
} 

bool Records::is_id_invalid(Student student) {
    for (const char& student : student.id_number) {
        if (!isdigit(student))
            return true;
    }
    return false;
} 


// Utilities

void clear() {
    #ifdef __linux__
    system("clear");
    #else
    system("cls");
    #endif
}

void pause() {
    #ifdef __linux__
    system("read");
    #else
    system("pause");
    #endif
}

void clear_err() {
    cin.clear();
    cin.ignore(100, '\n');
}
