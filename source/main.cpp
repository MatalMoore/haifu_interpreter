#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

#include "WordData.h"
#include "SyllableParser.h"
#include "TokenGenerator.h"
#include "ProgramExecutor.h"
#include "funcs.h"

#define INDENT "  "
#define INDENT_HYPHEN "--"

// string that will be displayed at the beginning of each command input prompt
#define COMMAND_PROMPT_STRING ">"

// file that stores the persistent data
#define PERSISTENT_DATA_FILENAME "__persistent_data.txt"

// commands
#define EXIT_COMMAND "exit"
#define HELP_COMMAND "help"
#define CHECK_COMMAND "check"
#define EDIT_COMMAND "edit"
#define INSERT_COMMAND "insert"
#define ERASE_COMMAND "erase"
#define LOOKUP_COMMAND "lookup"
#define UPDATE_COMMAND "update"
#define WARNINGS_COMMAND "warnings"
#define LOAD_DATA_COMMAND "load"
#define WRITE_DATA_COMMAND "write"
#define CLEAR_DATA_COMMAND "cleardata"
#define CLEAR_WARNINGS_COMMAND "clearwarnings"
#define RUN_COMMAND "run"
#define DUMP_COMMAND "dump"

// flags
#define FORCE_FLAG "-f"
#define NO_WARNINGS_FLAG "-nw"

// subcommands
#define BASE_WORD_STRING "baseword"
#define ELEMENT_STRING "element"
#define SYLLABLE_COUNT_STRING "syllables"

// other strings
#define DEFAULT_FIELD_STRING "."
#define SKIP_FIELD_STRING "-"
#define EXIT_COMMAND_STRING "*"
#define LOOKUP_DELIMITER_STRING " | "
#define UPDATE_DISPLAY_DELIMITER_STRING "--"

// displays the format of each command and what it does
bool displayHelp();

// checks the file indicated by the next value in the input stream for good Haifu form
//   warnings are accumulated in the word data
bool checkFile(istream& input);

// changes a single field of an existing entry in the persistent word data based on the values in the input stream
bool editEntry(istream& input);

// inserts an entry in the persistent word data based on the values in the input stream
bool insertEntry(istream& input);

// removes an entry from the persistent word data based on the values in the input stream
bool eraseEntry(istream& input);

// displays an entry in the persistent word data based on the values in the input stream
bool lookupEntry(istream& input);

// prompts the user to fix the accumulated warnings using the input stream
bool updateData(istream& input);

// displays the accumulated warnings
bool displayWarnings();

// loads word data into the persisten word data from the file indicated by the next value in the input stream
bool loadData(istream& input);

// writes the persistent word data to a file indicated by the next value in the input stream
bool writeData(istream& input);

// removes all entries from the persistent word data
bool clearData();

// removes all accumulated warnings
bool clearWarnings();

// checks the file indicated by the next value in the input stream
//   if it is of good Haifu form and it makes sense, it is executed as a Haifu program
bool runFile(istream& input);

// checks the file indicated by filename
//   if it is of good Haifu form and it makes sense, it is executed as a Haifu program
void runFile(const string& filename, const int argc, const char** args);

// toggles whether program execution is dumped to a log file
void toggleDump();

