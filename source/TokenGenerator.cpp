#include "TokenGenerator.h"

using namespace std;

string tokenTypeToString(const char tokenType) {
  switch (tokenType) {
  case TOKEN_TYPE_RESERVED_WORD:
    return "RESERVED_WORD";
  case TOKEN_TYPE_VARIABLE:
    return "VARIABLE";
  case TOKEN_TYPE_NUMBER:
    return "NUMBER";
  case TOKEN_TYPE_HYPHEN:
    return "HYPHEN";
  case TOKEN_TYPE_PUNCTUATION:
    return "PUNCTUATION";
  case TOKEN_TYPE_COMMENT:
    return "COMMENT";
  default:
    return "UNDEFINED";
  }
}

const NumberWordMap TokenGenerator::s_numberWordMap = {
  {"no", NumberWord(0)}, {"none", NumberWord(0)}
  ,{"nothing", NumberWord(0)}

  , {"a", NumberWord(1)}, {"an", NumberWord(1)}

  , {"couple", NumberWord(2)}
  , {"decade", NumberWord(10)}
  , {"dozen", NumberWord(12)}
  , {"score", NumberWord(20)}

  , {"century", NumberWord(100)}, {"centurial", NumberWord(100)}
  , {"centuries", NumberWord(100)}

  , {"millenium", NumberWord(1000)}, {"millenial", NumberWord(1000)}
  , {"millenia", NumberWord(1000)}, {"milleniums", NumberWord(1000)}

  , {"once", NumberWord(1)}, {"twice", NumberWord(2)}
  , {"thrice", NumberWord(3)}

  , {"zero", NumberWord(0)}, {"zeroth", NumberWord(0)}
  , {"one", NumberWord(1)}, {"first", NumberWord(1)}
  , {"two", NumberWord(2)}, {"second", NumberWord(2)}
  , {"three", NumberWord(3)}, {"third", NumberWord(3)}
  , {"four", NumberWord(4)}, {"fourth", NumberWord(4)}
  , {"five", NumberWord(5)}, {"fifth", NumberWord(5)}
  , {"six", NumberWord(6)}, {"sixth", NumberWord(6)}
  , {"seven", NumberWord(7)}, {"seventh", NumberWord(7)}
  , {"eight", NumberWord(8)}, {"eighth", NumberWord(8)}
  , {"nine", NumberWord(9)}, {"ninth", NumberWord(9)}
  , {"ten", NumberWord(10)}, {"tenth", NumberWord(10)}
  , {"eleven", NumberWord(11)}, {"eleventh", NumberWord(11)}
  , {"twelve", NumberWord(12)}, {"twelfth", NumberWord(12)}
  , {"thirteen", NumberWord(13)}, {"thirteenth", NumberWord(13)}
  , {"fourteen", NumberWord(14)}, {"fourteenth", NumberWord(14)}
  , {"fifteen", NumberWord(15)}, {"fifteenth", NumberWord(15)}
  , {"sixteen", NumberWord(16)}, {"sixteenth", NumberWord(16)}
  , {"seventeen", NumberWord(17)}, {"seventeenth", NumberWord(17)}
  , {"eighteen", NumberWord(18)}, {"eighteenth", NumberWord(18)}
  , {"nineteen", NumberWord(19)}, {"nineteenth", NumberWord(19)}
  , {"twenty", NumberWord(20)}, {"twentieth", NumberWord(20)}
  , {"thirty", NumberWord(30)}, {"thirtieth", NumberWord(30)}
  , {"forty", NumberWord(40)}, {"fortieth", NumberWord(40)}
  , {"fifty", NumberWord(50)}, {"fiftieth", NumberWord(50)}
  , {"sixty", NumberWord(60)}, {"sixtieth", NumberWord(60)}
  , {"seventy", NumberWord(70)}, {"seventieth", NumberWord(70)}
  , {"eighty", NumberWord(80)}, {"eightieth", NumberWord(80)}
  , {"ninety", NumberWord(90)}, {"ninetieth", NumberWord(90)}
  , {"hundred", NumberWord(100)}, {"hundredth", NumberWord(100)}
  , {"thousand", NumberWord(1000)}, {"thousandth", NumberWord(1000)}
  , {"million", NumberWord(1000000)}, {"millionth", NumberWord(1000000)}
  , {"billion", NumberWord(1000000000)}, {"billionth", NumberWord(1000000000)}
};
const ReservedWordMap TokenGenerator::s_reservedWordMap = {
  {"some", ReservedWord(RESERVED_WORD_SOME)}, {"few", ReservedWord(RESERVED_WORD_SOME)}

  , {"many", ReservedWord(RESERVED_WORD_MANY)}, {"plethora", ReservedWord(RESERVED_WORD_MANY)}
  , {"myriad", ReservedWord(RESERVED_WORD_MANY)}

  , {"heaven", ReservedWord(RESERVED_WORD_HEAVEN)}, {"heavens", ReservedWord(RESERVED_WORD_HEAVEN)}

  , {"nirvana", ReservedWord(RESERVED_WORD_HEAVEN)}

  , {"enlightenment", ReservedWord(RESERVED_WORD_HEAVEN)}
  , {"enlighten", ReservedWord(RESERVED_WORD_HEAVEN)}, {"enlightens", ReservedWord(RESERVED_WORD_HEAVEN)}
  , {"enlightened", ReservedWord(RESERVED_WORD_HEAVEN)}, {"enlightening", ReservedWord(RESERVED_WORD_HEAVEN)}

  , {"harmony", ReservedWord(RESERVED_WORD_HEAVEN)}, {"harmonies", ReservedWord(RESERVED_WORD_HEAVEN)}
  , {"harmonious", ReservedWord(RESERVED_WORD_HEAVEN)}

  , {"promote", ReservedWord(RESERVED_WORD_PROMOTE)}, {"promotes", ReservedWord(RESERVED_WORD_PROMOTE)}
  , {"promoted", ReservedWord(RESERVED_WORD_PROMOTE)}, {"promoting", ReservedWord(RESERVED_WORD_PROMOTE)}
  , {"promotion", ReservedWord(RESERVED_WORD_PROMOTE)}, {"promotions", ReservedWord(RESERVED_WORD_PROMOTE)}

  , {"more", ReservedWord(RESERVED_WORD_PROMOTE)}

  , {"increase", ReservedWord(RESERVED_WORD_PROMOTE)}, {"increases", ReservedWord(RESERVED_WORD_PROMOTE)}
  , {"increased", ReservedWord(RESERVED_WORD_PROMOTE)}, {"increasing", ReservedWord(RESERVED_WORD_PROMOTE)}

  , {"wax", ReservedWord(RESERVED_WORD_PROMOTE)}, {"waxes", ReservedWord(RESERVED_WORD_PROMOTE)}
  , {"waxed", ReservedWord(RESERVED_WORD_PROMOTE)}, {"waxing", ReservedWord(RESERVED_WORD_PROMOTE)}

  , {"demote", ReservedWord(RESERVED_WORD_DEMOTE)}, {"demotes", ReservedWord(RESERVED_WORD_DEMOTE)}
  , {"demoted", ReservedWord(RESERVED_WORD_DEMOTE)}, {"demoting", ReservedWord(RESERVED_WORD_DEMOTE)}
  , {"demotion", ReservedWord(RESERVED_WORD_DEMOTE)}, {"demotions", ReservedWord(RESERVED_WORD_DEMOTE)}

  , {"less", ReservedWord(RESERVED_WORD_DEMOTE)}
  , {"lessen", ReservedWord(RESERVED_WORD_DEMOTE)}, {"lessens", ReservedWord(RESERVED_WORD_DEMOTE)}
  , {"lessened", ReservedWord(RESERVED_WORD_DEMOTE)}, {"lessening", ReservedWord(RESERVED_WORD_DEMOTE)}

  , {"reduce", ReservedWord(RESERVED_WORD_DEMOTE)}, {"reduces", ReservedWord(RESERVED_WORD_DEMOTE)}
  , {"reduced", ReservedWord(RESERVED_WORD_DEMOTE)}, {"reducing", ReservedWord(RESERVED_WORD_DEMOTE)}
  , {"reduction", ReservedWord(RESERVED_WORD_DEMOTE)}, {"reductions", ReservedWord(RESERVED_WORD_DEMOTE)}

  , {"wane", ReservedWord(RESERVED_WORD_DEMOTE)}, {"wanes", ReservedWord(RESERVED_WORD_DEMOTE)}
  , {"waned", ReservedWord(RESERVED_WORD_DEMOTE)}, {"waning", ReservedWord(RESERVED_WORD_DEMOTE)}

  , {"blossom", ReservedWord(RESERVED_WORD_BLOSSOM)}, {"blossoms", ReservedWord(RESERVED_WORD_BLOSSOM)}
  , {"blossomed", ReservedWord(RESERVED_WORD_BLOSSOM)}, {"blossoming", ReservedWord(RESERVED_WORD_BLOSSOM)}

  , {"flower", ReservedWord(RESERVED_WORD_BLOSSOM)}, {"flowers", ReservedWord(RESERVED_WORD_BLOSSOM)}
  , {"flowered", ReservedWord(RESERVED_WORD_BLOSSOM)}, {"flowering", ReservedWord(RESERVED_WORD_BLOSSOM)}

  , {"petal", ReservedWord(RESERVED_WORD_BLOSSOM)}, {"petals", ReservedWord(RESERVED_WORD_BLOSSOM)}

  , {"rise", ReservedWord(RESERVED_WORD_RISE)}, {"rises", ReservedWord(RESERVED_WORD_RISE)}
  , {"rose", ReservedWord(RESERVED_WORD_RISE)}, {"rising", ReservedWord(RESERVED_WORD_RISE)}
  , {"risen", ReservedWord(RESERVED_WORD_RISE)}

  , {"float", ReservedWord(RESERVED_WORD_RISE)}, {"floats", ReservedWord(RESERVED_WORD_RISE)}
  , {"floated", ReservedWord(RESERVED_WORD_RISE)}, {"floating", ReservedWord(RESERVED_WORD_RISE)}

  , {"ascend", ReservedWord(RESERVED_WORD_RISE)}, {"ascends", ReservedWord(RESERVED_WORD_RISE)}
  , {"ascended", ReservedWord(RESERVED_WORD_RISE)}, {"ascending", ReservedWord(RESERVED_WORD_RISE)}
  , {"ascension", ReservedWord(RESERVED_WORD_RISE)}, {"ascensions", ReservedWord(RESERVED_WORD_RISE)}

  , {"up", ReservedWord(RESERVED_WORD_RISE)}, {"upward", ReservedWord(RESERVED_WORD_RISE)}

  , {"fall", ReservedWord(RESERVED_WORD_FALL)}, {"falls", ReservedWord(RESERVED_WORD_FALL)}
  , {"fell", ReservedWord(RESERVED_WORD_FALL)}, {"falling", ReservedWord(RESERVED_WORD_FALL)}
  , {"fallen", ReservedWord(RESERVED_WORD_FALL)}

  , {"drop", ReservedWord(RESERVED_WORD_FALL)}, {"drops", ReservedWord(RESERVED_WORD_FALL)}
  , {"dropped", ReservedWord(RESERVED_WORD_FALL)}, {"dropping", ReservedWord(RESERVED_WORD_FALL)}

  , {"descend", ReservedWord(RESERVED_WORD_FALL)}, {"descends", ReservedWord(RESERVED_WORD_FALL)}
  , {"descended", ReservedWord(RESERVED_WORD_FALL)}, {"descending", ReservedWord(RESERVED_WORD_FALL)}
  , {"descension", ReservedWord(RESERVED_WORD_FALL)}, {"descensions", ReservedWord(RESERVED_WORD_FALL)}

  , {"down", ReservedWord(RESERVED_WORD_FALL)}, {"downward", ReservedWord(RESERVED_WORD_FALL)}

  , {"listen", ReservedWord(RESERVED_WORD_LISTEN)}, {"listens", ReservedWord(RESERVED_WORD_LISTEN)}
  , {"listened", ReservedWord(RESERVED_WORD_LISTEN)}, {"listening", ReservedWord(RESERVED_WORD_LISTEN)}

  , {"hear", ReservedWord(RESERVED_WORD_LISTEN)}, {"hears", ReservedWord(RESERVED_WORD_LISTEN)}
  , {"heard", ReservedWord(RESERVED_WORD_LISTEN)}, {"hearing", ReservedWord(RESERVED_WORD_LISTEN)}

  , {"see", ReservedWord(RESERVED_WORD_LISTEN)}, {"sees", ReservedWord(RESERVED_WORD_LISTEN)}
  , {"saw", ReservedWord(RESERVED_WORD_LISTEN)}, {"seeing", ReservedWord(RESERVED_WORD_LISTEN)}

  , {"speak", ReservedWord(RESERVED_WORD_SPEAK)}, {"speaks", ReservedWord(RESERVED_WORD_SPEAK)}
  , {"spoke", ReservedWord(RESERVED_WORD_SPEAK)}, {"speaking", ReservedWord(RESERVED_WORD_SPEAK)}
  , {"spoken", ReservedWord(RESERVED_WORD_SPEAK)}

  , {"say", ReservedWord(RESERVED_WORD_SPEAK)}, {"says", ReservedWord(RESERVED_WORD_SPEAK)}
  , {"said", ReservedWord(RESERVED_WORD_SPEAK)}, {"saying", ReservedWord(RESERVED_WORD_SPEAK)}

  , {"draw", ReservedWord(RESERVED_WORD_SPEAK)}, {"draws", ReservedWord(RESERVED_WORD_SPEAK)}
  , {"drew", ReservedWord(RESERVED_WORD_SPEAK)}, {"drawing", ReservedWord(RESERVED_WORD_SPEAK)}
  , {"drawn", ReservedWord(RESERVED_WORD_SPEAK)}

  , {"count", ReservedWord(RESERVED_WORD_COUNT)}, {"counts", ReservedWord(RESERVED_WORD_COUNT)}
  , {"counted", ReservedWord(RESERVED_WORD_COUNT)}, {"counting", ReservedWord(RESERVED_WORD_COUNT)}

  , {"number", ReservedWord(RESERVED_WORD_COUNT)}, {"numbers", ReservedWord(RESERVED_WORD_COUNT)}
  , {"numbered", ReservedWord(RESERVED_WORD_COUNT)}, {"numbering", ReservedWord(RESERVED_WORD_COUNT)}

  , {"age", ReservedWord(RESERVED_WORD_COUNT)}, {"ages", ReservedWord(RESERVED_WORD_COUNT)}
  , {"aged", ReservedWord(RESERVED_WORD_COUNT)}, {"aging", ReservedWord(RESERVED_WORD_COUNT)}

  , {"create", ReservedWord(RESERVED_WORD_CREATE)}, {"creates", ReservedWord(RESERVED_WORD_CREATE)}
  , {"created", ReservedWord(RESERVED_WORD_CREATE)}, {"creating", ReservedWord(RESERVED_WORD_CREATE)}
  , {"creation", ReservedWord(RESERVED_WORD_CREATE)}, {"creations", ReservedWord(RESERVED_WORD_CREATE)}

  , {"produce", ReservedWord(RESERVED_WORD_CREATE)}, {"produces", ReservedWord(RESERVED_WORD_CREATE)}
  , {"produced", ReservedWord(RESERVED_WORD_CREATE)}, {"producing", ReservedWord(RESERVED_WORD_CREATE)}
  , {"production", ReservedWord(RESERVED_WORD_CREATE)}, {"productions", ReservedWord(RESERVED_WORD_CREATE)}
  , {"productive", ReservedWord(RESERVED_WORD_CREATE)}

  , {"build", ReservedWord(RESERVED_WORD_CREATE)}, {"builds", ReservedWord(RESERVED_WORD_CREATE)}
  , {"built", ReservedWord(RESERVED_WORD_CREATE)}, {"building", ReservedWord(RESERVED_WORD_CREATE)}

  , {"destroy", ReservedWord(RESERVED_WORD_DESTROY)}, {"destroys", ReservedWord(RESERVED_WORD_DESTROY)}
  , {"destroyed", ReservedWord(RESERVED_WORD_DESTROY)}, {"destroying", ReservedWord(RESERVED_WORD_DESTROY)}
  , {"destruction", ReservedWord(RESERVED_WORD_DESTROY)}
  , {"destructive", ReservedWord(RESERVED_WORD_DESTROY)}

  , {"damage", ReservedWord(RESERVED_WORD_DESTROY)}, {"damages", ReservedWord(RESERVED_WORD_DESTROY)}
  , {"damaged", ReservedWord(RESERVED_WORD_DESTROY)}, {"damaging", ReservedWord(RESERVED_WORD_DESTROY)}

  , {"kill", ReservedWord(RESERVED_WORD_DESTROY)}, {"kills", ReservedWord(RESERVED_WORD_DESTROY)}
  , {"killed", ReservedWord(RESERVED_WORD_DESTROY)}, {"killing", ReservedWord(RESERVED_WORD_DESTROY)}

  , {"fear", ReservedWord(RESERVED_WORD_FEAR)}, {"fears", ReservedWord(RESERVED_WORD_FEAR)}
  , {"feared", ReservedWord(RESERVED_WORD_FEAR)}, {"fearing", ReservedWord(RESERVED_WORD_FEAR)}
  , {"fearful", ReservedWord(RESERVED_WORD_FEAR)}

  , {"hate", ReservedWord(RESERVED_WORD_FEAR)}, {"hates", ReservedWord(RESERVED_WORD_FEAR)}
  , {"hated", ReservedWord(RESERVED_WORD_FEAR)}, {"hating", ReservedWord(RESERVED_WORD_FEAR)}
  , {"hatred", ReservedWord(RESERVED_WORD_FEAR)}, {"hatreds", ReservedWord(RESERVED_WORD_FEAR)}
  , {"hateful", ReservedWord(RESERVED_WORD_FEAR)}

  , {"doubt", ReservedWord(RESERVED_WORD_FEAR)}, {"doubts", ReservedWord(RESERVED_WORD_FEAR)}
  , {"doubted", ReservedWord(RESERVED_WORD_FEAR)}, {"doubting", ReservedWord(RESERVED_WORD_FEAR)}
  , {"doubtful", ReservedWord(RESERVED_WORD_FEAR)}

  , {"love", ReservedWord(RESERVED_WORD_LOVE)}, {"loves", ReservedWord(RESERVED_WORD_LOVE)}
  , {"loved", ReservedWord(RESERVED_WORD_LOVE)}, {"loving", ReservedWord(RESERVED_WORD_LOVE)}
  , {"lovely", ReservedWord(RESERVED_WORD_LOVE)}

  , {"desire", ReservedWord(RESERVED_WORD_LOVE)}, {"desires", ReservedWord(RESERVED_WORD_LOVE)}
  , {"desired", ReservedWord(RESERVED_WORD_LOVE)}, {"desiring", ReservedWord(RESERVED_WORD_LOVE)}
  , {"desirous", ReservedWord(RESERVED_WORD_LOVE)}

  , {"regard", ReservedWord(RESERVED_WORD_LOVE)}, {"regards", ReservedWord(RESERVED_WORD_LOVE)}
  , {"regarded", ReservedWord(RESERVED_WORD_LOVE)}, {"regarding", ReservedWord(RESERVED_WORD_LOVE)}

  , {"become", ReservedWord(RESERVED_WORD_BECOME)}, {"becomes", ReservedWord(RESERVED_WORD_BECOME)}
  , {"became", ReservedWord(RESERVED_WORD_BECOME)}, {"becoming", ReservedWord(RESERVED_WORD_BECOME)}

  , {"reach", ReservedWord(RESERVED_WORD_BECOME)}, {"reaches", ReservedWord(RESERVED_WORD_BECOME)}
  , {"reached", ReservedWord(RESERVED_WORD_BECOME)}, {"reaching", ReservedWord(RESERVED_WORD_BECOME)}

  , {"achieve", ReservedWord(RESERVED_WORD_BECOME)}, {"achieves", ReservedWord(RESERVED_WORD_BECOME)}
  , {"achieved", ReservedWord(RESERVED_WORD_BECOME)}, {"achieving", ReservedWord(RESERVED_WORD_BECOME)}
  , {"achievement", ReservedWord(RESERVED_WORD_BECOME)}, {"achievements", ReservedWord(RESERVED_WORD_BECOME)}

  , {"like", ReservedWord(RESERVED_WORD_LIKE)}, {"alike", ReservedWord(RESERVED_WORD_LIKE)}
  , {"liken", ReservedWord(RESERVED_WORD_LIKE)}, {"likens", ReservedWord(RESERVED_WORD_LIKE)}

  , {"as", ReservedWord(RESERVED_WORD_LIKE)}

  , {"is", ReservedWord(RESERVED_WORD_LIKE)}, {"are", ReservedWord(RESERVED_WORD_LIKE)}
  , {"was", ReservedWord(RESERVED_WORD_LIKE)}, {"were", ReservedWord(RESERVED_WORD_LIKE)}
  , {"are", ReservedWord(RESERVED_WORD_LIKE)}, {"be", ReservedWord(RESERVED_WORD_LIKE)}
  , {"being", ReservedWord(RESERVED_WORD_LIKE)}, {"been", ReservedWord(RESERVED_WORD_LIKE)}

  , {"resemble", ReservedWord(RESERVED_WORD_LIKE)}, {"resembles", ReservedWord(RESERVED_WORD_LIKE)}
  , {"resembled", ReservedWord(RESERVED_WORD_LIKE)}, {"resembling", ReservedWord(RESERVED_WORD_LIKE)}

  , {"tomorrow", ReservedWord(RESERVED_WORD_TOMORROW)}
  , {"today", ReservedWord(RESERVED_WORD_TOMORROW)}
  , {"yesterday", ReservedWord(RESERVED_WORD_TOMORROW)}

  , {"negative", ReservedWord(RESERVED_WORD_NEGATIVE)}, {"negatives", ReservedWord(RESERVED_WORD_NEGATIVE)}
  , {"negate", ReservedWord(RESERVED_WORD_NEGATIVE)}, {"negates", ReservedWord(RESERVED_WORD_NEGATIVE)}
  , {"negated", ReservedWord(RESERVED_WORD_NEGATIVE)}, {"negating", ReservedWord(RESERVED_WORD_NEGATIVE)}
  , {"negation", ReservedWord(RESERVED_WORD_NEGATIVE)}, {"negations", ReservedWord(RESERVED_WORD_NEGATIVE)}

  , {"not", ReservedWord(RESERVED_WORD_NEGATIVE)}

  , {"deny", ReservedWord(RESERVED_WORD_NEGATIVE)}, {"denies", ReservedWord(RESERVED_WORD_NEGATIVE)}
  , {"denied", ReservedWord(RESERVED_WORD_NEGATIVE)}, {"denying", ReservedWord(RESERVED_WORD_NEGATIVE)}
  , {"denial", ReservedWord(RESERVED_WORD_NEGATIVE)}, {"denials", ReservedWord(RESERVED_WORD_NEGATIVE)}

  , {"operate", ReservedWord(RESERVED_WORD_OPERATE)}, {"operates", ReservedWord(RESERVED_WORD_OPERATE)}
  , {"operated", ReservedWord(RESERVED_WORD_OPERATE)}, {"operating", ReservedWord(RESERVED_WORD_OPERATE)}
  , {"operation", ReservedWord(RESERVED_WORD_OPERATE)}, {"operations", ReservedWord(RESERVED_WORD_OPERATE)}

  , {"examine", ReservedWord(RESERVED_WORD_OPERATE)}, {"examines", ReservedWord(RESERVED_WORD_OPERATE)}
  , {"examined", ReservedWord(RESERVED_WORD_OPERATE)}, {"examining", ReservedWord(RESERVED_WORD_OPERATE)}
  , {"examination", ReservedWord(RESERVED_WORD_OPERATE)}, {"examinations", ReservedWord(RESERVED_WORD_OPERATE)}

  , {"study", ReservedWord(RESERVED_WORD_OPERATE)}, {"studies", ReservedWord(RESERVED_WORD_OPERATE)}
  , {"studied", ReservedWord(RESERVED_WORD_OPERATE)}, {"studying", ReservedWord(RESERVED_WORD_OPERATE)}
};

