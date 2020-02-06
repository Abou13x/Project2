// Bible class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

Bible::Bible() { // Default constructor
	infile = "/home/class/csc3004/Bibles/web-complete";
}

// Constructor – pass bible filename
Bible::Bible(const string s) { infile = s; }

// REQUIRED: lookup finds a given verse in this Bible
const Verse Bible::lookup(Ref ref, LookupResult& status) { 
    Verse aVerse;
    // TODO: scan the file to retrieve the line with ref ...
    // update the status variable,
	// create and return the verse object

	string buffer, verseText;
	/* read in every verse */

	if (!isOpen) {
		instream.open(infile.c_str(), ios::in);
		if (!instream) {
			cerr << "Error - can't open input file: " << infile << endl;
			exit(2);
		}
	}
	else {
	

			if (currentRef == ref) {
				aVerse = Verse(currentLine);
				status = SUCCESS;
				return(aVerse);
			}




	}

	

	isOpen = true;

	do {
		/* get the next verse */
		getline(instream, buffer); //
		Ref ref2(buffer);
		currentLine = buffer;
		currentRef = ref2;
		/* See if it is the right book */
		//ref2.display();
			if (ref == ref2) {
				//create a verse
				aVerse = Verse(buffer);
				status = SUCCESS;
				return(aVerse);
			}

			// if search is not successful
			if (ref.getBook() > 66 || ref.getBook() < 1) {
				status = NO_BOOK;
				return(aVerse);
			}

			if (ref.getBook() < currentRef.getBook()){ //ref.getBook() < currentRef.getBook()
				status = NO_CHAPTER;
				return(aVerse);
			}
			if (currentRef.getChap() > ref.getChap() && ref.getBook() == currentRef.getBook()) {
				status = NO_VERSE;
				return(aVerse);
			}
		
				
			

			

			
	} while (!instream.fail());

	status = OTHER;
	return(aVerse);


   // return(aVerse);
}

// REQUIRED: Return the reference after the given ref
const Ref Bible::next(const Ref ref, LookupResult& status) {
	string buffer, verseText;
	if (isOpen) {
		if (currentRef == ref) {
			getline(instream, buffer);
			currentLine = buffer;
			Ref ref3(buffer);
			status = SUCCESS;
			currentRef = ref3;
			return ref3;
		}
	}
	
	lookup(ref, status);

	if (status == SUCCESS) {
		if (currentRef == ref) {
			getline(instream, buffer);
			currentLine = buffer;
			Ref ref4(buffer);
			status = SUCCESS;
			currentRef = ref4;
			return ref4;
		}
	}
	 

	Ref ref5;
	return ref5;


	
};

// OPTIONAL: Return the reference before the given ref
const Ref prev(const Ref ref, LookupResult& status) {};

// Return an error message string to describe status
const string Bible::error(LookupResult status) {

	if (status == NO_BOOK) {
		
		return "Could not find the book \n";
	}
	
	if (status == NO_CHAPTER) {
		return "Could not find the chapter \n";
	}
	if (status == NO_VERSE) {
		return "Could not found the verse \n";
	}
	if(status == OTHER){
		return "Could not determine the error \n";
	}
	else {
		cout << "Strange!!!!!!";
	}
	
};

void Bible::display() {
	cout << "Bible file: " << infile << endl;
}