//-------------------------------------------------------------------------------
// main()
//-------------------------------------------------------------------------------
int main(int argc, char** argv) {
  ofstream output;
  string input;

  // sets the random seed that might be used in program execution
  srand((unsigned int)time(0));

  // loads persistent word data
  cout << endl;
  $WD::loadData_TXT("__persistent_data.txt");

  // command line execution with arguments (or drag-and-drop)
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      runFile(argv[i], 0, (const char**)0);
    }

    system("pause");
    return 0;
  }
  // standard execution
  else {
    cout << endl;
    cout << "Welcome to the Super-Mega Haifu Interpreter++ by Matthew Moore." << endl;
    cout << "Enter \"" << HELP_COMMAND << "\" to see the list of valid commands." << endl;

    // my G compiler does not have functions like fread_s()
#ifndef USE_G_COMPILER
#else
    cout << endl;
    cout << "Loading from and writing to \".hwd\" files is disabled for the GNU compiler." << endl;
#endif

    // repeat the prompt until there is a valid input
    cout << endl;
    prompt_repeat(COMMAND_PROMPT_STRING, input, cout, cin);
    input = lowerCase(input);

    // input loop
    while (input != EXIT_COMMAND) {
      if (input == HELP_COMMAND) {
        displayHelp();
      }
      else if (input == CHECK_COMMAND) {
        checkFile(cin);
      }
      else if (input == EDIT_COMMAND) {
        editEntry(cin);

        cout << endl;
        $WD::writeData_TXT(PERSISTENT_DATA_FILENAME);
      }
      else if (input == INSERT_COMMAND) {
        insertEntry(cin);

        cout << endl;
        $WD::writeData_TXT(PERSISTENT_DATA_FILENAME);
      }
      else if (input == ERASE_COMMAND) {
        eraseEntry(cin);

        cout << endl;
        $WD::writeData_TXT(PERSISTENT_DATA_FILENAME);
      }
      else if (input == LOOKUP_COMMAND) {
        lookupEntry(cin);
      }
      else if (input == UPDATE_COMMAND) {
        updateData(cin);

        cout << endl;
        $WD::writeData_TXT(PERSISTENT_DATA_FILENAME);
      }
      else if (input == WARNINGS_COMMAND) {
        displayWarnings();
      }
      else if (input == LOAD_DATA_COMMAND) {
        loadData(cin);

        cout << endl;
        $WD::writeData_TXT(PERSISTENT_DATA_FILENAME);
      }
      else if (input == WRITE_DATA_COMMAND) {
        writeData(cin);
      }
      else if (input == CLEAR_DATA_COMMAND) {
        clearData();

        cout << endl;
        $WD::writeData_TXT(PERSISTENT_DATA_FILENAME);
      }
      else if (input == CLEAR_WARNINGS_COMMAND) {
        clearWarnings();
      }
      else if (input == RUN_COMMAND) {
        runFile(cin);
      }
      else if (input == DUMP_COMMAND) {
        toggleDump();
      }
      else {
        cout << "Invalid command: " << input << endl;
      }

      // ignores the rest of the input on the current line
      cin.clear();
      cin.ignore(INT_MAX, '\n');

      // repeat the prompt until there is a valid input
      cout << endl;
      prompt_repeat(COMMAND_PROMPT_STRING, input, cout, cin);
      input = lowerCase(input);
    }

    return 0;
  }
}

