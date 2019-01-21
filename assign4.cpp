// comp3311-project.cpp : Defines entry point for the console application.
// No marks will be given if we can't compile your file
// No marks will be given if the SQL statement is fundementally incorrect and no partial credits will be given
// No marks will be given for a TODO task if it fails to get the same result from the check case in the assignment description
// If you want to appeal the score,  please run the suggested solution and compare the result
// you can print the retrieved results using printRecordIntoCol(HSTMT stmt) and printIntoRow(HSTMT stmt, int maxColumnWidth);
//Yang Shengmin   20529959   COMP3311
#include "stdafx.h"
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <iostream>
#include <conio.h>
#include <iomanip>
using namespace std;

#ifdef _MSC_VER
#define getch() _getch()
#endif



HENV   henv;
HDBC   hdbc;
HSTMT  hstmt;
RETCODE	ret;

SQLINTEGER staff_id, cbStaffId; // global variables




boolean ConnectDB();
void DisconnectDB();
boolean userLogin();


void prof_menu(); // the menu for prof.
void teachInfo(); // show teaching information
void superInfo(); // show supervision information
void adminInfo(); // show administrative information
void showTeach(); // show offerings being taught by the prof in the current semester 'Spring2018'
void showLead();  // show offerings being led by the prof in the current semester 'Spring2018'
void showPre();   // show prerequisites of all the courses
void showSuper(); // show students being supervised by the prof
void showSuperGroup(); // show supervision information of the school
void changePassword(); // change password for the prof
void addPhone();       // add a new phone for the prof
void showCourseTA();   // show all the TAs working with the prof in the current semester
void showTAPref();     // show the offering preference of all the TAs

void printRecordIntoCol(HSTMT stmt);
SQLCHAR*** printIntoRow(HSTMT stmt, int maxColumnWidth);


int main() {
	int command = 0;
	// set the width and line displayed in the command prompt
	system("mode con cols=80 lines=50");

	if (ConnectDB()) {
		while (userLogin()) {
			do {
				system("CLS"); // clear the screen
				prof_menu(); //menu for the prof

			} while (command != 0);
		}
	}
	else {
		cout << "Oracle Connection unsuccessful.\n";
		system("pause");
	}
	DisconnectDB();
}


void prof_menu() {

	int command = 0;
	int ISdone = 0;

	while (!ISdone) {
		system("CLS"); // Clear the screen
		cout << "============ Information System for Professors ===============\n\n";
		cout << "0. Return to the previous menu                     (input '0').\n";
		cout << "1. Show Teaching related information               (input '1').\n";
		cout << "2. Show Supervision information                    (input '2').\n";
		cout << "3. Show Administrative information                 (input '3').\n";
		cout << "Please enter your choice: ";

		cin >> command;

		char buf[2];
		cin.getline(buf, 2); // grab the endline character when the user press "Enter"

		cout << endl;

		switch (command) {
		case 1:
			teachInfo();
			ISdone = 0;
			break;

		case 2:
			superInfo();
			ISdone = 0;
			break;
		case 3:
			adminInfo();
			ISdone = 0;
			break;
		case 0:
			ISdone = 1;
			break;
		default:
			break;
		}
	}
}


void teachInfo() {

	int command = 0;
	int ISdone = 0;

	while (!ISdone) {
		system("CLS");
		cout << "=================== Teaching Information ======================\n\n";
		cout << "0. Return to the previous menu                        (input '0').\n";
		cout << "1. Display course(s) teaching in the current semester (input '1').\n";
		cout << "2. Display the course being led                       (input '2').\n";
		cout << "3. See prerequisites of the courses                   (input '3').\n";
		cout << "Please enter your choice: ";

		cin >> command;

		char buf[2];
		cin.getline(buf, 2); // grab the endline character when the user press "Enter"

		cout << endl;

		switch (command) {
		case 1:
			showTeach();
			ISdone = 0;
			break;

		case 2:
			showLead();
			ISdone = 0;
			break;
		case 3:
			showPre();
			ISdone = 0;
			break;
		case 0:
			ISdone = 1;
			break;
		default:
			break;
		}
	}

}

