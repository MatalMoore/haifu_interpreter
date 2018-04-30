#include "WordData.h"

using namespace std;

WordMap WordData::s_wordMap;
vector<WordMap> WordData::s_wordMaps;
vector<string> WordData::s_wordMaps_delimiters;

ostream* WordData::s_output = &cout;

WordWarningMap WordData::s_warnings_syllableCount_default;
WordWarningMap WordData::s_warnings_baseWord_default;
WordWarningMap WordData::s_warnings_baseWord_chain;
WordWarningMap WordData::s_warnings_element_default;
WordWarningMap WordData::s_warnings_baseWord_element_default;

// PUBLIC

//-------------------------------------------------------------------------------
// lookup()
//-------------------------------------------------------------------------------
const WordInfo& WordData::lookup(const std::string& key) {
  __this::checkWarnings(key);
  return __this::GET_INFO(key);
}

//-------------------------------------------------------------------------------
// editBaseWord()
//-------------------------------------------------------------------------------
void WordData::editBaseWord(const string& key, const string& baseWord, const bool skipWarnings) {
  if (baseWord == BASE_WORD_DEFAULT || baseWord == BASE_WORD_DEFAULT_STRING) {
    // warns if the base word is set to the default value
    *s_output << "Warning: "
      << "base word set to the default value" << endl;
  }

  if (baseWord == BASE_WORD_DEFAULT_STRING) {
    // sets the base word to the string that indicates the default value
    __this::GET_INFO(key).baseWord = BASE_WORD_DEFAULT;
  }
  else {
    // sets the base word
    __this::GET_INFO(key).baseWord = baseWord;
    __this::GET_INFO(baseWord);
  }

  if (!skipWarnings) {
    // checks if there are any warnings associated with the entry
    __this::checkWarnings(key);
  }

  // update any warnings that could have changed due to this entry
  __this::updateWarnings();
}
//-------------------------------------------------------------------------------
// editElement()
//-------------------------------------------------------------------------------
void WordData::editElement(const string& key, const char element, const bool skipWarnings) {
  if (toElementString(element) == ELEM_DEFAULT_STRING) {
    // warns if the element is set to the default value
    *s_output << "Warning: "
      << "element set to the default value" << endl;
  }

  // sets the element
  __this::GET_INFO(key).element = element;

  if (!skipWarnings) {
    // checks if there are any warnings associated with the entry
    __this::checkWarnings(key);
  }

  // update any warnings that could have changed due to this entry
  __this::updateWarnings();
}
//-------------------------------------------------------------------------------
// editSyllableCount()
//-------------------------------------------------------------------------------
void WordData::editSyllableCount(const string& key, const vector<int>& syllableCount, const bool skipWarnings) {
  if (syllableCount.empty()) {
    // warns if the syllable count is empty
    *s_output << "Warning: "
      << "syllable count is now empty" << endl;
  }

  // sets the sylable count
  __this::GET_INFO(key).syllableCount = syllableCount;

  if (!skipWarnings) {
    // checks if there are any warnings associated with the entry
    __this::checkWarnings(key);
  }

  // update any warnings that could have changed due to this entry
  __this::updateWarnings();
}

//-------------------------------------------------------------------------------
// insert()
//-------------------------------------------------------------------------------
void WordData::insert(const string& key, const WordInfo& wordInfo, const bool overwrite) {
  const WordInfo* wordInfo_lookup;

  // checks if the string is empty
  if (key.empty()) {
    *s_output << "Error: "
      << "an empty key is invalid" << endl;
    return;
  }

  // gets the entry from the data
  wordInfo_lookup = &__this::GET_INFO(key);

  // if the entry exists and overwriting is not specified
  if (*wordInfo_lookup != WORD_INFO_NULL && !overwrite) {
    *s_output << "Error: cannot insert \"" << key << "\" when its entry exists (overrideable)" << endl;
    return;
  }

  // sets the values of the entry
  __this::editBaseWord(key, wordInfo.baseWord);
  __this::editElement(key, wordInfo.element);
  __this::editSyllableCount(key, wordInfo.syllableCount);
}
//-------------------------------------------------------------------------------
// insert()
//-------------------------------------------------------------------------------
void WordData::insert(const string& key
  , const string& baseWord, const char element, const vector<int>& syllableCount
  , const bool overwrite)
{
  __this::insert(key, WordInfo(baseWord, element, syllableCount), overwrite);
}

//-------------------------------------------------------------------------------
// erase()
//-------------------------------------------------------------------------------
void WordData::erase(const string& key) {
  // gets the entry from the data
  WordMap* wordMap = &__this::GET_MAP(key);

  // if entry does not exist
  if ((*wordMap)[key] == WORD_INFO_NULL) {
    return;
  }

  // set entry to null value
  (*wordMap)[key] = WORD_INFO_NULL;
  // update any warnings that could have changed due to this entry
  __this::updateWarnings();
}