//-------------------------------------------------------------------------------
// displayHelp()
//-------------------------------------------------------------------------------
bool displayHelp() {
  cout << endl;

  cout << "Valid commands:" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << EXIT_COMMAND << endl;
  cout << INDENT << INDENT << "exits the interpreter" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << HELP_COMMAND << endl;
  cout << INDENT << INDENT << "displays this list of valid commands" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << CHECK_COMMAND << " [" << NO_WARNINGS_FLAG << "] filename" << endl;
  cout << INDENT << INDENT << "checks if filename matches the Haifu form" << endl;
  cout << INDENT << INDENT << "and accumulates any missing or incorrect word data" << endl;
  cout << INDENT << INDENT << "(" << NO_WARNINGS_FLAG << " disables the display of warnings)" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << EDIT_COMMAND << " entry_name (field_name field_value)" << endl;
  cout << INDENT << INDENT << "looks up entry_name in the persistent word data" << endl;
  cout << INDENT << INDENT << "and changes the value of its field_name field to field_value" << endl;
  cout << INDENT << INDENT << "with (field_name field_value) havng one of the following forms:" << endl;
  cout << INDENT << INDENT << INDENT << "\"" << BASE_WORD_STRING << "\""
    << " base_word" << endl;
  cout << INDENT << INDENT << INDENT << "\"" << ELEMENT_STRING << "\""
    << " element" << endl;
  cout << INDENT << INDENT << INDENT << "\"" << SYLLABLE_COUNT_STRING << "\""
    << " int [int ... int]" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << INSERT_COMMAND << " [" << FORCE_FLAG << "] entry_name base_word element int [int ... int]" << endl;
  cout << INDENT << INDENT << "inserts entry_name into the persistent word data" << endl;
  cout << INDENT << INDENT << "(if entry_name already exists, it is not overwritten unless " << FORCE_FLAG << " is entered)" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << ERASE_COMMAND << " entry_name" << endl;
  cout << INDENT << INDENT << "removes entry_name from the persistent word data" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << LOOKUP_COMMAND << " entry_name" << endl;
  cout << INDENT << INDENT << "displays the word data of entry_name in the persistent word data" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << UPDATE_COMMAND << " [filename]" << endl;
  cout << INDENT << INDENT << "checks if filename matches the Haifu form" << endl;
  cout << INDENT << INDENT << "and prompts the user to fix any accumulated missing or incorrect word data" << endl;
  cout << INDENT << INDENT << "(if filename is not entered" << endl;
  cout << INDENT << INDENT << " the user is prompted to fix any previously accumulated data)" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << WARNINGS_COMMAND << endl;
  cout << INDENT << INDENT << "displays any accumulated missing or incorrect word data" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << LOAD_DATA_COMMAND << " filename" << endl;
  cout << INDENT << INDENT << "loads word data from filename into the persistent word data" << endl;
  cout << INDENT << INDENT << "(the existing data are not erased but are overwritten)" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << WRITE_DATA_COMMAND << " [" << FORCE_FLAG << "] filename" << endl;
  cout << INDENT << INDENT << "writes the persistent word data to filename" << endl;
  cout << INDENT << INDENT << "(if filename already exists, it is not overwritten unless " << FORCE_FLAG << " is entered)" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << CLEAR_DATA_COMMAND << endl;
  cout << INDENT << INDENT << "erases all the persistent word data" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << CLEAR_WARNINGS_COMMAND << endl;
  cout << INDENT << INDENT << "erases any accumulated missing or incorrect word data" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << RUN_COMMAND << " filename [arg ... arg]" << endl;
  cout << INDENT << INDENT << "checks if filename matches the Haifu form," << endl;
  cout << INDENT << INDENT << "checks whether it makes sense," << endl;
  cout << INDENT << INDENT << "then executes it as a Haifu program if it does," << endl;
  cout << INDENT << INDENT << "using the supplied args as its input stream" << endl;
  cout << INDENT << INDENT << "(args are interpreted as decimal values," << endl;
  cout << INDENT << INDENT << " or ASCII character values failing that)" << endl;
  cout << endl;

  cout << INDENT_HYPHEN << DUMP_COMMAND << endl;
  cout << INDENT << INDENT << "toggles whether program execution is dumped to a log file" << endl;
  cout << INDENT << INDENT << "(by default this is set to FALSE)" << endl;

  return true;
}

//-------------------------------------------------------------------------------
// checkFile()
//-------------------------------------------------------------------------------
bool checkFile(istream& input) {
  bool isGoodForm;
  bool noWarnings_flag = false;
  string filename;
  string flag = "";

  // checks if there is an argument
  if (endOfStream(input)) {
    cout << "Error: \"" << CHECK_COMMAND << "\" a flag or file name is required" << endl;
    return false;
  }

  input >> filename;
  flag = lowerCase(filename);

  // checks if there is flag and an argument
  if (flag == NO_WARNINGS_FLAG) {
    noWarnings_flag = true;

    if (endOfStream(input)) {
      cout << "Error: \"" << CHECK_COMMAND << "\" a file name is required" << endl;
      return false;
    }

    input >> filename;
  }

  cout << endl;
  isGoodForm = $SP::checkFileForm(filename);

  // displays warnings if the n0-warnings flag is not present
  if (!noWarnings_flag) {
    displayWarnings();
  }

  return isGoodForm;
}

