/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * Mount Vernon Nazarene University
 * 
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching 
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 *     refers to the actual string entered in the form's "verse" field.
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
#include "Bible.h"
#include "Verse.h"
#include "Ref.h"
using namespace cgicc;

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


  /* TO DO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
   *        TO LOOK UP THE REQUESTED VERSES
   */


  Bible webBible("/home/class/csc3004/Bibles/web-complete");

  Verse verse2;
  LookupResult result = OTHER;

  if (nv != cgi.getElements().end()) {

	  int verseN = nv->getIntegerValue();

	  if (verseN == 0) {
		  verseN = 1;
	  }

	  Ref ref(bookNumber, chapterNumber, verseNumber);

	  cout << "Search Type: <b>" << **st << "</b>" << endl;
	  cout << "<p>Your result: </p>"<< endl;

	  int currentChapterNumber = ref.getChap();//will store the give chapter before it is updated

	  cout << ref.getBookName(bookNumber)<<" " <<ref.getChap();

	  for (int i = 0; i < verseN; i++) {
		  verse2 = webBible.lookup(ref, result);

		  if (result != SUCCESS) {//displays error massage if result is not successful
			  if (result == NO_CHAPTER) {
				  cout << "<p>Invalid Input: <em>" << ref.getBookName(bookNumber)<<" chapter " <<currentChapterNumber<<" "<< webBible.error(result) << "</em></p>" << endl;
			  }
			  if (result == NO_VERSE) {
				  cout << "<p>Invalid Input: <em>" << ref.getBookName(bookNumber) << " verse " << verseNumber << " " << webBible.error(result) << "</em></p>" << endl;
			  }
			 // cout << "<p>Invalid Input: <em>" << webBible.error(result) <<ref.getBookName(bookNumber)<< "</em></p>" << endl;
			  exit(result);
		  }


				  /* SEND BACK THE RESULTS
		   * Finally we send the result back to the client on the standard output stream
		   * in HTML text format.
		   * This string will be inserted as is inside a container on the web page,
		   * so we must include HTML formatting commands to make things look presentable!
		   */

		  if (validInput == true && validVerseNumber == true && result == SUCCESS && validBookNumber == true) {
		

			  if (currentChapterNumber == webBible.getCurrentRef().getChap()) {
				  cout << "<p>" << endl;
				  verse2.display();
				  cout << "</p>" << endl;
			  }
			  else {
				  if (currentChapterNumber != webBible.getCurrentRef().getChap()) {
					  currentChapterNumber = webBible.getCurrentRef().getChap();
					  cout << ref.getBookName(bookNumber) <<" " << ref.getChap();
				  }

				  cout << "<p>" << endl;
				  verse2.display();
				  cout << "</p>" << endl;
			  }

		  }
	
		  ref = webBible.next(ref, result);
	  }

  }
  
  
  return 0;
}
