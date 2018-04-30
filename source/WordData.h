#ifndef WORD_DATA_H
#define WORD_DATA_H

#define $WD WordData

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <thread>
#include <sstream>
#include <algorithm>

#include "funcs.h"
#include "elements.h"

// This class can use a custam file type called HWD (Haifu Word Data).
// This file type cannot currently be used when compiled using the G compiler.
// The format of this file type should be as follows:
// - 4 bytes indicating the size of the remaining data
//     (intended to prevent a buffer from needing to be resized)
// - a series of data entries until EOF, each having the following format:
//   - a series of bytes ending in 0x00 representing an ASCII string
//       and indicating the entry name
//   - a series of bytes ending in 0x00 representing an ASCII string
//       and indicating the base word
//   - 1 byte indicating the element
//   - a series of bytes ending in 0x00 representing a vector of bytes
//       and indicating possible syllable counts
//       (a single possible syllable count can thus be at most 255,
//        and a count of 0 indicates the end of the vector)
//   - 1 byte indicating the end of the entry
//     (not strictly necessary, but allows for handling of file corruption)

// I cannot guarantee that using multiple threads is error-free.
// Increasing NUM_THREADS generally leads to
//   a shorter initialization time and slightly longer lookup times
// Using multiple threads is not generally needed,
//   and it only provides a noticeable improvement
//   when the data file is sufficiently large.
// I have also not sufficiently tested loading multiple files in this manner.
#define NUM_THREADS 1

// defines type and size of HWD data
#define HWD_DATA_TYPE char
#define HWD_DATA_SIZE sizeof(HWD_DATA_TYPE)

// hexadecimal values for delimiting word data
#define DELIMITER_FIELD 0
#define DELIMITER_INFO -1

// sets up functions based on threads to be used
#if NUM_THREADS > 1
#define STREAM_OUT(a) streamOut_multiple(a)
#define GET_MAP(a) getMap_multiple(a)
#define GET_MAP_SIZE() getMapSize_multiple()
#define LOAD_STRING_DATA(a,b) loadStringData_multiple(a,b)
#define LOAD_HWD_DATA(a,b,c) loadHWDData_multiple(a,b,c)
#define WRITE_HWD(a) writeHWD_multiple(a)
#else
#define STREAM_OUT(a) streamOut(a)
#define GET_MAP(a) getMap(a)
#define GET_MAP_SIZE() getMapSize()
#define LOOKUP(a) s_wordMap[a]
#define LOAD_STRING_DATA(a,b) loadStringData(a)
#define LOAD_HWD_DATA(a,b,c) loadHWDData(a,b)
#define WRITE_HWD(a) writeHWD(a)
#endif

#define GET_INFO(a) GET_MAP(a)[a]

// base word constants
#define BASE_WORD_DEFAULT ""
#define BASE_WORD_DEFAULT_STRING "_"
#define BASE_WORD_IDENTITY "."

// base word constants
#define SYLLABLE_COUNT_DEFAULT {}
#define SYLLABLE_COUNT_DEFAULT_STRING "_"

// word info codes for tuple access
#define WORD_INFO_BASE_WORD 0
#define WORD_INFO_ELEMENT 1
#define WORD_INFO_SYLLABLE_COUNT 2

// warning codes
#define SYLLABLE_COUNT_DEFAULT_WARNING 0
#define BASE_WORD_DEFAULT_WARNING 1
#define BASE_WORD_CHAIN_WARNING 2
#define ELEMENT_DEFAULT_WARNING 3
#define BASE_WORD_ELEMENT_DEFAULT_WARNING 4

// word info struct
struct WordInfo {
  std::string baseWord;
  char element;
  std::vector<int> syllableCount;

  WordInfo(
    const std::string& i_baseWord = BASE_WORD_DEFAULT
    , const char i_element = ELEM_DEFAULT
    , const std::vector<int>& i_syllableCount = SYLLABLE_COUNT_DEFAULT
    )
  {
    baseWord = i_baseWord;
    element = i_element;
    syllableCount = i_syllableCount;
  }

  bool operator==(const WordInfo& rhs) const {
    return
      baseWord == rhs.baseWord
      && element == rhs.element
      && syllableCount == rhs.syllableCount
      ;
  }

  bool operator!=(const WordInfo& rhs) const {
    return
      baseWord != rhs.baseWord
      || element != rhs.element
      || syllableCount != rhs.syllableCount
      ;
  }
};

// null word info constant
const WordInfo WORD_INFO_NULL = WordInfo();

typedef std::map<std::string, WordInfo> WordMap;
typedef std::map<std::string, const WordInfo*> WordWarningMap;

class WordData {
public:
  // returns the entry in the data indicated by key
  static const WordInfo& lookup(const std::string& key);

  // changes the base word field of the entry indicated by key in the data
  static void editBaseWord(const std::string& key, const std::string& baseWord, const bool skipWarnings = false);
  // changes the element field of the entry indicated by key in the data
  static void editElement(const std::string& key, const char element, const bool skipWarnings = false);
  // changes the syllable count field of the entry indicated by key in the data
  static void editSyllableCount(const std::string& key, const std::vector<int>& syllableCount, const bool skipWarnings = false);