void showTeach() {
	SQLAllocStmt(hdbc, &hstmt);
	char query[1000];
	char currentSem[100] = "Spring2018";

	system("CLS");

	cout << "Here are the courses you are teaching in the current semester:\n";

	// TODO 1: display the course_ID, course_name, offering_no, classroom, no_of_stds, and credits of all the courses he/she is teaching in the current semester (assume the current semester is Spring2018).
	// for the expect behaviour of this part please refer to the executable program
	// Add your code here
    //row form
    //"teaching"
    char course_ID[20];
    char course_name[20];
    int offering_no;
    int classroom;
    int no_of_stds;
    SQLINTEGER cbcourse_ID, cbcourse_name, cboffering_no, cbclassroom, cbno_of_stds; //string length of returned data
    //prepare the statement for SQLExecDirectA
    sprintf(query,"SELECT course.course_ID AS \"Course ID\", course.course_name AS \"Course name\", offering.offering_no AS \"Offering number\", offering.classroom AS \"Classroom\", offering.no_of_stds AS \"Number of students\" FROM course, offering, prof_teach WHERE course.course_ID = offering.course_ID AND offering.YearSemester =\'%s\' AND prof_teach.offering_no=offering.offering_no AND prof_teach.staff_ID=\'%i\'", currentSem, staff_id);
    SQLExecDirectA(hstmt, (SQLCHAR *)query, SQL_NTS);
    
    SQLBindCol(hstmt,1,SQL_C_CHAR,course_ID,20,&cbcourse_ID);
    SQLBindCol(hstmt,2,SQL_C_CHAR,course_name,20,&cbcourse_name);
    SQLBindCol(hstmt,3,SQL_INTEGER,&offering_no,10,&cboffering_no);
    SQLBindCol(hstmt,4,SQL_INTEGER,&classroom,10,&cbclassroom);
    SQLBindCol(hstmt,5,SQL_INTEGER,&no_of_stds,10,&cbno_of_stds);
    printIntoRow(hstmt,15); //max column width=15;
    SQLFreeStmt(hstmt,SQL_CLOSE);
	system("pause");

}

void showLead() {
	SQLAllocStmt(hdbc, &hstmt);
	char query[1000];
	char currentSem[100] = "Spring2018";

	system("CLS");
	cout << "Here are the courses you are leading in the current semester:\n";
	cout << "-------------------------------------------------------------\n";

	// TODO 2: display the course_ID, course_name, offering_no, classroom, no_of_stds, and credits of the course he/she is leading in the current semester (assume the current semester is Spring2018)
	// Add your code here
    //column form
    //"leading"
    char course_ID[20];
    char course_name[20];
    int offering_no;
    int classroom;
    int no_of_stds;
    SQLINTEGER cbcourse_ID, cbcourse_name, cboffering_no, cbclassroom, cbno_of_stds;
    
    sprintf(query,"SELECT course.course_ID AS \"Course ID\", course.course_name AS \"Course name\", offering.offering_no AS \"Offering number\", offering.classroom AS \"Classroom\", offering.no_of_stds AS \"Number of students\" FROM course, offering WHERE course.course_ID = offering.course_ID AND offering.YearSemester =\'%s\' AND offering.staff_ID=\'%i\'", currentSem, staff_id);
    SQLExecDirectA(hstmt, (SQLCHAR *)query, SQL_NTS);
    
    SQLBindCol(hstmt,1,SQL_C_CHAR,course_ID,20,&cbcourse_ID);
    SQLBindCol(hstmt,2,SQL_C_CHAR,course_name,20,&cbcourse_name);
    SQLBindCol(hstmt,3,SQL_INTEGER,&offering_no,10,&cboffering_no);
    SQLBindCol(hstmt,4,SQL_INTEGER,&classroom,10,&cbclassroom);
    SQLBindCol(hstmt,5,SQL_INTEGER,&no_of_stds,10,&cbno_of_stds);
    printRecordIntoCol(hstmt);
    SQLFreeStmt(hstmt,SQL_CLOSE);
	system("pause");


}
void showPre() {
	SQLAllocStmt(hdbc, &hstmt);
	char query[1000];

	system("CLS");
	cout << "Here are the prerequisites of the courses:\n";

	// TODO 3: group the prerequisites (course_IDs) by the course_IDs of the main courses and display the prerequisites (course_ID) in a list. See the screen shot in the assignment output section for the expected output.
	// Hint: you will find the aggregate function LISTAGG() function useful. You can refer to http://www.oracle-developer.net/display.php?id=515 for the exact syntax of LISTAGG().
	// Add your code here
    //LISTAGG( [,]) WITHIN GROUP (ORDER BY ) [OVER (PARTITION BY )]
    //row form
    
    sprintf(query,"SELECT prerequisite.main_course_ID AS \"Main Course\" ,LISTAGG(prerequisite.prereq_course_ID,',')  WITHIN GROUP (order by prerequisite.prereq_course_ID) AS \"Prerequisite list\" FROM prerequisite GROUP BY prerequisite.main_course_ID");
    SQLExecDirectA(hstmt, (SQLCHAR *)query, SQL_NTS);
    printIntoRow(hstmt,15);
    SQLFreeStmt(hstmt,SQL_CLOSE);
	system("pause");


}


