// biblereader.cpp
// main function for Project 1

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
using namespace std;

main (int argc, char **argv) {
	// Create Bible object to process the raw text file
	Bible webBible("/home/class/csc3004/Bibles/web-complete");
	
	Verse verse;
	int b, c, v, vNumber;
	LookupResult result;
	cout << "Using Bible from: ";
	webBible.display();
	// Prompt for input: get reference to lookup
	// TODO: your program should get input from command line arguments instead.
	//cout << "Enter 3 integers for book, chapter and verse to find: " << flush;
	//cin >> b >> c >> v>>vNumber;
	
	
	//Ref ref(b, c, v);
	


	if (argc == 4) {
		b = atoi(argv[1]);
		c = atoi(argv[2]);
		v = atoi(argv[3]);

		Ref ref(b, c, v);
		cout << "Looking up reference: ";


		ref.display();
		cout << endl;

		cout << "Result: " << endl;

		verse = webBible.lookup(ref, result);

		if (result != SUCCESS) {
			cout << webBible.error(result);
			exit(result);
		}

		verse.display();
		cout << endl;
		//ref = webBible.next(ref, result);
	}
	else if (argc > 4) {
		b = atoi(argv[1]);
		c = atoi(argv[2]);
		v = atoi(argv[3]);
		vNumber = atoi(argv[4]);


		Ref ref(b, c, v);
		cout << "Looking up reference: ";


		ref.display();
		cout << endl;

		cout << "Result: " << endl;


		for (int i = 0; i < vNumber; i++) {
			verse = webBible.lookup(ref, result);

			if (result != SUCCESS) {
				cout << webBible.error(result);
				exit(result);
			}

			verse.display();
			cout << endl;
			ref = webBible.next(ref, result);
		}
	}
	else {
		cerr << "Invalid arguments." << endl;
		exit(2);
		}


	}


