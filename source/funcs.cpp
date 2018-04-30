#include "funcs.h"

#include <cmath>

using namespace std;

//-------------------------------------------------------------------------------
// lowerCase()
//-------------------------------------------------------------------------------
string lowerCase(const string& stringValue) {
  string result = stringValue;

  for (int i = 0; i < (int)result.size(); i++) {
    if (result[i] >= 'A' && result[i] <= 'Z') {
      result[i] += 32;
    }
  }

  return result;
}

//-------------------------------------------------------------------------------
// prefix()
//-------------------------------------------------------------------------------
string prefix(const string& stringValue, const int size) {
  return stringValue.substr(0, min(size, (int)stringValue.size()));
}
//-------------------------------------------------------------------------------
// suffix()
//-------------------------------------------------------------------------------
string suffix(const string& stringValue, const int size) {
  return stringValue.substr(
    max(0, (int)stringValue.size() - size)
    , min(size, (int)stringValue.size())
  );
}

//-------------------------------------------------------------------------------
// removeNonLetters()
//-------------------------------------------------------------------------------
string removeNonLetters(const string& stringValue) {
  string result;
  for (int i = 0; i < (int)stringValue.size(); i++) {
    if (isLetterChar(stringValue[i])) {
      result += stringValue[i];
    }
  }
  return result;
}
//-------------------------------------------------------------------------------
// removeNonLetters_hyphenated()
//-------------------------------------------------------------------------------
string removeNonLetters_hyphenated(const string& stringValue) {
  string result;
  for (int i = 0; i < (int)stringValue.size(); i++) {
    if (isLetterChar(stringValue[i]) || stringValue[i] == '-') {
      result += stringValue[i];
    }
  }
  return result;
}

//-------------------------------------------------------------------------------
// getNextHaifuWord()
//-------------------------------------------------------------------------------
string getNextHaifuWord(const string& line, const int look_start
  , int& word_start, int& word_size)
{
  int i;

  word_size = 0;

  for (i = look_start; i < (int)line.size(); i++) {
    if (isHaifuChar(line[i])) {
      break;
    }
  }

  word_start = i;

  for (; i < (int)line.size(); i++) {
    if (!isHaifuChar(line[i])) {
      break;
    }
    word_size++;
  }

  if (word_start + word_size <= (int)line.size()) {
    return line.substr(word_start, word_size);
  }
  else {
    return "";
  }
}

//-------------------------------------------------------------------------------
// getNextHaifuTokenWord()
//-------------------------------------------------------------------------------
string getNextHaifuTokenWord(const string& line, const int look_start
  , int& word_start, int& word_size)
{
  int i;

  if (look_start >= (int)line.size()) {
    word_size = 0;
    return "";
  }

  word_size = 0;

  for (i = look_start; i < (int)line.size(); i++) {
    if (!isWhitespace(line[i])) {
      break;
    }
  }

  word_start = i;

  if (!isHaifuChar(line[i])) {
    word_size = 1;
    return string("") + line[i];
  }

  for (; i < (int)line.size(); i++) {
    if (!isHaifuChar(line[i])) {
      break;
    }
    word_size++;
  }

  if (word_start + word_size <= (int)line.size()) {
    return line.substr(word_start, word_size);
  }
  else {
    return "";
  }
}

//-------------------------------------------------------------------------------
// prompt()
//-------------------------------------------------------------------------------
void prompt(const string& message, string& input, ostream& output_stream, istream& input_stream) {
  output_stream << message;
  input_stream >> input;
}
//-------------------------------------------------------------------------------
// prompt()
//-------------------------------------------------------------------------------
void prompt(const string& message, int& input, ostream& output_stream, istream& input_stream) {
  output_stream << message;
  input_stream >> input;
}
//-------------------------------------------------------------------------------
// prompt()
//-------------------------------------------------------------------------------
void prompt(const string& message, double& input, ostream& output_stream, istream& input_stream) {
  output_stream << message;
  input_stream >> input;
}

//-------------------------------------------------------------------------------
// prompt_repeat()
//-------------------------------------------------------------------------------
void prompt_repeat(const string& message, string& input, ostream& output_stream, istream& input_stream) {
  output_stream << message;
  input_stream >> input;
  while (input_stream.fail()) {
    input_stream.clear();
    input_stream.ignore(INT_MAX,'\n');
    output_stream << message;
    input_stream >> input;
  }
}
//-------------------------------------------------------------------------------
// prompt_repeat()
//-------------------------------------------------------------------------------
void prompt_repeat(const string& message, int& input, ostream& output_stream, istream& input_stream) {
  output_stream << message;
  input_stream >> input;
  while (input_stream.fail()) {
    input_stream.clear();
    input_stream.ignore(INT_MAX,'\n');
    output_stream << message;
    input_stream >> input;
  }
}
//-------------------------------------------------------------------------------
// prompt_repeat()
//-------------------------------------------------------------------------------
void prompt_repeat(const string& message, double& input, ostream& output_stream, istream& input_stream) {
  output_stream << message;
  input_stream >> input;
  while (input_stream.fail()) {
    input_stream.clear();
    input_stream.ignore(INT_MAX,'\n');
    output_stream << message;
    input_stream >> input;
  }
}