//-------------------------------------------------------------------------------
// getWarnings_store()
//-------------------------------------------------------------------------------
void WordData::getWarnings_store(const int warningCode, std::vector<std::string>& warningVector) {
  WordWarningMap::iterator iter;

  warningVector.clear();

  // check which type of warnings to store
  switch (warningCode) {
  case SYLLABLE_COUNT_DEFAULT_WARNING:
    // store the syllable count warnings
    warningVector.reserve(s_warnings_syllableCount_default.size());
    for (iter = s_warnings_syllableCount_default.begin(); iter != s_warnings_syllableCount_default.end(); iter++) {
      warningVector.push_back(iter->first);
    }
    break;

  case BASE_WORD_DEFAULT_WARNING:
    // store the default base word warnings
    warningVector.reserve(s_warnings_baseWord_default.size());
    for (iter = s_warnings_baseWord_default.begin(); iter != s_warnings_baseWord_default.end(); iter++) {
      warningVector.push_back(iter->first);
    }
    break;

  case BASE_WORD_CHAIN_WARNING:
    // store the base-word-has-base-word warnings
    warningVector.reserve(s_warnings_baseWord_chain.size());
    for (iter = s_warnings_baseWord_chain.begin(); iter != s_warnings_baseWord_chain.end(); iter++) {
      warningVector.push_back(iter->first);
    }
    break;

  case ELEMENT_DEFAULT_WARNING:
    // store the default element warnings
    warningVector.reserve(s_warnings_element_default.size());
    for (iter = s_warnings_element_default.begin(); iter != s_warnings_element_default.end(); iter++) {
      warningVector.push_back(iter->first);
    }
    break;

  case BASE_WORD_ELEMENT_DEFAULT_WARNING:
    // store the base-word-has-default-element warnings
    warningVector.reserve(s_warnings_baseWord_element_default.size());
    for (iter = s_warnings_baseWord_element_default.begin(); iter != s_warnings_baseWord_element_default.end(); iter++) {
      warningVector.push_back(iter->first);
    }
    break;

  default:
    break;
  }
}

//-------------------------------------------------------------------------------
// displayWarnings()
//-------------------------------------------------------------------------------
void WordData::displayWarnings(const int warningCode) {
  WordWarningMap::iterator iter;
  const WordInfo* baseWordInfo;

  // check which warnings to display
  switch (warningCode) {
  case SYLLABLE_COUNT_DEFAULT_WARNING:
    // display the syllable count warnings
    *s_output << "Words with no syllable count:" << endl;
    for (iter = s_warnings_syllableCount_default.begin(); iter != s_warnings_syllableCount_default.end(); iter++) {
      *s_output << iter->first << endl;
    }
    break;

  case BASE_WORD_DEFAULT_WARNING:
    // display the default base word warnings
    *s_output << "Words with the default base word:" << endl;
    for (iter = s_warnings_baseWord_default.begin(); iter != s_warnings_baseWord_default.end(); iter++) {
      *s_output << iter->first << endl;
    }
    break;

  case BASE_WORD_CHAIN_WARNING:
    // display the base-word-has-base-word warnings
    *s_output << "Words whose base word has a base word:" << endl;
    for (iter = s_warnings_baseWord_chain.begin(); iter != s_warnings_baseWord_chain.end(); iter++) {
      baseWordInfo = &__this::lookup(iter->first);
      *s_output << iter->first
        << " : " << iter->second->baseWord
        << " : " << __this::lookup(iter->second->baseWord).baseWord
        << endl;
    }
    break;

  case ELEMENT_DEFAULT_WARNING:
    // display the default element warnings
    *s_output << "Words with the default element:" << endl;
    for (iter = s_warnings_element_default.begin(); iter != s_warnings_element_default.end(); iter++) {
      *s_output << iter->first << endl;
    }
    break;

  case BASE_WORD_ELEMENT_DEFAULT_WARNING:
    // display the base-word-has-default-element warnings
    *s_output << "Base Words with the default element:" << endl;
    for (iter = s_warnings_baseWord_element_default.begin(); iter != s_warnings_baseWord_element_default.end(); iter++) {
      *s_output << iter->first << " : " << iter->second->baseWord << endl;
    }
    break;

  default:
    break;
  }
}

//-------------------------------------------------------------------------------
// displaySelectiveWarnings()
//-------------------------------------------------------------------------------
void WordData::displaySelectiveWarnings(const int warningCode) {
  // check which warnings to display
  switch (warningCode) {
  case SYLLABLE_COUNT_DEFAULT_WARNING:
    // display the syllable count warnings
    if (!s_warnings_syllableCount_default.empty()) {
      __this::displayWarnings(SYLLABLE_COUNT_DEFAULT_WARNING);
    }
    break;

  case BASE_WORD_DEFAULT_WARNING:
    // display the default base word warnings
    if (!s_warnings_baseWord_default.empty()) {
      __this::displayWarnings(BASE_WORD_DEFAULT_WARNING);
    }
    break;

  case BASE_WORD_CHAIN_WARNING:
    // display the base-word-has-base-word warnings
    if (!s_warnings_baseWord_chain.empty()) {
      __this::displayWarnings(BASE_WORD_CHAIN_WARNING);
    }
    break;

  case ELEMENT_DEFAULT_WARNING:
    // display the default element warnings
    if (!s_warnings_element_default.empty()) {
      __this::displayWarnings(ELEMENT_DEFAULT_WARNING);
    }
    break;

  case BASE_WORD_ELEMENT_DEFAULT_WARNING:
    // display the base-word-has-default-element warnings
    if (!s_warnings_baseWord_element_default.empty()) {
      __this::displayWarnings(BASE_WORD_ELEMENT_DEFAULT_WARNING);
    }
    break;

  default:
    break;
  }
}

