#include	<iostream>
#include	<cstdlib>
#include	<cstdio>
#include    <fstream>
#include    <string>
#include	<iomanip>
#include	"List.h"
#include    "LibStudent.h"
#include    "LibBook.h"


using namespace std;

bool ReadFile(string, List*);
bool DeleteRecord(List*, char*);
bool Display(List*, int, int);
bool InsertBook(string, List*);
bool SearchStudent(List*, char* id, LibStudent&);
bool ComputeAndDisplayStatistics(List*);
bool PrintStuWithSameBook(List*, char*);
bool DisplayWarnedStudent(List*, List*, List*);
int menu();

//subfunction
void PerformDate(char[], int&, int&, int&);
double CalculateJulianDay(int, int, int);

int main() {
	char callNum[20], id[10];
	int source, detail, choice;
	List list, type1, type2;
	LibStudent stu;
	string filename;
	do {
		system("cls");
		choice = menu();
		switch (choice)
		{
		case 1:
			filename = "student.txt";
			if (ReadFile(filename, &list))
				cout << list.size() << " records have been successfully read." << endl;
			else
				cout << "Unable to read the record. " << endl;
			break;
		case 2:
			cout << "Enter the student's ID that you want to delete the record: ";
			cin >> id;
			if (DeleteRecord(&list, id))
				cout << "One record has been successfully deleted" << endl;
			else
				cout << "Unable to delete the record." << endl;
			break;
		case 3:
			cout << "Enter the id: ";
			cin >> id;
			if (SearchStudent(&list, id, stu))
			{
				cout << "One record found." << endl;
				stu.print(cout);
			}
			else
				cout << "Unable to find the record." << endl;
			break;
		case 4:
			filename = "book.txt";
			if (InsertBook(filename, &list))
				cout << "Books inserted successfully to student list." << endl;
			else
				cout << "Unable to insert books to student list." << endl;
			break;
		case 5:
			cout << endl;
			cout << "Where do you want to display the output (1 - File / 2 - Screen): ";
			cin >> source;
			cout << endl;
			cout << "Do you want to display book list for every student (1 - YES / 2 - NO): ";
			cin >> detail;
			cout << endl;

			if (Display(&list, source, detail))
				cout << "Successfully display output" << endl << endl;
			else
				cout << "Unable to display output" << endl << endl;
			break;
		case 6:
			if (ComputeAndDisplayStatistics(&list))
				cout << "Successfully display statistics" << endl;
			else
				cout << "Unable to display statistics" << endl << endl;
			break;
		case 7:
			cout << "Enter the callnum of the book: ";
			cin >> callNum;
			cout << endl;
			if (PrintStuWithSameBook(&list, callNum))
				cout << "Successfully display student's info with same book." << endl;
			else
				cout << "No student's info with same book." << endl;
			break;
		case 8:
			cout << endl;
			if (DisplayWarnedStudent(&list, &type1, &type2))
				cout << "Successfully display warned student" << endl;
			else
				cout << "Unable to display warned student" << endl;
			break;
		case 9:
			break;
		default:
			cout << "Please enter a valid number!" << endl;
		}
		cout << endl;
		system("pause");
	} while (choice != 9);
	cout << "\n\n";
	return 0;
}

bool ReadFile(string filename, List* list)
{
	LibStudent StudentInfo, stu;
	fstream inFile;
	char c[20], ch;
	inFile.open(filename);
	if (inFile.is_open())
	{
		while (!inFile.eof() && inFile >> ch && ch != ' ')				// Continue reading characters from the file until the end of the file is reached or a space character is encountered.
		{
			for (int i = 0; i < 3; i++)									// Skip the first 3 characters (likely header or delimiter)
				inFile >> c;
			inFile >> StudentInfo.id;									// Read the student ID from the file

			for (int i = 0; i < 2; i++)									// Skip the next 2 characters (likely a delimiter)
				inFile >> c;
			inFile.getline(StudentInfo.name, 30);						// Read the student's name from the file, up to 30 characters

			for (int i = 0; i < 2; i++)									// Skip the next 2 characters (likely a delimiter)
				inFile >> c;
			inFile >> StudentInfo.course;								// Read the student's course from the file

			for (int i = 0; i < 3; i++)									// Skip the next 3 characters (likely a delimiter)
				inFile >> c;
			inFile >> StudentInfo.phone_no;								// Read the student's phone number from the file

			bool exist = false;											// Initialize a boolean flag to check if the student already exists in the list
			for (int i = 1; i <= list->size(); ++i)						// Loop through the list to check if the student ID already exists
			{
				if (list->get(i, stu) && strcmp(StudentInfo.id, stu.id) == 0)	// Get the student info at the current position in the list and compare the IDs with the current student's ID
				{
					exist = true;
					break;
				}
			}
			if (!exist)													// If the student does not exist in the list, insert the new student info
				list->insert(StudentInfo);
		}
		inFile.close();													// Close the input file
		return true;
	}
	else
		return false;
}

