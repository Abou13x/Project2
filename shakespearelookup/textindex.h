/*  Filename:     textindex.h
 *  Author(s):    Jim Skon, Bob Kasper
 *  Last Update:  2/15/2018
 *
 *  Definitions: build and access Inverted Index
 *               of TEXT_FILENAME
 */

#include <list>
using namespace std;

#define TEXT_FILENAME "/var/www/html/class/csc3004/Shakespeare.txt"
#define LOG_FILENAME "/tmp/abodiomande-sslookup.log"

// interface functions

string getNextWord(string&);
int buildTextIndex (string);
list<int> indexSearch(string);