//-------------------------------------------------------------------------------
// checkWarnings()
//-------------------------------------------------------------------------------
void WordData::checkWarnings(const std::string& key) {
  // gets the entry from the data
  const WordInfo* wordInfo = &__this::GET_INFO(key);
  const WordInfo* baseWordInfo;
  string baseWord = wordInfo->baseWord;

  // if the syllable count is empty
  if (wordInfo->syllableCount.empty()) {
    s_warnings_syllableCount_default[key] = wordInfo;
  }

  // if the key is its base word
  if (baseWord == BASE_WORD_IDENTITY || baseWord == key) {
    // if the element is the default element
    if (wordInfo->element == ELEM_DEFAULT) {
      s_warnings_element_default[key] = wordInfo;
    }
  }
  // if the base word is the default base word
  else if (baseWord == BASE_WORD_DEFAULT ) {
    s_warnings_baseWord_default[key] = wordInfo;
  }
  // if the key is not its base word and the base word is not the deffault base word
  else {
    // gets the entry of its base word
    baseWordInfo = &__this::GET_INFO(baseWord);

    // if its base word is not default or its own base word
    if (
      baseWordInfo->baseWord != BASE_WORD_DEFAULT
      && baseWordInfo->baseWord != BASE_WORD_IDENTITY
      && baseWordInfo->baseWord != baseWord
      )
    {
      s_warnings_baseWord_chain[key] = wordInfo;
    }

    // if its element is  the default element
    if (baseWordInfo->element == ELEM_DEFAULT) {
      s_warnings_baseWord_element_default[key] = wordInfo;
    }
  }
}

//-------------------------------------------------------------------------------
// updateWarnings()
//-------------------------------------------------------------------------------
void WordData::updateWarnings() {
  WordWarningMap::iterator iter;
  vector<string> keysToUpdate;

  // accumulates the keys from all the warnings and clears the warnings
  //   (allows keys associated with warnings to change category without error)

  for (iter = s_warnings_syllableCount_default.begin(); iter != s_warnings_syllableCount_default.end(); iter++) {
    keysToUpdate.push_back(iter->first);
  }
  s_warnings_syllableCount_default.clear();

  for (iter = s_warnings_baseWord_default.begin(); iter != s_warnings_baseWord_default.end(); iter++) {
    keysToUpdate.push_back(iter->first);
  }
  s_warnings_baseWord_default.clear();

  for (iter = s_warnings_baseWord_chain.begin(); iter != s_warnings_baseWord_chain.end(); iter++) {
    keysToUpdate.push_back(iter->first);
  }
  s_warnings_baseWord_chain.clear();

  for (iter = s_warnings_element_default.begin(); iter != s_warnings_element_default.end(); iter++) {
    keysToUpdate.push_back(iter->first);
  }
  s_warnings_element_default.clear();

  for (iter = s_warnings_baseWord_element_default.begin(); iter != s_warnings_baseWord_element_default.end(); iter++) {
    keysToUpdate.push_back(iter->first);
  }
  s_warnings_baseWord_element_default.clear();

  // check the warnings for each accumulated key
  //   (redundant keys can be checked)
  for (int i = 0; i < (int)keysToUpdate.size(); i++) {
    __this::checkWarnings(keysToUpdate[i]);
  }
}

//-------------------------------------------------------------------------------
// loadData()
//-------------------------------------------------------------------------------
void WordData::loadData(const std::string& filename) {
  string fileType = lowerCase(suffix(filename, 4));

  // checks file type

  if (fileType == ".txt") {
    __this::loadData_TXT(filename);
  }
  else if (fileType == ".hwd") {
    __this::loadData_HWD(filename);
  }
  else {
    *s_output << "Warning: "
      << "assuming file extension of \"" << filename << "\" to be \".txt\"" << endl;
    __this::loadData_TXT(filename + ".txt");
  }

  __this::updateWarnings();
}
//-------------------------------------------------------------------------------
// loadData_TXT()
//-------------------------------------------------------------------------------
void WordData::loadData_TXT(const string& fileTitle) {
  string filename;
  ifstream source;
  string line;
  vector<string> lines;

  // ensures file is a TXT file
  //   (disallows files with no indicated file type)
  if (lowerCase(suffix(fileTitle, 4)) == ".txt") {
    filename = fileTitle;
  }
  else {
    filename = fileTitle + ".txt";
  }

  *s_output << "Loading word data from \"" << filename << "\"... ";

  // open file
  source.open(filename);
  if (source.fail()) {
    *s_output << " Failed." << endl;
    return;
  }

  // get the data from the file
  getline(source, line);
  lines.push_back(line);
  while (!source.eof()) {
    getline(source, line);
    lines.push_back(line);
  }

  // loads the file data based on the number of threads
  //   (determined at compile time)
  __this::LOAD_STRING_DATA(lines, NUM_THREADS);

  *s_output << "Done." << endl;

  *s_output << "Removing redundant syllable data... ";
  __this::compressSyllables();
  *s_output << "Done." << endl;
}
//-------------------------------------------------------------------------------
// loadData_HWD()
//-------------------------------------------------------------------------------
void WordData::loadData_HWD(const string& fileTitle) {
  string filename;
  FILE* source;

  HWD_DATA_TYPE* buffer;
  size_t bufferSize;

  // ensures file is an HWD file
  //   (disallows files with no indicated file type)
  if (lowerCase(suffix(fileTitle, 4)) == ".hwd") {
    filename = fileTitle;
  }
  else {
    filename = fileTitle + ".hwd";
  }

  *s_output << "Loading word data from \"" << filename << "\"... ";

  // opens the file based on whether this environment variable is defined
#ifndef USE_G_COMPILER
  if (fopen_s(&source, filename.data(), "rb") != 0) {
    *s_output << " Failed." << endl;
    return;
  }
#else
  source = fopen(filename.data(), "rb");
  if (source == nullptr) {
    *s_output << " Failed." << endl;
    return;
  }
#endif

  // load the indicated size of the data
#ifndef USE_G_COMPILER
  fread_s(&bufferSize, sizeof(size_t), sizeof(size_t), 1, source);
#else
  fread(&bufferSize, sizeof(size_t), 1, source);
#endif

  // allocate buffer based on this size
  buffer = new HWD_DATA_TYPE[bufferSize];

  // gets all the remaining data from the file
  //fread_s(buffer, numLines, HWD_DATA_SIZE, numLines, source);
  fread(buffer, HWD_DATA_SIZE, bufferSize, source);
  fclose(source);

  // loads the data from the file
  __this::LOAD_HWD_DATA(buffer, bufferSize, NUM_THREADS);

  // deallocate the buffer
  delete[] buffer;
  *s_output << "Done." << endl;
}