vector<string> TokenGenerator::s_fileData;
vector<HaifuToken> TokenGenerator::s_tokens;
const vector<HaifuToken> TokenGenerator::s_TOKENS_NULL;

vector<HaifuTokenError> TokenGenerator::s_tokenWarnings;
vector<HaifuTokenError> TokenGenerator::s_tokenErrors;

void TokenGenerator::clearFileData() {
  s_fileData.clear();
}

void TokenGenerator::generateFileTokens(const std::vector<std::string>& data) {
  s_tokens.clear();
  clearWarnings();
  clearErrors();

  setFileData(data);
  generateInitialTokens();
  combineTokens();
}
void TokenGenerator::generateFileTokens() {
  s_tokens.clear();
  clearWarnings();
  clearErrors();

  generateInitialTokens();
  combineTokens();
}

const vector<HaifuToken>& TokenGenerator::getTokens() {
  if (s_tokenErrors.size() > 0) {
    return s_TOKENS_NULL;
  }
  else {
    return s_tokens;
  }
}

void TokenGenerator::displayWarnings(ostream& output) {
  for (int i = 0; i < (int)s_tokenWarnings.size(); i++) {
    output << "Warning at (" << s_tokenWarnings[i].lineNumber
      << ", " << s_tokenWarnings[i].columnNumber << ") " << s_tokenWarnings[i].debugLineNumber << ": "
      << s_tokenWarnings[i].message << endl;
  }
}
void TokenGenerator::displayErrors(ostream& output) {
  for (int i = 0; i < (int)s_tokenErrors.size(); i++) {
    output << "Error at (" << s_tokenErrors[i].lineNumber
      << ", " << s_tokenErrors[i].columnNumber << ") " << s_tokenErrors[i].debugLineNumber << ": "
      << s_tokenErrors[i].message << endl;
  }
}