//-------------------------------------------------------------------------------
// editEntry()
//-------------------------------------------------------------------------------
bool editEntry(istream& input) {
  string key;
  string field;
  string baseWord;
  string element;
  string intString;
  int numSyllables;
  vector<int> syllableCount;

  // checks if there is an argument
  if (endOfStream(input)) {
    cout << "Error: an entry name is required" << endl;
    return false;
  }

  input >> key;
  key = lowerCase(key);

  // prevents editing of reserved words
  if ($TG::isReservedWord(key)) {
    cout << "Error: \"" << key << "\" is a reserved word" << endl;
    return false;
  }

  // checks if there is another argument
  if (endOfStream(input)) {
    cout << "Error: the name of a field is required" << endl;
    return false;
  }

  input >> field;
  field = lowerCase(field);

  // change base word
  if (field == BASE_WORD_STRING) {
    // checks if there is another argument
    if (endOfStream(input)) {
      cout << "Error: a word is required" << endl;
      return false;
    }

    input >> baseWord;
    baseWord = lowerCase(baseWord);

    $WD::editBaseWord(key, baseWord);
    return true;
  }
  // change eleent
  else if (field == ELEMENT_STRING) {
    // checks if there is another argument
    if (endOfStream(input)) {
      cout << "Error: the name of an element is required" << endl;
      return false;
    }

    input >> element;
    element = lowerCase(element);

    // edit the field in the entry
    $WD::editElement(key, toElement(element));
    return true;
  }
  // change syllable count
  else if (field == SYLLABLE_COUNT_STRING) {
    // checks if there is another argument
    if (endOfStream(input)) {
      cout << "Error: a sequence of integers is required" << endl;
      return false;
    }

    // create a vector of int using the remaining values in the input stream
    input >> intString;
    numSyllables = atoi(intString.data());
    if (numSyllables > 0) {
      syllableCount.push_back(numSyllables);
    }

    // continue creating vector
    while (!endOfStream(input)) {
      input >> intString;
      numSyllables = atoi(intString.data());
      if (numSyllables > 0) {
        syllableCount.push_back(numSyllables);
      }
    }

    // removes redundant values in the vector
    compressVector(syllableCount);

    // edit the field in the entry
    $WD::editSyllableCount(key, syllableCount);
    return true;
  }
  // invalid field name
  else {
    cout << "Error: \"" << field << "\" is not a valid field name" << endl;
    cout << INDENT << "valid fields:"
      << " \"" << BASE_WORD_STRING << "\""
      << " \"" << ELEMENT_STRING << "\""
      << " \"" << SYLLABLE_COUNT_STRING << "\""
      << endl;
    return false;
  }
}
//-------------------------------------------------------------------------------
// insertEntry()
//-------------------------------------------------------------------------------
bool insertEntry(istream& input) {
  bool overwrite = false;
  string key;
  string baseWord;
  string element;
  string intString;
  int numSyllables;
  vector<int> syllableCount;

  // checks if there is an argument
  if (endOfStream(input)) {
    cout << "Error: a flag or entry name or is required" << endl;
    return false;
  }

  input >> key;
  key = lowerCase(key);

  // checks if there is an argument or flag
  if (key == FORCE_FLAG) {
    overwrite = true;

    // checks if there is another argument
    if (endOfStream(input)) {
      cout << "Error: an entry name or is required" << endl;
      return false;
    }

    input >> key;
    key = lowerCase(key);
  }

  // prevents editing of reserved words
  if ($TG::isReservedWord(key)) {
    cout << "Error: \"" << key << "\" is a reserved word" << endl;
    return false;
  }

  // checks if there is another argument
  if (endOfStream(input)) {
    cout << "Error: a word is required" << endl;
    return false;
  }

  input >> baseWord;
  baseWord = lowerCase(baseWord);

  // checks if there is another argument
  if (endOfStream(input)) {
    cout << "Error: the name of an element is required" << endl;
    return false;
  }

  input >> element;
  element = lowerCase(element);

  // checks if there is another argument
  if (endOfStream(input)) {
    cout << "Error: a sequence of integers is required" << endl;
    return false;
  }

  // creates a vector of int using th remaining values in the input stream
  input >> intString;
  numSyllables = atoi(intString.data());
  if (numSyllables > 0) {
    syllableCount.push_back(numSyllables);
  }

  // continue creating vector
  while (!endOfStream(input)) {
    input >> intString;
    numSyllables = atoi(intString.data());
    if (numSyllables > 0) {
      syllableCount.push_back(numSyllables);
    }
  }

  // removes redundant values in the vector
  compressVector(syllableCount);

  // inserts entry into data
  $WD::insert(key, baseWord, toElement(element), syllableCount, overwrite);
  return true;
}
//-------------------------------------------------------------------------------
// eraseEntry()
//-------------------------------------------------------------------------------
bool eraseEntry(istream& input) {
  string key;

  // checks if there is an argument
  if (endOfStream(input)) {
    cout << "Error: the name of an entry is required" << endl;
    return false;
  }

  input >> key;
  key = lowerCase(key);

  // removes entry from data
  $WD::erase(key);
  return true;
}
//-------------------------------------------------------------------------------
// lookupEntry()
//-------------------------------------------------------------------------------
bool lookupEntry(istream& input) {
  string key;
  const WordInfo* wordInfo;

  // checks if there is an argument
  if (endOfStream(input)) {
    cout << "Error: the name of an entry is required" << endl;
    return false;
  }

  input >> key;
  key = lowerCase(key);

  // looks up entry in data
  wordInfo = &$WD::lookup(key);

  // chexcks if entry exists
  if (*wordInfo == WORD_INFO_NULL) {
    cout << "Entry \"" << key << "\" does not exist.";
  }
  // displays entry
  else {
    cout << "Entry: \"" << key << "\"";
    if (wordInfo->baseWord == BASE_WORD_IDENTITY || wordInfo->baseWord == key) {
      cout << LOOKUP_DELIMITER_STRING << "Base Word: " << BASE_WORD_IDENTITY;
    }
    else if (wordInfo->baseWord == BASE_WORD_DEFAULT) {
      cout << LOOKUP_DELIMITER_STRING << "Base Word: " << BASE_WORD_DEFAULT_STRING;
    }
    else {
      cout << LOOKUP_DELIMITER_STRING << "Base Word: \"" << wordInfo->baseWord << "\"";
    }
    cout << LOOKUP_DELIMITER_STRING << "Element: " << toElementString(wordInfo->element);
    if (wordInfo->syllableCount.empty()) {
      cout << LOOKUP_DELIMITER_STRING << "Syllable Count: " << SYLLABLE_COUNT_DEFAULT_STRING;
    }
    else {
      cout << LOOKUP_DELIMITER_STRING << "Syllable Count: " << wordInfo->syllableCount;
    }
  }
  cout << endl;

  return true;
}

