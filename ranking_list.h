#pragma once
#include "list.h"
#include <string>
#include <vector>
#include <iostream>


using namespace std;

class Couple { // this is a struct to hold only book isbn and rating sorted by isbn
public:
	int ISBN, rating;  // this members are public because they will be accessed n^2 times when book recommendations is asked
					   // if we use getter/setter, this will have an overhead. Furthermore Couple is used only in this file so no problem

	Couple(string i, int r) { ISBN = stoi(i); rating = r; }

	bool operator<(Couple& c2) { // compare ratings, used for quick sort
		return rating < c2.rating;
	}
	bool operator>(Couple& c2) { // compare ratings, used for quick sort
		return rating > c2.rating;
	}

};

class ranking_list {
private:
	list<Couple> li;

public:
	// insert the couple ( ISBN, rating ) in the right place (ascending ISBN) 
	void insert(string isbn, int rank) { // insert a rating for the book with isbn
		Couple c(isbn, rank);
		if (li.empty()) li.push_back(c);
		else {
			list<Couple>::iterator iter = li.begin();
			while (iter != li.end()) {
				if (iter->ISBN == c.ISBN) {
					iter->rating = rank; // update previous rating
					return; 
				}
				if (iter->ISBN > c.ISBN) {
					li.insert(iter, c); // insert before
					return;
				}
				iter++;
			}
			li.push_back(c); // it goes last place
		}
	}

	// calculate affinity between two ranking lists
	double operator%(ranking_list rhs) {
		if (li.empty() || rhs.li.empty()) return 0;

		double affinity = 0;
		int count = 0;
		list<Couple>::iterator iter = li.begin();
		list<Couple>::iterator iter2 = rhs.li.begin();
		
		while (iter != li.end() && iter2 != rhs.li.end()) {
			if (iter->ISBN < iter2->ISBN) iter++;
			else if (iter->ISBN > iter2->ISBN) iter2++;
			else {
				affinity += 5 - abs(iter->rating - iter2->rating ); // 5 if same rating to 0 if totaly different
				count++;
				++iter;
				++iter2;
			}
		}

		if (count == 0) return 0; // no same book read
		else affinity /= count; // average of affinity in rating /5

		return affinity;

	}

	// print list of rated books
	void printRatedBooks(vector<Book> & the_books) {
		if (li.empty()) {
			cout << " You have not rated any book yet.\n\n";
			return;
		}

		cout << " You have rated: \n";

		list<Couple>::iterator iter = li.begin();
		for (int j = 0; j < li.size(); j++) { // parcours all book rated by user
			int ISBN = iter->ISBN, bookIndex;
			string title = "";
			for (int i = 0; i < the_books.size(); i++) {	// parcours all book in library to find title corresponding to ISBN
				if (stoi((the_books.at(i)).getISBN()) == ISBN) {	// we can improve this with binary search if we sort the_books
					title = (the_books.at(i)).getTitle();
					bookIndex = i;
					break;
				}
			}
			string stringISBN = to_string(ISBN);
			while (stringISBN.length() < 9) stringISBN = "0" + stringISBN;
			cout << stringISBN << ", " << title << ": " << iter->rating << "/5.\n";
			++iter;
		}
		cout << endl;
	}

	


	// return list of books recommanded by another customer passed to this function
	vector<Book> getRecommandationList(ranking_list & li2, vector<Book> & the_books) { 
		list<Couple>::iterator iter = li.begin();
		list<Couple>::iterator iter2 = li2.li.begin();

		vector<Couple> recommanded;

		while (iter != li.end() && iter2 != li2.li.end()) {
			if (iter->ISBN < iter2->ISBN) { // book is different
				if (iter2->rating > 3) {	// if the other customer rated the book well
					recommanded.push_back(*iter2);
				}
				iter++; // difference between cases 1 and 2
			}
			else if (iter->ISBN > iter2->ISBN) { // book is different
				if (iter2->rating > 3) {
					recommanded.push_back(*iter2);
				}
				iter2++;
			}
			else { // book already read by user
				++iter;
				++iter2;
			}
		}
		// sort the vector of recommandations by rating
		middle_quick_sort_wrapper(recommanded);

		vector<Book> bookList;
		for (int i = 0; i < recommanded.size(); i++) {
			// find the book title
			int ISBN = recommanded.at(i).ISBN;
			string title;
			int bookIndex;
			for (int i = 0; i < the_books.size(); i++) {
				if (stoi( (the_books.at(i)).getISBN() ) == ISBN) {
					title = (the_books.at(i)).getTitle();
					bookIndex = i;
					break;
				}
			}
			// add book to the list we want to return
			bookList.push_back(the_books.at(bookIndex));
		}

		return bookList; // this list is sorted by rating
		
	}




	/// quick sort function: 
	void sort_median(std::vector<Couple>& arr, int first, int last) {
		bool exchanges = true;
		int middle = (first + last) / 2;
		if (arr[first] > arr[middle]) {
			swap(arr[first], arr[middle]);
		}
		if (arr[middle] > arr[last]) {
			swap(arr[middle], arr[last]);
		}
		if (arr[first] > arr[middle]) {
			swap(arr[first], arr[middle]);
		}


		// swap the middle with the left 
		swap(arr[middle], arr[first]);
	}

	int partitionB(int first, int last, std::vector<Couple>& arr) {
	
		// Start up and down at either end of the sequence.
		// The first table element is the pivot value.

		int up = first + 1;
		int down = last - 1;
		sort_median(arr, first, last - 1); // always 3 comparisons
		int mid = first;
		do {
			while ((up != last - 1) && !(arr[mid] < arr[up])) {
				++up;
			}
			while (arr[mid] < arr[down]) {
				--down;
			}
			if (up < down) {
				// if up is to the left of down,
				swap(arr[up], arr[down]);
			}
		} while (up < down); // Repeat while up is left of down.
		swap(arr[mid], arr[down]);

		return down;
	}

	void middle_quick_sort(int first, int last, std::vector<Couple>& arr) {
		if (last - first > 1) {
			// There is data to be sorted.
			// Partition the table.
			int pivot = partitionB(first, last, arr);

			// Sort the left half.
			middle_quick_sort(first, pivot, arr);

			// Sort the right half.
			middle_quick_sort(pivot + 1, last, arr);
		}
	}

	void middle_quick_sort_wrapper(vector<Couple>& arr) {
		middle_quick_sort(0, arr.size(), arr);
	}
	/// end of quick sort functions
	
};