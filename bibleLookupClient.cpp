#include <iostream>
#include <stdio.h>
#include <string.h>


//Stuff for pipes                                                               
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include "fifo.h"
using namespace std;

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
#include "Bible.h"
#include "Verse.h"
#include "Ref.h"
using namespace cgicc;


#define logging // enable log file
#define LOG_FILENAME "/tmp/abodiomande-PROGNAME.log"
#include "logfile.h"

// Pipes for communication                                                      
string receive_pipe = "Biblereply";
string send_pipe = "Biblerequest";

int main() {
	/* A CGI program must send a response header with content type
	 * back to the web client before any other output.
	 * For an AJAX request, our response is not a complete HTML document,
	 * so the response type is just plain text to insert into the web page.
	 */
	cout << "Content-Type: text/plain\n\n";

	Cgicc cgi;  // create object used to access CGI request data

	// GET THE INPUT DATA
	// browser sends us a string of field name/value pairs from HTML form
	// retrieve the value for each appropriate field name
	form_iterator st = cgi.getElement("search_type");
	form_iterator book = cgi.getElement("book");
	form_iterator chapter = cgi.getElement("chapter");
	form_iterator verse = cgi.getElement("verse");
	form_iterator nv = cgi.getElement("num_verse");


	Fifo recfifo(receive_pipe);
	Fifo sendfifo(send_pipe);

#ifdef logging
	logFile.open(logFilename.c_str(), ios::out);
#endif

	int verseNumber;
	int  chapterNumber;
	int bookNumber;
	// Convert and check input data
	bool validInput = false;
	if (chapter != cgi.getElements().end()) {
		chapterNumber = chapter->getIntegerValue();
		if (chapterNumber > 150) {
			cout << "<p>The chapter number (" << chapterNumber << ") is too high.</p>" << endl;
		}
		else if (chapterNumber <= 0) {
			cout << "<p>The chapter must be a positive number.</p>" << endl;
		}
		else
			validInput = true;
	}

	/* TO DO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you! */

	//checks for validity of chapter number

	bool validVerseNumber = false;

	if (verse != cgi.getElements().end()) {
		verseNumber = verse->getIntegerValue();

		if (verseNumber > 176) {
			cout << "<p>The verse number (" << verseNumber << ") is too high.</p>" << endl;
		}
		else if (verseNumber <= 0) {
			cout << "<p>The verse must be a positive number.</p>" << endl;
		}
		else {
			validVerseNumber = true;
		}
	}
	//checks for validity of book number

	bool validBookNumber = false;

	if (book != cgi.getElements().end()) {
		bookNumber = book->getIntegerValue();

		if (bookNumber > 66) {
			cout << "<p>The verse number (" << bookNumber << ") is too high.</p>" << endl;
		}
		else if (bookNumber <= 0) {
			cout << "<p>The verse must be a positive number.</p>" << endl;
		}
		else {
			validBookNumber = true;
		}
	}
//	log("after fifo open");

	int verseN;
	
	if (nv != cgi.getElements().end()) {
		if ((**nv).size() == 0) {
			verseN = 1;
		}
		else {
			verseN = nv->getIntegerValue();
		}
		
		std::ostringstream oss;
		oss << bookNumber << ":" << chapterNumber << ":" << verseNumber;
		std::string refString = oss.str();

		//log("created ref" + refString);//

		cout << "Search Type: <b>" << **st << "</b>" << endl;
		cout << "<p>Your result: </p>" << endl;
		//log("after first ouput");
	
		sendfifo.openwrite();
		log("Open Request Pipe");
		// Call server to get results                                                 
		sendfifo.send(refString);
		log("Request: " + refString);

		//variables to contain split words
		string word = "";
		string newMessage;
		string newStatus;
		string newResult;
		string next = "next";

		recfifo.openread();
		for (int i = 0; i < verseN; i++) {
			newResult = recfifo.recv();
			log("Reply1: " + newResult);
			for (auto splitString : newResult) {
				if (splitString == '&') {
					newMessage = word;
					log("ReplyNewMessage: " + newMessage);
					word = "";
				}
				else {
					word = word + splitString;
				}
			}
			log("ReplyWord: " + word);
			newStatus = word;

			log("Reply: " + newResult);

			//conver newStatus to int
			int intStatus = stoi(newStatus);
			log("ReplyIntStatus: " + intStatus);
			LookupResult statusResult = static_cast<LookupResult>(intStatus);
			log("ReplyStatus: " + statusResult);

			if (statusResult != SUCCESS) {//displays error massage if result is not successful
				//log("im here for the errors");
				//cout << "<p>" << endl;
			   // cout << newMessage;
				//cout << "</p>" << endl;
				cout << "<p>Invalid Input: <em>" << newMessage << "</em></p>" << endl;
				exit(statusResult);
			}

			if (validInput == true && validVerseNumber == true && statusResult == SUCCESS && validBookNumber == true) {

				cout << "<p>" << endl;
				cout << newMessage;
				cout << "</p>" << endl;

			}

			if (i < verseN - 1) {
				sendfifo.send(next);
				log("Request: " + next);
			}
		
		}

	}
	cout << endl; // flush output when done
	recfifo.fifoclose();
	log("close reply fifo");
	sendfifo.fifoclose();
	log("close request fifo");
	return 0;
}