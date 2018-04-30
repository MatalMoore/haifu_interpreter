#include "SyllableParser.h"

using namespace std;

string SyllableParser::s_filename = "";
vector<string> SyllableParser::s_fileData;
vector<ParseError> SyllableParser::s_parseErrors;
map<string, bool> SyllableParser::s_wordErrors;
ostream* SyllableParser::s_output = &cout;

//-------------------------------------------------------------------------------
// SyllableParser::loadFile()
//-------------------------------------------------------------------------------
bool SyllableParser::loadFile(const string& filename) {
  ifstream source;
  string fileLine;

  __this::displayOutput_cout("Loading file \"" + filename + "\"... ");

  // open the file
  source.open(filename);
  if (source.fail()) {
    __this::displayOutput_cout("Failed.\n");
    return false;
  }

  s_filename = filename;
  // clear the current file data
  __this::clearFileData();

  // load the first lineNum
  getline(source, fileLine);
  __this::loadLine(fileLine);
  // load each lineNum until the end of file is reached
  while (!source.eof()) {
    getline(source, fileLine);
    __this::loadLine(fileLine);
  }

  // close the file
  source.close();

  __this::displayOutput_cout("Done.\n");
  return true;
}

//-------------------------------------------------------------------------------
// SyllableParser::clear()
//-------------------------------------------------------------------------------
void SyllableParser::clear() {
  __this::clearFileData();
  __this::clearParseErrors();
}
//-------------------------------------------------------------------------------
// SyllableParser::clearFileData()
//-------------------------------------------------------------------------------
void SyllableParser::clearFileData() {
  vector<string> new_vector;

  // if the data is empty
  if (s_fileData.empty()) {
    return;
  }

  // clear the data
  s_fileData.clear();

  // reduce the allocated data
  new_vector.reserve(CLEAR_DATA_SIZE);
  s_fileData.swap(new_vector);
}
//-------------------------------------------------------------------------------
// SyllableParser::clearParseErrors()
//-------------------------------------------------------------------------------
void SyllableParser::clearParseErrors() {
  vector<ParseError> new_vector;

  if (s_parseErrors.empty()) {
    return;
  }

  // clear the data
  s_parseErrors.clear();

  // reduce the allocated data
  new_vector.reserve(CLEAR_ERRORS_SIZE);
  s_parseErrors.swap(new_vector);
}

//-------------------------------------------------------------------------------
// SyllableParser::setOutput()
//-------------------------------------------------------------------------------
void SyllableParser::setOutput(ostream& output) {
  s_output = &output;
}

//-------------------------------------------------------------------------------
// SyllableParser::string()
//-------------------------------------------------------------------------------
const vector<string>& SyllableParser::getFileData() {
  return s_fileData;
}

//-------------------------------------------------------------------------------
// SyllableParser::getOutput()
//-------------------------------------------------------------------------------
ostream& SyllableParser::getOutput() {
  return *s_output;
}
//-------------------------------------------------------------------------------
// SyllableParser::displayOutput_cout()
//-------------------------------------------------------------------------------
ostream& SyllableParser::displayOutput_cout(const string& value) {
  *s_output << value;
  if (s_output != &cout) {
    cout << value;
  }
  return *s_output;
}
//-------------------------------------------------------------------------------
// SyllableParser::displayErrors()
//-------------------------------------------------------------------------------
ostream& SyllableParser::displayErrors(ostream& o) {
  for (int i = 0; i < (int)s_parseErrors.size(); i++) {
    displayError(s_parseErrors[i], o) << endl;
  }

  return o;
}

//-------------------------------------------------------------------------------
// SyllableParser::checkFileForm()
//-------------------------------------------------------------------------------
bool SyllableParser::checkFileForm(const string& filename
  , ostream& output)
{
  bool didLoadFail = false;
  ostream* output_hold = s_output;

  // sets the output stream
  s_output = &output;

  cout << OUTPUT_LINE << endl;
  if (filename == "") {
    __this::displayOutput_cout("Checking form of file data from \"" + s_filename + "\"...\n");
    // uses the current data
  }
  else {
    __this::displayOutput_cout("Checking form of \"" + filename + "\"...\n");
    // checks if the file is valid
    didLoadFail = !__this::loadFile(filename);
  }

  __this::displayOutput_cout("\n");

  // if file cannot be loaded
  if (didLoadFail) {
    __this::displayOutput_cout("Form of \"" + filename + "\": BAD\n");

    cout << OUTPUT_LINE << endl;
    s_output = output_hold;
    __this::clearFileData();
    return false;
  }

  // clears the error data
  s_wordErrors.clear();
  __this::clearParseErrors();

  // checks the data
  __this::checkStanzas();

  // display found errors
  __this::displayOutput_cout("Errors found: " + to_string(s_parseErrors.size()) + "\n");
  displayErrors(*s_output);

  __this::displayOutput_cout("\n");

  // displays whether file is good
  __this::displayOutput_cout("Form of \"" + s_filename + "\": ");
  if (s_parseErrors.size() == 0) {
    __this::displayOutput_cout("GOOD\n");
    cout << OUTPUT_LINE << endl;
    s_output = output_hold;
    return true;
  }
  else {
    __this::displayOutput_cout("BAD\n");
    cout << OUTPUT_LINE << endl;
    s_output = output_hold;
    return false;
  }
}