//-------------------------------------------------------------------------------
// writeData()
//-------------------------------------------------------------------------------
void WordData::writeData(const std::string& filename) {
  string fileType = lowerCase(suffix(filename, 4));
  // checks the type of the intended file
  if (fileType == ".txt") {
    // writes in TXT format
    __this::writeData_TXT(filename);
  }
  else if (fileType == ".hwd") {
    // writes in HWD format
    __this::writeData_HWD(filename);
  }
  else {
    // assumes TXT format
    cout << __FUNCTION__ << "(): "
      << "assuming file extension of \"" << filename << "\" to be \".txt\"" << endl;
    __this::writeData_TXT(filename + ".txt");
  }
}
//-------------------------------------------------------------------------------
// writeData_TXT()
//-------------------------------------------------------------------------------
void WordData::writeData_TXT(const string& fileTitle) {
  string filename;
  ofstream target;

  // assumes TXT format
  if (lowerCase(suffix(fileTitle, 4)) == ".txt") {
    filename = fileTitle;
  }
  else {
    filename = fileTitle + ".txt";
  }

  *s_output << "Writing word data to \"" << filename << "\"... ";

  // open file
  target.open(filename);
  if (target.fail()) {
    *s_output << " Failed." << endl;
    return;
  }

  // stream the data to the file
  __this::STREAM_OUT(target);

  target.close();

  *s_output << "Done." << endl;
}
//-------------------------------------------------------------------------------
// writeData_HWD()
//-------------------------------------------------------------------------------
void WordData::writeData_HWD(const string& fileTitle) {
  string filename;
  FILE* target;
  size_t mapSize;

  // assumes HWD format
  if (lowerCase(suffix(fileTitle, 4)) == ".hwd") {
    filename = fileTitle;
  }
  else {
    filename = fileTitle + ".hwd";
  }

  *s_output << "Writing word data to \"" << filename << "\"... ";

  // opens the file
#ifndef USE_G_COMPILER
  if (fopen_s(&target, filename.data(), "w") != 0) {
    *s_output << " Failed." << endl;
    return;
  }
#else
  fopen(filename.data(), "w");
  if (target == nullptr) {
    *s_output << " Failed." << endl;
    return;
  }
#endif

  // gets the size of the data
  mapSize = __this::GET_MAP_SIZE();

  // writes this value to the file
  fwrite(&mapSize, sizeof(size_t), 1, target);

  // writes all the data to the file
  __this::WRITE_HWD(target);

  fclose(target);

  *s_output << "Done." << endl;
}

//-------------------------------------------------------------------------------
// clearWordMap()
//-------------------------------------------------------------------------------
void WordData::clearWordMap() {
  // clear single word map
  s_wordMap.clear();
  // clear vector of word maps
  s_wordMaps.clear();
  // clear keys for faster access to vector of word maps
  s_wordMaps_delimiters.clear();
}
//-------------------------------------------------------------------------------
// clearWarnings()
//-------------------------------------------------------------------------------
void WordData::clearWarnings() {
  s_warnings_syllableCount_default.clear();
  s_warnings_baseWord_default.clear();
  s_warnings_baseWord_chain.clear();
  s_warnings_element_default.clear();
  s_warnings_baseWord_element_default.clear();
}

//-------------------------------------------------------------------------------
// stream()
//-------------------------------------------------------------------------------
void WordData::stream(ostream& target) {
  __this::STREAM_OUT(target);
}

//-------------------------------------------------------------------------------
// setOutput()
//-------------------------------------------------------------------------------
void WordData::setOutput(ostream& output) {
  s_output = &output;
}

//-------------------------------------------------------------------------------
// getOutput()
//-------------------------------------------------------------------------------
ostream& WordData::getOutput() {
  return *s_output;
}

// PRIVATE

//-------------------------------------------------------------------------------
// getMap()
//-------------------------------------------------------------------------------
WordMap& WordData::getMap(const std::string& key) {
  return s_wordMap;
}
//-------------------------------------------------------------------------------
// getMap_multiple()
//-------------------------------------------------------------------------------
WordMap& WordData::getMap_multiple(const std::string& key) {
  // if data have not been initialized or loaded
  if (s_wordMaps.empty()) {
    // initialize data
    s_wordMaps.resize(1);
    return s_wordMaps[0];
  }

  // search the delimiters like a dictionary to determine the map to return
  for (int i = 1; i < (int)s_wordMaps.size(); i++) {
    if (key < s_wordMaps_delimiters[i]) {
      return s_wordMaps[i - 1];
    }
  }

  // by default, return the last one
  return s_wordMaps[s_wordMaps.size() - 1];
}