void superInfo() {

	int command = 0;
	int ISdone = 0;

	while (!ISdone) {
		system("CLS");
		cout << "=================== Supervision Information ======================\n\n";
		cout << "0. Return to the previous menu                        (input '0').\n";
		cout << "1. Display students being supervised                  (input '1').\n";
		cout << "2. Display students by their supervisor staff_ID      (input '2').\n";
		cout << "Please enter your choice: ";

		cin >> command;

		char buf[2];
		cin.getline(buf, 2); // grab the endline character when the user press "Enter"

		cout << endl;

		switch (command) {
		case 1:
			showSuper();
			ISdone = 0;
			break;

		case 2:
			system("mode con cols=160 lines=50"); //make the screen larger
			showSuperGroup();
			system("mode con cols=80 lines=50"); //restore the screen size
			ISdone = 0;
			break;
		case 0:
			ISdone = 1;
			break;
		default:
			break;
		}
	}
}

void showSuper() {

	SQLAllocStmt(hdbc, &hstmt);
	char query[1000];

	system("CLS");
	cout << "Here are the students you are supervising:\n";

	// TODO 4: display all the student_ID, last_name, first_name, phone of all the students the professor supervises.
	// Add your code here
    int student_ID;
    char last_name[20];
    char first_name[20];
    int phone;
    
    SQLINTEGER cbstudent_ID, cblast_name, cbfirst_name, cbphone;
    
    sprintf(query,"SELECT TA.student_ID AS \"Student ID\", TA.last_name AS \"Last Name\", TA.first_name AS \"First Name\", TA.phone AS \"Phone number\" FROM TA, supervise WHERE supervise.student_ID=TA.student_ID AND supervise.staff_ID=\'%i\' ",staff_id);
    SQLExecDirectA(hstmt, (SQLCHAR *)query, SQL_NTS);
    SQLBindCol(hstmt,1,SQL_INTEGER,&student_ID,10,&cbstudent_ID);
    SQLBindCol(hstmt,2,SQL_C_CHAR,last_name,20,&cblast_name);
    SQLBindCol(hstmt,3,SQL_C_CHAR,first_name,20,&cbfirst_name);
    SQLBindCol(hstmt,4,SQL_INTEGER,&phone,10,&cbphone);
    printIntoRow(hstmt, 15);
    SQLFreeStmt(hstmt,SQL_CLOSE);
	system("pause");
}

