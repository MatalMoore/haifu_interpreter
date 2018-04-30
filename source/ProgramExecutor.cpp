#include "ProgramExecutor.h"

#include <cmath>

using namespace std;

vector<Rung> ProgramExecutor::s_program;
int ProgramExecutor::s_bureaucrat;
int ProgramExecutor::s_delegate;
int ProgramExecutor::s_inputCounter;
int ProgramExecutor::s_executionCounter;

bool ProgramExecutor::s_wasBureaucratChanged;
bool ProgramExecutor::s_areExecutionsDumped = false;
bool ProgramExecutor::s_areVariableExecutionsDumped = false;

map<string, Variable> ProgramExecutor::s_variables;

ofstream ProgramExecutor::logFileStream;

static Variable DNE_variable = Variable();

std::string rungTypeToString(const char rungType) {
  switch (rungType) {
  case RUNG_TYPE_COMMAND:
    return "COMMAND";
  case RUNG_TYPE_VARIABLE:
    return "VARIABLE";
  case RUNG_TYPE_LITERAL:
    return "LITERAL";
  case RUNG_TYPE_PUNCTUATION:
    return "PUNCTUATION";
  default:
    return "INVALID_TYPE";
  }
}
string rungCommandToString(const double rungValue) {
  switch ((int)rungValue) {
  case RESERVED_WORD_SOME:
    return "SOME";
  case RESERVED_WORD_MANY:
    return "MANY";
  case RESERVED_WORD_HEAVEN:
    return "HEAVEN";
  case RESERVED_WORD_PROMOTE:
    return "PROMOTE";
  case RESERVED_WORD_DEMOTE:
    return "DEMOTE";
  case RESERVED_WORD_BLOSSOM:
    return "BLOSSOM";
  case RESERVED_WORD_RISE:
    return "RISE";
  case RESERVED_WORD_FALL:
    return "FALL";
  case RESERVED_WORD_LISTEN:
    return "LISTEN";
  case RESERVED_WORD_SPEAK:
    return "SPEAK";
  case RESERVED_WORD_COUNT:
    return "COUNT";
  case RESERVED_WORD_CREATE:
    return "CREATE";
  case RESERVED_WORD_DESTROY:
    return "DESTROY";
  case RESERVED_WORD_FEAR:
    return "FEAR";
  case RESERVED_WORD_LOVE:
    return "LOVE";
  case RESERVED_WORD_BECOME:
    return "BECOME";
  case RESERVED_WORD_LIKE:
    return "LIKE";
  case RESERVED_WORD_TOMORROW:
    return "TOMORROW";
  case RESERVED_WORD_NEGATIVE:
    return "NEGATIVE";
  case RESERVED_WORD_OPERATE:
    return "OPERATE";
  default:
    return "INVALID_COMMAND";
  }
}

void ProgramExecutor::loadProgram(const vector<HaifuToken>& tokens) {
  std::vector<Rung> program_new;

  s_program.clear();
  s_variables.clear();

  for (int i = 0; i < (int)tokens.size(); i++) {
    appendRung_token(tokens[i]);
  }

  reverse(s_program.begin(), s_program.end());
}

void ProgramExecutor::executeProgram(istream& input) {
  s_bureaucrat = 0;
  s_delegate = 0;
  s_inputCounter = 0;
  s_executionCounter = 0;
  s_variables.clear();

  output("Starting execution...\n");

  if (s_areExecutionsDumped) {
    logFileStream.open(EXECUTION_DUMP_FILE_STRING);
    if (logFileStream.fail()) {
      logFileStream.close();
    }
  }

  while (s_bureaucrat < (int)s_program.size()) {
    s_wasBureaucratChanged = false;

    // if a quit condition is returned
    if (executeRung(s_program[s_bureaucrat], input)) {
      break;
    }

    if (!s_wasBureaucratChanged) {
      s_bureaucrat += 1;
    }
  }

  output(" ");
  output(" ");
  output("Done.");

  if (s_areExecutionsDumped) {
    logFileStream << " " << endl;
    logFileStream << OUTPUT_LINE_STRING << endl;
    logFileStream << "Termination:" << endl;
    logFileStream << OUTPUT_LINE_STRING << endl;
    outputProgram(logFileStream);

    logFileStream << " " << endl;
    logFileStream << OUTPUT_LINE_STRING << endl;
    logFileStream << "Variables at termination:" << endl;
    logFileStream << OUTPUT_LINE_STRING << endl;
    outputVariables(logFileStream);
    logFileStream << endl;

    logFileStream.close();
  }
}

