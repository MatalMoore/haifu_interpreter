#ifndef SYLLABLE_PARSER_H
#define SYLLABLE_PARSER_H

#define $SP SyllableParser

#include <map>
#include <vector>
#include <string>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>

#include "WordData.h"
#include "funcs.h"

// error codes for ParseError
#define ERROR_WORD_LOOKUP 0
#define ERROR_STANZA_INCOMPLETE 1
#define ERROR_STANZA_SPACING 2
#define ERROR_STANZA_SYLLABLES 3

// maximum number of generated errors
#define ERROR_THRESHOLD 100

// the default vector size of the file data and generated errors
#define CLEAR_DATA_SIZE 52
#define CLEAR_ERRORS_SIZE 8

// field codes of the ParseError tuple
#define E_POS 0
#define E_STRING 1
#define E_ERROR 2

// a separating line that can be output
#define OUTPUT_LINE "----------------------------------------"

typedef std::map<std::string, std::vector<int>> SyllableMap;
typedef std::pair<int, int> Position;
typedef std::tuple<Position, std::string, int> ParseError;

class SyllableParser {
public:
  // loads file data from filename
  static bool loadFile(const std::string& filename);

  // calls clearFileData() and clearParseErrors
  static void clear();
  // clears the file data
  static void clearFileData();
  // clears the parse errors
  static void clearParseErrors();

  // sets the output stream field
  static void setOutput(std::ostream& output);

  // returns the loaded file data
  static const std::vector<std::string>& getFileData();

  // returns the output stream field
  static std::ostream& getOutput();
  // displays value to the output stream variable, and to cout if it is not cout,
  //   and returns the output stream field
  static std::ostream& displayOutput_cout(const std::string& value);
  // displays the errors to the indicates output stream
  static std::ostream& displayErrors(std::ostream& o = std::cout);

  // loads the data from filename into the file data and checks the stanzas of the file data
  static bool checkFileForm(const std::string& filename = ""
    , std::ostream& output = std::cout);

private:
  typedef SyllableParser __this;

  // the filename that is loaded
  static std::string s_filename;
  // the file data
  static std::vector<std::string> s_fileData;
  // the errors that are generated
  static std::vector<ParseError> s_parseErrors;
  // the words that have generated errors
  static std::map<std::string, bool> s_wordErrors;
  // the output stream
  static std::ostream* s_output;

  // loads line into the file data
  static void loadLine(const std::string& line);

  // generates an error and puts it into the error list
  static void makeError(
    const int pos_y, const int pos_x, const std::string& data, const int errorCode);

  // returns whether each stanza of the file data is of good form (generates errors)
  static bool checkStanzas();
  // returns whether the stanza starting at linNum in the file data is of good form (generates errors)
  static bool checkStanza(int& lineNum);
  // returns the syllable count of the line at lineNum in the file data (generates errors)
  static std::vector<int> checkLine(const std::string& line, const int lineNum);

  // returns whether an error has been generated from the line at lineNum in the file data
  static bool isSyllableErrorInLine(const int lineNum);
};

// displays the error to the output stream o
std::ostream& displayError(const ParseError& error, std::ostream& o = std::cout);

// displays the position to the output stream o
std::ostream& operator<<(std::ostream & o, Position pos);

#endif