  // sets the entry of key in the data to wordInfo
  static void insert(const std::string& key, const WordInfo& wordInfo, const bool overwrite = false);
  // sets the entry of key in the data to the respective values indicated by the parameters
  static void insert(const std::string& key
    , const std::string& baseWord, const char element, const std::vector<int>& syllableCount
    , const bool overwrite = false);

  // removes the entry of key in the data
  static void erase(const std::string& key);

  // stores the warnings indicated by warningCode in warningVector
  static void getWarnings_store(const int warningCode, std::vector<std::string>& warningVector);

  // displays all warnings indicated by warningCode to the output stream indicated by the class field
  static void displayWarnings(const int warningCode);
  // displays all warnings indicated by warningCode to the output stream indicated by the class field
  //   only if there are any such warnings
  static void displaySelectiveWarnings(const int warningCode);

  // checks if there are any warnings associated with key
  static void checkWarnings(const std::string& key);

  // updates the warnings based on possibly updated data
  static void updateWarnings();

  // loads data from filename using loadData_TXT() or loadData_HWD(), based on filename
  static void loadData(const std::string& filename = "data_in.hwd");
  // loads data from filename, treated as a TXT file
  static void loadData_TXT(const std::string& fileTitle = "data_in");
  // loads data from filename, treated as an HWD file
  static void loadData_HWD(const std::string& fileTitle = "data_in");

  // writes data to filename using writeData_TXT() or writeData_HWD(), based on filename
  static void writeData(const std::string& filename = "data_out.hwd");
  // writes data to filename using TXT format
  static void writeData_TXT(const std::string& fileTitle = "data_out");
  // writes data to filename using HWD format
  static void writeData_HWD(const std::string& fileTitle = "data_out");

  // removes all entries from the data
  static void clearWordMap();
  // removes all warnings
  static void clearWarnings();

  // streams the data to target
  static void stream(std::ostream& target);

  // changes the output stream class field
  static void setOutput(std::ostream& output);

  // returns the output stream class field
  static std::ostream& getOutput();

private:
  typedef WordData __this;

  static WordMap s_wordMap;
  static std::vector<WordMap> s_wordMaps;
  static std::vector<std::string> s_wordMaps_delimiters;

  static std::ostream* s_output;

  // warnings
  static WordWarningMap s_warnings_syllableCount_default;
  static WordWarningMap s_warnings_baseWord_default;
  static WordWarningMap s_warnings_baseWord_chain;
  static WordWarningMap s_warnings_element_default;
  static WordWarningMap s_warnings_baseWord_element_default;

  // returns the word map that contains key
  static WordMap& getMap(const std::string& key);
  // returns the word map that contains key
  static WordMap& getMap_multiple(const std::string& key);

  // returns the size of the word map
  static size_t getMapSize();
  // returns the sum of the sizes of all word maps
  static size_t getMapSize_multiple();

  // loads data from a vector of string without using threads
  static void loadStringData(const std::vector<std::string>& lines);
  // loads data from a vector of string using multiple threads
  static void loadStringData_multiple(const std::vector<std::string>& lines
    , const int numThreads);
  // loads data from a vector of string, executed by each thread
  static void loadStringData_thread(const std::vector<std::string>& lines,
    const int startIndex, const int numLines, const int threadID);

  // removes all blanks lines from lines, sorts the lines based on first word, and stores them as treatedLines
  static void treatLines_store(const std::vector<std::string>& lines, std::vector<std::string>& treatedLines);

  // loads data from an HWD buffer without using threads
  static void loadHWDData(const HWD_DATA_TYPE* buffer, const size_t bufferSize);
  // loads data from an HWD buffer using multple threads
  static void loadHWDData_multiple(const HWD_DATA_TYPE* buffer, const size_t bufferSize
    , const int numThreads);
  // loads data from an HWD buffer, executed by each thread
  static void loadHWDData_thread(const HWD_DATA_TYPE* buffer, const size_t bufferSize
    , const int threadID);

  // writes the data to a file pointer from a single word map using HWD format
  static void writeHWD(FILE* target);
  // writes the data to a file pointer from multiple word maps using HWD format
  static void writeHWD_multiple(FILE* target);

  // writes the data to a file pointer from a single word map using TXT format
  static void streamOut(std::ostream& target);
  // writes the data to a file pointer from multiple word maps using TXT format
  static void streamOut_multiple(std::ostream& target);

  // returns the index after first field delimiter found in buffer starting from index and bounded by bufferSize,
  //   storing the found word as word
  static int getWordFromBuffer(
    const HWD_DATA_TYPE* buffer
    , const size_t bufferSize
    , const size_t index
    , std::string& word
  );

  // returns the index after the first word info delimiter found in buffer starting from index and bounded by bufferSize,
  //   storing the found set of syllable counts as syllableCount
  static int getSyllableCountFromBuffer(
    const HWD_DATA_TYPE* buffer
    , const size_t bufferSize
    , const size_t index
    , std::vector<int>& syllableCount
  );

  // removes redundant values from the syllable count field of all entires in the data
  static void compressSyllables();

  // displays the string to the output stream indicated by the class field,
  //   as well as cout if this field does not indicate cout
  static std::ostream& displayOutput_cout(const std::string& value);
};

// the comparison function for a sort() call on pair<string, size_t> values
bool sortCompare(const std::pair<std::string, size_t>& item0, const std::pair<std::string, size_t>& item1);

#endif