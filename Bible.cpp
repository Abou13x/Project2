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
#include<map>
#include<iterator>
#include<list>
using namespace std;

Bible::Bible() { // Default constructor
	infile = "/home/class/csc3004/Bibles/web-complete";
	buildTextIndex(infile);
}

// Constructor – pass bible filename
Bible::Bible(const string s) { 
	infile = s; 
	buildTextIndex(infile);
}

// REQUIRED: lookup finds a given verse in this Bible
const Verse Bible::lookup(Ref ref, LookupResult& status) { 

	map<Ref, int>::iterator it;

	list<Verse> blank; 
	string aVerse;
	Verse verse = Verse();




	it = index.find(ref);

	if (it == index.end()) {
		//status = OTHER;

		if (ref.getBook() > 66 || ref.getBook() < 1) {
			status = NO_BOOK;
		}

		Ref ref1 = Ref(ref.getBook(), ref.getChap(), 1);
		it = index.find(ref1);

		if (it == index.end()) {
			status = NO_CHAPTER;
		}
		else {
			Ref ref6 = Ref(ref.getBook(), ref.getChap(), ref.getVerse());
			it = index.find(ref6);

			if (it == index.end()) {
				status = NO_VERSE;
			}
		}

		return (verse);
	}
	else {

		instream.open(infile.c_str(), ios::in);
		if (!instream) {
			cerr << "Error - can't open input file: " << infile << endl;
			status = OTHER;
			return verse;
		}

		
		int pos = index[ref];
		instream.seekg(pos);
		getline(instream, aVerse);
		verse = Verse(aVerse);
		status = SUCCESS;
		instream.close();
		return(verse);
	}



    //Verse aVerse;
    // TODO: scan the file to retrieve the line with ref ...
    // update the status variable,
	// create and return the verse object

	//string buffer, verseText;
	/* read in every verse */

	//if (!isOpen) {
		//instream.open(infile.c_str(), ios::in);
		//if (!instream) {
		//	cerr << "Error - can't open input file: " << infile << endl;
		//	exit(2);
		//}
	//}
	//else {

		//	if (currentRef == ref) {
			//	aVerse = Verse(currentLine);
			//	status = SUCCESS;
			//	return(aVerse);
			//}

	//}

	//isOpen = true;

	//do {
		/* get the next verse */
		//getline(instream, buffer); //
		//Ref ref2(buffer);
		//currentLine = buffer;
		//currentRef = ref2;
		/* See if it is the right book */
		//ref2.display();
		//	if (ref == ref2) {
				//create a verse
			//	aVerse = Verse(buffer);
			//	status = SUCCESS;
			//	return(aVerse);
		//	}

			// if search is not successful
		//	if (ref.getBook() > 66 || ref.getBook() < 1) {
			//	status = NO_BOOK;
			//	return(aVerse);
		//	}

		//	if (ref.getBook() < currentRef.getBook()){ //ref.getBook() < currentRef.getBook()
			//	status = NO_CHAPTER;
			//	return(aVerse);
			//}
			//if (currentRef.getChap() > ref.getChap() && ref.getBook() == currentRef.getBook()) {
			//	status = NO_VERSE;
			//	return(aVerse);
		//	}
			
	//} while (!instream.fail());

	//status = OTHER;
	//return(aVerse);


   // return(aVerse);


}

// REQUIRED: Return the reference after the given ref
const Ref Bible::next(const Ref ref, LookupResult& status) {
	map<Ref, int>::iterator it;
	Ref ref2;
	Verse verse2 = Verse();

	it = index.find(ref);

	if (it == index.end()) {
		

		if (ref.getBook() > 66 || ref.getBook() < 1) {
			status = NO_BOOK;
		}

		Ref ref1 = Ref(ref.getBook(), ref.getChap(), 1);
		it = index.find(ref1);

		if (it == index.end()) {
			status = NO_CHAPTER;
		}
		else {
			Ref ref6 = Ref(ref.getBook(), ref.getChap(), ref.getVerse());
			it = index.find(ref6);

			if (it == index.end()) {
				status = NO_VERSE;
			}
		}


		return (ref2);
	}
	else {

		string aVerse;

		instream.open(infile.c_str(), ios::in);
		if (!instream) {

			cerr << "Error - can't open input file: " << infile << endl;
			status = OTHER;
			return ref2;
		}


		int pos = index[ref];
		instream.seekg(pos);
		getline(instream, aVerse);

		getline(instream, aVerse);
		Ref ref3 = Ref(aVerse);
		status = SUCCESS;

		instream.close();
		return(ref3);
	}






	//string buffer, verseText;
	//if (isOpen) {
	//	if (currentRef == ref) {
		//	getline(instream, buffer);
		//	currentLine = buffer;
		//	Ref ref3(buffer);
		//	status = SUCCESS;
		//	currentRef = ref3;
		//	return ref3;
		//}
	//}
	
	//lookup(ref, status);

	//if (status == SUCCESS) {
	//	if (currentRef == ref) {
		//	getline(instream, buffer);
		//	currentLine = buffer;
		//	Ref ref4(buffer);
		//	status = SUCCESS;
		//	currentRef = ref4;
		//	return ref4;
		//}
	//}	 

//	Ref ref5;
//	return ref5;
};

// OPTIONAL: Return the reference before the given ref
const Ref prev(const Ref ref, LookupResult& status) {};

// Return an error message string to describe status
const string Bible::error(LookupResult status) {

	if (status == NO_BOOK) {
		
		return "Could not find the book ";
	}
	
	if (status == NO_CHAPTER) {
		return "The chapter does not exist!";
	}
	if (status == NO_VERSE) {
		return "The verse does not exist! ";
	}
	if(status == OTHER){
		return "Could not determine the error ";
	}
	else {
		cout << "Strange!!!!!!";
	}
	
};

void Bible::display() {
	cout << "Bible file: " << infile << endl;
}

Ref Bible::getCurrentRef() {//return current ref to be used when displaying book names
	return currentRef;
}

int Bible::buildTextIndex(string filename) {
	ifstream infile ;
	int position;
	string line;
	
	infile.open(filename.c_str(), ios::in);

	if (!infile) {
		cerr << "Error - can't open input file: " << filename << endl;
		return 0;
	}

	while (!infile.fail()) {
		position = infile.tellg();
		getline(infile, line);

		if (infile.fail()) {
			break;
		}
		//cout << line << "line" << endl;
		Ref newRef(line);
		index[newRef] = position;
		

	}

	cout <<"Size of map " <<index.size()<<endl;
}
