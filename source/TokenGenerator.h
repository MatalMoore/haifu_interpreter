#ifndef TOKEN_GENERATOR_H
#define TOKEN_GENERATOR_H

#define $TG TokenGenerator

#include <vector>
#include <string>
#include <map>

#include "WordData.h"
#include "funcs.h"
#include "elements.h"

#define TOKEN_TYPE_UNDEFINED 0
#define TOKEN_TYPE_RESERVED_WORD 1
#define TOKEN_TYPE_VARIABLE 2
#define TOKEN_TYPE_NUMBER 3
#define TOKEN_TYPE_HYPHEN 4
#define TOKEN_TYPE_PUNCTUATION 5
#define TOKEN_TYPE_COMMENT 6

#define TOKEN_LINE_NUMBER_DEFAULT 0
#define TOKEN_COLUMN_NUMBER_DEFAULT 0
#define TOKEN_NAME_DEFAULT ""
#define TOKEN_TYPE_DEFAULT TOKEN_TYPE_UNDEFINED
#define TOKEN_VALUE_DEFAULT 0
#define TOKEN_ELEMENT_DEFAULT ELEM_EARTH

std::string tokenTypeToString(const char tokenType);

struct HaifuToken {
  int lineNumber;
  int columnNumber;
  std::string name;
  char type;
  int value;
  char element;

  HaifuToken(
    const int i_lineNumber = TOKEN_LINE_NUMBER_DEFAULT
    , const int i_columnNumber = TOKEN_COLUMN_NUMBER_DEFAULT
    , const std::string& i_name = TOKEN_NAME_DEFAULT
    , const char i_type = TOKEN_TYPE_DEFAULT
    , const int i_value = TOKEN_VALUE_DEFAULT
    , const char i_element = TOKEN_ELEMENT_DEFAULT
    )
  {
    lineNumber = i_lineNumber;
    columnNumber = i_columnNumber;
    name = i_name;
    type = i_type;
    value = i_value;
    element = i_element;
  }
};

struct HaifuTokenError {
  int lineNumber;
  int columnNumber;
  std::string message;

  int debugLineNumber;

  HaifuTokenError(const int i_lineNumber, const int i_columnNumber, const std::string& i_message = "", const int i_debugLineNumber = 0) {
    lineNumber = i_lineNumber;
    columnNumber = i_columnNumber;
    message = i_message;
    debugLineNumber = i_debugLineNumber;
  }
};

// default value for a word representing a number
#define NUMBER_WORD_VALUE_DEFAULT -1

struct NumberWord {
  int value;

  NumberWord(const int i_value = NUMBER_WORD_VALUE_DEFAULT) {
    value = i_value;
  }
};

// reserved word codes
#define RESERVED_WORD_UNDEFINED 0
#define RESERVED_WORD_SOME 1
#define RESERVED_WORD_MANY 2
#define RESERVED_WORD_HEAVEN 3
#define RESERVED_WORD_PROMOTE 4
#define RESERVED_WORD_DEMOTE 5
#define RESERVED_WORD_BLOSSOM 6
#define RESERVED_WORD_RISE 7
#define RESERVED_WORD_FALL 8
#define RESERVED_WORD_LISTEN 9
#define RESERVED_WORD_SPEAK 10
#define RESERVED_WORD_COUNT 11
#define RESERVED_WORD_CREATE 12
#define RESERVED_WORD_DESTROY 13
#define RESERVED_WORD_FEAR 14
#define RESERVED_WORD_LOVE 15
#define RESERVED_WORD_BECOME 16
#define RESERVED_WORD_LIKE 17
#define RESERVED_WORD_TOMORROW 18
#define RESERVED_WORD_NEGATIVE 19
#define RESERVED_WORD_OPERATE 20

// default reserved word code
#define RESERVED_WORD_VALUE_DEFAULT RESERVED_WORD_UNDEFINED

struct ReservedWord {
  char value;

  ReservedWord(const char i_value = RESERVED_WORD_VALUE_DEFAULT) {
    value = i_value;
  }
};

typedef std::map<std::string, NumberWord> NumberWordMap;
typedef std::map<std::string, ReservedWord> ReservedWordMap;

class TokenGenerator {
public:
  // clears the vector of lines representing the file data
  static void clearFileData();

  // generates program tokens from the line data
  static void generateFileTokens(const std::vector<std::string>& data);
  // generates program tokens from the line data
  static void generateFileTokens();

  // returns a read-only reference to the program tokens
  //   (generates tokens if there are no tokens)
  static const std::vector<HaifuToken>& getTokens();

  // displays the warnings generated from token generation
  static void displayWarnings(std::ostream& output = std::cout);
  // displays the errors generated from token generation
  static void displayErrors(std::ostream& output = std::cout);

  // returns whether key is a reserved word
  static bool isReservedWord(const std::string& word);

private:
  static const NumberWordMap s_numberWordMap;
  static const ReservedWordMap s_reservedWordMap;

  static std::vector<std::string> s_fileData;
  static std::vector<HaifuToken> s_tokens;
  static const std::vector<HaifuToken> s_TOKENS_NULL;

  static std::vector<HaifuTokenError> s_tokenWarnings;
  static std::vector<HaifuTokenError> s_tokenErrors;

  static void setFileData(const std::vector<std::string>& data);
  static void generateInitialTokens();
  static void combineTokens();

  // returns a Token Type
  static char getHyphenatedTokens_type(
    const std::vector<HaifuToken>& sourceVector
    , std::vector<HaifuToken>& targetVector
    , int& offset
    );

  // combines tokens separated by HYPHEN tokens by concatenating their text data and using the position of the first token
  static HaifuToken combineVariableTokens(const std::vector<HaifuToken>& sourceVector);
  // combines NUMBER tokens separated by HYPHEN tokens by combining their interpreted values and concatenating their text data
  static HaifuToken combineNumberTokens(const std::vector<HaifuToken>& sourceVector);
  // combines NUMBER tokens separated by HYPHEN tokens by multiplying thier values and concatenating their text data
  static HaifuToken combineNumberTokens_multiply(const std::vector<HaifuToken>& sourceVector, int& offset);
  // combines NUMBER tokens separated by HYPHEN tokens by adding thier values and concatenating their text data
  static HaifuToken combineNumberTokens_add(const std::vector<HaifuToken>& sourceVector, int& offset);

  // generrates a warning
  static void makeWarning(const int lineNumber, const int columnNumber, const std::string& message, const int debugLineNumber);
  // generrates an error
  static void makeError(const int lineNumber, const int columnNumber, const std::string& message, const int debugLineNumber);

  // clears all warnings
  static void clearWarnings();
  // clears all errors
  static void clearErrors();
};

#endif