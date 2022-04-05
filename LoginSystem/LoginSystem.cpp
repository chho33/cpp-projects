// LoginSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void login();

void registration();

void reset();

int main()
{
	system("cls");
	cout << "=================  Login System  ==================\n" << endl;
	cout << "1. Login" << endl;
	cout << "2. Register" << endl;
	cout << "3. Forgot Password" << endl;
	cout << "4. Exit" << endl;
	cout << "Your option: ";
	int option;
	cin >> option;
	cout << endl;

	switch (option) {
	case 1:
		login();
		break;
	case 2:
		registration();
		break;
	case 3:
		reset();
		break;
	case 4:
		exit(1);
	default:
		cout << "Please enter one the above options.\n";
		system("pause");
		main();
	}
}

void login() {
	string username, password, user, passwd;
	bool found = false;

	system("cls");
	cout << "Enter Username: " << endl;
	cin >> username;
	cout << "Enter Password: " << endl;
	cin >> password;

	ifstream ifs("users.db");

	while (ifs >> user >> passwd) {
		if (user == username && password == passwd) {
			found = true;
			break;
		}
	}
	ifs.close();

	system("cls");

	if (found) {
		cout << "Welcome, " << username << "!\n" << endl;
		system("pause");
		main();
	}
	else {
		cout << "Your Username or Password is not correct, please try again.\n" << endl;
		system("pause");
		main();
	}
}

bool checkUser(string &username) {
	string user, passwd;
	ifstream ifs("users.db");

	while (ifs >> user >> passwd) {
		if (user == username) return true;
	}

	return false;
}

void registration() {
	string username, password;

	system("cls");

	cout << "Enter Your Username: " << endl;
	cin >> username;
	cout << "Enter Your Password: " << endl;
	cin >> password;

	bool exists = checkUser(username);

	if (exists) {
		cout << "Username already exists, please try again!" << endl;
		system("pause");
		registration();
	}

	ofstream ofs("users.db", ios::app);
	ofs << username << ' ' << password << endl;
	ofs.close();

	system("cls");

	cout << "Thank you for registration!\n" << endl;
	system("pause");

	main();
}

void reset() {
	string username, password, user, passwd;
	bool found = false;

	system("cls");

	cout << "Enter Your Username: " << endl;
	cin >> username;

	ifstream ifs("users.db");
	ofstream tmp("tmp");

	while (ifs >> user >> passwd) {
		if (user == username) {
			found = true;
		}
		else {
			tmp << user << ' ' << passwd << endl;
		}
	}
	ifs.close();
	tmp.close();

	if (found) {
		cout << "Please Reset Your Password: " << endl;
		cin >> password;
		
		ifstream tmp("tmp");
		//ofstream clear("users.db", ios::trunc);
		//clear.close();
		ofstream ofs("users.db");

		while (tmp >> user >> passwd) {
			cout << user << ' ' << passwd << endl;
			ofs << user << ' ' << passwd << endl;
		}
		ofs << username << ' ' << password << endl;

		ofs.close();
		tmp.close();

		cout << "Reset Successfully! Bye.\n";
		system("pause");
		main();
	}
	else {
		cout << "The username doesn't exist, bye.\n" << endl;
		system("pause");
		main();
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