bool DeleteRecord(List* list, char* id)
{
	LibStudent stu;
	for (int i = 1; i <= list->size(); i++)								// Loop in the list
	{
		if (list->get(i, stu) && strcmp(stu.id, id) == 0)				// Find the item and the item match with input
		{
			list->remove(i);											// If found, remove the item
			return true;												// and return true
		}
	}
	return false;
}

bool SearchStudent(List* list, char* id, LibStudent& stu)
{
	for (int i = 1; i <= list->size(); i++)								// Loop in the list
	{
		if (list->get(i, stu) && strcmp(stu.id, id) == 0)				// Find the item and the item match with input
			return true;												// If found, return true
	}
	return false;														// If not, return false
}

bool InsertBook(string filename, List* list)
{
	char id[10] = { }, author[10][50] = { }, tmpAuthor[100] = { }, tmpDate[15] = { };
	LibStudent stu;
	LibBook newbook;
	fstream inFile;
	inFile.open(filename);
	if (inFile.is_open())
	{
		while (!inFile.eof())
		{
			inFile >> id;
			inFile >> tmpAuthor;

			int k = 0, j = 0;
			for (int i = 0; i < strlen(tmpAuthor) + 1; i++)
			{
				if (tmpAuthor[i] == '/' || tmpAuthor[i] == '\0')         //if there is / or null terminator
				{
					author[k][j] = '\0';                                 //assign null terminator to end of string
					if (tmpAuthor[i] == '/')
					{
						k++;                                             //increment of first index for next loop
						j = 0;                                           //set to zero again 
					}
					else
						break;
				}
				else
				{
					author[k][j] = tmpAuthor[i];                         //set character by character into author array
					j++;
				}
			}

			for (int i = 0; i < 10; i++)
				newbook.author[i] = NULL;                               //set 10 pointer array to null

			for (int i = 0; i <= k; i++)
			{
				for (int j = 0; j <= strlen(author[i]); j++)            //replace the _ to space
				{
					if (author[i][j] == '_')
						author[i][j] = ' ';
				}
				newbook.author[i] = new char[strlen(author[i]) + 1];    //assign dynamic memory to each pointer array
				strcpy(newbook.author[i], author[i]);                   //newbook.author array point to author address
			}

			inFile >> newbook.title;
			for (int i = 0; i <= strlen(newbook.title); i++)
			{
				if (newbook.title[i] == '_')                            //replace the _ to space
					newbook.title[i] = ' ';
			}

			inFile >> newbook.publisher;
			inFile >> newbook.ISBN;
			inFile >> newbook.yearPublished;
			inFile.ignore();
			inFile >> newbook.callNum;

			inFile >> tmpDate;
			PerformDate(tmpDate, newbook.borrow.year, newbook.borrow.month, newbook.borrow.day);
			inFile >> tmpDate;
			inFile.ignore();
			PerformDate(tmpDate, newbook.due.year, newbook.due.month, newbook.due.day);

			double curJD = CalculateJulianDay(2020, 3, 29);
			double dueJD = CalculateJulianDay(newbook.due.year, newbook.due.month, newbook.due.day);

			if (curJD > dueJD)
				newbook.fine = static_cast<int>(curJD - dueJD) * 0.50;
			else
				newbook.fine = 0.0;

			if (list->empty())
			{
				cout << "Student list is empty." << endl;
				return false;
			}

			for (int y = 1; y <= list->size(); y++)
			{
				if (list->get(y, stu) && strcmp(stu.id, id) == 0)   //if student exist
				{
					if (stu.totalbook == 0)                         //direct insert if no book
					{
						stu.book[stu.totalbook] = newbook;          //copy the element in newbook array into stu.book array
						stu.totalbook += 1;
						stu.calculateTotalFine();                   //calculate total fine
						list->set(y, stu);                          //set the student into the node
					}
					else if (stu.totalbook == 14)
					{
						cout << "StudentID: " << stu.id << " has borrowed 15 books!" << endl;
						break;
					}
					else
					{
						bool exist = false;
						for (int i = 0; i < stu.totalbook; i++)
						{
							if (stu.book[i].compareCallNum(newbook)) //checking to prevent book duplication
							{
								exist = true;
								break;                               //if exist, break the loop
							}
						}

						if (!exist)                                  //if not exist, insert the book 
						{
							stu.book[stu.totalbook] = newbook;
							stu.totalbook += 1;
							stu.calculateTotalFine();
							list->set(y, stu);
						}
					}
				}
			}

		}
		for (int i = 0; i < 10; i++)
		{
			if (newbook.author[i] != NULL)                       //if not point to null, delete the dynamic memory and point to null
			{
				delete[] newbook.author[i];
				newbook.author[i] = NULL;
			}
		}
		inFile.close();
		return true;
	}
	else
	{
		cout << "Unable to open book.txt." << endl;
		return false;
	}
}