void showSuperGroup() {


	SQLAllocStmt(hdbc, &hstmt);
	char query[1000];

	system("CLS");
	cout << "Here are the student supervision information of the school:\n";

	// TODO 5: group the students (student_ID, last_name, first_name) according to the supervisors' staff_IDs, and display the student information in a list in ascending order of the student_IDs,
	// see the screen shot for the exact output. Hint: you may find the LISTAGG() function and the concatenation operator are useful.
	// Add your code here
    //If a student does not have first or second name, replace that with a space
    //LISTAGG( [,]) WITHIN GROUP (ORDER BY ) [OVER (PARTITION BY )]
    
    sprintf(query,"SELECT prof.staff_ID AS \"Professor staff ID\",prof.last_name AS \"Last Name\" ,prof.first_name AS \"First Name\" ,LISTAGG(supervise.student_ID || ' ' || nvl(TA.last_name,' ')|| ' '||nvl(TA.first_name,' '),',') WITHIN GROUP(ORDER BY supervise.student_ID) AS \"Supervising students\" FROM prof,supervise,TA WHERE supervise.staff_ID=prof.staff_ID AND supervise.student_ID=TA.student_ID GROUP BY prof.staff_ID,prof.last_name,prof.first_name");
    SQLExecDirectA(hstmt, (SQLCHAR *)query, SQL_NTS);

    printIntoRow(hstmt, 15);
    
    SQLFreeStmt(hstmt,SQL_CLOSE);
	system("pause");

}

void adminInfo() {

	int command = 0;
	int ISdone = 0;

	while (!ISdone) {
		system("CLS");
		cout << "=================== Administrative Information ======================\n\n";
		cout << "0. Return to the previous menu                          (input '0').\n";
		cout << "1. Change your password                                 (input '1').\n";
		cout << "2. Add a new phone                                      (input '2').\n";
		cout << "3. Show the TAs of your courses in the current semester (input '3').\n";
		cout << "4. Show preferred offerings for the TAs                 (input '4').\n";
		cout << "Please enter your choice: ";

		cin >> command;

		char buf[2];
		cin.getline(buf, 2); // grab the endline character when the user press "Enter"

		cout << endl;

		switch (command) {
		case 1:
			changePassword();
			ISdone = 0;
			break;

		case 2:
			addPhone();
			ISdone = 0;
			break;

		case 3:
			system("mode con cols=160 lines=50"); //make the screen larger
			showCourseTA();
			system("mode con cols=80 lines=50"); //restore the screen
			ISdone = 0;
			break;

		case 4:
			system("mode con cols=160 lines=50"); //make the screen larger
			showTAPref();
			system("mode con cols=80 lines=50"); //restore the screen
			ISdone = 0;
			break;
		case 0:
			ISdone = 1;
			break;
		default:
			break;
		}
	}
}

void changePassword() {
	char newPassword[11];
	char query[1000];

	cout << "\nPlease input your new password: (at most 10 characters):";
	cin >> newPassword;
	// TODO 6: Update the database for prof. password.
	// for the expect behaviour of this part please refer to the executable program
	// Add your code here
    SQLAllocStmt(hdbc, &hstmt);
    sprintf(query,"UPDATE prof SET password=\'%s\' WHERE staff_id=\'%i\'",newPassword, staff_id);
    SQLExecDirectA(hstmt, (SQLCHAR *)query, SQL_NTS);
    cout<<"Password updated!\n";
    SQLFreeStmt(hstmt,SQL_CLOSE);
	system("pause");

}

void addPhone() {
	int newPhone;
	char query[1000];

	cout << "\nPlease input the new phone number you want to add:";
	cin >> newPhone;
	// TODO 7:  add a new phone number for the prof. assume user always enter a new phone number so you do not need to check
	// for the expect behaviour of this part please refer to the executable program
	// Add your code here
    //the prof does not abandon his/her old phone
    SQLAllocStmt(hdbc, &hstmt);
    sprintf(query,"INSERT INTO prof_phone(staff_ID,phone_number) VALUES (\'%i\',\'%i\')",staff_id,newPhone);
    SQLExecDirectA(hstmt, (SQLCHAR *)query, SQL_NTS);
    cout<<"Phone number added!\n";
    SQLFreeStmt(hstmt,SQL_CLOSE);
	system("pause");

}

