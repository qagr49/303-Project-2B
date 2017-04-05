#include <fstream>
#include "user_interface.h"



using namespace std;



int main() {
	// define the maximum number of threads to use in parallel programing
	int threads = 3;

	// creating the original data of the library

	// read books
	ifstream bookFile("books.txt");
	vector<Book> the_books;	// vector because the books will be referenced at by their index in the customers rated books list

	// ignore 1st line
	string firstLine;
	getline(bookFile, firstLine);

	while (bookFile.good()) { // create the vector of all books
		Book newBook;
		bookFile >> newBook;
		the_books.push_back(newBook);
	}

	bookFile.close();


	// read customers
	ifstream customersFile("customers.txt");
	vector<Customer> the_customers; // vector because the customers will be acceced by ID wich is their index

	// ignore 1st line
	getline(customersFile, firstLine);

	while (customersFile.good()) { // create the vector of all customers
		Customer newCustomer;
		customersFile >> newCustomer;
		the_customers.push_back(newCustomer);
	}

	customersFile.close();


	// read ratings
	ifstream rateFile("ratings.txt");

	// ignore 1st line
	getline(rateFile, firstLine);

	while (rateFile.good()) {
		string line;
		getline(rateFile, line);

		// read data
		string customerID =  line.substr(0, line.find(',')) ;
		int custID = stoi(customerID);  // convert to string to use it later

		int rating = stoi( line.substr(customerID.length() + 2, 1) );

		string ISBN = line.substr(customerID.length() + 4);

		// add book to this customer list
		(the_customers.at(custID)).rateBook(ISBN, rating);


	}

	// print the user interface
	user_interface interface(threads);
	interface.run(the_customers, the_books);

	system("pause");

}