//-------------------------------------------------------------------------------
// getMapSize()
//-------------------------------------------------------------------------------
size_t WordData::getMapSize() {
  size_t result = 0;
  WordMap::iterator iter;

  // calculate the size of the data
  for (iter = s_wordMap.begin(); iter != s_wordMap.end(); iter++) {
    // key size plus end-of-string byte
    result += iter->first.size() + 1;
    // base key size plus end-of-string byte
    result += iter->second.baseWord.size() + 1;
    // element size
    result += 1;
    // syllable count size plus end-of-vector byte
    result += iter->second.syllableCount.size() + 1;
    // entry end
    result += 1;
  }

  return result;
}
//-------------------------------------------------------------------------------
// getMapSize_multiple()
//-------------------------------------------------------------------------------
size_t WordData::getMapSize_multiple() {
  size_t result = 0;
  WordMap::iterator iter;
  WordMap::iterator iter_end;

  // calculate the size of the data in all maps
  for (int j = 0; j < (int)s_wordMaps.size(); j++) {
    iter_end = s_wordMaps[j].end();
    for (iter = s_wordMaps[j].begin(); iter != iter_end; iter++) {
      // key size plus end-of-string byte
      result += iter->first.size() + 1;
      // base key size plus end-of-string byte
      result += iter->second.baseWord.size() + 1;
      // element size
      result += 1;
      // syllable count size plus end-of-vector byte
      result += iter->second.syllableCount.size() + 1;
      // entry end
      result += 1;
    }
  }

  return result;
}


//-------------------------------------------------------------------------------
// loadStringData()
//-------------------------------------------------------------------------------
void WordData::loadStringData(const vector<string>& lines) {
  stringstream lineStream;
  string intString;
  string elementString;

  string key;
  string baseWord;
  vector<int> syllableCount;

  for (int j = 0; j < (int)lines.size(); j++) {
    lineStream.ignore(SIZE_MAX);
    lineStream.clear();
    syllableCount.clear();

    // load line
    lineStream << lines[j];
    if (lineStream.fail()) {
      continue;
    }

    // load key
    lineStream >> key;
    if (lineStream.fail()) {
      continue;
    }

    // load base key
    lineStream >> baseWord;
    if (lineStream.fail()) {
      continue;
    }

    // load string representing element
    lineStream >> elementString;
    if (lineStream.fail()) {
      continue;
    }

    // load strings representing ints
    lineStream >> intString;
    while (!lineStream.fail()) {
      if (intString == SYLLABLE_COUNT_DEFAULT_STRING) {
        lineStream >> intString;
        continue;
      }
      syllableCount.emplace_back(atoi(intString.data()));
      lineStream >> intString;
    }

    if (baseWord == BASE_WORD_DEFAULT_STRING) {
      baseWord = BASE_WORD_DEFAULT;
    }
    s_wordMap[key] = WordInfo(baseWord, toElement(lowerCase(elementString)), syllableCount);
  }
}
//-------------------------------------------------------------------------------
// loadStringData_multiple()
//-------------------------------------------------------------------------------
void WordData::loadStringData_multiple(const vector<string>& lines
  , const int numThreads)
{
  vector<string> treatedLines;

  vector<thread> threads;
  size_t threadNumLines;

  size_t numLines;
  size_t numLines_remaining;

  // ensure lines are sorted
  __this::treatLines_store(lines, treatedLines);

  numLines = treatedLines.size();

  // allocate vector of thread
  threads.reserve(numThreads);

  // ininitialize maps
  s_wordMaps.resize(numThreads);
  s_wordMaps_delimiters.resize(numThreads);

  // divide up the lines among the threads using integer division
  //   (if number of threads is 16, first thread gets 1/16 of total lines,
  //    second gets 1/15 of what is left, and so on)

  // first thread
  threadNumLines = numLines / numThreads;
  threads.push_back(
    thread(__this::loadStringData_thread, treatedLines, 0, threadNumLines, 0)
  );

  // remaining threads
  numLines_remaining = numLines - threadNumLines;
  for (int i = 1; i < numThreads; i++) {
    threadNumLines = numLines_remaining / (numThreads - i);
    threads.push_back(
      // start each thread
      thread(__this::loadStringData_thread, treatedLines, numLines - numLines_remaining, threadNumLines, i)
    );
    numLines_remaining -= threadNumLines;
  }

  // end each thread
  for (int i = 0; i < numThreads; i++) {
    threads[i].join();
  }
}
//-------------------------------------------------------------------------------
// loadStringData_thread()
//-------------------------------------------------------------------------------
void WordData::loadStringData_thread(const std::vector<std::string>& lines,
  const int startIndex, const int numLines, const int threadID)
{
  stringstream lineStream;
  string intString;
  string elementString;

  string key;
  string baseWord;
  vector<int> syllableCount;

  if (lines.empty()) {
    return;
  }

  lineStream.ignore(SIZE_MAX);
  lineStream.clear();
  syllableCount.clear();

  // load line
  lineStream << lines[startIndex];
  if (lineStream.fail()) {
    return;
  }

  // load key
  lineStream >> key;
  if (lineStream.fail()) {
    return;
  }

  s_wordMaps_delimiters[threadID] = key;

  // load base key
  lineStream >> baseWord;
  if (lineStream.fail()) {
    return;
  }

  // load string representing element
  lineStream >> elementString;
  if (lineStream.fail()) {
    return;
  }

  // load strings representing ints
  lineStream >> intString;
  while (!lineStream.fail()) {
    syllableCount.emplace_back(atoi(intString.data()));
    lineStream >> intString;
  }

  // if all fields were loaded
  if (syllableCount.size() > 0) {
    if (baseWord == BASE_WORD_DEFAULT_STRING) {
      baseWord = BASE_WORD_DEFAULT;
    }
    s_wordMaps[threadID][key] = WordInfo(baseWord, toElement(lowerCase(elementString)), syllableCount);
  }

  for (size_t i = 1; i < (size_t)numLines; i++) {
    lineStream.ignore(SIZE_MAX);
    lineStream.clear();
    syllableCount.clear();

    // load line
    lineStream << lines[startIndex + i];
    if (lineStream.fail()) {
      return;
    }

    // load key
    lineStream >> key;
    if (lineStream.fail()) {
      return;
    }

    // load base key
    lineStream >> baseWord;
    if (lineStream.fail()) {
      return;
    }

    // load string representing element
    lineStream >> elementString;
    if (lineStream.fail()) {
      return;
    }

    // load strings representing ints
    lineStream >> intString;
    while (!lineStream.fail()) {
      syllableCount.emplace_back(atoi(intString.data()));
      lineStream >> intString;
    }

    // if all fields were loaded
    if (syllableCount.size() > 0) {
      if (baseWord == BASE_WORD_DEFAULT_STRING) {
        baseWord = BASE_WORD_DEFAULT;
      }
      s_wordMaps[threadID][key] = WordInfo(baseWord, toElement(lowerCase(elementString)), syllableCount);
    }
  }
}

