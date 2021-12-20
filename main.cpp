//Project on Student Management
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Course //structure decleratin for taken courses data
{
    string courseName;
    string grade;
    string status;
};

class Student  //Class decleration for student data
{
	int id;
    int year;
    float cgpa;
    vector<Course> courses;
    string academic_advisor;
    
public:  //Public vector for declerations in the project scope
    Student();
    void getData();
    void showData() const;
    int getID() const { return id; }
    friend ostream& operator<<(ostream &out, const Student &s);
    friend istream& operator>>(istream &in, Student &s);
};

void writeStr(ostream &out, const string &str)  //Function to enable writing multiple objects to the file
{
    size_t size = str.size();
    out.write((char*)&size, sizeof(size));
    if (size)
        out.write(str.c_str(), size);
}

void readStr(istream &in, string &str)  //Function to enable reading multiple objects from the file
{
    str.clear();

    size_t size;
    if (in.read((char*)&size, sizeof(size)))
    {
        if (size > 0)
        {
            str.resize(size);
            in.read(&str[0], size);
        }
    }
}

ostream& operator<<(ostream &out, const Course &c)
{
    writeStr(out, c.courseName);
    writeStr(out, c.grade);
    writeStr(out, c.status);
    return out;
}

istream& operator>>(istream &in, Course &c)
{
    readStr(in, c.courseName);
    readStr(in, c.grade);
    readStr(in, c.status);
    return in;
}

ostream& operator<<(ostream &out, const Student &s)
{
    out.write((char*)&s.id, sizeof(s.id));
    out.write((char*)&s.year, sizeof(s.year));
    out.write((char*)&s.cgpa, sizeof(s.cgpa));
    writeStr(out, s.academic_advisor);

    size_t NumberOfcourses = s.courses.size();
    out.write((char*)&NumberOfcourses, sizeof(NumberOfcourses));

    for(int i = 0; i < NumberOfcourses; ++i)
    {
        if (!(out << s.courses[i]))
            break;
    }

    return out;
}

istream& operator>>(istream &in, Student &s)
{
    s.courses.clear();

    in.read((char*)&s.id, sizeof(s.id));
    in.read((char*)&s.year, sizeof(s.year));
    in.read((char*)&s.cgpa, sizeof(s.cgpa));
    readStr(in, s.academic_advisor);

    size_t NumberOfcourses;  //To create a vector to hold the multiple courses per student
    if (in.read((char*)&NumberOfcourses, sizeof(NumberOfcourses)))
    {
        s.courses.reserve(NumberOfcourses);
        Course c;

        for(size_t i = 0; i < NumberOfcourses; ++i)
        {
            if (in >> c)
                s.courses.push_back(c);
            else
                break;
        }
    }

    return in;
}

Student::Student()
{
    id = 0;  //Decimal set up (Type integer)
    year = 0;
    cgpa = 0.0f; //Decemal set up (Type float)
}

void Student::getData()  //Function to prompt the user to enter the required data
{
    courses.clear();

    cout << "\n\nEnter Student Details......\n";
    cout << "Enter ID No.     : "; cin >> id;
    cout << "Enter Intake Year of the Student: "; cin >> year;
    cout << "Enter number of Taken courses: ";

    size_t NumberOfcourses;
    cin >> NumberOfcourses;
    cin.ignore();

    courses.reserve(NumberOfcourses);
    Course c;

    for(int a = 0; a < NumberOfcourses; ++a) //Loop function to prompt the user to enter taken subjects details
    {
        cout << "\nEnter Subject Name: ";
        getline(cin, c.courseName);
            
        cout << "Enter Grade of Subject: ";
        getline(cin, c.grade);
            
        cout << "Enter Subject Status: ";
        getline(cin, c.status);

        courses.push_back(c);
    }

    cout << "Enter student CGPA: ";
    cin >> cgpa;
    cin.ignore();

    cout << endl;
    cout << "Enter Name of Academic Advisor of Student: ";
    getline(cin, academic_advisor);
}  //end of getData function

void Student::showData() const  //Function to set up what data to display based on the getData function
{
    cout << "\n\n.......Student Details......\n";
    cout << "ID No.                                   : " << id << endl;
    cout << "Intake Year                              : " << year << endl;
    cout << "Subjects Taken in Previous Semester      : " << endl;
    for(size_t t = 0; t < courses.size(); ++t)
    {
        cout << "\t" << courses[t].courseName << ": " << courses[t].grade << " (" << courses[t].status << ") ";
        cout << endl;
    }
    cout << "CGPA                                     : " << cgpa << endl;
    cout << "Name of academic advisor of Student      : " << academic_advisor << endl;
    cout << endl;
} //end of showData function