void showCourseTA() {
	SQLAllocStmt(hdbc, &hstmt);
	char query[1000];
	char currentSem[100] = "Spring2018";

	system("CLS");

	cout << "Here are TA information of all the courses you are teaching in the current semester:\n";

	// TODO 8: displays the student_ID, last_name, first_name, and phone number for each TA of the course offerings he/she teaches in the current semester (assume the current semester is Spring2018).
	// Add your code here
    int student_ID;
    char last_name[20];
    char first_name[20];
    int phone, offering_no;
    char course_ID[20];
    char course_name[20];
    SQLINTEGER cbstudent_ID, cblast_name, cbfirst_name, cbphone,cbcourse_ID,cbcourse_name, cboffering_no;
    
    sprintf(query,"SELECT prof_teach.course_ID AS \"Course ID\",course.course_name AS \"Course name\",prof_teach.offering_no AS \"Offering number\",TA.student_ID AS \"TA student ID\",TA.last_name AS \"TA Last Name\", TA.first_name AS \"TA First Name\",TA.phone AS \"TA phone no.\" FROM prof_teach,course,TA,offering where prof_teach.course_ID=TA.course_ID AND prof_teach.course_ID=course.course_ID AND offering.offering_no=prof_teach.offering_no AND offering.offering_no=TA.offering_no AND offering.YearSemester=\'%s\' AND prof_teach.staff_ID=\'%i\'",currentSem,staff_id);

    SQLExecDirectA(hstmt, (SQLCHAR *)query, SQL_NTS);
    SQLBindCol(hstmt,1,SQL_C_CHAR,course_ID,20,&cbcourse_ID);
    SQLBindCol(hstmt,2,SQL_C_CHAR,course_name,20,&cbcourse_name);
    SQLBindCol(hstmt,3,SQL_INTEGER,&offering_no,10,&cboffering_no);
    SQLBindCol(hstmt,4,SQL_INTEGER,&student_ID,10,&cbstudent_ID);
    SQLBindCol(hstmt,5,SQL_C_CHAR,last_name,20,&cblast_name);
    SQLBindCol(hstmt,6,SQL_C_CHAR,first_name,20,&cbfirst_name);
    SQLBindCol(hstmt,7,SQL_INTEGER,&phone,10,&cbphone);
    printIntoRow(hstmt, 15);
    SQLFreeStmt(hstmt,SQL_CLOSE);

	system("pause");

}

void showTAPref() {
	SQLAllocStmt(hdbc, &hstmt);
	char query[1000];
	char currentSem[100] = "Spring2018";

	system("CLS");

	cout << "Here are course preference information of the TAs:\n";

	// TODO 9: display the lists of all the preferred offerings (course_ID, offering_no) for all the TAs, group the result by the TAs� student_IDs.
	// Hint: you will find the aggregate function LISTAGG() function useful.
	// Add your code here
    //TA student ID, Last Name, First Name, Preference list
    //use table TA and pref-offering
  
    sprintf(query,"SELECT TA.student_ID AS \"TA student ID\",TA.last_name AS \"Last Name\",TA.first_name AS \"First Name\",listagg(pref_offering.course_ID || ' ' || nvl(to_char(pref_offering.offering_no),' '),',') WITHIN GROUP(ORDER BY pref_offering.course_ID) AS \"Preference list\" FROM TA,pref_offering where TA.student_ID=pref_offering.student_ID GROUP BY TA.student_ID,TA.last_name,TA.first_name");
    SQLExecDirectA(hstmt, (SQLCHAR *)query, SQL_NTS);
    printIntoRow(hstmt,15);
    SQLFreeStmt(hstmt,SQL_CLOSE);
    

	system("pause");
}