bool TokenGenerator::isReservedWord(const string& word) {
  return
    (s_numberWordMap.find(word) != s_numberWordMap.end())
    || (s_reservedWordMap.find(word) != s_reservedWordMap.end())
    ;
}

void TokenGenerator::setFileData(const vector<string>& data) {
  s_fileData = data;
}
void TokenGenerator::generateInitialTokens() {
  int fileIndex;
  int lineIndex;
  string tokenString;
  int wordSize;

  char firstChar;
  bool inComment = false;

  const string* linePointer;

  ReservedWordMap::const_iterator reservedWord_iter;
  NumberWordMap::const_iterator numberWord_iter;

  for (fileIndex = 0; fileIndex < (int)s_fileData.size(); fileIndex++) {
    linePointer = &s_fileData[fileIndex];
    tokenString = lowerCase(getNextHaifuTokenWord(*linePointer, 0, lineIndex, wordSize));
    while (lineIndex < (int)linePointer->size()) {
      if (inComment) {
        if (tokenString == ",") {
          inComment = false;
        }
      }
      else if (tokenString.size() == 1) {
        firstChar = tokenString[0];
        if (firstChar == '-') {
          s_tokens.push_back(
            HaifuToken(
              fileIndex, lineIndex, tokenString
              , TOKEN_TYPE_HYPHEN, TOKEN_VALUE_DEFAULT, TOKEN_ELEMENT_DEFAULT
              )
            );
        }
        else if (firstChar == ',') {
          s_tokens.push_back(
            HaifuToken(
              fileIndex, lineIndex, tokenString
              , TOKEN_TYPE_COMMENT, TOKEN_VALUE_DEFAULT, TOKEN_ELEMENT_DEFAULT
              )
            );
          inComment = true;
        }
        else if (firstChar >= '0' && firstChar <= '9') {
          makeWarning(fileIndex, lineIndex
            , string("unexpected character '") + firstChar + string("'")
            , __LINE__
            );
        }
        else if (isWhitespace(firstChar)) {
          makeWarning(fileIndex, lineIndex
            , "unexpected whitespace character '"
            , __LINE__
            );
        }
        else if (tokenString == "a") {
          s_tokens.push_back(
            HaifuToken(
              fileIndex, lineIndex, tokenString
              , TOKEN_TYPE_NUMBER, 1, ELEM_EARTH
              )
            );
        }
        else if (isHaifuChar(firstChar)) {
          s_tokens.push_back(
            HaifuToken(
              fileIndex, lineIndex, tokenString
              , TOKEN_TYPE_VARIABLE, TOKEN_VALUE_DEFAULT, $WD::lookup(tokenString).element
              )
            );
        }
        else {
          s_tokens.push_back(
            HaifuToken(
              fileIndex, lineIndex, tokenString
              , TOKEN_TYPE_PUNCTUATION, TOKEN_VALUE_DEFAULT, TOKEN_ELEMENT_DEFAULT
              )
            );
        }
      } // !(tokenString.size() == 1)
      else {
        reservedWord_iter = s_reservedWordMap.find(tokenString);
        numberWord_iter = s_numberWordMap.find(tokenString);

        if (reservedWord_iter != s_reservedWordMap.end()) {
          s_tokens.push_back(
            HaifuToken(
              fileIndex, lineIndex, tokenString
              , TOKEN_TYPE_RESERVED_WORD, reservedWord_iter->second.value, TOKEN_ELEMENT_DEFAULT
              )
            );
        }
        else if (numberWord_iter != s_numberWordMap.end()) {
          s_tokens.push_back(
            HaifuToken(
              fileIndex, lineIndex, tokenString
              , TOKEN_TYPE_NUMBER, numberWord_iter->second.value, ELEM_EARTH
              )
            );
        }
        else {
          s_tokens.push_back(
            HaifuToken(
              fileIndex, lineIndex, tokenString
              , TOKEN_TYPE_VARIABLE, TOKEN_VALUE_DEFAULT, TOKEN_ELEMENT_DEFAULT
              )
            );
        }
      }

      lineIndex += wordSize;
      tokenString = lowerCase(getNextHaifuTokenWord(*linePointer, lineIndex, lineIndex, wordSize));
    }
  }
}
void TokenGenerator::combineTokens() {
  char tokenType;
  char hyphenatedTokensType;
  vector<HaifuToken> new_tokens;
  vector<HaifuToken> hyphenatedTokens;
  int lastIndex = (int)s_tokens.size() - 1;

  for (int i = 0; i + 1 < (int)s_tokens.size(); i++) {
    tokenType = s_tokens[i].type;
    if (s_tokens[i + 1].type == TOKEN_TYPE_HYPHEN) {
      hyphenatedTokensType = getHyphenatedTokens_type(s_tokens, hyphenatedTokens, i);
      if (hyphenatedTokensType == TOKEN_TYPE_NUMBER) {
        new_tokens.push_back(combineNumberTokens(hyphenatedTokens));
      }
      else if (hyphenatedTokensType == TOKEN_TYPE_VARIABLE) {
        new_tokens.push_back(combineVariableTokens(hyphenatedTokens));
      }
      else {
        // Error, but one caught by getHyphenatedTokens_type()
      }
    }
    else if (tokenType == TOKEN_TYPE_UNDEFINED) {
      // should not happen
      makeError(s_tokens[i].lineNumber, s_tokens[i].columnNumber
        , "unexpected token of type " + tokenTypeToString(TOKEN_TYPE_UNDEFINED)
        , __LINE__
        );
    }
    else if (tokenType != TOKEN_TYPE_COMMENT) {
      new_tokens.push_back(s_tokens[i]);
    }
  }

  if (lastIndex - 1 >= 0) {
    if (s_tokens[lastIndex - 1].type != TOKEN_TYPE_HYPHEN) {
      tokenType = s_tokens[lastIndex].type;
      if (tokenType == TOKEN_TYPE_UNDEFINED) {
        // should not happen
        makeError(s_tokens[lastIndex].lineNumber, s_tokens[lastIndex].columnNumber
          , "unexpected token of type " + tokenTypeToString(TOKEN_TYPE_UNDEFINED)
          , __LINE__
          );
      }
      else if (tokenType != TOKEN_TYPE_COMMENT) {
        new_tokens.push_back(s_tokens[lastIndex]);
      }
    }
  }

  s_tokens.swap(new_tokens);
}