int ProgramExecutor::toggleExecutionDump() {
  if (s_areExecutionsDumped) {
    if (s_areVariableExecutionsDumped) {
      s_areVariableExecutionsDumped = false;
      s_areExecutionsDumped = false;
      return DUMP_NO_EXECUTIONS;
    }
    else {
      s_areVariableExecutionsDumped = true;
      return DUMP_ALL_EXECUTIONS;
    }
  }
  else {
    s_areExecutionsDumped = true;
    s_areVariableExecutionsDumped = false;
    return DUMP_NON_VARIABLE_EXECUTIONS;
  }
}

void ProgramExecutor::output(const std::string& value) {
  if (!value.empty()) {
    cout << value << endl;
  }
}

void ProgramExecutor::appendRung_token(const HaifuToken& token) {
  switch (token.type) {
  case TOKEN_TYPE_RESERVED_WORD:
    s_program.push_back(
      Rung(
        token.lineNumber
        , token.columnNumber
        , token.name
        , RUNG_TYPE_COMMAND
        , token.value
        , RUNG_ELEMENT_DEFAULT
        )
      )
      ;
    break;
  case TOKEN_TYPE_VARIABLE:
    s_program.push_back(
      Rung(
        token.lineNumber
        , token.columnNumber
        , token.name
        , RUNG_TYPE_VARIABLE
        , RUNG_VALUE_DEFAULT
        , token.element
        )
      )
      ;
    break;
  case TOKEN_TYPE_NUMBER:
    s_program.push_back(
      Rung(
        token.lineNumber
        , token.columnNumber
        , token.name
        , RUNG_TYPE_LITERAL
        , token.value
        , ELEM_EARTH
        )
      )
      ;
    break;
  case TOKEN_TYPE_PUNCTUATION:
    s_program.push_back(
      Rung(
        token.lineNumber
        , token.columnNumber
        , token.name
        , RUNG_TYPE_PUNCTUATION
        , RUNG_VALUE_DEFAULT
        , RUNG_ELEMENT_DEFAULT
        )
      )
      ;
    break;
  default:
    cout << "Warning: token \"" << token.name << "\" has an unexpected rung type" << endl;
  }
}

void ProgramExecutor::insertRung(const Rung& rung, const int index) {
  if (index < 0 || index >= (int)s_program.size() + 1) {
    return;
  }

  if (index <= s_bureaucrat) {
    s_bureaucrat++;
  }
  if (index <= s_delegate) {
    s_delegate++;
  }

  s_program.insert(s_program.begin() + index, rung);
}
void ProgramExecutor::removeRung(const int index) {
  if (index < 0 || index >= (int)s_program.size()) {
    return;
  }

  if (index < s_bureaucrat) {
    s_bureaucrat--;
  }
  if (index < s_delegate) {
    s_delegate--;
  }

  s_program.erase(s_program.begin() + index);
}