//-------------------------------------------------------------------------------
// treatLines_store()
//-------------------------------------------------------------------------------
void WordData::treatLines_store(const vector<string>& lines, vector<string>& treatedLines) {
  // stores each line and its original index
  vector<pair<string, size_t>> linesAndIndices;
  stringstream lineStream;
  string lineString;

  size_t linesAndIndices_index;

  treatedLines.clear();

  // simply return if there ae no lines
  if (lines.empty()) {
    return;
  }

  // set initial size of this vector to the number of lines
  linesAndIndices.resize(lines.size());
  linesAndIndices_index = 0;
  for (size_t i = 0; i < lines.size(); i++) {
    // resets the stream
    lineStream.ignore(SIZE_MAX);
    lineStream.clear();

    // trims surrounding whitespace
    lineStream << lines[i];
    lineStream >> lineString;
    // if line has no content after trimming
    if (lineStream.fail() || lineString.empty()) {
      continue;
    }

    // append the line and its original index
    linesAndIndices[linesAndIndices_index] = make_pair(lineString, i);
    // increment the number of lines
    linesAndIndices_index++;
  }

  // set the size value of the vector
  linesAndIndices.resize(linesAndIndices_index);
  // sort the vector
  sort(linesAndIndices.begin(), linesAndIndices.end(), sortCompare);

  // resize the storage vector
  treatedLines.resize(linesAndIndices.size());
  for (size_t i = 0; i < linesAndIndices.size(); i++) {
    // fill the storage vector with the sorted lines
    treatedLines[i] = lines[linesAndIndices[i].second];
  }
}