boolean userLogin() {

	int loginOption;
	char username[20], password[11];
	char query[1000];
	char inputUsername[20];
	string inputPassword;
	SQLINTEGER cbUsername, cbPassword;


	boolean success = false;

	while (!success) {
		system("CLS"); // Clear the screen
		cout << "===Welcome to Information System of the University of ST===\n\n";
		cout << "Please choose one of the follow options:\n";
		cout << "0. to terminate the program                      (input '0').\n";
		cout << "1. Log in as a professor                         (input '1').\n";
		cout << "Please enter your choice: ";
		cin >> loginOption;

		char buf[2];
		cin.getline(buf, 2); // grab the endline character when the user press "Enter"

							 // Exit if the user keys in "0"
		if (loginOption == 0)
			break;

		if (loginOption == 1) {

			cout << "Please enter your username: ";
			cin >> inputUsername;

			// Exit if the user keys in "0"
			//if (strcmp(inputUsername,"0") == 0)
			//	break;

			cout << "Please enter your Password: ";
			inputPassword = "";

			// Get the input character by character and mask the password
			boolean flag = true;
			while (flag)
			{
				char chr = getch();
				if (chr == '\r') flag = false;

				else { cout << '*'; inputPassword += chr; }
			}

			cin.clear();
			cin.ignore(256, '\n');
			cout << endl;

			// This part has been Done for you.
			// Check whether the user is a valid user. This is done through checking inputUsername and password and see whether
			// they match with prof.user_name and prof.password. The user_name and password are new added attributes, please
			// refer to the 'insert_record.sql' for the exact values. Note that we are using SQLExecDirectA(), and we copy the
			// returned values from the Oracle server using the SQLBindCol() function. We then SQLFetch() to retrieve the results
			//(the columns: user_name,password,staff_id) from Oralce and copy them to the local variables username,password,
			//staff_id.

			SQLAllocStmt(hdbc, &hstmt);
			sprintf_s(query, "select user_name, password, staff_id from prof where user_name=\'%s\'", inputUsername);
			SQLExecDirectA(hstmt, (SQLCHAR *)query, SQL_NTS);
			SQLBindCol(hstmt, 1, SQL_C_CHAR, username, 20, &cbUsername);
			SQLBindCol(hstmt, 2, SQL_C_CHAR, password, 11, &cbPassword);
			SQLBindCol(hstmt, 3, SQL_INTEGER, &staff_id, 1, &cbStaffId);
			ret = SQLFetch(hstmt);

			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				if (strcmp(inputPassword.c_str(), password) != 0) {
					cout << "Password Incorrect, please try again.\n";
					success = false;
					system("pause");
					system("CLS");

				}
				else {
					success = true;
				}
			}
			else {
				cout << "User does not exist, please try again.\n";
				success = false;
				system("pause");
				system("CLS");

			}
			SQLFreeStmt(hstmt, SQL_CLOSE);
			cout << endl;
		}
	}

	return success;
}












boolean ConnectDB() {
	RETCODE        ret;
	/* Allocate environment handle */
	ret = SQLAllocEnv(&henv);
	/* Allocate connection handle */
	ret = SQLAllocConnect(henv, &hdbc);
	/* Connect to the service */

	char oracleAccountName[20];
	string inputPassword;

	cout << "========Information System DB manager logon page=========\n\n";

	cout << "Please enter your Oracle account username: ";
	cin >> oracleAccountName;
	cout << "Please enter your Oracle account Password: ";
	inputPassword = "";

	// Mask the password
	boolean flag = true;
	while (flag) {
		char chr = getch();
		if (chr == '\r') flag = false;

		else { cout << '*'; inputPassword += chr; }
	}
	cout << endl;


	ret = SQLConnectA(hdbc, (SQLCHAR*) "comp3311.cse.ust.hk", SQL_NTS, (SQLCHAR*)oracleAccountName, SQL_NTS, (SQLCHAR*)inputPassword.c_str(), SQL_NTS);
	system("CLS"); // clear the screen
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		return TRUE;
	return FALSE;
}

void DisconnectDB() {
	SQLDisconnect(hdbc);
	SQLFreeConnect(hdbc);
	SQLFreeEnv(henv);
}