bool ProgramExecutor::executeRung(const Rung& rung, istream& input, const string& command_variable, const int index_command) {
  bool shouldTerminate;

  if (s_areExecutionsDumped) {
    if (index_command < 0) {
      logFileStream << endl;
      logFileStream << OUTPUT_LINE_STRING << endl;
      logFileStream << "Execution " << s_executionCounter << ":" << endl;
      logFileStream << OUTPUT_LINE_STRING << endl;
      outputProgram(logFileStream);

      logFileStream << endl;
      logFileStream << OUTPUT_LINE_STRING << endl;
      logFileStream << "Variables at execution " << s_executionCounter << ":" << endl;
      logFileStream << OUTPUT_LINE_STRING << endl;
      outputVariables(logFileStream, command_variable, index_command);
    }
    else if (s_areVariableExecutionsDumped) {
      logFileStream << endl;
      logFileStream << OUTPUT_LINE_STRING << endl;
      logFileStream << "Execution " << s_executionCounter << ", Execution of \"" << command_variable << "\":" << endl;
      logFileStream << OUTPUT_LINE_STRING << endl;
      outputProgram(logFileStream);

      logFileStream << endl;
      logFileStream << OUTPUT_LINE_STRING << endl;
      logFileStream << "Variables at execution " << s_executionCounter << ", Execution of \"" << command_variable << "\":" << endl;
      logFileStream << OUTPUT_LINE_STRING << endl;
      outputVariables(logFileStream, command_variable, index_command);
    }
  }

  s_executionCounter++;

  switch (rung.type) {
  case RUNG_TYPE_COMMAND:
    shouldTerminate = executeRung_command(rung, input);
    break;
  case RUNG_TYPE_VARIABLE:
    shouldTerminate = executeRung_variable(rung.name, input);
    break;
  case RUNG_TYPE_LITERAL:
    shouldTerminate = false;
    break;
  case RUNG_TYPE_PUNCTUATION:
    shouldTerminate = executeRung_punctuation();
    break;
  default:
    cout << "Warning: unknown command \"" << rung.name << "\"" << endl;
    shouldTerminate = false;
    break;
  }

  return shouldTerminate;
}
bool ProgramExecutor::executeRung_variable(const string& rungName, istream& input) {
  map<string, Variable>::iterator iter;
  const vector<Rung>* commands;
  const string variableName = getVariableName(rungName);

  iter = s_variables.find(variableName);

  if (iter != s_variables.end() && iter->second.isCommand) {
    commands = &iter->second.commands;
    for (int i = 0; i < (int)commands->size(); i++) {
      // if rung should indicate program termination
      if (executeRung(commands->at(i), input, variableName, i)) {
        // return and indicate program termination
        return true;
      }
    }
  }

  return false;
}
bool ProgramExecutor::executeRung_punctuation() {
  const Rung* rung_named;
  const Rung* rung_current;
  vector<Rung> commands;
  Variable* variable;

  if (s_bureaucrat + 1 >= (int)s_program.size()) {
    cout << "Warning: punctuation at end of program" << endl;
    return false;
  }
  else {
    rung_named = &s_program[s_bureaucrat + 1];
    for (s_bureaucrat += 2; s_bureaucrat < (int)s_program.size(); s_bureaucrat++) {
      rung_current = &s_program[s_bureaucrat];
      // end of command sequence
      if (rung_current->type == RUNG_TYPE_PUNCTUATION
        || haveSameName(*rung_named, *rung_current)
        )
      {
        if (rung_named->type == RUNG_TYPE_VARIABLE) {
          variable = getVariable(rung_named->name);
          variable->isCommand = true;
          variable->value = 0.0;
          // initialize element of variable
          if (variable->element == ELEM_NONE) {
            variable->element = rung_named->element;
          }
          variable->commands = commands;
        }
        else {
          cout << "Warning: command sequence cannot be stored as non-variable \""
            << rung_named->name << "\"" << endl;
        }
        return false;
      }
      // append variableCommands
      else {
        commands.push_back(*rung_current);
      }
    }
  }

  cout << "Warning: end of program was reached before command sequence ended" << endl;
  return true;
}
bool ProgramExecutor::executeRung_command(const Rung& rung, istream& input) {
  switch ((int)rung.value) {
  case RESERVED_WORD_SOME:
    return false;
  case RESERVED_WORD_MANY:
    return false;
  case RESERVED_WORD_HEAVEN:
    return command_heaven();
  case RESERVED_WORD_PROMOTE:
    return command_promote();
  case RESERVED_WORD_DEMOTE:
    return command_demote();
  case RESERVED_WORD_BLOSSOM:
    return command_blossom();
  case RESERVED_WORD_RISE:
    return command_rise();
  case RESERVED_WORD_FALL:
    return command_fall();
  case RESERVED_WORD_LISTEN:
    return command_listen(input);
  case RESERVED_WORD_SPEAK:
    return command_speak();
  case RESERVED_WORD_COUNT:
    return command_count();
  case RESERVED_WORD_CREATE:
    return command_create();
  case RESERVED_WORD_DESTROY:
    return command_destroy();
  case RESERVED_WORD_FEAR:
    return command_fear();
  case RESERVED_WORD_LOVE:
    return command_love();
  case RESERVED_WORD_BECOME:
    return command_become();
  case RESERVED_WORD_LIKE:
    return command_like();
  case RESERVED_WORD_TOMORROW:
    return command_tomorrow();
  case RESERVED_WORD_NEGATIVE:
    return command_negative();
  case RESERVED_WORD_OPERATE:
    return command_operate();
  default:
    cout << "Warning: command \"" << rung.name << "\" cannot be executed" << endl;
    return false;
  }
}