void PerformDate(char date[], int& year, int& month, int& day)
{
	char tmpDate[3][5] = { };
	int a = 0, b = 0;
	for (int i = 0; i < strlen(date); i++)
	{
		if (date[i] == '/')                  //if / is found
		{
			b = 0;                           //second index set to 0
			if (a == 0)                      //if first index is 0 (day)
				day = stoi(tmpDate[a]);
			else if (a == 1)                 //if first index is 1 (month)
				month = stoi(tmpDate[a]);
			a++;                             //first index increment
		}
		else if (a == 2)                     //if first index is 2 (year)
		{
			tmpDate[a][b] = date[i];
			if (b == 3)                      //there is 4 char in tmpDate[2]
			{
				year = stoi(tmpDate[a]);     //convert string to integer
				break;                       //break the loop
			}
			b++;
		}
		else
		{
			tmpDate[a][b] = date[i];         //set character by character
			b++;
		}
	}
}

double CalculateJulianDay(int year, int month, int day)
{
	if (month <= 2)							// Adjust for January and February
	{
		year--;
		month += 12;
	}

	double a = floor(year / 100.0);			// Julian Day Number calculation formula
	double b = 2 - a + floor(a / 4.0);
	return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + b - 1524.5;
}

bool Display(List* list, int source, int detail)
{
	fstream outFile;
	LibStudent stu;
	if (source == 1)												// display to output to a 'file'
	{
		if (detail == 1)											// display output "student_info & book_info"
		{
			outFile.open("student_booklist.txt");
			if (outFile.is_open())
			{
				if (list->empty())
					return false;

				for (int i = 1; i <= list->size(); i++)				// iterates through the list of students, printing each student's information followed by the list of books associated with that student to the file
				{
					list->get(i, stu);								// retrieves the 'i'-th student from the list and stores it in the 'stu'
					outFile << "STUDENT " << i;
					stu.print(outFile);								// print the student information

					outFile << "BOOK LIST : " << endl;
					for (int j = 0; j < stu.totalbook; j++)		    // iterates over all the books that the student has, total num of books is stored in 'stu.totalbook'
					{
						outFile << "Book " << j + 1 << endl;	
						stu.book[j].print(outFile);				    // print out the book details for j-th
						outFile << endl;
					}
					outFile << "******************************************************" << endl << endl;
				}
				cout << endl;
				cout << "Successfully display output to student_booklist.txt" << endl << endl;
				outFile.close();
				return true;
			}
			else
			{
				cout << "Unable to display output to student_booklist.txt" << endl << endl;
				return false;
			}
		}
		else if (detail == 2)										// display output student_info in file
		{
			outFile.open("student_info.txt");
			if (outFile.is_open())
			{
				if (list->empty())
					return false;

				for (int i = 1; i <= list->size(); i++)				// iterates through the list of students, printing each student's information followed by the list
				{
					outFile << "STUDENT " << i;
					list->get(i, stu);								// retrieves the 'i'-th student from the list and stores it in the 'stu'
					stu.print(outFile);								//print the student information
					outFile << endl;
					outFile << "******************************************************" << endl;
				}
				cout << endl;
				cout << "Successfully display output to student_info.txt" << endl << endl;
				outFile.close();
				return true;
			}
			else
			{
				cout << "Unable to display output to student_info.txt" << endl << endl;
				return false;
			}
		}
		else
			return false;
	}
	else if (source == 2)											// display output on screen
	{
		if (detail == 1)											// display output "student_info & book_info"	
		{
			if (list->empty())
				return false;

			for (int i = 1; i <= list->size(); i++)					// iterates through the list of students, printing each student's information followed by the list of books associated with that student to the file
			{
				cout << "STUDENT " << i;
				list->get(i, stu);									// retrieves the 'i'-th student from the list and stores it in the 'stu'
				stu.print(cout);									// print the student information
				cout << endl;

				cout << "BOOK LIST : " << endl << endl;
				for (int j = 0; j < stu.totalbook; j++)				// iterates through the list of students, printing each student's information followed by the list
				{
					cout << "Book " << j + 1 << endl;
					stu.book[j].print(cout);						// print out the book details for j-th
					cout << endl;
				}
				cout << "**************************************************************************" << endl << endl;
			}
			return true;
		}
		else if (detail == 2)										// display student_info only in file
		{
			if (list->empty())
				return false;

			for (int i = 1; i <= list->size(); i++)					// iterates through the list of students, printing each student's information 
			{
				cout << "STUDENT " << i;
				list->get(i, stu);									// retrieves the 'i'-th student from the list and stores it in the 'stu'
				stu.print(cout);									// print the student information
				cout << endl;
				cout << "**************************************************************************" << endl << endl;
			}
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool ComputeAndDisplayStatistics(List* list)
{
	LibStudent stu;
	char crs[5][3] = { "CS", "IA", "IB", "CN", "CT" };
	int numStu[5] = { 0 }, totalBook[5] = { 0 }, totalBookOverDue[5] = { 0 };
	double totalFine[5] = { 0.0 };
	if (list->empty())
	{
		cout << "Student list is empty." << endl;
		return false;
	}

	int count = 5;
	for (int i = 1; i <= list->size(); i++)
	{
		list->get(i, stu);
		if (strcmp(stu.course, "CS") == 0)         //match the student's course with the record
			count = 0;                             //set count to certain value for next operation
		else if (strcmp(stu.course, "IA") == 0)
			count = 1;
		else if (strcmp(stu.course, "IB") == 0)
			count = 2;
		else if (strcmp(stu.course, "CN") == 0)
			count = 3;
		else if (strcmp(stu.course, "CT") == 0)
			count = 4;

		if (count >= 0 && count <= 4)             //make sure within the range
		{
			numStu[count] += 1;
			totalBook[count] += stu.totalbook;
			totalFine[count] += stu.total_fine;
			for (int i = 0; i < stu.totalbook; i++)    //check if there is fine for every books
			{
				if (stu.book[i].fine > 0.0)
					totalBookOverDue[count] += 1;
			}
		}
	}
	cout << "\t" << "Course" << "\t" << "Number of Students" << "\t" << "Total Books Borrowed" << "\t" << "Total Overdue Books" << "\t" << "Total Overdue Fine (RM)" << endl;
	for (int i = 0; i < 5; i++)
	{
		cout << "          " << crs[i] << "\t" << "\t" << numStu[i] << "\t" << "\t" << "\t" << totalBook[i] << "\t" << "\t" << "\t" << totalBookOverDue[i] << "\t" << "\t" << "\t" << fixed << setprecision(2) << totalFine[i] << endl;
	}
	return true;

}

bool PrintStuWithSameBook(List* list, char* callNum)
{
	LibBook tmpbook;
	LibStudent stu;
	strcpy(tmpbook.callNum, callNum);
	int count = 0;												// Initialize a counter to keep track of how many students have borrowed the book
	for (int i = 1; i < list->size(); i++)						// Loop through the list of students to check if any have borrowed the book
	{
		list->get(i, stu);										// Get the student information at the current position in the list
		for (int k = 0; k < stu.totalbook; k++)
		{
			if (stu.book[k].compareCallNum(tmpbook))			// If the book's call number matches the temporary book's call number, increment the counter
			{
				count++;
			}
		}
	}
	cout << "There are " << count << " students that borrow the book with call number " << tmpbook.callNum << " as shown below :" << endl << endl;
	for (int i = 1; i < list->size(); i++)						// Loop again through the list to print detailed information about the students who borrowed the book
	{
		list->get(i, stu);
		for (int k = 0; k < stu.totalbook; k++)
		{
			if (stu.book[k].compareCallNum(tmpbook))
			{
				cout << "Student Id = " << stu.id << endl;
				cout << "Name = " << stu.name << endl;
				cout << "Course = " << stu.course << endl;
				cout << "Phone Number = " << stu.phone_no << endl;
				cout << "Borrow Date: ";
				stu.book[k].borrow.print(cout);
				cout << endl;
				cout << "Due Date: ";
				stu.book[k].due.print(cout);
				cout << endl << endl;
			}
		}
	}
	if (count == 0)												// Return false if no students were found, otherwise return true
		return false;
	else
		return true;
}

bool DisplayWarnedStudent(List* list, List* type1, List* type2)
{
	LibStudent stu, tmpStu;
	bool exist = true;
	int count;
	int count2;
	if (list->empty())
		return false;

	for (int i = 1; i <= list->size(); i++)							// iterates through each student in the list
	{
		count = 0;
		list->get(i, stu);											// retrieves the 'i'-th student from the list and stores it in the 'stu'
		for (int j = 0; j < stu.totalbook; j++)						// iterates the book borrowed by student
		{
			if ((stu.book[j].fine >= 5))							// if the fine is $5 or more over, (overdue more than 10 days)
				count++;											//increment the count
			if (count == 3)											// if count = 3 (indicating student has 3 or more books with fines $5 or more)
			{

				if (type1->empty())
				{
					type1->insert(stu);								// inserted student into type1
					break;
				}
				else
				{
					for (int k = 1; k <= type1->size(); k++)
					{
						if (type1->get(k, tmpStu) && stu.compareName2(tmpStu))		// checks if student already exist in 'type1' by comparing names
						{
							exist = false;
							break;
						}
					}
					if (exist)										// if exist is true (name not find in type 1)
					{
						type1->insert(stu);							// insert students that haven't add in type1
						break;
					}
				}
			}
		}
	}

	exist = true;													// reset state

	for (int i = 1; i <= list->size(); i++)							// iterates through the list of students
	{
		count2 = 0;													// track how many books have fine > 0
		list->get(i, stu);
		for (int j = 0; j < stu.totalbook; j++)						// iterates the book borrowed by student
		{
			if (stu.book[j].fine > 0)
				count2++;											//increment count2
			if (stu.total_fine > 50 && count2 == stu.totalbook)		// if total fine is over $50 and and equals the total num of books borrowed by stu which all books have fines
			{
				if (type2->empty())
				{
					type2->insert(stu);								// insert into type2
					break;
				}
				else
				{
					for (int i = 1; i <= type2->size(); i++)						// iterates through the list of students
					{
						if (type2->get(i, tmpStu) && stu.compareName2(tmpStu))		// checks if student already exist in 'type2' by comparing names
						{
							exist = false;
							break;
						}
					}
					if (exist)										// if exist is true (name not find in type2)
					{
						type2->insert(stu);							// insert students that haven't add in type2
						break;
					}
				}
			}
		}
	}

	if (!type1->empty())						
	{
		cout << "Students under type 1: ";
		for (int i = 1; i <= type1->size(); i++)					// iterates through the list of students, printing each student's information followed by the list (type1)
		{
			type1->get(i, stu);										// retrieves the 'i'-th student from the list and stores it in the 'stu' (Borrowed books more than 2 & overdue for 10 days = $5)
			stu.print(cout);										// print the details of each student in type1
		}
	}

	cout << endl << endl;

	if (!type2->empty())
	{
		cout << "Students under type 2: ";
		for (int i = 1; i <= type2->size(); i++)					// iterates through the list of students, printing each student's information followed by the list (type2)
		{
			type2->get(i, stu);										// retrieves the 'i'-th student from the list and stores it in the 'stu' (Borrowed book all overdue & total fine is more than $50.00)
			stu.print(cout);										// print the details of each student in type2
		}
	}
	return true;
}

int menu()
{
	int choice;
	cout << "1. Read file" << endl;
	cout << "2. Delete record" << endl;
	cout << "3. Search student" << endl;
	cout << "4. Insert book" << endl;
	cout << "5. Display output" << endl;
	cout << "6. Compute and Display Statistics" << endl;
	cout << "7. Student with Same Book" << endl;
	cout << "8. Display Warned Student" << endl;
	cout << "9. Exit" << endl;
	cout << "Please enter your selection: ";
	cin >> choice;
	return choice;
}