// A generic function prints each row of return value into a column of information
void printRecordIntoCol(HSTMT stmt) {
	SQLCHAR columnName[10][64];
	SQLSMALLINT  noOfcolumns, columNameLength[10], dataType[10], decimalDigits[10], nullable[10];
	SQLULEN columnSize[10];
	SQLCHAR buf[10][64];
	SQLINTEGER indicator[10];

	// Find the total number of columns
	SQLNumResultCols(stmt, &noOfcolumns);

	for (int i = 0; i < noOfcolumns; i++) {
		// Retrieve the column metadata
		ret = SQLDescribeColA(hstmt, i + 1, columnName[i], sizeof(columnName[i]), &columNameLength[i], &dataType[i], &columnSize[i], &decimalDigits[i], &nullable[i]);
		// Bind the column result
		SQLBindCol(stmt, i + 1, SQL_C_CHAR, buf[i], sizeof(buf[i]), &indicator[i]);
	}

	// Print out the query result
	while (SQL_SUCCEEDED(SQLFetch(stmt))) {
		for (int i = 0; i < noOfcolumns; i++) {
			cout << setw(15) << left << columnName[i] << ": " << buf[i] << endl;
		}
		//cout<<endl;
	}
}

// A generic function prints out the query result by column with a maximum column width
SQLCHAR*** printIntoRow(HSTMT stmt, int maxColumnWidth) {
	SQLCHAR columnName[10][64];
	SQLSMALLINT  noOfcolumns, columNameLength[10], dataType[10], decimalDigits[10], nullable[10];
	SQLULEN columnSize[10];
	SQLCHAR buf[10][64];
	SQLCHAR*** result;
	SQLINTEGER indicator[10];

	// Find the total number of columns
	SQLNumResultCols(stmt, &noOfcolumns);


	for (int i = 0; i<maxColumnWidth*noOfcolumns; i++)
		cout << "-";
	cout << endl;

	// Initialize the result pointer
	result = new SQLCHAR**[200];
	for (int i = 0; i<100; i++) {
		result[i] = new SQLCHAR*[10];
		for (int j = 0; j<noOfcolumns; j++) {
			result[i][j] = new SQLCHAR[64];
		}
	}

	for (int i = 0; i < noOfcolumns; i++) {
		// Retreive the column metadata
		ret = SQLDescribeColA(hstmt, i + 1, columnName[i], sizeof(columnName[i]), &columNameLength[i], &dataType[i], &columnSize[i], &decimalDigits[i], &nullable[i]);

		// Bind the column result
		SQLBindCol(stmt, i + 1, SQL_C_CHAR, buf[i], sizeof(buf[i]), &indicator[i]);
	}

	int totalLength = 0;
	// Print out the column name
	for (int i = 0; i < noOfcolumns; i++) {
		if (columnSize[i]<columNameLength[i]) {
			cout << setw(columNameLength[i] + 2) << left << columnName[i];
			totalLength += columNameLength[i] + 2;
		}
		else if (columnSize[i]<maxColumnWidth) {
			cout << setw(columnSize[i] + 2) << left << columnName[i];
			totalLength += columnSize[i] + 2;
		}
		else {
			cout << setw(maxColumnWidth) << left << columnName[i];
			totalLength += maxColumnWidth;
		}
	}
	cout << endl;

	// Print out separate line
	for (int i = 0; i<maxColumnWidth*noOfcolumns; i++)
		cout << "-";
	cout << endl;

	// Print out the query result row by row
	int j = 0;
	while (SQL_SUCCEEDED(SQLFetch(stmt))) {
		for (int i = 0; i < noOfcolumns; i++) {
			// deep copy
			for (int k = 0; k<sizeof(buf[i]); k++) {
				result[j][i][k] = buf[i][k];
			}
			if (columnSize[i]<columNameLength[i]) {
				cout << setw(columNameLength[i] + 2) << left << buf[i];
			}
			else if (columnSize[i]<maxColumnWidth) {
				cout << setw(columnSize[i] + 2) << left << buf[i];
			}
			else {
				cout << setw(maxColumnWidth) << left << buf[i];
			}
		}
		j++;
		cout << endl;
	}

	return result;
}