bool ProgramExecutor::isNumeric_store(const Rung& rung, double& value) {
  const Variable* variable;
  double commandRungValue;

  switch (rung.type) {
  case RUNG_TYPE_COMMAND:
    commandRungValue = getCommandValue((int)rung.value);
    if (commandRungValue == RUNG_COMMAND_VALUE_DEFAULT) {
      return false;
    }
    else {
      value = commandRungValue;
      return true;
    }
  case RUNG_TYPE_VARIABLE:
    variable = getExistingVariable(rung.name);
    if (variable == &DNE_variable || variable->isCommand) {
      value = RUNG_VALUE_DEFAULT;
      return false;
    }
    else {
      value = variable->value;
      return true;
    }
  case RUNG_TYPE_LITERAL:
    value = rung.value;
    return true;
  case RUNG_TYPE_PUNCTUATION:
    value = RUNG_VALUE_DEFAULT;
    return false;
  default:
    value = RUNG_VALUE_DEFAULT;
    return false;
  }
}
bool ProgramExecutor::isNumeric_store(const int programIndex, double& value) {
  const Rung* rung = &s_program[programIndex];
  const Variable* variable;
  double commandRungValue;

  switch (rung->type) {
  case RUNG_TYPE_COMMAND:
    commandRungValue = getCommandValue((int)rung->value);
    if (commandRungValue == RUNG_COMMAND_VALUE_DEFAULT) {
      return false;
    }
    else {
      value = commandRungValue;
      return true;
    }
  case RUNG_TYPE_VARIABLE:
    variable = getExistingVariable(rung->name);
    if (variable == &DNE_variable || variable->isCommand) {
      value = RUNG_VALUE_DEFAULT;
      return false;
    }
    else {
      value = variable->value;
      return true;
    }
  case RUNG_TYPE_LITERAL:
    value = rung->value;
    return true;
  case RUNG_TYPE_PUNCTUATION:
    value = RUNG_VALUE_DEFAULT;
    return false;
  default:
    value = RUNG_VALUE_DEFAULT;
    return false;
  }
}

bool ProgramExecutor::haveSameName(const Rung& rung0, const Rung& rung1) {
  return
    // commands with the same code
    (rung0.type == RUNG_TYPE_VARIABLE && rung1.type == RUNG_TYPE_VARIABLE
      && getVariableName(rung0.name) == getVariableName(rung1.name)
      )
    // commands with the same code
    || (rung0.type == RUNG_TYPE_COMMAND
      && rung1.type == RUNG_TYPE_COMMAND
      && rung0.value == rung1.value
      )
    // literals with the same value
    || (rung0.type == RUNG_TYPE_LITERAL
      && rung1.type == RUNG_TYPE_LITERAL
      && rung0.value == rung1.value
      )
    ;
}

char ProgramExecutor::yin_yang(const double value) {
  return (int)round_away(value) % 2 == 0 ? YIN : YANG;
}

double ProgramExecutor::getCommandValue(const int commandCode) {
  switch (commandCode) {
  case RESERVED_WORD_SOME:
    return rand() % RAND_MAX_SOME + 1;
  case RESERVED_WORD_MANY:
    return rand() % (RAND_MAX_MANY - RAND_MAX_SOME) + RAND_MAX_SOME + 1;
  default:
    return RUNG_VALUE_DEFAULT;
  }
}

char ProgramExecutor::getRungElement(const Rung& rung) {
  Variable* variable;

  if (rung.type == RUNG_TYPE_VARIABLE) {
    variable = getExistingVariable(rung.name);
    if (variable == &DNE_variable) {
      return rung.element;
    }
    else {
      return variable->element;
    }
  }
  else {
    return rung.element;
  }
}

Variable* ProgramExecutor::getVariable(const std::string& rungName) {
  return &s_variables[getVariableName(rungName)];
}

Variable* ProgramExecutor::getExistingVariable(const std::string& rungName) {
  map<string, Variable>::iterator iter;
  iter = s_variables.find(getVariableName(rungName));
  if (iter == s_variables.end()) {
    return &DNE_variable;
  }

  return &iter->second;
}

const string& ProgramExecutor::getVariableName(const string& rungName) {
  const string& baseWordString = $WD::lookup(rungName).baseWord;
  if (baseWordString == BASE_WORD_DEFAULT || baseWordString == BASE_WORD_IDENTITY) {
    return rungName;
  }
  else {
    return baseWordString;
  }
}

void ProgramExecutor::assignRungValue(Rung& rung, const double value) {
  Variable* variable;

  switch (rung.type) {
  case RUNG_TYPE_VARIABLE:
    variable = getVariable(rung.name);
    variable->isCommand = false;
    variable->value = value;
    // initializes the element of the variable to that of the word
    if (variable->element == ELEM_NONE) {
      variable->element = ELEM_EARTH;
    }
    else {
      variable->element = rung.element;
    }
    variable->commands.clear();
    break;
  case RUNG_TYPE_LITERAL:
    rung.value = value;
    break;
  default:
    ;
  }
}

