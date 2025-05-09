#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>
#include <iomanip>
using namespace std;

class universitysystemexception : public exception {
protected:
  string message;

public:
  universitysystemexception(string msg) : message(msg) {}
  const char *what() const noexcept override { return message.c_str(); }
};

class cgpaexception : public universitysystemexception {
public:
  cgpaexception(string msg) : universitysystemexception("cgpa error: " + msg) {}
};

class person {
protected:
  string name, contactinfo, id;
  int age;

public:
  person(string name = "", int age = 0, string id = "", string contact = "") {
    setname(name);
    setage(age);
    setid(id);
    setcontactinfo(contact);
  }

  virtual ~person() {}

  void setname(const string &n) {
    if (n.empty()) throw invalid_argument("name cannot be empty.");
    name = n;
  }

  void setage(int a) {
    if (a <= 0 || a > 120)
      throw invalid_argument("age must be between 1 and 120.");
    age = a;
  }

  void setid(const string &id) {
    if (id.empty()) throw invalid_argument("id cannot be empty.");
    id = id;
  }

  void setcontactinfo(const string &contact) {
    if (contact.empty()) throw invalid_argument("contact cannot be empty.");
    contactinfo = contact;
  }

  virtual void displaydetails() const {
    cout << "name: " << name << "\nage: " << age << "\nid: " << id
         << "\ncontact: " << contactinfo << endl;
  }

  virtual float calculatepayment() = 0;
};

class student : public person {
protected:
  string enrollmentdate, program;
  float cgpa;

public:
  student(string name = "", int age = 0, string id = "", string contact = "",
          string enrolldate = "", string prog = "", float cgpa = 0.0f)
      : person(name, age, id, contact), enrollmentdate(enrolldate),
        program(prog) {
    setcgpa(cgpa);
  }

  void setcgpa(float c) {
    if (c < 0.0 || c > 10.0)
      throw cgpaexception("cgpa must be between 0.0 and 10.0.");
    cgpa = c;
  }

  float getcgpa() const { return cgpa; }

  void displaydetails() const override {
    person::displaydetails();
    cout << "program: " << program << "\nenrollment date: " << enrollmentdate
         << "\ncgpa: " << fixed << setprecision(2) << cgpa << endl;
  }

  float calculatepayment() override { return 10000.0f; }
};

class professor : public person {
protected:
  string department, specialization, hiredate;

public:
  professor(string name = "", int age = 0, string id = "", string contact = "",
            string dept = "", string spec = "", string hire = "")
      : person(name, age, id, contact), department(dept),
        specialization(spec), hiredate(hire) {}

  void displaydetails() const override {
    person::displaydetails();
    cout << "department: " << department << "\nspecialization: " << specialization
         << "\nhire date: " << hiredate << endl;
  }

  float calculatepayment() override { return 50000.0f; }
};

void clearinputbuffer() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

vector<student> students;
vector<professor> professors;

void handlestudent() {
  while (true) {
    cout << "\n--- student menu ---\n";
    cout << "1. show existing students\n2. add new student\n3. show all "
            "students\n4. exit\nchoose an option: ";
    int choice;
    cin >> choice;
    clearinputbuffer();

    if (choice == 1) {
      if (students.empty()) {
        cout << "no students available.\n";
      } else {
        cout << "\n--- existing students ---\n";
        for (const auto &s : students) {
          s.displaydetails();
        }
      }
    } else if (choice == 2) {
      string name, id, contact, program, enrolldate;
      int age;
      float cgpa;

      cout << "\n--- add student ---\n";
      cout << "name: ";
      getline(cin, name);
      cout << "age: ";
      cin >> age;
      clearinputbuffer();
      cout << "id: ";
      getline(cin, id);
      cout << "contact: ";
      getline(cin, contact);
      cout << "program: ";
      getline(cin, program);
      cout << "enrollment date: ";
      getline(cin, enrolldate);
      cout << "enter cgpa (0.0 - 10.0): ";
      cin >> cgpa;
      clearinputbuffer();

      try {
        students.push_back(
            student(name, age, id, contact, enrolldate, program, cgpa));
        cout << "new student added successfully.\n";
      } catch (const cgpaexception &e) {
        cout << e.what() << " try again.\n";
      }

    } else if (choice == 3) {
      if (students.empty()) {
        cout << "no students available.\n";
      } else {
        cout << "\n--- all students ---\n";
        for (const auto &s : students) {
          s.displaydetails();
        }
      }
    } else if (choice == 4) {
      cout << "exiting student menu...\n";
      break;
    } else {
      cout << "invalid option. try again.\n";
    }
  }
}

void handleprofessor() {
  while (true) {
    cout << "\n--- professor menu ---\n";
    cout << "1. show existing professors\n2. add new professor\n3. show all "
            "professors\n4. exit\nchoose an option: ";
    int choice;
    cin >> choice;
    clearinputbuffer();

    if (choice == 1) {
      if (professors.empty()) {
        cout << "no professors available.\n";
      } else {
        cout << "\n--- existing professors ---\n";
        for (const auto &p : professors) {
          p.displaydetails();
        }
      }
    } else if (choice == 2) {
      string name, id, contact, dept, spec, hire;
      int age;

      cout << "\n--- add professor ---\n";
      cout << "name: ";
      getline(cin, name);
      cout << "age: ";
      cin >> age;
      clearinputbuffer();
      cout << "id: ";
      getline(cin, id);
      cout << "contact: ";
      getline(cin, contact);
      cout << "department: ";
      getline(cin, dept);
      cout << "specialization: ";
      getline(cin, spec);
      cout << "hire date: ";
      getline(cin, hire);

      professors.push_back(professor(name, age, id, contact, dept, spec, hire));
      cout << "new professor added successfully.\n";

    } else if (choice == 3) {
      if (professors.empty()) {
        cout << "no professors available.\n";
      } else {
        cout << "\n--- all professors ---\n";
        for (const auto &p : professors) {
          p.displaydetails();
        }
      }
    } else if (choice == 4) {
      cout << "exiting professor menu...\n";
      break;
    } else {
      cout << "invalid option. try again.\n";
    }
  }
}

int main() {
  while (true) {
    cout << "\n--- main menu ---\n";
    cout << "1. enter as student\n2. enter as professor\n3. exit\nchoose an "
            "option: ";
    int option;
    cin >> option;
    clearinputbuffer();

    if (option == 1) {
      handlestudent();
    } else if (option == 2) {
      handleprofessor();
    } else if (option == 3) {
      cout << "exiting program.\n";
      break;
    } else {
      cout << "invalid option. try again.\n";
    }
  }

  return 0;
}