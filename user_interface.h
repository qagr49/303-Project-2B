#pragma once
#include "Customer.h"
#include <vector>
#include <iostream>

class user_interface {
	int threads; // number of threads for parallel programming

public:
	// constructor
	user_interface(int thread) { threads = thread; }

	// welcome message + ask to log in with ID
	int askID(vector<Customer> & the_customers) {
		cout << "--------------------------------------------------" << endl;
		cout << "            Welcome to the Library!" << endl;
		cout << "--------------------------------------------------" << endl;
		cout << endl;
		
		int ID;
		string answer;
		cout << " Do you have an ID [Y: any key|N: no]: ";
		getline(cin, answer);

		if (answer == "N" || answer == "n") { 
			// create a new account
			cout << " Enter you name: ";
			getline(cin, answer);
			ID = the_customers.size() ;
			cout << " Your name is " << answer << ". Your ID is now " << ID << ". Thank you for registering.\n\n";
			the_customers.push_back(Customer(ID, answer));
		}
		else {
			cout << " ID: ";
			getline(cin, answer);
			ID = stoi(answer);

			// repeat untill valid ID is entered
			while (ID <0 || ID>the_customers.size() - 1) {
				cout << " Invalid ID, try again: ";
				getline(cin, answer);
				ID = stoi(answer);
			}
		}

		return ID;

	}

	// ask for the next action 
	int mainMenu(vector<Customer> & the_customers, int ID) {

		cout << " Hello " << (the_customers.at(ID)).getName() << ", press:";
		cout << "\n\t(1) Search for a book";
		cout << "\n\t(2) See your rated books";
		cout << "\n\t(3) Recommended for you";
		cout << "\n\t(4) Log out";
		cout << "\n Please select a key: ";

		string answer;
		bool continueFlag = 1;

		do {
			getline(cin, answer);
			if (answer != "1" && answer != "2" && answer != "3" && answer != "4" ) {
				cout << "  Enter a valid key: ";
			}
			else continueFlag = 0;
		} while (continueFlag);

		cout << endl;
		return stoi(answer);
	}

	// search book and rate it 
	void searchBook(vector<Customer> & the_customers, vector<Book> & the_books, int ID) {

		while (1) { // no stop condition but return when the user ask to go back to main menu
			cout << "\n\tDo you want to search with ISBN or Title/Author? ";
			cout << "\n\t\t(1) ISBN";
			cout << "\n\t\t(2) Title/Author";
			cout << "\n\t\t(4) Back to main menu\n";

			// do until valid key
			string answer, ISBN = "", title = "";
			int bookIndex; // this is what we really want to know
			bool continueFlag = 1;
			do {
				getline(cin, answer);
				if (answer != "1" && answer != "2" && answer != "4") {
					cout << "  Enter a valid key: ";
				}
				else continueFlag = 0;
			} while (continueFlag);


			if (answer == "4") return; // exit

			else if (answer == "2") { // search for indexes corresponding to title
				cout << " Enter a part of the title or author (case sensitive). Ex: type 'Homer' for 'The Iliad by Homer '\n";
				
				vector<int> bookIndexes;
				getline(cin, title);

				// paralel programming here
				omp_set_num_threads(threads);
#pragma omp parallel for
				for (int i = 0; i < the_books.size(); i++) {
					for (int j = 0; j < the_books.at(i).getTitle().size(); j++) {
						if (the_books.at(i).getTitle().substr(j, title.size()) == title) {
							bookIndexes.push_back(i);
							break;
						}
					}
				}
				
				if (bookIndexes.size() == 0) cout << "No book found!";
				else {
					cout << " " << bookIndexes.size() << " book(s) found:\n";

					for (int i = 0; i < bookIndexes.size(); i++) {
						string to_print = the_books.at(bookIndexes.at(i)).getTitle() + "\n";
						cout << to_print;
					}


				}
			}


			else { // case 1
				cout << " Exact ISBN: ";
				getline(cin, ISBN);

				for (int i = 0; i < the_books.size(); i++) {
					if ((the_books.at(i)).getISBN() == ISBN) {
						title = (the_books.at(i)).getTitle();
						bookIndex = i;
						break;
					}
				}



				if (ISBN == "" || title == "") cout << " Book not found. Back to search menu\n"; // go back to menu "search with isbn or author?"
				else {
					cout << title << " has ISBN " << ISBN << ". Do you want to rate it? [Y:yes|Any key: no]\n";
					getline(cin, answer);
					if (answer == "Y" || answer == "y") {
						cout << " Enter an integer between 0 and 5: ";
						getline(cin, answer);
						(the_customers.at(ID)).rateBook(ISBN, stoi(answer));
					}
					cout << " Search again? [Y:yes|Any key: no]\n";
					getline(cin, answer);
					if (answer != "Y" && answer != "y") return;
				}
			}
			

		}
	}

	// show all the books that have been rated by the customer and their rating
	void seeRatedBooks(vector<Customer> & the_customers, vector<Book> & the_books, int ID) {
		the_customers.at(ID).printRatedBooks(the_books);
	}

	// show recommandations for the customer
	void recommandations(vector<Customer> & the_customers, vector<Book> & the_books, int ID) {
		the_customers.at(ID).printRecommandations(the_customers, the_books, ID);
	}

	// goodbye message
	void outro() {
		cout << "--------------------------------------------------" << endl;
		cout << "       Thank you for visiting our library! " << endl;
		cout << "--------------------------------------------------" << endl;
	}

	void run(vector<Customer> & the_customers, vector<Book> & the_books) {

		int ID = askID(the_customers);

		bool continueFlag = 1;

		do {
			switch (mainMenu(the_customers, ID)) {
			case(1): searchBook(the_customers, the_books, ID);
				break;
			case(2): seeRatedBooks(the_customers, the_books, ID);
				break;
			case(3): recommandations(the_customers, the_books, ID);
				break;
			case(4): continueFlag = 0;
				outro();
			}

		} while (continueFlag);
	}
};