bool ProgramExecutor::areOperatorsInRange() {
  if (s_delegate + 1 >= (int)s_program.size()) {
    //TODO: make error
    return false;
  }
  else {
    return true;
  }
}

bool ProgramExecutor::command_heaven() {
  return true;
}
bool ProgramExecutor::command_promote() {
  double value;

  if (isNumeric_store(s_delegate, value)) {
    if (value == 0.0) {
      return false;
    }
    s_wasBureaucratChanged = true;

    if (value < 0) {
      cout << "Warning: Bureaucrat promoted by a negative value" << endl;
    }

    s_bureaucrat += (int)round_away(value);

    if (s_bureaucrat >= (int)s_program.size()) {
      cout << "Warning: Bureaucrat promoted above the program" << endl;
      s_bureaucrat = (int)s_program.size();
      return true;
    }
    else if (s_bureaucrat < 0) {
      cout << "Warning: Bureaucrat promoted below the program" << endl;
      s_bureaucrat = 0;
    }

    if (s_delegate > s_bureaucrat) {
      s_delegate = s_bureaucrat;
    }
  }

  return false;
}
bool ProgramExecutor::command_demote() {
  double value;

  if (isNumeric_store(s_delegate, value)) {
    if (value == 0.0) {
      return false;
    }
    s_wasBureaucratChanged = true;

    if (value < 0) {
      cout << "Warning: Bureaucrat demoted by a negative value" << endl;
    }

    s_bureaucrat -= (int)round_away(value);

    if (s_bureaucrat >= (int)s_program.size()) {
      cout << "Warning: Bureaucrat demoted above the program" << endl;
      s_bureaucrat = (int)s_program.size();
      return true;
    }
    else if (s_bureaucrat < 0) {
      cout << "Warning: Bureaucrat demoted below the program" << endl;
      s_bureaucrat = 0;
    }

    if (s_delegate > s_bureaucrat) {
      s_delegate = s_bureaucrat;
    }
  }

  return false;
}
bool ProgramExecutor::command_blossom() {
  double value;

  if (isNumeric_store(s_delegate, value)) {
    value = round_away(value);
    if (yin_yang((int)value) == YIN) {
      s_bureaucrat += (int)value;
    }
    else {
      s_bureaucrat -= (int)value;
    }

    if (s_bureaucrat >= (int)s_program.size()) {
      cout << "Warning: Bureaucrat blossomed to above the program" << endl;
      s_bureaucrat = (int)s_program.size();
      return true;
    }
    else if (s_bureaucrat < 0) {
      cout << "Warning: Bureaucrat blossomed to below the program" << endl;
      s_bureaucrat = 0;
    }

    if (s_delegate > s_bureaucrat) {
      s_delegate = s_bureaucrat;
    }
  }

  return false;
}
bool ProgramExecutor::command_rise() {
  double value;
  if (s_bureaucrat == 0) {
    value = 1.0;
  }
  else if (!isNumeric_store(s_bureaucrat - 1, value)) {
    value = 1.0;
  }

  value = round_away(value);

  if (value < 0.0) {
    cout << "Warning: Delegate rose by a negative value" << endl;
  }

  s_delegate += (int)value;

  if (s_delegate < 0) {
    cout << "Warning: Delegate rose to below the program" << endl;
    s_delegate = 0;
  }

  if (s_delegate > s_bureaucrat) {
    cout << "Warning: Delegate rose to above the delegate" << endl;
    s_delegate = s_bureaucrat;
  }

  return false;
}
bool ProgramExecutor::command_fall() {
  double value;
  if (s_bureaucrat == 0) {
    value = 1.0;
  }
  else if (!isNumeric_store(s_bureaucrat - 1, value)) {
    value = 1.0;
  }

  value = round_away(value);

  if (value < 0.0) {
    cout << "Warning: Delegate fell by a negative value" << endl;
  }

  s_delegate -= (int)value;

  if (s_delegate < 0) {
    cout << "Warning: Delegate fell to below the program" << endl;
    s_delegate = 0;
  }

  if (s_delegate > s_bureaucrat) {
    cout << "Warning: Delegate fell to above the delegate" << endl;
    s_delegate = s_bureaucrat;
  }

  return false;
}
bool ProgramExecutor::command_listen(std::istream& input) {
  bool isNumber;

  char input_char;
  char next_char;

  int input_int_whole;
  int input_int_fractional;
  int input_int_fractional_size = 0;

  double input_double;

  string input_string;
  string input_string_partial;
  string input_int_fractional_string;

  stringstream itoa_stream;

  Rung swappedRung;

  if (endOfStream(input)) {
    if (s_bureaucrat + 1 < (int)s_program.size()) {
      swappedRung = s_program[s_bureaucrat + 1];
      removeRung(s_bureaucrat + 1);
      insertRung(swappedRung, 0);
    }
  }
  else {
    // get the first character in case input is not a number, and backtrack
    input >> input_char;
    input.unget();

    // see if the first part of the stream is formatted like an int
    input >> input_int_whole;
    if (input.fail()) {
      // if not, input is not a number
      isNumber = false;
      // clear error flags
      input.clear();
      if (input_char != '+' && input_char != '-') {
        // backtrack in case the istream removed a '-'
        input.get();
      }
    }
    else {
      // the first part of the stream is formatted like an int,
      //   so input is a number
      isNumber = true;
      // see what the next character is
      next_char = input.get();
      if (next_char != '.') {
        // if it is not a '.', the input number does not have a fractional part
        input_int_fractional = 0;
        // backtrack past '.' since is not part of the input
        input.putback(next_char);
      }
      else {
        // the next character is '.',
        //   so the input number could have a fractional part
        next_char = input.peek();
        // check if the next peeked character is a numeral
        if (next_char >= '0' && next_char <= '9') {
          // if so, the number of characters in the fractional part relates to its value
          while (next_char >= '0' && next_char <= '9') {
            // keep track of all the numerals
            input_int_fractional_string.push_back(next_char);

            input.get();
            next_char = input.peek();
          }

          input_int_fractional_size = (int)input_int_fractional_string.size();

          // put back all the numerals
          for (int i = (int)input_int_fractional_string.size() - 1; i >= 0; i--) {
            input.putback(input_int_fractional_string[i]);
          }

          // stream out these numerals to an int
          input >> input_int_fractional;
        }
        // the next peeked character is not a numeral
        else {
          // so the input has no fractional part
          input_int_fractional = 0;
          // backtrack past the '.' since it is not part of the input
          input.putback('.');
        }
      }
    }

    // if the input is a number
    if (isNumber) {
      // convert whole part to string
      itoa_stream.clear();
      itoa_stream << input_int_whole;
      itoa_stream >> input_string;

      // convert whole part to double
      input_double = input_int_whole;

      if (input_int_fractional != 0) {
        // convert fractional part to string
        itoa_stream.clear();
        itoa_stream << input_int_fractional;
        itoa_stream >> input_string_partial;

        // append fractional string
        input_string.push_back('.');
        input_string += input_string_partial;

        // divide fractional part by 10^(its size) and combine it with the whole part
        if (input_double >= 0.0) {
          input_double += input_int_fractional / pow(10.0, (int)input_int_fractional_string.size());
        }
        else {
          input_double -= input_int_fractional / pow(10.0, (int)input_int_fractional_string.size());
        }
      }

      // insert the Rung at the beginning of the program
      insertRung(
        Rung(INPUT_LINE_NUMBER, s_inputCounter, input_string, RUNG_TYPE_LITERAL, input_double, ELEM_EARTH)
        , 0
        );

      // add 1 to the recorded number of input operations
      s_inputCounter++;
    }
    else {
      // set the string to contain only the input character
      input_string.push_back(input_char);
      // insert the Rung at the beginning of the program
      insertRung(
        Rung(INPUT_LINE_NUMBER, s_inputCounter, input_string, RUNG_TYPE_LITERAL, input_char, ELEM_EARTH)
        , 0
        );

      // add 1 to the recorded number of input operations
      s_inputCounter++;
    }
  }

  return false;
}
bool ProgramExecutor::command_speak() {
  double value;
  string valueString;

  if (isNumeric_store(s_delegate, value)) {
    valueString.push_back((char)(int)round_away(value));
    cout << valueString;

    if (s_areExecutionsDumped) {
      logFileStream << endl;
      logFileStream << "Output: \"" << valueString << "\"" << endl;
    }
  }

  return false;
}
bool ProgramExecutor::command_count() {
  double value;
  stringstream valueStream;
  string valueString;

  if (isNumeric_store(s_delegate, value)) {
    valueStream << value;
    valueStream >> valueString;
    cout << valueString;

    if (s_areExecutionsDumped) {
      logFileStream << endl;
      logFileStream << "Output: \"" << valueString << "\"" << endl;
    }
  }

  return false;
}
bool ProgramExecutor::command_create() {
  double value;
  Rung* rung;
  Variable* variable;

  rung = &s_program[s_delegate];
  if (isNumeric_store(*rung, value)) {
    switch (rung->type) {
    case RUNG_TYPE_VARIABLE:
      variable = getExistingVariable(rung->name);
      if (variable != &DNE_variable) {
        variable->element = progressElement_create(variable->element);
      }
      break;
    case RUNG_TYPE_LITERAL:
      rung->element = progressElement_create(rung->element);
      break;
    default:
      ;
    }
  }

  return false;
}
bool ProgramExecutor::command_destroy() {
  double value;
  Rung* rung;
  Variable* variable;

  rung = &s_program[s_delegate];
  if (isNumeric_store(*rung, value)) {
    switch (rung->type) {
    case RUNG_TYPE_VARIABLE:
      variable = getExistingVariable(rung->name);
      if (variable != &DNE_variable) {
        variable->element = progressElement_destroy(variable->element);
      }
      break;
    case RUNG_TYPE_LITERAL:
      rung->element = progressElement_destroy(rung->element);
      break;
    default:
      ;
    }
  }

  return false;
}
bool ProgramExecutor::command_fear() {
  double value;
  Rung* rung;
  Variable* variable;

  rung = &s_program[s_delegate];
  if (isNumeric_store(*rung, value)) {
    switch (rung->type) {
    case RUNG_TYPE_VARIABLE:
      variable = getExistingVariable(rung->name);
      if (variable != &DNE_variable) {
        variable->element = progressElement_fear(variable->element);
      }
      break;
    case RUNG_TYPE_LITERAL:
      rung->element = progressElement_fear(rung->element);
      break;
    default:
      ;
    }
  }

  return false;
}
bool ProgramExecutor::command_love() {
  double value;
  Rung* rung;
  Variable* variable;

  rung = &s_program[s_delegate];
  if (isNumeric_store(*rung, value)) {
    switch (rung->type) {
    case RUNG_TYPE_VARIABLE:
      variable = getExistingVariable(rung->name);
      if (variable != &DNE_variable) {
        variable->element = progressElement_love(variable->element);
      }
      break;
    case RUNG_TYPE_LITERAL:
      rung->element = progressElement_love(rung->element);
      break;
    default:
      ;
    }
  }

  return false;
}
bool ProgramExecutor::command_become() {
  double value;
  double value_new;
  Rung* rung;
  Variable* variable;

  map<string, Variable>::iterator iter;

  if (isNumeric_store(s_delegate, value)) {
    rung = &s_program[s_delegate];

    if (value == 0.0) {
      rung->type = RUNG_TYPE_COMMAND;
      rung->value = RESERVED_WORD_HEAVEN;
      rung->element = ELEM_EARTH;
    }
    else {
      value_new = round_away(value);
      if (value == value_new) {
        value = value < 0.0 ? value - 1.0 : value + 1.0;
      }

      switch (rung->type) {
      case RUNG_TYPE_VARIABLE:
        // variable already exists since it has a value
        variable = getVariable(rung->name);
        variable->value = value;
        variable->element = progressElement_create(variable->element);
        break;
      case RUNG_TYPE_LITERAL:
        rung->value = value;
        break;
      default:
        ;
      }
    }
  }

  return false;
}
bool ProgramExecutor::command_like() {
  double value;
  Rung* rung;
  bool foundNumber = false;

  if (s_bureaucrat - 1 < 0) {
    return false;
  }

  rung = &s_program[s_bureaucrat - 1];
  if (rung->type != RUNG_TYPE_VARIABLE) {
    return false;
  }

  for (int i = s_delegate; i >= 0; i--) {
    if (isNumeric_store(i, value)) {
      foundNumber = true;
      s_delegate = i;
      break;
    }
  }

  if (!foundNumber) {
    value = 0.0;
  }

  assignRungValue(*rung, value);

  return false;
}
bool ProgramExecutor::command_tomorrow() {
  // definition is not in the specification

  return false;
}
bool ProgramExecutor::command_negative() {
  double value;
  Rung* rung;

  rung = &s_program[s_delegate];

  if (isNumeric_store(*rung, value)) {
    assignRungValue(*rung, -value);
  }

  return false;
}
bool ProgramExecutor::command_operate() {
  if (!areOperatorsInRange()) {
    command_heaven();
  }

  double value_A;
  double value_B;

  char element_A;
  char element_B;

  Rung* rung_A;
  Rung* rung_B;

  if (s_delegate + 1 >= (int)s_program.size()) {
    return false;
  }

  rung_B = &s_program[s_delegate];
  command_heaven();
  if (!isNumeric_store(*rung_B, value_B)) {
    command_heaven();
    return false;
  }

  rung_A = &s_program[s_delegate + 1];
  if (!isNumeric_store(*rung_A, value_A)) {
    return false;
  }

  element_B = getRungElement(*rung_B);
  element_A = getRungElement(*rung_A);

  if (element_B == element_A) {
    if (yin_yang(value_A) == YANG && yin_yang(value_B) == YANG) {
      assignRungValue(*rung_B, YANG);
    }
    else {
      assignRungValue(*rung_B, YIN);
    }
  }
  else if (testElements_create(element_B, element_A)) {
    assignRungValue(*rung_B, value_A + value_B);
  }
  else if (testElements_destroy(element_B, element_A)) {
    assignRungValue(*rung_B, value_A - value_B);
  }
  else if (testElements_fear(element_B, element_A)) {
    assignRungValue(*rung_B, value_A / value_B);
  }
  else if (testElements_love(element_B, element_A)) {
    assignRungValue(*rung_B, value_A * value_B);
  }
  else {
    cout << "Warning: unknown element relation for operation" << endl;
  }

  return false;
}