//-------------------------------------------------------------------------------
// updateData()
//-------------------------------------------------------------------------------
bool updateData(istream& input) {
  vector<string> warningVector;
  string input_string;
  string filename = "";
  bool skip = false;

  map<string, bool> baseWords;
  map<string, bool>::iterator iter;

  int numSyllables;
  vector<int> syllableCount;

  // checks if there is an argument
  if (!endOfStream(input)) {
    input >> filename;
    filename = lowerCase(filename);
  }

  // checks the file if one is specified
  if (!filename.empty()) {
    $SP::checkFileForm(filename);
  }

  // information for user
  cout << endl;
  cout << "Enter \"" << DEFAULT_FIELD_STRING << "\" for a Base Word to indicate the name of the entry." << endl;
  cout << "Enter \"" << DEFAULT_FIELD_STRING << "\" for an Element to indicate the element \"" << ELEM_NONE_STRING << "\"." << endl;
  cout << "Enter \"" << SKIP_FIELD_STRING << "\" for any field to skip that field." << endl;
  cout << "Enter \"" << EXIT_COMMAND_STRING << "\" for any field to quit updating." << endl;

  // prompts the user to update the accumulated warnings
  $WD::updateWarnings();

  // skips remaining entries if specified
  if (!skip) {
    // gets the syllable count warnings
    $WD::getWarnings_store(SYLLABLE_COUNT_DEFAULT_WARNING, warningVector);
    // if there are warnings
    if (!warningVector.empty()) {
      cout << endl;
      cout << "Updating entries with an empty syllable count:" << endl;

      for (int i = 0; i < (int)warningVector.size(); i++) {
        cout << "Set " << SYLLABLE_COUNT_STRING << " of \"" << warningVector[i] << "\": ";
        input >> input_string;

        // skips all remaining warnings of all types
        if (input_string == EXIT_COMMAND_STRING) {
          skip = true;
          break;
        }
        // skips the current field
        else if (input_string == SKIP_FIELD_STRING) {
          continue;
        }
        else {
          syllableCount.clear();

          // create a vector of int using the remaining values in the input stream
          numSyllables = atoi(input_string.data());
          if (numSyllables > 0) {
            syllableCount.push_back(numSyllables);
          }

          // continue creating vector
          while (!endOfStream(input)) {
            input >> input_string;
            numSyllables = atoi(input_string.data());
            if (numSyllables > 0) {
              syllableCount.push_back(numSyllables);
            }
          }

          // removes redundant values in the vector
          compressVector(syllableCount);

          // changes syllable count of entry
          $WD::editSyllableCount(warningVector[i], syllableCount);
        }
      }
      // recalculates warnings
      $WD::updateWarnings();
    }
  }

  // skips remaining entries if specified
  if (!skip) {
    // gets the default base wordwarnings
    $WD::getWarnings_store(BASE_WORD_DEFAULT_WARNING, warningVector);
    // if there are warnings
    if (!warningVector.empty()) {
      cout << endl;
      cout << "Updating entries with the default base word:" << endl;

      for (int i = 0; i < (int)warningVector.size(); i++) {
        prompt_repeat("Set " + string(BASE_WORD_STRING) + " of \"" + warningVector[i] + "\": "
          , input_string, cout, input);

        // skips all remaining warnings of all types
        if (input_string == EXIT_COMMAND_STRING) {
          skip = true;
          break;
        }
        // skips the current field
        else if (input_string == SKIP_FIELD_STRING) {
          continue;
        }
        else {
          // changes base word of entry
          $WD::editBaseWord(warningVector[i], input_string);
        }
      }
      // recalculates warnings
      $WD::updateWarnings();
    }
  }

  // skips remaining entries if specified
  if (!skip) {
    // gets the default element warnings
    $WD::getWarnings_store(ELEMENT_DEFAULT_WARNING, warningVector);
    // if there are warnings
    if (!warningVector.empty()) {
      cout << endl;
      cout << "Updating entries with the default element:" << endl;

      for (int i = 0; i < (int)warningVector.size(); i++) {
        prompt_repeat("Set " + string(ELEMENT_STRING) + " of \"" + warningVector[i] + "\": "
          , input_string, cout, input);

        // skips all remaining warnings of all types
        if (input_string == EXIT_COMMAND_STRING) {
          skip = true;
          break;
        }
        // skips the current field
        else if (input_string == SKIP_FIELD_STRING) {
          continue;
        }
        // sets the field to the acceptable default value
        else if (input_string == DEFAULT_FIELD_STRING) {
          $WD::editElement(warningVector[i], ELEM_NONE);
        }
          // changes element of entry
        else {
          $WD::editElement(warningVector[i], toElement(input_string));
        }
      }
      // recalculates warnings
      $WD::updateWarnings();
    }
  }

  // skips remaining entries if specified
  if (!skip) {
    // gets the element-of-base-word-is-default warnings
    $WD::getWarnings_store(BASE_WORD_ELEMENT_DEFAULT_WARNING, warningVector);
    // if there are warnings
    if (!warningVector.empty()) {
      cout << endl;
      cout << "Updating Base Word entries with the default element:" << endl;

      baseWords.clear();
      // creates map of words whose base word is the default element
      for (int i = 0; i < (int)warningVector.size(); i++) {
        baseWords[$WD::lookup(warningVector[i]).baseWord] = true;
      }

      for (iter = baseWords.begin(); iter != baseWords.end(); iter++) {
        prompt_repeat("Set " + string(ELEMENT_STRING) + " of \"" + iter->first + "\": "
          , input_string, cout, input);

        // skips all remaining warnings of all types
        if (input_string == EXIT_COMMAND_STRING) {
          skip = true;
          break;
        }
        // skips the current field
        else if (input_string == SKIP_FIELD_STRING) {
          continue;
        }
        // changes element of entry of base word to the acceptable default element
        else if (input_string == DEFAULT_FIELD_STRING) {
          $WD::editElement(iter->first, ELEM_NONE, true);
        }
        // changes element of entry of base word
        else {
          $WD::editElement(iter->first, toElement(input_string), true);
        }
      }
      // recalculates warnings
      $WD::updateWarnings();
    }
  }

  cout << endl;
  cout << "Warnings after update:" << endl;

  // displays any remaining warnings
  displayWarnings();

  return true;
}

