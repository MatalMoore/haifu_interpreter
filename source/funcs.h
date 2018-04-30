#include <string>
#include <vector>
#include <iostream>
#include <map>

#ifndef FUNCS_H
#define FUNCS_H

#ifndef USE_G_COMPILER
#define max(a,b) a < b ? b : a
#endif

#ifndef USE_G_COMPILER
#define min(a,b) a > b ? b : a
#endif

//#ifdef USE_G_COMPILER
//#define nullptr ((void*)0);
//#endif

// returns the lower case version of the string
std::string lowerCase(const std::string& stringValue);

// returns a string of the first size character of the string
std::string prefix(const std::string& stringValue, const int size);
// returns a string of the last size character of the string
std::string suffix(const std::string& stringValue, const int size);

// returns a string with all non-letter characters removed
std::string removeNonLetters(const std::string& stringValue);
// returns a string with all non-letter characters removed except for hyphens
std::string removeNonLetters_hyphenated(const std::string& stringValue);

// returns the next word in line starting at look_start according to Haifu specifications,
//   setting word_start to the beginning of the first word found
//   and word_size to how many characters are in the first word found
std::string getNextHaifuWord(const std::string& line, const int look_start
  , int& word_start, int& word_size);

// returns the next word in line starting at look_start according to Haifu token specifications,
//   setting word_start to the beginning of the first word found
//   and word_size to how many characters are in the first word found
std::string getNextHaifuTokenWord(const std::string& line, const int look_start
  , int& word_start, int& word_size);

// displays the message to the output stream and interprets the input from the input stream as a string
void prompt(const std::string& message, std::string& input, std::ostream& output_stream = std::cout, std::istream& input_stream = std::cin);
// displays the message to the output stream and interprets the input from the input stream as an int
void prompt(const std::string& message, int& input, std::ostream& output_stream = std::cout, std::istream& input_stream = std::cin);
// displays the message to the output stream and interprets the input from the input stream as a double
void prompt(const std::string& message, double& input, std::ostream& output_stream = std::cout, std::istream& input_stream = std::cin);

// repeats the respective prompt() call until a valid input is received
void prompt_repeat(const std::string& message, std::string& input, std::ostream& output_stream = std::cout, std::istream& input_stream = std::cin);
// repeats the respective prompt() call until a valid input is received
void prompt_repeat(const std::string& message, int& input, std::ostream& output_stream = std::cout, std::istream& input_stream = std::cin);
// repeats the respective prompt() call until a valid input is received
void prompt_repeat(const std::string& message, double& input, std::ostream& output_stream = std::cout, std::istream& input_stream = std::cin);

// returns whether the character is a whitespace character
bool isWhitespace(const char value);
// returns whether the character is a word character
bool isWordChar(const char value);
// returns whether the character is a letter character
bool isLetterChar(const char value);
// returns whether the character is a valid Haifu character
bool isHaifuChar(const char value);

// returns whether value is in the vector
bool isInVector(const std::vector<int>& values, const int value);
// returns whether the value at index exists at a lower index in the vector
bool isBeforeInVector(const int index, const std::vector<int>& values);

// returns whether the files indicated by the file names have the same data
bool areFilesIdentical(const std::string& filename0, const std::string& filename1);

// returns whether end of the input stream has been reached
bool endOfStream(std::istream& input_stream);

// removes redundant values from the vector
void compressVector(std::vector<int>& value);

// returns the loag_10 of the operand
int log10_int(const int operand);

// returns value rounded away from 0 (0.5 goes to 1.0, -0.5 goes to -1.0)
double round_away(const double value);

// adds all the values of values1 into values0
const std::vector<int>& add_insert(std::vector<int>& values0, const std::vector<int>& values1);

// streams out a vector of int
std::ostream& operator<<(std::ostream& o, const std::vector<int>& values);

#endif