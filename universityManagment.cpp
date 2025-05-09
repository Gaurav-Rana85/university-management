#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <stdexcept>
using namespace std;

class universitysystemexception : public exception {
protected:
    string message;
public:
    universitysystemexception(string msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class cgpaexception : public universitysystemexception {
public:
    cgpaexception(string msg) : universitysystemexception("CGPA Error: " + msg) {}
};
class person {
protected:
    string name, contactinfo, id;
    int age;

public:
    person(string name = "", int age = 0, string id = "", string contact = "") {
        setname(name); setage(age); setid(id); setcontactinfo(contact);
    }
    virtual ~person() {}
    void setname(const string& n) {
        if (n.empty()) throw invalid_argument("Name cannot be empty.");
        name = n;
    }
    void setage(int a) {
        if (a <= 0 || a > 120) throw invalid_argument("Age must be 1-120.");
        age = a;
    }
    void setid(const string& i) {
        if (i.empty()) throw invalid_argument("ID cannot be empty.");
        id = i;
    }
    void setcontactinfo(const string& c) {
        if (c.empty()) throw invalid_argument("Contact cannot be empty.");
        contactinfo = c;
    }
    string getid() const { return id; }
    string getname() const { return name; }

    virtual void displaydetails() const {
        cout << "Name: " << name << "\nAge: " << age << "\nID: " << id
             << "\nContact: " << contactinfo << endl;
    }

    virtual float calculatepayment() = 0;
};
class student : public person {
    string enrollmentdate, program;
    float cgpa;

public:
    student(string name = "", int age = 0, string id = "", string contact = "",
        string enrolldate = "", string prog = "", float cgpa = 0.0f)
        : person(name, age, id, contact), enrollmentdate(enrolldate), program(prog) {
        setcgpa(cgpa);
    }

    void setcgpa(float c) {
        if (c < 0.0 || c > 10.0) throw cgpaexception("CGPA must be 0.0 - 10.0.");
        cgpa = c;
    }

    float getcgpa() const { return cgpa; }

    void displaydetails() const override {
        person::displaydetails();
        cout << "Program: " << program << "\nEnrollment: " << enrollmentdate
             << "\nCGPA: " << fixed << setprecision(2) << cgpa << endl;
    }

    float calculatepayment() override { return 10000.0f + (cgpa < 6.0f ? 2000 : 0); }

    string serialize() const {
        return name + "," + to_string(age) + "," + id + "," + contactinfo + "," + program + "," + enrollmentdate + "," + to_string(cgpa) + "\n";
    }
};

// Professor class
class professor : public person {
    string department, specialization, hiredate;

public:
    professor(string name = "", int age = 0, string id = "", string contact = "",
        string dept = "", string spec = "", string hire = "")
        : person(name, age, id, contact), department(dept), specialization(spec), hiredate(hire) {}

    void displaydetails() const override {
        person::displaydetails();
        cout << "Department: " << department << "\nSpecialization: " << specialization
             << "\nHire Date: " << hiredate << endl;
    }

    float calculatepayment() override { return 50000.0f; }

    string serialize() const {
        return name + "," + to_string(age) + "," + id + "," + contactinfo + "," + department + "," + specialization + "," + hiredate + "\n";
    }
};

// Helper Functions
void clearinputbuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

vector<student> students;
vector<professor> professors;

void savestudents() {
    ofstream out("students.txt");
    for (const auto& s : students) out << s.serialize();
    out.close();
}

void saveprofessors() {
    ofstream out("professors.txt");
    for (const auto& p : professors) out << p.serialize();
    out.close();
}

void loadstudents() {
    students.clear();
    ifstream in("students.txt");
    string name, age, id, contact, prog, enroll, cgpastr;
    while (getline(in, name, ',') &&
           getline(in, age, ',') &&
           getline(in, id, ',') &&
           getline(in, contact, ',') &&
           getline(in, prog, ',') &&
           getline(in, enroll, ',') &&
           getline(in, cgpastr)) {
        students.emplace_back(name, stoi(age), id, contact, enroll, prog, stof(cgpastr));
    }
    in.close();
}

void loadprofessors() {
    professors.clear();
    ifstream in("professors.txt");
    string name, age, id, contact, dept, spec, hire;
    while (getline(in, name, ',') &&
           getline(in, age, ',') &&
           getline(in, id, ',') &&
           getline(in, contact, ',') &&
           getline(in, dept, ',') &&
           getline(in, spec, ',') &&
           getline(in, hire)) {
        professors.emplace_back(name, stoi(age), id, contact, dept, spec, hire);
    }
    in.close();
}

void searchstudent() {
    string search;
    cout << "Enter student ID or name to search: ";
    getline(cin, search);
    bool found = false;
    for (const auto& s : students) {
        if (s.getid() == search || s.getname() == search) {
            s.displaydetails();
            found = true;
        }
    }
    if (!found) cout << "Student not found.\n";
}

void handlestudent() {
    while (true) {
        cout << "\n== Student Menu ==\n1. Add Student\n2. Show All\n3. Search\n4. Save\n5. Back\nChoice: ";
        int choice; cin >> choice; clearinputbuffer();

        if (choice == 1) {
            string name, id, contact, program, enroll;
            int age; float cgpa;
            cout << "Name: "; getline(cin, name);
            cout << "Age: "; cin >> age; clearinputbuffer();
            cout << "ID: "; getline(cin, id);
            cout << "Contact: "; getline(cin, contact);
            cout << "Program: "; getline(cin, program);
            cout << "Enroll Date: "; getline(cin, enroll);
            cout << "CGPA: "; cin >> cgpa; clearinputbuffer();
            try {
                students.emplace_back(name, age, id, contact, enroll, program, cgpa);
                cout << "Student added.\n";
            } catch (const exception& e) {
                cout << e.what() << endl;
            }
        } else if (choice == 2) {
            for (const auto& s : students) s.displaydetails();
        } else if (choice == 3) {
            searchstudent();
        } else if (choice == 4) {
            savestudents(); cout << "Students saved.\n";
        } else if (choice == 5) break;
        else cout << "Invalid.\n";
    }
}

void handleprofessor() {
    while (true) {
        cout << "\n== Professor Menu ==\n1. Add Professor\n2. Show All\n3. Save\n4. Back\nChoice: ";
        int choice; cin >> choice; clearinputbuffer();

        if (choice == 1) {
            string name, id, contact, dept, spec, hire;
            int age;
            cout << "Name: "; getline(cin, name);
            cout << "Age: "; cin >> age; clearinputbuffer();
            cout << "ID: "; getline(cin, id);
            cout << "Contact: "; getline(cin, contact);
            cout << "Department: "; getline(cin, dept);
            cout << "Specialization: "; getline(cin, spec);
            cout << "Hire Date: "; getline(cin, hire);
            professors.emplace_back(name, age, id, contact, dept, spec, hire);
            cout << "Professor added.\n";
        } else if (choice == 2) {
            for (const auto& p : professors) p.displaydetails();
        } else if (choice == 3) {
            saveprofessors(); cout << "Professors saved.\n";
        } else if (choice == 4) break;
        else cout << "Invalid.\n";
    }
}

// Main Menu
int main() {
    loadstudents();
    loadprofessors();

    while (true) {
        cout << "\n== University System ==\n1. Student\n2. Professor\n3. Exit\nChoice: ";
        int option; cin >> option; clearinputbuffer();

        if (option == 1) handlestudent();
        else if (option == 2) handleprofessor();
        else if (option == 3) {
            savestudents(); saveprofessors();
            cout << "Exiting...\n"; break;
        } else cout << "Invalid.\n";
    }

    return 0;
}