char TokenGenerator::getHyphenatedTokens_type(
  const vector<HaifuToken>& sourceVector
  , vector<HaifuToken>& targetVector
  , int& offset
  )
{
  char returnType;
  char tokenType;

  targetVector.clear();
  targetVector.reserve(sourceVector.size());

  // hyphenated words may only contain Reserved Words, Variables, and Numbers
  returnType = TOKEN_TYPE_NUMBER;
  for (; offset + 1 < (int)sourceVector.size(); offset += 2) {
    tokenType = sourceVector.at(offset).type;
    switch (tokenType) {
    case TOKEN_TYPE_RESERVED_WORD:
    case TOKEN_TYPE_VARIABLE:
    case TOKEN_TYPE_NUMBER:
      if (tokenType != returnType) {
        returnType = TOKEN_TYPE_VARIABLE;
      }
      break;
    default:
      makeError(sourceVector.at(offset).lineNumber, sourceVector.at(offset).columnNumber,
        "token type " + tokenTypeToString(tokenType) + " cannot be hyphenated"
        , __LINE__
        );
      offset += 1;
      return TOKEN_TYPE_UNDEFINED;
    }

    targetVector.push_back(sourceVector.at(offset));

    if (sourceVector.at(offset + 1).type != TOKEN_TYPE_HYPHEN) {
      return returnType;
    }
  }

  if (offset < (int)sourceVector.size()) {
    tokenType = sourceVector.at(offset).type;
    switch (tokenType) {
    case TOKEN_TYPE_RESERVED_WORD:
    case TOKEN_TYPE_VARIABLE:
    case TOKEN_TYPE_NUMBER:
      if (tokenType != returnType) {
        returnType = TOKEN_TYPE_VARIABLE;
      }
      break;
    default:
      makeError(sourceVector.at(offset).lineNumber, sourceVector.at(offset).columnNumber,
        "token type " + tokenTypeToString(tokenType) + " cannot be hyphenated"
        , __LINE__
        );
      tokenType = sourceVector.at(offset).type;
      offset += 1;
      return TOKEN_TYPE_UNDEFINED;
    }

    targetVector.push_back(sourceVector.at(offset));
    offset += 1;
  }
  else {
    makeError(sourceVector.back().lineNumber, sourceVector.back().columnNumber,
      "dangling hyphen"
      , __LINE__
      );
    return TOKEN_TYPE_UNDEFINED;
  }

  return returnType;
}