//-------------------------------------------------------------------------------
// loadData()
//-------------------------------------------------------------------------------
bool loadData(istream& input) {
  string filename;

  // checks if there is an argument
  if (endOfStream(input)) {
    cout << "Error: \"" << LOAD_DATA_COMMAND << "\" a file name is required" << endl;
    return false;
  }

  input >> filename;

#ifndef USE_G_COMPILER
  $WD::loadData(filename);
#else
  $WD::loadData_TXT(filename);
#endif

  return true;
}
//-------------------------------------------------------------------------------
// writeData()
//-------------------------------------------------------------------------------
bool writeData(istream& input) {
  bool overwrite = false;
  bool fileExists;
  ifstream testFile;
  string filename;
  string filename_suffix;

  // checks if there is an argument
  if (endOfStream(input)) {
    cout << "Error: \"" << WRITE_DATA_COMMAND << "\" a flag or file name is required" << endl;
    return false;
  }

  input >> filename;
  // checks if there is an argument or flag
  if (lowerCase(filename) == FORCE_FLAG) {
    overwrite = true;

    // checks if there is another argument
    if (endOfStream(input)) {
      cout << "Error: \"" << WRITE_DATA_COMMAND << "\" a file name is required" << endl;
      return false;
    }

    input >> filename;
  }

  // gets the file type
  filename_suffix = lowerCase(suffix(filename, 4));

  // if it is not a recognized file type, append ".txt" to it
#ifndef USE_G_COMPILER
  if (filename_suffix != ".hwd" && filename_suffix != ".txt") {
    filename += ".txt";
  }
#else
  if (filename_suffix != ".txt") {
    filename += ".txt";
  }
#endif

  // checks if file exists without overwriting it
  testFile.open(filename);
  fileExists = testFile.is_open();
  testFile.close();

  // prevents overwriting if not specified
  if (fileExists && !overwrite) {
    cout << "Error: \"" << filename << "\" already exists (overrideable with " << FORCE_FLAG << ")" << endl;
    return false;
  }

  // write word data to file
  $WD::writeData(filename);

  return true;
  }

