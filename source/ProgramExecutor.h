#ifndef PROGRAM_EXECUTOR_H
#define PROGRAM_EXECUTOR_H

#define $PE ProgramExecutor

#include "elements.h"
#include "TokenGenerator.h"

#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <sstream>
#include <fstream>

#define RAND_MAX_SOME 50
#define RAND_MAX_MANY 1000

#define INPUT_LINE_NUMBER -2

#define YIN 0
#define YANG 1

#define RUNG_TYPE_UNDEFINED 0
#define RUNG_TYPE_COMMAND 1
#define RUNG_TYPE_VARIABLE 2
#define RUNG_TYPE_LITERAL 3
#define RUNG_TYPE_PUNCTUATION 4

#define RUNG_LINE_NUMBER_DEFAULT -1
#define RUNG_COLUMN_NUMBER_DEFAULT -1
#define RUNG_NAME_DEFAULT ""
#define RUNG_TYPE_DEFAULT RUNG_TYPE_UNDEFINED
#define RUNG_VALUE_DEFAULT 0.0
#define RUNG_COMMAND_VALUE_DEFAULT -1.0
#define RUNG_ELEMENT_DEFAULT ELEM_EARTH

#define DUMP_NO_EXECUTIONS 0
#define DUMP_NON_VARIABLE_EXECUTIONS 1
#define DUMP_ALL_EXECUTIONS 2

#define EXECUTION_DUMP_FILE_STRING "__Haifu_execution_log.txt"
#define OUTPUT_LINE_STRING "-------------------------------------------------------------"

std::string rungTypeToString(const char rungType);
std::string rungCommandToString(const double rungType);

struct Rung {
  int lineNumber;
  int columnNumber;
  std::string name;
  char type;
  double value;
  char element;

  Rung(
    int i_lineNumber = RUNG_LINE_NUMBER_DEFAULT
    , int i_columnNumber = RUNG_COLUMN_NUMBER_DEFAULT
    , std::string i_name = RUNG_NAME_DEFAULT
    , char i_type = RUNG_TYPE_DEFAULT
    , double i_value = RUNG_VALUE_DEFAULT
    , char i_element = RUNG_ELEMENT_DEFAULT
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

const Rung RUNG_NULL = Rung();

struct Variable {
  bool isCommand;
  double value;
  char element;
  std::vector<Rung> commands;

  Variable(
    bool i_isCommand = false
    , double i_value = 0.0
    , char i_element = ELEM_NONE
    , const std::vector<Rung>& i_commands = {}
    )
  {
    isCommand = i_isCommand;
    value = i_value;
    element = i_element;
    commands = i_commands;
  }
};

class ProgramExecutor {
public:
  static void loadProgram(const std::vector<HaifuToken>& tokens);

  static void executeProgram(std::istream& input);

  static int toggleExecutionDump();

private:
  typedef ProgramExecutor __this;

  static std::vector<Rung> s_program;
  static int s_bureaucrat;
  static int s_delegate;

  static int s_inputCounter;
  static int s_executionCounter;

  static bool s_wasBureaucratChanged;
  static bool s_areExecutionsDumped;
  static bool s_areVariableExecutionsDumped;

  static std::map<std::string, Variable> s_variables;

  static std::ofstream logFileStream;

  static void output(const std::string& value);

  static void appendRung_token(const HaifuToken& token);

  static void insertRung(const Rung& rung, const int index);
  static void removeRung(const int index);

  static bool executeRung(const Rung& rung, std::istream& input, const std::string& command_variable = "", const int index_command = -1);
  static bool executeRung_variable(const std::string& rungName, std::istream& input);
  static bool executeRung_punctuation();
  static bool executeRung_command(const Rung& rung, std::istream& input);

  static bool isNumeric_store(const Rung& rung, double& value);
  static bool isNumeric_store(const int programIndex, double& value);

  static bool haveSameName(const Rung& rung0, const Rung& rung1);

  static char yin_yang(const double value);

  static double getCommandValue(const int commandCode);
  static char getRungElement(const Rung& rung);

  static Variable* getVariable(const std::string& rungName);
  static Variable* getExistingVariable(const std::string& rungName);

  static const std::string& getVariableName(const std::string& rungName);

  static void assignRungValue(Rung& rung, const double value);

  // checks if s_delegate+1 is out of range of s_program and crates an error
  //  (should not happen if ProgramExecutor adheres to specifications)
  static bool areOperatorsInRange();

  // the various possible commands
  //  return value of true indicates that execution should stop
  static bool command_heaven();
  static bool command_promote();
  static bool command_demote();
  static bool command_blossom();
  static bool command_rise();
  static bool command_fall();
  static bool command_listen(std::istream& input);
  static bool command_speak();
  static bool command_count();
  static bool command_create();
  static bool command_destroy();
  static bool command_fear();
  static bool command_love();
  static bool command_become();
  static bool command_like();
  static bool command_tomorrow();
  static bool command_negative();
  static bool command_operate();

  static void outputRung(const Rung& rung, std::ostream& output, const std::string& indent = "");
  static void outputProgram(std::ostream& output);

  static void outputVariable(const Variable& variable, std::ostream& output, const int index_command = -1);
  static void outputVariables(std::ostream& output, const std::string& command_variable = "", const int index_command = -1);
};

#endif