//-------------------------------------------------------------------------------
// SyllableParser::makeError()
//-------------------------------------------------------------------------------
void SyllableParser::makeError(
  const int pos_y, const int pos_x, const string& data, const int errorCode)
{
  // allows only 1 error to be generated for each word lookup error
  if (errorCode == ERROR_WORD_LOOKUP) {
    if (s_wordErrors[lowerCase(data)]) {
      return;
    }
    else {
      s_wordErrors[lowerCase(data)] = true;
    }
  }

  s_parseErrors.push_back(make_tuple(make_pair(pos_y, pos_x), data, errorCode));
}

//-------------------------------------------------------------------------------
// SyllableParser::loadLine()
//-------------------------------------------------------------------------------
void SyllableParser::loadLine(const string& line) {
  for (int i = 0; i < (int)line.size(); i++) {
    // does not load lineNum with only whitespace characters
    if (!isWhitespace(line[i])) {
      s_fileData.push_back(line);
      return;
    }
  }

  s_fileData.push_back("");
}

//-------------------------------------------------------------------------------
// SyllableParser::checkStanzas()
//-------------------------------------------------------------------------------
bool SyllableParser::checkStanzas() {
  int line = 0;
  bool isGoodForm = true;

  while (line < (int)s_fileData.size()) {
    // keeps track of whether all stanzas are good
    isGoodForm &= __this::checkStanza(line);
    // quits checking after a certain number of errors
    if (s_parseErrors.size() >= ERROR_THRESHOLD) {
      *s_output << "Error threshold exceeded." << endl;
      break;
    }
  }

  return isGoodForm;
}
//-------------------------------------------------------------------------------
// SyllableParser::checkStanza()
//-------------------------------------------------------------------------------
bool SyllableParser::checkStanza(int& lineNum) {
  int stanzaIndex0;
  int stanzaIndex1;
  int stanzaIndex2;
  vector<int> lineSyllables;

  bool isGoodForm = true;

  bool isStanza1Bad = false;
  bool isStanza2Bad = false;

  // looks for the first lineNum with content
  for (stanzaIndex0 = lineNum; stanzaIndex0 < (int)s_fileData.size(); stanzaIndex0++) {
    if (s_fileData[stanzaIndex0].size() > 0) {
      break;
    }
  }

  // if there is no content, the stanza is good
  if (stanzaIndex0 >= (int)s_fileData.size()) {
    lineNum = s_fileData.size();
    return true;
  }

  // if this is not the first stanza to be checked and the first lineNum of the stanza is more than two lines after the starting lineNum
  if (lineNum > 0 && stanzaIndex0 > lineNum + 1) {
    // this indicates that there is more than 1 lineNum between this stanza and the previous one
    __this::makeError(stanzaIndex0, 0, "", ERROR_STANZA_SPACING);
  }

  // sets the second stanza index to the lineNum after the first, or the last lineNum
  stanzaIndex1 = min(stanzaIndex0 + 1, (int)s_fileData.size() - 1);
  // sets the third stanza index to the lineNum after the second, or the last lineNum
  stanzaIndex2 = min(stanzaIndex1 + 1, (int)s_fileData.size() - 1);

  // if the second lineNum is at or before the first lineNum
  if (stanzaIndex1 <= stanzaIndex0 || s_fileData[stanzaIndex1].size() <= 0) {
    // the stanza must not have enough lines
    __this::makeError(stanzaIndex0, 0, "", ERROR_STANZA_INCOMPLETE);
    lineNum = stanzaIndex0 + 1;
    isStanza1Bad = true;
    isGoodForm = false;
  }

  // if the third lineNum is at or before the second lineNum
  if (stanzaIndex2 <= stanzaIndex1 || s_fileData[stanzaIndex2].size() <= 0) {
    // the stanza must not have enough lines
    if (!isStanza1Bad) {
      // but an error is only generated if this is not already known
      __this::makeError(stanzaIndex1, 0, "", ERROR_STANZA_INCOMPLETE);
    }
    lineNum = stanzaIndex1 + 1;
    isStanza2Bad = true;
    isGoodForm = false;
  }

  // checks the first lineNum for errors
  lineSyllables = __this::checkLine(s_fileData[stanzaIndex0], stanzaIndex0);
  // if the first lineNum cannot have 5 syllables
  if (!isInVector(lineSyllables, 5) && !__this::isSyllableErrorInLine(stanzaIndex0)) {
    // there is an error
    __this::makeError(stanzaIndex0, 0, "5", ERROR_STANZA_SYLLABLES);
    isGoodForm = false;
  }

  // if there has not been a stanza syllables error
  if (!isStanza1Bad) {
    // checks the second lineNum for errors
    lineSyllables = __this::checkLine(s_fileData[stanzaIndex1], stanzaIndex1);
    // if the first lineNum cannot have 7 syllables
    if (!isInVector(lineSyllables, 7) && !__this::isSyllableErrorInLine(stanzaIndex1)) {
      // there is an error
      __this::makeError(stanzaIndex1, 0, "7", ERROR_STANZA_SYLLABLES);
      isGoodForm = false;
    }
  }

  // if there has not been a stanza syllables error
  if (!isStanza2Bad) {
    // checks the third lineNum for errors
    lineSyllables = __this::checkLine(s_fileData[stanzaIndex2], stanzaIndex2);
    // if the first lineNum cannot have 5 syllables
    if (!isInVector(lineSyllables, 5) && !__this::isSyllableErrorInLine(stanzaIndex2)) {
      // there is an error
      __this::makeError(stanzaIndex2, 0, "5", ERROR_STANZA_SYLLABLES);
      isGoodForm = false;
    }
  }

  // if the third lineNum is not the last lineNum in the data
  if (stanzaIndex2 + 1 < (int)s_fileData.size()) {
    // and there is content in the next lineNum
    if (s_fileData[stanzaIndex2 + 1].size() > 0) {
      // there is an error
      __this::makeError(stanzaIndex2 + 1, 0, "", ERROR_STANZA_SPACING);
      isGoodForm = false;
    }
  }

  // indicates the lineNum after this stanza
  lineNum = stanzaIndex2 + 1;
  return isGoodForm;
}
//-------------------------------------------------------------------------------
// SyllableParser::checkLine()
//-------------------------------------------------------------------------------
vector<int> SyllableParser::checkLine(const string& line, const int lineNum) {
  int lineIndex;
  int word_size;
  string word;
  vector<int> syllableCount_word;
  vector<int> syllableCount_line;

  lineIndex = 0;
  // gets the next word as determined by the Haifu format
  word = getNextHaifuWord(line, 0, lineIndex, word_size);
  while (word_size > 0) {
    // get the number of syllables of this word
    syllableCount_word = $WD::lookup(lowerCase(word)).syllableCount;
    // if there is no syllable information for this word
    if (syllableCount_word.size() <= 0) {
      // there is an error
      __this::makeError(lineNum, lineIndex, word, ERROR_WORD_LOOKUP);
    }
    // otherwise
    else {
      // adds the syllable counts of the word into those of the lineNum
      add_insert(syllableCount_line, syllableCount_word);
    }

    // moves the index to after the word
    lineIndex += word_size;
    // gets the next word as determined by the Haifu format
    word = getNextHaifuWord(line, lineIndex, lineIndex, word_size);
  }

  return syllableCount_line;
}