//-------------------------------------------------------------------------------
// clearData()
//-------------------------------------------------------------------------------
bool clearData() {
  // clears word data
  $WD::clearWordMap();
  return true;
}
//-------------------------------------------------------------------------------
// clearWarnings()
//-------------------------------------------------------------------------------
bool clearWarnings() {
  // clears accumulated warnings
  $WD::clearWarnings();
  return true;
}

//-------------------------------------------------------------------------------
// displayWarnings()
//-------------------------------------------------------------------------------
bool displayWarnings() {
  cout << endl;
  $WD::displaySelectiveWarnings(SYLLABLE_COUNT_DEFAULT_WARNING);

  cout << UPDATE_DISPLAY_DELIMITER_STRING << endl;
  $WD::displaySelectiveWarnings(BASE_WORD_DEFAULT_WARNING);

  cout << UPDATE_DISPLAY_DELIMITER_STRING << endl;
  $WD::displaySelectiveWarnings(BASE_WORD_CHAIN_WARNING);

  cout << UPDATE_DISPLAY_DELIMITER_STRING << endl;
  $WD::displaySelectiveWarnings(ELEMENT_DEFAULT_WARNING);

  cout << UPDATE_DISPLAY_DELIMITER_STRING << endl;
  $WD::displaySelectiveWarnings(BASE_WORD_ELEMENT_DEFAULT_WARNING);

  return true;
}