//-------------------------------------------------------------------------------
// loadHWDData()
//-------------------------------------------------------------------------------
void WordData::loadHWDData(const HWD_DATA_TYPE* buffer, const size_t bufferSize) {
  size_t bufferIndex = 0;
  string key;
  string baseWord;
  char element;
  vector<int> syllableCount;

  if (buffer == nullptr) {
    return;
  }

  while (bufferIndex < bufferSize) {
    // get key
    bufferIndex = __this::getWordFromBuffer(buffer, bufferSize, bufferIndex, key);
    if (bufferIndex + 1 < bufferSize) {
      // get base key
      bufferIndex = __this::getWordFromBuffer(buffer, bufferSize, bufferIndex, baseWord);
      if (bufferIndex + 1 < bufferSize) {
        // get element
        element = buffer[bufferIndex];
        bufferIndex++;
        if (bufferIndex + 1 < bufferSize) {
          // get syllable count
          bufferIndex = __this::getSyllableCountFromBuffer(buffer, bufferSize, bufferIndex, syllableCount);
          s_wordMap[key] = WordInfo(baseWord, element, syllableCount);
        }
      }
    }
    bufferIndex++;
  }
}
//-------------------------------------------------------------------------------
// loadHWDData_multiple()
//-------------------------------------------------------------------------------
void WordData::loadHWDData_multiple(const HWD_DATA_TYPE* buffer, const size_t bufferSize
  , const int numThreads)
{
  vector<thread> threads;
  vector<const HWD_DATA_TYPE*> threadBuffers;
  vector<size_t> threadBufferSizes;

  size_t bufferSize_current;
  size_t bufferSize_remaining;

  if (buffer == nullptr || numThreads <= 0) {
    return;
  }

  // allocate vector of thread
  threads.reserve(numThreads);
  threadBuffers.resize(numThreads);
  threadBufferSizes.resize(numThreads);

  // initialize maps
  s_wordMaps.resize(numThreads);
  s_wordMaps_delimiters.resize(numThreads);

  // divide up the data among the threads using integer division
  //   (if number of threads is 16, first thread gets 1/16 of total data,
  //    then adds a little more to ensure this data ends with the end-of-entry byte;
  //    for the second, this is repeated, but for 1/15 of the datta that remain;
  //    the remaining threads follow suite)

  // set buffer of first thread
  threadBuffers[0] = buffer;
  bufferSize_current = bufferSize / numThreads;
  // ensure buffer ends with the end-of-entry byte
  while (threadBuffers[0][bufferSize_current - 1] != DELIMITER_INFO) {
    bufferSize_current++;
  }
  // set size of buffer of first thread
  threadBufferSizes[0] = bufferSize_current;
  // start first thread
  threads.push_back(
    thread(__this::loadHWDData_thread, threadBuffers[0], threadBufferSizes[0], 0)
  );

  // update remaining buffer size
  bufferSize_remaining = bufferSize - threadBufferSizes[0];
  for (int i = 1; i < numThreads; i++) {
    // set start of data buffer for thread
    threadBuffers[i] = threadBuffers[i - 1] + threadBufferSizes[i - 1];
    // get initial buffer size
    bufferSize_current = bufferSize_remaining / (numThreads - i);
    // ensure buffer ends with the end-of-entry byte
    while (threadBuffers[i][bufferSize_current - 1] != DELIMITER_INFO) {
      bufferSize_current++;
    }
    // set final buffer size
    threadBufferSizes[i] = bufferSize_current;
    // start thread
    threads.push_back(
      thread(__this::loadHWDData_thread, threadBuffers[i], threadBufferSizes[i], i)
    );
    // update remaining buffer size
    bufferSize_remaining -= threadBufferSizes[i];
  }

  // end threads
  for (int i = 0; i < numThreads; i++) {
    threads[i].join();
  }
}
//-------------------------------------------------------------------------------
// loadHWDData_thread()
//-------------------------------------------------------------------------------
void WordData::loadHWDData_thread(const HWD_DATA_TYPE* buffer, const size_t bufferSize
  , const int threadID)
{
  size_t bufferIndex = 0;

  string key;
  string baseWord;
  char element;
  vector<int> syllableCount;

  if (buffer == nullptr) {
    return;
  }

  s_wordMaps_delimiters[threadID] = "";

  // get key
  bufferIndex = __this::getWordFromBuffer(buffer, bufferSize, bufferIndex, key);
  s_wordMaps_delimiters[threadID] = key;
  if (bufferIndex + 1 < bufferSize) {
    // get base key
    bufferIndex = __this::getWordFromBuffer(buffer, bufferSize, bufferIndex, baseWord);
    if (bufferIndex + 1 < bufferSize) {
      // get element
      element = buffer[bufferIndex];
      bufferIndex++;
      if (bufferIndex + 1 < bufferSize) {
        // get syllable count
        bufferIndex = __this::getSyllableCountFromBuffer(buffer, bufferSize, bufferIndex, syllableCount);
        s_wordMaps_delimiters[threadID] = key;
        s_wordMaps[threadID][key] = WordInfo(baseWord, element, syllableCount);
      }
    }
  }
  bufferIndex++;

  while (bufferIndex < bufferSize) {
    bufferIndex = __this::getWordFromBuffer(buffer, bufferSize, bufferIndex, key);
    if (bufferIndex + 1 < bufferSize) {
      // get base key
      bufferIndex = __this::getWordFromBuffer(buffer, bufferSize, bufferIndex, baseWord);
      if (bufferIndex + 1 < bufferSize) {
        // get element
        element = buffer[bufferIndex];
        bufferIndex++;
        if (bufferIndex + 1 < bufferSize) {
          // get syllable count
          bufferIndex = __this::getSyllableCountFromBuffer(buffer, bufferSize, bufferIndex, syllableCount);
          s_wordMaps[threadID][key] = WordInfo(baseWord, element, syllableCount);
        }
      }
    }
    bufferIndex++;
  }
}

//-------------------------------------------------------------------------------
// writeHWD()
//-------------------------------------------------------------------------------
void WordData::writeHWD(FILE* target) {
  WordMap::iterator iter;
  WordMap::iterator iter_end = s_wordMap.end();

  for (iter = s_wordMap.begin(); iter != iter_end; iter++) {
    if (iter->second == WORD_INFO_NULL) {
      continue;
    }

    // map key (chars)
    fwrite(iter->first.data(), iter->first.size(), HWD_DATA_SIZE, target);
    fputc(DELIMITER_FIELD, target);

    // base key (chars)
    fwrite(iter->second.baseWord.data(), iter->second.baseWord.size(), HWD_DATA_SIZE, target);
    fputc(DELIMITER_FIELD, target);

    // element (char)
    fputc(iter->second.element, target);

    // syllable count (chars)
    for (int i = 0; i < (int)iter->second.syllableCount.size(); i++) {
      fputc(iter->second.syllableCount[i], target);
    }
    fputc(DELIMITER_FIELD, target);
    fputc(DELIMITER_INFO, target);
  }
}
//-------------------------------------------------------------------------------
// writeHWD_multiple()
//-------------------------------------------------------------------------------
void WordData::writeHWD_multiple(FILE* target) {
  WordMap::iterator iter;
  WordMap::iterator iter_end;

  for (int j = 0; j < (int)s_wordMaps.size(); j++) {
    // accounts for the word data being sparse
    if (j < (int)s_wordMaps.size() - 1 && s_wordMaps_delimiters[j] == s_wordMaps_delimiters[j + 1]) {
      continue;
    }

    iter_end = s_wordMaps[j].end();
    for (iter = s_wordMaps[j].begin(); iter != iter_end; iter++) {
      if (iter->second == WORD_INFO_NULL) {
        continue;
      }

      // map key (chars)
      fwrite(iter->first.data(), iter->first.size(), HWD_DATA_SIZE, target);
      fputc(DELIMITER_FIELD, target);

      // base key (chars)
      fwrite(iter->second.baseWord.data(), iter->second.baseWord.size(), HWD_DATA_SIZE, target);
      fputc(DELIMITER_FIELD, target);

      // element (char)
      fputc(iter->second.element, target);

      // syllable count (chars)
      for (int i = 0; i < (int)iter->second.syllableCount.size(); i++) {
        fputc(iter->second.syllableCount[i], target);
      }
      fputc(DELIMITER_FIELD, target);
      fputc(DELIMITER_INFO, target);
    }
  }
}