void ProgramExecutor::outputRung(const Rung& rung, std::ostream& output, const string& indent) {
  map<string, Variable>::iterator iter;

  if (rung.lineNumber == INPUT_LINE_NUMBER) {
    output << "(INPUT, " << rung.columnNumber << ") ";
  }
  else {
    output << "(" << rung.lineNumber << ", " << rung.columnNumber << ") ";
  }
  output << "\"" << rung.name << "\" ";
  output << rungTypeToString(rung.type) << " ";
  switch (rung.type) {
  case RUNG_TYPE_COMMAND:
    output << rungCommandToString(rung.value) << " ";
    break;
  case RUNG_TYPE_VARIABLE:
    iter = s_variables.find(rung.name);
    if (iter == s_variables.end()) {
      output << "UNINITIALIZED";
    }
    else if (iter->second.isCommand) {
      output << "COMMAND";
    }
    else {
      output << "VALUE";
    }
    output << "_VARIABLE ";
    break;
  case RUNG_TYPE_LITERAL:
    output << rung.value << " ";
    break;
  case RUNG_TYPE_PUNCTUATION:
    output << rung.value << " ";
    break;
  default:
    break;
  }
  output << toElementString(rung.element);
}

void ProgramExecutor::outputProgram(ostream& output) {
  for (int i = (int)s_program.size()-1; i >= 0; i--) {
    // indicates position of delegate
    if (i == s_delegate) {
      output << "D>";
    }
    else {
      output << "  ";
    }

    // indicates position of bureaucrat
    if (i == s_bureaucrat) {
      output << "B>";
    }
    else {
      output << "  ";
    }

    output << i << ": ";

    outputRung(s_program[i], output);
    output << endl;
  }
}

void ProgramExecutor::outputVariable(const Variable& variable, ostream& output, const int index_command) {
  const vector<Rung>* variableCommands;

  if(variable.isCommand){
    output << "COMMAND_VARIABLE ";
    output << variable.value << " ";
    output << toElementString(variable.element);
    variableCommands = &variable.commands;
    for (int i = (int)variableCommands->size() - 1; i >= 0; i--) {
      output << endl;

      // indicates position of indexed command
      if (i == index_command) {
        output << "  ->" << i << ": ";
      }
      else {
        output << "    " << i << ": ";
      }

      outputRung((*variableCommands)[i], output);
    }
  }
  else {
    output << "VALUE_VARIABLE ";
    output << variable.value << " ";
    output << toElementString(variable.element);
  }
}

void ProgramExecutor::outputVariables(ostream& output, const string& command_variable, const int index_command) {
  map<string, Variable>::iterator iter;

  for (iter = s_variables.begin(); iter != s_variables.end(); iter++) {
    output << "  " << iter->first << ": ";
    if (iter->first == command_variable) {
      outputVariable(iter->second, output, index_command);
    }
    else {
      outputVariable(iter->second, output);
    }

    output << endl;
  }
}