void addData()  //Function to store data to file
{
    Student s;
    s.getData();

    ofstream fout("Students.txt", ios::binary|ios::app);  //To create and enable wtriting to the file
    if (fout << s)
        cout << "\n\nData Successfully Saved to File....\n";
    else
        cerr << "\n\nError Saving Data to File!\n";
}  //end of addData function

void displayData() //Function to display the data from the file
{
    ifstream fin("Students.txt", ios::binary);  //Function to enable reading from the file

    Student s;
    while (fin >> s)
    {
        s.showData();
    }

    if(fin.is_open())
        cout << "\n\nData Reading from File Successfully Done....\n";
    else
        cerr << "\n\nError Reading Data from File!\n";
}  //end of  displayData function

void searchData()  //Function to search for specif stduent data
{
    int n;
    cout << "Enter ID Number you want to search for : ";
    cin >> n;    

    ifstream fin("Students.txt", ios::binary); //To access file in bianry mode

    Student s;
    bool flag = false;

    while (fin >> s)
    {
        if (n == s.getID())
        {
            cout << "The Details of ID No. " << n << " are: \n";
            s.showData();
            flag = true;
            break;
        }
    }

    if(fin.is_open())
    {
        if (!flag)
            cout << "The ID No. " << n << " not found....\n\n";
        cout << "\n\nData Reading from File Successfully Done....\n";
    }
    else
        cerr << "\n\nError Reading Data from File!\n";
} //end of searchData function

void deleteData() //Function to delete specific student data
{
    int n;
    cout << "Enter ID Number you want to delete : ";
    cin >> n;
    
    ifstream fin("Students.txt", ios::binary);
    ofstream fout("TempStud.txt", ios::binary);
    ofstream tout("TrashStud.txt", ios::binary|ios::app);

    Student s;
    bool flag = false;

    while (fin >> s)
    {
        if (n == s.getID())
        {
            cout << "The Following ID No. " << n << " has been deleted:\n";
            s.showData();
            tout << s;
            flag = true;
        }
        else
        {
            fout << s;
        }
    }

    if(fin.is_open())
    {
        if(fout.is_open())
        {
            fin.close();
            fout.close();

            if (!flag)
            {
                cout << "The ID No. " << n << " not found....\n\n";
                remove("tempStud.txt");
            }
            else
            {
                remove("Students.txt");
                rename("tempStud.txt", "Students.txt");

                cout << "\n\nData Successfully Deleted from File....\n";
            }
        }
        else
        {
            cerr << "\n\nError Saving Data to File!\n";
        }
    }
    else
    {
        cerr << "\n\nError Reading Data from File!\n";
    }
} //end of deleteData function


void modifyData()  //Function to modify a specific student data
{
    int n;    
    cout << "Enter ID Number you want to Modify : ";
    cin >> n;
    
    ifstream fin("Students.txt", ios::binary);
    ofstream fout("TempStud.txt", ios::binary);

    Student s;
    bool flag = false;

    while (fin >> s)
    {
        if (n == s.getID())  //To check if ID entered exist in the database
        {
            cout << "The Following ID No. " << n << " will be modified with new data:\n";
            s.showData();
            cout << "\n\nNow Enter the New Details....\n";
            s.getData();
            flag = true;
        }

        fout << s;
    }
    
    if(fin.is_open())
    {
        if(fout.is_open())
        {
            fin.close();
            fout.close();

            if (!flag)  //If the ID entered does not exist
            {
                cout << "The ID No. " << n << " not found....\n\n";
                remove("TempStud.txt");
            }
            else
            {
                remove("Students.txt");
                rename("TempStud.txt", "Students.txt");

                cout << "\n\nData Successfully Updated in File....\n";
            }
        }
        else
        {
            cerr << "\n\nError Saving Data to File!\n";
        }
    }
    else
    {
        cerr << "\n\nError Reading Data from File!\n";
    }
}

void project()  //Function to create user inteface menu
{
    int ch;
    do
    {
        system("cls");
        cout << "...............STUDENT MANAGEMENT SYSTEM..............\n";
        cout << "====================SEGI University=================\n";
        cout << "0. Exit from Program\n";
        cout << "1. Write Data to File\n";
        cout << "2. Read Data From File\n";
        cout << "3. Search Data From File\n";
        cout << "4. Delete Data From File\n";
        cout << "5. Modify Data in File\n";
        cout << "Enter your choice  : ";
        cin >> ch;
        system("cls");  //To cleare screen and go to menu option choosen by user
        switch (ch)  //Switch-case function corresponding 
        {
            case 1: addData(); break; //Menu option 1
            case 2: displayData(); break; //Menu option 2
            case 3: searchData(); break; //Menu option 3
            case 4: deleteData(); break; //Menu option 4
            case 5: modifyData(); break; //Menu o[tion 5
        }
        system("pause");  
    }
    while (ch != 0); //Forever loop to keep the program running as long as the menu option is valid
} //end of project function 

int main() //Main function
{
    project();  //run the function void project()
} //end of main function
