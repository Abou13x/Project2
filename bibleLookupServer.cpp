#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sstream>
#include <map>
#include "fifo.h"
#include "Ref.h"
#include "Bible.h"
#include "Verse.h"
#include <string.h>



using namespace std;

string receive_pipe = "Biblerequest";
string send_pipe = "Biblereply";

string convertInt(int number)
{
	stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}

/* Server main line,create name MAP, wait for and serve requests */
int main() {
	/* Build an index of the text */
	Bible webBible("/home/class/csc3004/Bibles/web-complete");
	

	/* Create the communication fifos */
	Fifo recfifo(receive_pipe);
	Fifo sendfifo(send_pipe);

	string refString;
	Ref ref1;
	Ref currentRef;
	Verse aVerse;
	string message;
	LookupResult status;

	while (1) { // "infinite loop" for server: repeat each time a request is received
		
		/* get the stem of the search word */
		recfifo.openread();
		refString = recfifo.recv();
		if (refString != "next") {
			ref1 = Ref(refString);

			cout << "Receiving: |" << refString << "|" << endl;
			aVerse = webBible.lookup(ref1, status);
			currentRef = aVerse.getRef();//
		}
		else {
			cout << "Receiving: |" << refString << "|" << endl;
			ref1 = webBible.next(ref1, status);
			aVerse = webBible.lookup(ref1, status);
			currentRef = aVerse.getRef();
		}
		/* Send back the search term message */
		sendfifo.openwrite();
		if (status == SUCCESS) {

			message = aVerse.getVerse();
			//create a string message to send to the client
			std::ostringstream oss;
			//oss <<currentRef.getBook()<<":"<<currentRef.getChap()<<":"<<currentRef.getVerse()<<" "<<message << "&" << status;
			oss << currentRef.getBookName(currentRef.getBook()) << ":" << currentRef.getChap() << ":" << currentRef.getVerse() << " " << message << "&" << status;
			std::string newMessage = oss.str();
	
			cout << "Sending: |" << newMessage << "|" << endl;
				sendfifo.send(newMessage);
		}
		else {
			//create a string message to send to the client
			message = webBible.error(status);
			std::ostringstream oss;
			oss << message << "&" << status;
			std::string newMessage = oss.str();
			cout << "sending: |" << newMessage << "|" << endl;
			sendfifo.send(newMessage);
		}
		sendfifo.fifoclose();
		recfifo.fifoclose();
	}
}