//-------------------------------------------------------------------------------
// isWhitespace()
//-------------------------------------------------------------------------------
bool isWhitespace(const char value) {
  return (
    value == ' '
    || value == '\t'
    || value == '\r'
    );
}
//-------------------------------------------------------------------------------
// isWordChar()
//-------------------------------------------------------------------------------
bool isWordChar(const char value) {
  return value > ' ' && value < 0x7f;
}
//-------------------------------------------------------------------------------
// isLetterChar()
//-------------------------------------------------------------------------------
bool isLetterChar(const char value) {
  return (value >= 'A' && value <= 'Z')
    || (value >= 'a' && value <= 'z')
    ;
}
//-------------------------------------------------------------------------------
// isHaifuChar()
//-------------------------------------------------------------------------------
bool isHaifuChar(const char value) {
  return (value >= 'A' && value <= 'Z')
    || (value >= 'a' && value <= 'z')
    || (value == '\'')
    ;
}

//-------------------------------------------------------------------------------
// isInVector()
//-------------------------------------------------------------------------------
bool isInVector(const std::vector<int>& values, const int value) {
  for (int i = 0; i < (int)values.size(); i++) {
    if (values[i] == value) {
      return true;
    }
  }
  return false;
}
//-------------------------------------------------------------------------------
// isBeforeInVector()
//-------------------------------------------------------------------------------
bool isBeforeInVector(const int index, const vector<int>& values) {
  int new_index;

  new_index = min(index, (int)values.size() - 1);
  new_index = max(new_index, 0);

  for (int i = 0; i < new_index; i++) {
    if (values[i] == values[index]) {
      return true;
    }
  }

  return false;
}

//-------------------------------------------------------------------------------
// areFilesIdentical()
//-------------------------------------------------------------------------------
bool areFilesIdentical(const string& filename0, const string& filename1) {
  FILE* source0;
  FILE* source1;

  size_t index;

  bool areIdentical;

#ifndef USE_G_COMPILER
  if (fopen_s(&source0, filename0.data(), "rb") != 0) {
    cout << "Failed to open \"" << filename0 << "\"" << endl;
    return 0;
  }
#else
  source0 = fopen(filename0.data(), "rb");
  if (source0 == NULL) {
    cout << "Failed to open \"" << filename0 << "\"" << endl;
    return 0;
  }
#endif

#ifndef USE_G_COMPILER
  if (fopen_s(&source1, filename1.data(), "rb") != 0) {
    cout << "Failed to open \"" << filename1 << "\"" << endl;
    fclose(source0);
    return 0;
  }
#else
  source1 = fopen(filename1.data(), "rb");
  if (source1 == NULL) {
    cout << "Failed to open \"" << filename1 << "\"" << endl;
    fclose(source0);
    return 0;
  }
#endif

  areIdentical = true;
  index = 1;
  if (fgetc(source0) != fgetc(source1)) {
    cout << __FUNCTION__ << "(): Difference at 0" << endl;
    areIdentical = false;
  }
  if (areIdentical) {
    while (!feof(source0) && !feof(source1)) {
      if (fgetc(source0) != fgetc(source1)) {
        areIdentical = false;
        cout << __FUNCTION__ << "(): Difference at " << index << endl;
        break;
      }
      index++;
    }
  }

  fclose(source1);
  fclose(source0);

  return areIdentical;
}

//-------------------------------------------------------------------------------
// endOfStream()
//-------------------------------------------------------------------------------
bool endOfStream(istream& input_stream) {
  char next;

  if (input_stream.eof()) {
    return true;
  }

  next = input_stream.peek();
  while (next == ' ') {
    input_stream.ignore(1);
    next = input_stream.peek();
  }

  return next == '\n';
}

//-------------------------------------------------------------------------------
// compressVector()
//-------------------------------------------------------------------------------
void compressVector(vector<int>& value) {
  map<int, bool> intMap;
  for (int i = 0; i < (int)value.size(); i++) {
    if (intMap[value[i]]) {
      value.erase(value.begin() + i);
      i--;
    }
    else {
      intMap[value[i]] = true;
    }
  }
}

//-------------------------------------------------------------------------------
// log10_int()
//-------------------------------------------------------------------------------
int log10_int(const int operand) {
  int new_operand = operand < 0 ? -operand : operand;
  int power = 0;

  while (new_operand >= 10) {
    new_operand /= 10;
    power += 1;
  }

  return power;
}

//-------------------------------------------------------------------------------
// round_away()
//-------------------------------------------------------------------------------
double round_away(const double value) {
  return value < 0.0 ? floor(value) : ceil(value);
}

//-------------------------------------------------------------------------------
// vector()
//-------------------------------------------------------------------------------
const vector<int>& add_insert(vector<int>& values0, const vector<int>& values1){
  map<int, bool> intMap;

  if (values0.size() <= 0) {
    values0 = values1;
    return values0;
  }
  else if (values1.size() <= 0) {
    return values0;
  }

  for (int j = 0; j < (int)values0.size(); j++) {
    for (int i = 0; i < (int)values1.size(); i++) {
      intMap[values0[j] + values1[i]] = true;
    }
  }

  values0.clear();
  for (map<int, bool>::iterator iter = intMap.begin(); iter != intMap.end(); iter++) {
    values0.emplace_back(iter->first);
  }

  return values0;
}

//-------------------------------------------------------------------------------
// operator<<()
//-------------------------------------------------------------------------------
ostream& operator<<(ostream& o, const vector<int>& values) {
  o << "{";
  if (!values.empty()) {
    o << values[0];
  }

  for (int i = 1; i < (int)values.size(); i++) {
    o << ", " << values[i];
  }
  o << "}";

  return o;
}