//-------------------------------------------------------------------------------
// runFile()
//-------------------------------------------------------------------------------
bool runFile(istream& input) {
  string filename;
  bool isFileGood;

  // checks if there is an argument
  if (!endOfStream(input)) {
    input >> filename;
  }
  else {
    cout << "Error: \"" << LOAD_DATA_COMMAND << "\" a file name is required" << endl;
    return false;
  }

  cout << endl;
  if (!filename.empty()) {
    // checks Haifu form of file
    isFileGood = $SP::checkFileForm(filename);
  }

  $WD::updateWarnings();

  if (isFileGood) {
    // checks if file makes sense
    $TG::generateFileTokens($SP::getFileData());
    $TG::displayErrors();
    $TG::displayWarnings();

    if ($TG::getTokens().empty()) {
      cout << endl;
      cout << "Program \"" << filename << "\" could not be executed." << endl;
      return false;
    }

    // loads program (if it does not make sense, program is empty)
    $PE::loadProgram($TG::getTokens());
    // executes Haifu program
    $PE::executeProgram(input);
  }

  return true;
}

//-------------------------------------------------------------------------------
// runFile()
//-------------------------------------------------------------------------------
void runFile(const string& filename, const int argc, const char** argv) {
  bool isFileGood;
  stringstream input;

  for (int i = 0; i < argc; i++) {
    input << argv[i] << endl;
  }

  cout << endl;
  if (!filename.empty()) {
    // checks Haifu form of file
    isFileGood = $SP::checkFileForm(filename);
  }

  $WD::updateWarnings();

  if (isFileGood) {
    // checks if file makes sense
    $TG::generateFileTokens($SP::getFileData());
    $TG::displayErrors();
    $TG::displayWarnings();

    if ($TG::getTokens().empty()) {
      cout << endl;
      cout << "Program \"" << filename << "\" could not be executed." << endl;
      return;
    }

    // loads program (if it does not make sense, program is empty)
    $PE::loadProgram($TG::getTokens());
    // executes Haifu program
    $PE::executeProgram(input);
  }
}

//-------------------------------------------------------------------------------
// toggleDump()
//-------------------------------------------------------------------------------
void toggleDump() {
  switch ($PE::toggleExecutionDump()) {
  case DUMP_NO_EXECUTIONS:
    cout << "Dump of program executions set to NONE" << endl;
    break;
  case DUMP_NON_VARIABLE_EXECUTIONS:
    cout << "Dump of program executions set to NON_VARIABLES" << endl;
    break;
  case DUMP_ALL_EXECUTIONS:
    cout << "Dump of program executions set to ALL" << endl;
    break;
  default:
    cout << "Dump of program executions set to UNDEFINED" << endl;
  }
}