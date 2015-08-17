#ifndef UTILITIES_H
#define UTILITIES_H
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

/* treating this as a library: utilities
for string functions like search term etc..
*/

bool searchTerm(const string& client, const string& matchee);
void deleteChar(string& word, char del);

#endif