#pragma once
#include <string>

using namespace std;

class Book {
private:
	string ISBN;
	string title;
public:
	friend istream& operator >> (istream& in, Book& book) {
		string line;
		getline(in, line);
		book.ISBN =  line.substr(0, line.find(',')); // extract ISBN
		book.title = line.substr(line.find(',') + 2); // extract title
		return in;
	}

	string getISBN() { return ISBN; }
	string getTitle() { return title; }

	// constructor
	Book(){}
	Book(string is, string ti) { ISBN = is; title = ti; }
};