HaifuToken TokenGenerator::combineVariableTokens(const vector<HaifuToken>& sourceVector) {
  string resultName;
  char resultElement;
  const HaifuToken* firstToken;

  if (sourceVector.empty()) {
    return HaifuToken();
  }

  firstToken = &sourceVector.front();
  resultName = firstToken->name;
  resultElement = firstToken->element;

  for (int i = 1; i < (int)sourceVector.size(); i++) {
    resultName += "-" + sourceVector.at(i).name;
    if (sourceVector.at(i).element != resultElement) {
      resultElement = ELEM_EARTH;
    }
  }

  return HaifuToken(firstToken->lineNumber, firstToken->columnNumber, resultName
    , TOKEN_TYPE_VARIABLE, TOKEN_VALUE_DEFAULT, resultElement
    );
}

HaifuToken TokenGenerator::combineNumberTokens(const vector<HaifuToken>& sourceVector) {
  vector<HaifuToken> tokenVector_current;
  vector<int> magnitudeVector_current;

  vector<HaifuToken> tokenVector_next;
  vector<int> magnitudeVector_next;

  int size;

  HaifuToken token;

  int sum;
  int product;
  int magnitude;
  string name;

  size = (int)sourceVector.size();

  tokenVector_current.reserve(size);
  magnitudeVector_current.reserve(size);

  tokenVector_next.reserve(size);
  magnitudeVector_next.reserve(size);

  for (int i = 0; i < size; i++) {
    tokenVector_current.push_back(sourceVector[i]);
    magnitudeVector_current.push_back(log10_int(sourceVector[i].value));
  }

  for (int i = 0; i < size; i++) {
    if (magnitudeVector_current[i] < 2 && i + 1 < size) {
      token = tokenVector_current[i];

      sum = tokenVector_current[i].value;
      magnitude = magnitudeVector_current[i];
      name = tokenVector_current[i].name;
      if (magnitudeVector_current[i] > magnitudeVector_current[i + 1] && tokenVector_current[i].value % 10 == 0) {
        sum += tokenVector_current[i + 1].value;
        name += "-" + tokenVector_current[i + 1].name;
        i++;
      }
      else if (magnitudeVector_current[i] == magnitudeVector_current[i + 1] || tokenVector_current[i].value % 10 != 0) {
        makeError(tokenVector_current[i + 1].lineNumber, tokenVector_current[i + 1].lineNumber
          , "cannot combine number \"" + tokenVector_current[i].name + "\" with number \"" + tokenVector_current[i + 1].name + "\""
          , __LINE__
          );
        return tokenVector_current[i];
      }

      token.value = sum;
      token.name = name;
      tokenVector_next.push_back(token);

      magnitudeVector_next.push_back(magnitude);
    }
    else {
      tokenVector_next.push_back(tokenVector_current[i]);
      magnitudeVector_next.push_back(magnitudeVector_current[i]);
    }
  }

  size = (int)tokenVector_next.size();
  tokenVector_current.swap(tokenVector_next);
  magnitudeVector_current.swap(magnitudeVector_next);

  tokenVector_next.clear();
  magnitudeVector_next.clear();

  for (int i = 0; i < size; i++) {
    if (magnitudeVector_current[i] < 2 && i + 1 < size) {
      token = tokenVector_current[i];

      product = tokenVector_current[i].value;
      magnitude = magnitudeVector_current[i];
      name = tokenVector_current[i].name;
      if (magnitudeVector_current[i + 1] == 2) {
        product *= tokenVector_current[i + 1].value;
        magnitude = magnitudeVector_current[i + 1];
        name += "-" + tokenVector_current[i + 1].name;
        i++;
      }
      else if (magnitudeVector_current[i + 1] < 2) {
        makeError(tokenVector_current[i + 1].lineNumber, tokenVector_current[i + 1].lineNumber
          , "cannot combine number \"" + tokenVector_current[i].name + "\" with number \"" + tokenVector_current[i + 1].name + "\""
          , __LINE__
          );
        return tokenVector_current[i];
      }

      token.value = product;
      token.name = name;
      tokenVector_next.push_back(token);

      magnitudeVector_next.push_back(magnitude);
    }
    else {
      tokenVector_next.push_back(tokenVector_current[i]);
      magnitudeVector_next.push_back(magnitudeVector_current[i]);
    }
  }

  size = (int)tokenVector_next.size();
  tokenVector_current.swap(tokenVector_next);
  magnitudeVector_current.swap(magnitudeVector_next);

  tokenVector_next.clear();
  magnitudeVector_next.clear();

  for (int i = 0; i < size; i++) {
    if (magnitudeVector_current[i] == 2 && i + 1 < size) {
      token = tokenVector_current[i];

      sum = tokenVector_current[i].value;
      magnitude = magnitudeVector_current[i];
      name = tokenVector_current[i].name;
      if (magnitudeVector_current[i] > magnitudeVector_current[i + 1]) {
        sum += tokenVector_current[i + 1].value;
        name += "-" + tokenVector_current[i + 1].name;
        i++;
      }
      else if (magnitudeVector_current[i] == magnitudeVector_current[i + 1]) {
        makeError(tokenVector_current[i + 1].lineNumber, tokenVector_current[i + 1].lineNumber
          , "cannot combine number \"" + tokenVector_current[i].name + "\" with number \"" + tokenVector_current[i + 1].name + "\""
          , __LINE__
          );
        return tokenVector_current[i];
      }

      token.value = sum;
      token.name = name;
      tokenVector_next.push_back(token);

      magnitudeVector_next.push_back(magnitude);
    }
    else {
      tokenVector_next.push_back(tokenVector_current[i]);
      magnitudeVector_next.push_back(magnitudeVector_current[i]);
    }
  }

  size = (int)tokenVector_next.size();

  tokenVector_current.swap(tokenVector_next);
  magnitudeVector_current.swap(magnitudeVector_next);

  tokenVector_next.clear();
  magnitudeVector_next.clear();

  for (int i = 0; i < size; i++) {
    token = tokenVector_current[i];

    product = tokenVector_current[i].value;
    magnitude = magnitudeVector_current[i];
    name = tokenVector_current[i].name;
    while (i + 1 < size) {
      if (magnitudeVector_current[i] < magnitudeVector_current[i + 1]) {
        product *= tokenVector_current[i + 1].value;
        magnitude += magnitudeVector_current[i + 1];
        name += "-" + tokenVector_current[i + 1].name;
        i++;
      }
      else if (magnitudeVector_current[i] == magnitudeVector_current[i + 1]) {
        makeError(tokenVector_current[i + 1].lineNumber, tokenVector_current[i + 1].lineNumber
          , "cannot combine number \"" + tokenVector_current[i].name + "\" with number \"" + tokenVector_current[i + 1].name + "\""
          , __LINE__
          );
        return tokenVector_current[i];
        break;
      }
      else {
        break;
      }
    }

    token.value = product;
    token.name = name;
    tokenVector_next.push_back(token);

    magnitudeVector_next.push_back(magnitude);
  }

  size = (int)tokenVector_next.size();

  tokenVector_current.swap(tokenVector_next);
  magnitudeVector_current.swap(magnitudeVector_next);

  tokenVector_next.clear();
  magnitudeVector_next.clear();

  for (int i = 0; i < size; i++) {
    token = tokenVector_current[i];

    sum = tokenVector_current[i].value;
    magnitude = magnitudeVector_current[i];
    name = tokenVector_current[i].name;
    while (i + 1 < size) {
      if (magnitudeVector_current[i] > magnitudeVector_current[i + 1]) {
        sum += tokenVector_current[i + 1].value;
        name += "-" + tokenVector_current[i + 1].name;
        i++;
      }
      else if (magnitudeVector_current[i] == magnitudeVector_current[i + 1]) {
        makeError(tokenVector_current[i + 1].lineNumber, tokenVector_current[i + 1].lineNumber
          , "cannot combine number \"" + tokenVector_current[i].name + "\" with number \"" + tokenVector_current[i + 1].name + "\""
          , __LINE__
          );
        return tokenVector_current[i];
        //break;
      }
      else {
        break;
      }
    }

    token.value = sum;
    token.name = name;
    tokenVector_next.push_back(token);

    magnitudeVector_next.push_back(magnitude);
  }

  if (tokenVector_next.size() > 1) {
    makeError(tokenVector_next[1].lineNumber, tokenVector_next[1].lineNumber
      , "cannot combine number \"" + tokenVector_next[0].name + "\" with number \"" + tokenVector_next[1].name + "\""
      , __LINE__
      );
  }

  return tokenVector_next[0];
}