//-------------------------------------------------------------------------------
// SyllableParser::isSyllableErrorInLine()
//-------------------------------------------------------------------------------
bool SyllableParser::isSyllableErrorInLine(const int lineNum) {
  for (int i = s_parseErrors.size() - 1; i >= 0; i--) {
    // if the lineNum number of an error matches the lineNum parameter
    //   and it is a word lookup error
    if (get<E_POS>(s_parseErrors.back()).first == lineNum
      && get<E_ERROR>(s_parseErrors.back()) == ERROR_WORD_LOOKUP)
    {
      return true;
    }
  }

  return false;
}

//-------------------------------------------------------------------------------
// displayError()
//-------------------------------------------------------------------------------
ostream& displayError(const ParseError& error, ostream& o) {
  o << get<E_POS>(error) << ": ";
  switch (get<E_ERROR>(error)) {
  case ERROR_WORD_LOOKUP:
    o << "no syllable data for \"" << get<E_STRING>(error) << "\"";
    break;
  case ERROR_STANZA_INCOMPLETE:
    o << "stanza must have 3 lines";
    break;
  case ERROR_STANZA_SPACING:
    o << "there must be 1 empty line between stanzas";
    break;
  case ERROR_STANZA_SYLLABLES:
    o << "line does not have the requisite " << get<E_STRING>(error) << " syllables";
    break;
  default:
    o << "unspecified error with string \"" << get<E_STRING>(error) << "\"";
    break;
  }

  return o;
}

//-------------------------------------------------------------------------------
// operator<<()
//-------------------------------------------------------------------------------
ostream& operator<<(ostream & o, Position pos) {
  o << "{" << pos.first << ", " << pos.second << "}";
  return o;
}