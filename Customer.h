#pragma once
#include "Book.h"
#include "ranking_list.h"
#include "omp.h"

using namespace std;


class Customer {
private:
	int ID;
	string name;
	ranking_list li;
public:

	// constructor
	Customer(int id, string NAME) { ID = id; name = NAME; }
	Customer(){}

	// overload stream operator
	friend istream& operator >> (istream& in, Customer& customer) {
		string line;
		getline(in, line);
		customer.ID = stoi(line.substr(0, line.find(','))); // extract ID
		customer.name = line.substr(line.find(',') + 2); // extract name
		return in;
	}

	int getID() { return ID; }
	string getName() { return name; }

	// rate a book and store it into the customer's list
	void rateBook(string isbn, int rate) { 
		li.insert(isbn, rate);
	}

	// return affinity between 2 customers
	double operator%(Customer rhs) {
		double affinity = li % (rhs.li);
		return affinity;
	}

	// print list of rated books
	void printRatedBooks(vector<Book> & the_books) {
		li.printRatedBooks(the_books);
	}

	ranking_list& getRankList(){return li;}

	
	

	// print list of recommandations        
	void printRecommandations(vector<Customer> & the_customers, vector<Book> & the_books, int ID) {
		cout << " Recommanded books for you: \n\n";

		int count = 0;
		vector<double> affinity;

		for (int i = 0; i < the_customers.size(); i++) {
			if (ID == i) affinity.push_back(0.0); // to ignore affinity of customer with himself
			else affinity.push_back(li % the_customers.at(i).li);         
		}

		vector<Book> bookList;
		vector<Book> printedList; // not to print a book twice
		int maxAffinityIndex = 0;
		int nbOfBooksToPrint = 6; // number of books to print

		while (count < nbOfBooksToPrint) { // loop untill we have printed enough books
			for (int i = 1; i < affinity.size(); i++) { // search for customer with best affinity
				if (affinity.at(i) > affinity.at(maxAffinityIndex)) maxAffinityIndex = i;
			}


			if (affinity.at(maxAffinityIndex) == 0) { // if no more matching customers
				if (count == 0) {
					cout << "No book found... sorry. " << endl << endl; // because no book to printed
					return;
				}
				else {														// because no more books to print
					cout << endl;
					return;
				}
			}

			bookList = li.getRecommandationList(the_customers.at(maxAffinityIndex).li, the_books); // find the books this customer rated well and our loged customer didn't read 

			for (int j = 0; j < bookList.size(); j++) { // for each book in this list
				bool alreadyPrinted = 0;
				for (int k = 0; k < printedList.size(); k++) { // this looks like O(n^3) but k will never do more than nbOFBooksPrinted iterations so it is O(n^2)
					if (bookList.at(j).getISBN() == printedList.at(k).getISBN()) alreadyPrinted = 1;
				}
				if (alreadyPrinted == 0) { // if it wasn't printed yet (by another customer) print it
					cout << "\t" << bookList.at(j).getISBN() << ", " << bookList.at(j).getTitle() << endl;
					count++;
					printedList.push_back(bookList.at(j));
					if (count == nbOfBooksToPrint) {
						cout << endl;
						return;
					}
				}
			}
			affinity.at(maxAffinityIndex) = 0.0; // ignore this customer in next loop
		}
		cout << endl;
	}
};