HaifuToken TokenGenerator::combineNumberTokens_multiply(const vector<HaifuToken>& sourceVector, int& offset) {
  string resultName;
  int resultValue;
  int resultValue_log10;
  int currentValue;
  int currentValue_log10;
  const HaifuToken* firstToken;

  bool multipliedByOverHundred = false;

  if (sourceVector.empty() || offset < 0 || offset >= (int)sourceVector.size()) {
    return HaifuToken();
  }

  firstToken = &sourceVector.at(offset);
  resultName = firstToken->name;
  resultValue = firstToken->value;

  if (resultName == "dozen" || resultName == "score") {
    if (offset + 1 < (int)sourceVector.size()
      && (sourceVector.at(offset + 1).name == "dozen"
        || sourceVector.at(offset + 1).name == "score"
        )
      )
    {
      // program reads something like "dozen-dozen"
      makeError(sourceVector.at(offset + 1).lineNumber, sourceVector.at(offset + 1).columnNumber
        , "improper number token \"" + sourceVector.at(offset).name + "\""
        , __LINE__
        );
    }
  }

  for (offset += 1; offset < (int)sourceVector.size(); offset++) {
    resultValue_log10 = log10_int(resultValue);

    currentValue = sourceVector.at(offset).value;
    currentValue_log10 = log10_int(currentValue);

    if (currentValue_log10 > 2) {
      if (multipliedByOverHundred) {
        // program reads something like "thousand-million"
        makeError(sourceVector.at(offset).lineNumber, sourceVector.at(offset).columnNumber
          , "improper number token \"" + sourceVector.at(offset).name + "\""
          , __LINE__
          );
      }
      else {
        multipliedByOverHundred = true;
      }
    }
    else if (sourceVector.at(offset).name == "dozen" || sourceVector.at(offset).name == "score") {
      resultName += "-" + sourceVector.at(offset).name;

      // overflow control
      if (INT_MAX / currentValue <= resultValue) {
        resultValue = INT_MAX;
      }
      else {
        resultValue *= currentValue;
      }

      if (offset + 2 == (int)sourceVector.size() && sourceVector.back().value < currentValue) {
        if (sourceVector.back().name == "dozen" || sourceVector.back().name == "score") {
          makeError(sourceVector.at(offset + 1).lineNumber, sourceVector.at(offset + 1).columnNumber
            , "improper number token \"" + sourceVector.at(offset + 1).name + "\""
            , __LINE__
            );
        }
        offset += 1;
        break;
      }
      else if (offset + 1 < (int)sourceVector.size()) {
        makeError(sourceVector.at(offset + 1).lineNumber, sourceVector.at(offset + 1).columnNumber
          , "improper number token \"" + sourceVector.at(offset + 1).name + "\""
          , __LINE__
          );
      }
      else {
        offset += 1;
        break;
      }

    }
    else if (currentValue_log10 < 2 && resultValue_log10 < 1) {
      // program reads something like "one-ten"
      makeError(sourceVector.at(offset).lineNumber, sourceVector.at(offset).columnNumber
        , "improper number token \"" + sourceVector.at(offset).name + "\""
        , __LINE__
        );
    }

    if (resultValue_log10 < currentValue_log10) {
      resultName += "-" + sourceVector.at(offset).name;

      // overflow control
      if (INT_MAX / currentValue <= resultValue) {
        resultValue = INT_MAX;
      }
      else {
        resultValue *= currentValue;
      }
    }
    else if (resultValue_log10 > currentValue_log10) {
      break;
    }
    else {
      // program reads something like "ten-ten"
      makeError(sourceVector.at(offset).lineNumber, sourceVector.at(offset).columnNumber
        , "improper number token \"" + sourceVector.at(offset).name + "\""
        , __LINE__
        );

      return HaifuToken(firstToken->lineNumber, firstToken->columnNumber);
    }
  }

  return HaifuToken(firstToken->lineNumber, firstToken->columnNumber, resultName
    , TOKEN_TYPE_NUMBER, resultValue, ELEM_EARTH
    );
}
HaifuToken TokenGenerator::combineNumberTokens_add(const vector<HaifuToken>& sourceVector, int& offset) {
  string resultName;
  int resultValue;
  int resultValue_log10;
  int currentValue;
  int currentValue_log10;
  const HaifuToken* firstToken;

  if (sourceVector.empty() || offset < 0 || offset >= (int)sourceVector.size()) {
    return HaifuToken();
  }

  firstToken = &sourceVector.at(offset);
  resultName = firstToken->name;
  resultValue = firstToken->value;

  if (resultValue % 100 == 0) {
    // program reads something like "thousand-hundred"
    makeError(sourceVector.at(offset).lineNumber, sourceVector.at(offset).columnNumber
      , "improper number token \"" + sourceVector.at(offset).name + "\""
      , __LINE__
      );
  }

  for (offset += 1; offset < (int)sourceVector.size(); offset++) {
    resultValue_log10 = log10_int(resultValue);

    currentValue = sourceVector.at(offset).value;
    currentValue_log10 = log10_int(currentValue);

    if (resultValue % 100 == 10) {
      // program reads something like "ten-two"
      makeError(sourceVector.at(offset).lineNumber, sourceVector.at(offset).columnNumber
        , "improper number token \"" + sourceVector.at(offset).name + "\""
        , __LINE__
        );
    }

    if (resultValue_log10 < currentValue_log10) {
      break;
    }
    else if (resultValue_log10 > currentValue_log10) {
      resultName += "-" + sourceVector.at(offset).name;

      // overflow control
      if (INT_MAX - currentValue <= resultValue) {
        resultValue = INT_MAX;
      }
      else {
        resultValue += currentValue;
      }
    }
    // ==
    else {
      // program reads something like "ten-ten"
      makeError(sourceVector.at(offset).lineNumber, sourceVector.at(offset).columnNumber
        , "improper number token \"" + sourceVector.at(offset).name + "\""
        , __LINE__
        );

      return HaifuToken(firstToken->lineNumber, firstToken->columnNumber);
    }
  }

  return HaifuToken(firstToken->lineNumber, firstToken->columnNumber, resultName
    , TOKEN_TYPE_NUMBER, resultValue, ELEM_EARTH
    );
}

void TokenGenerator::makeWarning(const int lineNumber, const int columnNumber, const std::string& message, const int debugLineNumber) {
  s_tokenWarnings.push_back(HaifuTokenError(lineNumber, columnNumber, message, debugLineNumber));
}
void TokenGenerator::makeError(const int lineNumber, const int columnNumber, const std::string& message, const int debugLineNumber) {
  s_tokenErrors.push_back(HaifuTokenError(lineNumber, columnNumber, message, debugLineNumber));
}

void TokenGenerator::clearWarnings() {
  s_tokenWarnings.clear();
}
void TokenGenerator::clearErrors() {
  s_tokenErrors.clear();
}