//-------------------------------------------------------------------------------
// streamOut()
//-------------------------------------------------------------------------------
void WordData::streamOut(ostream& target) {
  WordMap::iterator iter;
  WordMap::iterator iter_end = s_wordMap.end();

  for (iter = s_wordMap.begin(); iter != iter_end; iter++) {
    if (iter->second == WORD_INFO_NULL) {
      continue;
    }

    // key
    target << iter->first;

    // base key
    if (iter->second.baseWord == BASE_WORD_DEFAULT) {
      target << " " << BASE_WORD_DEFAULT_STRING;
    }
    else {
      target << " " << iter->second.baseWord;
    }

    // element
    target << " " << toElementString(iter->second.element);

    // syllable count
    if (iter->second.syllableCount.empty()) {
      target << " " << SYLLABLE_COUNT_DEFAULT_STRING;
    }
    else {
      for (int i = 0; i < (int)iter->second.syllableCount.size(); i++) {
        target << " " << iter->second.syllableCount[i];
      }
    }

    target << endl;
  }
}
//-------------------------------------------------------------------------------
// streamOut_multiple()
//-------------------------------------------------------------------------------
void WordData::streamOut_multiple(ostream& target) {
  WordMap::iterator iter;
  WordMap::iterator iter_end;

  for (int j = 0; j < (int)s_wordMaps.size(); j++) {
    // accounts for the word data being sparse
    if (j < (int)s_wordMaps.size() - 1 && s_wordMaps_delimiters[j] == s_wordMaps_delimiters[j + 1]) {
      continue;
    }

    iter_end = s_wordMaps[j].end();
    for (iter = s_wordMaps[j].begin(); iter != iter_end; iter++) {
      if (iter->second == WORD_INFO_NULL) {
        continue;
      }

      // key
      target << iter->first;

      // base key
      if (iter->second.baseWord == BASE_WORD_DEFAULT) {
        target << " " << BASE_WORD_DEFAULT_STRING;
      }
      else {
        target << " " << iter->second.baseWord;
      }

      // element
      target << " " << toElementString(iter->second.element);

      // syllable count
      if (iter->second.syllableCount.empty()) {
        target << SYLLABLE_COUNT_DEFAULT_STRING;
      }
      else {
        for (int i = 0; i < (int)iter->second.syllableCount.size(); i++) {
          target << " " << iter->second.syllableCount[i];
        }
      }

      target << endl;
    }
  }
}

//-------------------------------------------------------------------------------
// getWordFromBuffer()
//-------------------------------------------------------------------------------
int WordData::getWordFromBuffer(
  const HWD_DATA_TYPE* buffer
  , const size_t bufferSize
  , const size_t index
  , string& word
)
{
  // ensures word is empty
  word.clear();
  for (size_t i = index; i < bufferSize; i++) {
    if (buffer[i] == DELIMITER_FIELD) {
      // return index after delimiter
      return i + 1;
    }
    // append character to word
    word += (char)buffer[i];
  }

  // if end of buffer is reached
  return bufferSize;
}

//-------------------------------------------------------------------------------
// getSyllableCountFromBuffer()
//-------------------------------------------------------------------------------
int WordData::getSyllableCountFromBuffer(
  const HWD_DATA_TYPE* buffer
  , const size_t bufferSize
  , const size_t index
  , vector<int>& syllableCount
)
{
  // ensures syllableCount is empty
  syllableCount.clear();
  for (size_t i = index; i < bufferSize; i++) {
    if (buffer[i] == DELIMITER_FIELD) {
      // return index after delimiter
      return i + 1;
    }
    // append value to syllableCount
    syllableCount.emplace_back(buffer[i]);
  }

  // if end of buffer is reached
  return bufferSize;
}

//-------------------------------------------------------------------------------
// compressSyllables()
//-------------------------------------------------------------------------------
void WordData::compressSyllables() {
  vector<int>* syllableCount;
  for (WordMap::iterator iter = s_wordMap.begin(); iter != s_wordMap.end(); iter++) {
    syllableCount = &iter->second.syllableCount;
    for (int i = 0; i < (int)syllableCount->size(); i++) {
      // if the value at i appears before i in the vector
      if (isBeforeInVector(i, *syllableCount)) {
        // remove it and move entries after it
        syllableCount->erase(syllableCount->begin() + i);
        i--;
      }
    }
  }
}

//-------------------------------------------------------------------------------
// displayOutput_cout()
//-------------------------------------------------------------------------------
ostream& WordData::displayOutput_cout(const string& value) {
  *s_output << value;
  if (s_output != &cout) {
    cout << value;
  }
  return *s_output;
}

//-------------------------------------------------------------------------------
// sortCompare()
//-------------------------------------------------------------------------------
bool sortCompare(const std::pair<std::string, size_t>& item0, const std::pair<std::string, size_t>& item1) {
  // compares keys
  return item0.first < item1.first;
}