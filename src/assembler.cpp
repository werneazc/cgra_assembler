/*
 * Copyright (C) 2019  andrewerner <andre.werner-w2m@ruhr-uni-bochum.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "assembler.h"
#include "add.h"
#include "addinteger.h"
#include "loop.h"
#include "mul.h"
#include "mulinteger.h"
#include "myException.h"
#include "nooperand.h"
#include "oneoperand.h"
#include "parseobjectconst.h"
#include "parseobjectvariable.h"
#include "resetvariable.h"
#include "sub.h"
#include "subinteger.h"
#include "threeoperand.h"
#include "twooperand.h"
#include <boost/format.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include <cstdint>
#include <iostream>
#include <locale>

namespace
{

const boost::regex c_eVariable("VAR\\s+(\\w+)\\s+([+-]?\\w+)\\s*$");
//!< \brief Regular expression to parse a variable line
const boost::regex c_eConstant("CONST\\s+(\\w+)\\s+(0x[[:xdigit:]]+|[+-]?[[:digit:]]+)\\s*$", boost::regex::extended);
//!< \brief Regular expression to parse a constant line
const boost::regex c_eLoop("LOOP\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s*$");
//!< \brief Regular expression to parse a start line of a loop construct
const boost::regex c_ePool("POOL");
//!< \brief Regular expression to parse a end line of a loop construct
const boost::regex c_eCommand("([\\w_]+)(\\s+\\w+){0,3}\\s*$");
//!< \brief Regular expression to parse an assembler command
const boost::regex c_eOne("\\s*(\\w+)\\s+(\\w+)");
//!< \brief Regular expression to parse one argument
const boost::regex c_eTwo("\\s*(\\w+)\\s+(\\w+)\\s+(\\w+)");
//!< \brief Regular expression to parse two arguments
const boost::regex c_eThree("\\s*(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)");
//!< \brief Regular expression to parse three arguments

/**
 * @brief Check that string is a number.
 *
 * @param strA Constant reference to search string.
 * @return True if all characters in search string are digits
 */
bool is_number(const std::string &strA)
{
    bool t_status{true};

    if (strA.find("0x") != std::string::npos)
        {
            for (auto it = strA.cbegin() + 2; it != strA.cend(); ++it)
                {
                    if (!std::isxdigit(*it))
                        {
                            t_status = false;
                            break;
                        }
                }
        }
    else
        {

            for (auto it = strA.cbegin(); it != strA.cend(); ++it)
                {
                    if (!std::isdigit(*it) && *it != '+' && *it != '-')
                        {
                            t_status = false;
                            break;
                        }
                }
        }

    return t_status;
}

/** @brief Structure for creating a list of references for createTwoOpParseObj function */
typedef struct
{
    std::array<std::string, 2> &Ops; //!< @brief Reference to array of operands
    uint16_t &count;                 //!< @brief Reference to the line counter
    std::string &command;            //!< @brief Reference to the command string
    const std::string &match;        //!< @brief reference to the line match string
    const uint8_t &machineId;        //!< @brief Reference to machine ID (unused for arithmetic operations)
    const char *const &op_type;      //!< @brief Reference to latest operator type
    boost::smatch &commandMatch;     //!< @brief Reference to match object for two operand commands
} createTwoOpParseObjParam_t;

/**
 * @brief Get two parse object from latest parameter set
 *
 * @param paramA        Parameter set
 * @return Two-operand or arithmetic parse object
 */
as::ParseObjBase *createTwoOpParseObj(createTwoOpParseObjParam_t &paramA)
{

    as::ParseObjBase *t_parseObj(nullptr);
    as::ParseObjBase *t_first{nullptr};
    as::ParseObjBase *t_second{nullptr};
    bool firstOp{true};

    for (const auto &op : paramA.Ops)
        {
            auto t_op = as::Level::getCurrentLevel()->findParseObj(op);

            if (!t_op)
                {
                    if (std::find_if(op.begin(), op.end(), [](unsigned char c) { return !std::isdigit(c); }) !=
                        op.end())
                        {
                            std::ostringstream t_msg{""};
                            t_msg << "Syntax error line " << paramA.count << ". Unknown variable." << std::endl;
                            throw as::AssemblerException(t_msg.str(), 1051);
                        }
                    else
                        {
                            auto t_parseObj =
                                new as::ParseObjectConst(op, std::stoi(op, nullptr, 0), as::Level::getCurrentLevel(),
                                                         paramA.commandMatch[0].str(), paramA.count);
                            as::Level::getCurrentLevel()->addParseObj(t_parseObj);

                            t_op = t_parseObj;
                        }
                }

            if (firstOp)
                {
                    t_first = t_op;
                    firstOp = false;
                }
            else
                {
                    t_second = t_op;
                }
        }

    if (std::strcmp(paramA.op_type, "TwoOperator") == 0)
        {
            t_parseObj = new as::TwoOperand(as::Level::getCurrentLevel(), paramA.match, paramA.count, t_first, t_second,
                                            paramA.machineId);

            // Show properties of variable for debugging
            std::cout << *static_cast<as::TwoOperand *>(t_parseObj) << "\n";
        }
    else
        {
            if (std::strcmp(paramA.command.c_str(), "ADD") == 0)
                {
                    t_parseObj =
                        new as::Add(as::Level::getCurrentLevel(), paramA.match, paramA.count, t_first, t_second);

                    // Show properties of variable for debugging
                    std::cout << *static_cast<as::Add *>(t_parseObj) << "\n";
                }
            else if (std::strcmp(paramA.command.c_str(), "ADDI") == 0)
                {
                    if (t_second->getCommandClass() == as::COMMANDCLASS::CONSTANT)
                        {
                            t_parseObj = new as::AddInteger(as::Level::getCurrentLevel(), paramA.match, paramA.count,
                                                            t_first, t_second);
                            // Show properties of variable for debugging
                            std::cout << *static_cast<as::AddInteger *>(t_parseObj) << "\n";
                        }
                    else
                        {
                            std::ostringstream t_msg{""};
                            t_msg << "Value error line " << paramA.count
                                  << ". Second Argument for ADDI is not a constant." << std::endl;
                            throw as::AssemblerException(t_msg.str(), 1058);
                        }
                }
            else if (std::strcmp(paramA.command.c_str(), "SUB") == 0)
                {
                    t_parseObj =
                        new as::Sub(as::Level::getCurrentLevel(), paramA.match, paramA.count, t_first, t_second);

                    // Show properties of variable for debugging
                    std::cout << *static_cast<as::Sub *>(t_parseObj) << "\n";
                }
            else if (std::strcmp(paramA.command.c_str(), "SUBI") == 0)
                {
                    if (t_second->getCommandClass() == as::COMMANDCLASS::CONSTANT)
                        {
                            t_parseObj = new as::SubInteger(as::Level::getCurrentLevel(), paramA.match, paramA.count,
                                                            t_first, t_second);
                            // Show properties of variable for debugging
                            std::cout << *static_cast<as::SubInteger *>(t_parseObj) << "\n";
                        }
                    else
                        {
                            std::ostringstream t_msg{""};
                            t_msg << "Value error line " << paramA.count
                                  << ". Second Argument for SUBI is not a constant." << std::endl;
                            throw as::AssemblerException(t_msg.str(), 1068);
                        }
                }
            else if (std::strcmp(paramA.command.c_str(), "MUL") == 0)
                {
                    t_parseObj =
                        new as::Mul(as::Level::getCurrentLevel(), paramA.match, paramA.count, t_first, t_second);

                    // Show properties of variable for debugging
                    std::cout << *static_cast<as::Mul *>(t_parseObj) << "\n";
                }
            else if (std::strcmp(paramA.command.c_str(), "MULI") == 0)
                {
                    if (t_second->getCommandClass() == as::COMMANDCLASS::CONSTANT)
                        {
                            t_parseObj = new as::MulInteger(as::Level::getCurrentLevel(), paramA.match, paramA.count,
                                                            t_first, t_second);
                            // Show properties of variable for debugging
                            std::cout << *static_cast<as::MulInteger *>(t_parseObj) << "\n";
                        }
                    else
                        {
                            std::ostringstream t_msg{""};
                            t_msg << "Value error line " << paramA.count
                                  << ". Second Argument for MULI is not a constant." << std::endl;
                            throw as::AssemblerException(t_msg.str(), 1075);
                        }
                }
            else
                {
                    std::ostringstream t_msg{""};
                    t_msg << "Command error line " << paramA.count << ". Unknown command." << std::endl;
                    throw as::AssemblerException(t_msg.str(), 1059);
                }
        }
    return t_parseObj;
}

} // End anonymous namespace

namespace as
{

Assembler::Assembler(boost::filesystem::path &filePathA, boost::property_tree::ptree &configA, std::ostream &logA)
    : m_filePath(filePathA), m_config(configA), m_log(logA), m_firstLevel{new Level()}
{

    try
        {
            // Initialize output file path variable and validate.
            m_outPath = m_config.get<std::string>("General.Output");
        }
    catch (boost::property_tree::ptree_error &e)
        {
            throw as::AssemblerException("Option \"General.Output\" missing in configuration file", 1000);
        }

    if (boost::filesystem::exists(m_outPath))
        m_log << "Warning: File" << m_outPath.filename() << " will be replaced." << std::endl;
    else
        {
            if (m_outPath.parent_path().string() != ".")
                {
                    if (boost::filesystem::create_directories(m_outPath.parent_path()))
                        m_log << "Info: Create output file directory." << std::endl;
                }
        }

    // Store filename to variable
    m_outFileName = m_outPath.filename();
    // Validate file extension.
    if (m_outFileName.extension() != ".hpp")
        throw as::AssemblerException("Output file has wrong file extension. Expected extension \".vmc\"", 1001);

    Level::setCurrentLevel(m_firstLevel);

    return;
}

Assembler::~Assembler()
{
    delete m_firstLevel;
}

void Assembler::parse(void)
{
    m_log << "Start parsing assembler file" << std::endl;
    m_log << "----------------------------" << std::endl;

    // Load list of available operations and there machine ID from configuration file
    std::unordered_map<std::string, std::vector<std::pair<std::string, uint8_t>>> t_commandMap{};

    try
        {
            // Temporary create vector of available operations and there machine Id of an command class.
            std::vector<std::pair<std::string, uint8_t>> t_vec;

            // Iterate over available command classes
            for (const std::string &iter :
                 {"Assembler_Property.NoOperator", "Assembler_Property.OneOperator", "Assembler_Property.TwoOperator",
                  "Assembler_Property.ThreeOperator", "Assembler_Property.ArithOperator"})
                {
                    for (const auto &op : m_config.get_child(iter))
                        t_vec.emplace_back(op.second.get<std::string>("Name"),
                                           op.second.get<uint8_t>("MachineId", UINT8_MAX));

                    t_commandMap.emplace(iter.substr(iter.find_first_of('.') + 1), std::move(t_vec));
                }

            m_log << "Available commands: " << '\n';
            m_log << "====================" << '\n';

            for (const auto &iter : t_commandMap)
                for (const auto &com : iter.second)
                    m_log << com.first << ",";

            m_log << "\n\n\n" << std::endl;
        }
    catch (boost::property_tree::ptree_error &e)
        {
            std::cerr << e.what() << std::endl;
        }

    // Open file
    std::filebuf t_fb;
    if (t_fb.open(m_filePath.c_str(), std::ios::in))
        {
            // Temporary variables to handle lines of file
            std::istream t_is(&t_fb);
            std::string t_str;
            uint16_t t_count{1};
            boost::smatch t_LineMatch;

            // Iterate over file lines:
            do
                {
                    std::getline(t_is, t_str);

                    m_log << "Parsed Assembler line " << t_count << ": " << t_str << std::endl;

                    if (t_str.find_first_of('#') != std::string::npos || t_str.empty()) // Comment or empty line
                        {
                            ++t_count;
                            continue;
                        }
                    else if (boost::regex_search(t_str, t_LineMatch, c_eLoop))
                        {
                            // Temporary variables to store loop parameters from parsed command string
                            ParseObjBase *t_start{0};
                            ParseObjBase *t_end{0};
                            ParseObjBase *t_step{0};
                            uint8_t t_countval{1}; // This counter iterates over group matches; 0 is whole match.

                            // Get start, end and step value for Loop
                            for (auto val : {&t_start, &t_end, &t_step})
                                {

                                    if (is_number(t_LineMatch[t_countval].str()))
                                        {
                                            auto t_searchResult =
                                                Level::getCurrentLevel()->findParseObj(t_LineMatch[t_countval].str());

                                            if (t_searchResult)
                                                {
                                                    *val = t_searchResult;
                                                }
                                            else
                                                {
                                                    auto t_val = stoi(t_LineMatch[t_countval], nullptr, 0);
                                                    auto t_pConst = new ParseObjectConst(t_LineMatch[t_countval], t_val,
                                                                                         Level::getCurrentLevel(),
                                                                                         t_LineMatch[0], t_count);

                                                    Level::getCurrentLevel()->addParseObj(t_pConst);
                                                    *val = t_pConst;
                                                }
                                        }
                                    else
                                        {
                                            auto t_searchResult =
                                                Level::getCurrentLevel()->findParseObj(t_LineMatch[t_countval].str());
                                            if (t_searchResult)
                                                {
                                                    if (t_searchResult->getCommandClass() == COMMANDCLASS::VARIABLE ||
                                                        t_searchResult->getCommandClass() == COMMANDCLASS::CONSTANT)
                                                        {
                                                            *val = t_searchResult;
                                                        }
                                                    else
                                                        {
                                                            std::ostringstream t_msg{""};
                                                            t_msg << "Syntax error line " << t_count
                                                                  << ". Arguments for LOOP command invalid."
                                                                  << std::endl;
                                                            throw AssemblerException(t_msg.str(), 1020);
                                                        }
                                                }
                                        }

                                    ++t_countval;
                                }

                            if (t_step == 0)
                                throw as::AssemblerException("Infinitive Loop, because stepwidth is set to zero.",
                                                             1066);

                            // Add Loop start point to actual level
                            auto t_pObj = new ParseObjBase(Level::getCurrentLevel(), COMMANDCLASS::LOOP,
                                                           t_LineMatch[0].str(), t_count);
                            Level::getCurrentLevel()->addParseObj(t_pObj);

                            // Create new level as a loop
                            Loop *t_loopPtr = new Loop(Loop::getCurrentLevel(), t_count, t_start, t_end, t_step,
                                                       t_LineMatch[0].str());
                            Level::getCurrentLevel()->addChildLevel(static_cast<Level *>(t_loopPtr));

                            // Set actual level to new created loop
                            Level::setCurrentLevel(static_cast<Level *>(t_loopPtr));

                            ++t_count;
                        }
                    else if (boost::regex_search(t_str, t_LineMatch, c_ePool))
                        {
                            Level::getCurrentLevel()->leave();
                            ++t_count;
                        }
                    else if (boost::regex_search(t_str, t_LineMatch, c_eVariable))
                        {
                            // Temporary variable to store value of assembler variable
                            int32_t t_value{0};

                            if (std::isdigit(t_LineMatch[1].str().front()))
                                {
                                    std::ostringstream t_msg{""};
                                    t_msg << "Syntax error line " << t_count << ". Variable name starts with number."
                                          << std::endl;
                                    throw AssemblerException(t_msg.str(), 1024);
                                }

                            if (is_number(t_LineMatch[2].str()))
                                {
                                    t_value = std::stoi(t_LineMatch[2].str(), nullptr, 0);
                                }
                            else
                                {

                                    auto t_valPtr = Level::getCurrentLevel()->findParseObj(t_LineMatch[2].str());
                                    if (t_valPtr)
                                        {
                                            if (t_valPtr->getCommandClass() == COMMANDCLASS::VARIABLE)
                                                t_value =
                                                    static_cast<ParseObjectVariable *>(t_valPtr)->getVariableValue();
                                            else if (t_valPtr->getCommandClass() == COMMANDCLASS::CONSTANT)
                                                t_value = static_cast<ParseObjectConst *>(t_valPtr)->getConstValue();
                                            else
                                                {
                                                    std::ostringstream t_msg{""};
                                                    t_msg << "Syntax error line " << t_count
                                                          << ". Arguments for VARIABLE command invalid." << std::endl;
                                                    throw AssemblerException(t_msg.str(), 1030);
                                                }
                                        }
                                    else
                                        {
                                            std::ostringstream t_msg{""};
                                            t_msg << "Syntax error line " << t_count << ". Variable value is invalid."
                                                  << std::endl;
                                            throw AssemblerException(t_msg.str(), 1023);
                                        }
                                }

                            auto t_var = as::Level::getCurrentLevel()->findParseObj(t_LineMatch[1].str());
                            as::ParseObjBase *t_pvPtr{nullptr};

                            if (t_var)
                                {
                                    auto t_valPtr = Level::getCurrentLevel()->findParseObj(t_LineMatch[2].str());

                                    if (t_valPtr)
                                        {
                                            t_pvPtr = static_cast<as::ParseObjBase *>(
                                                new ResetVariable(Level::getCurrentLevel(), t_LineMatch[0].str(),
                                                                  t_count, t_valPtr, t_var));
                                        }
                                    else
                                        {
                                            if (is_number(t_LineMatch[2].str()))
                                                {
                                                    t_value = std::stoi(t_LineMatch[2].str(), nullptr, 0);

                                                    auto t_pcPtr = new ParseObjectConst(t_LineMatch[2].str(), t_value,
                                                                                        Level::getCurrentLevel(),
                                                                                        t_LineMatch[0].str(), t_count);

                                                    // At parse object to current level
                                                    Level::getCurrentLevel()->addParseObj(t_pcPtr);

                                                    // Show properties of variable for debugging
                                                    std::cout << *t_pcPtr << "\n";

                                                    t_pvPtr = static_cast<as::ParseObjBase *>(new ResetVariable(
                                                        Level::getCurrentLevel(), t_LineMatch[0].str(), t_count,
                                                        t_pcPtr, t_var));
                                                }
                                            else
                                                {
                                                    std::ostringstream t_msg{""};
                                                    t_msg << "Error line" << t_count
                                                          << ". Variable reset value is invalid." << std::endl;
                                                    throw AssemblerException(t_msg.str(), 1023);
                                                }
                                        }
                                }
                            else
                                {
                                    t_pvPtr = static_cast<as::ParseObjBase *>(
                                        new ParseObjectVariable(t_LineMatch[1].str(), t_value, Level::getCurrentLevel(),
                                                                t_LineMatch[0].str(), t_count));
                                }

                            // At parse object to current level
                            Level::getCurrentLevel()->addParseObj(t_pvPtr);

                            // Show properties of variable for debugging
                            std::cout << *t_pvPtr << "\n";

                            ++t_count;
                        }
                    else if (boost::regex_search(t_str, t_LineMatch, c_eConstant))
                        {
                            // Temporary variable to store value of assembler variable
                            int32_t t_value{0};

                            if (std::isdigit(t_LineMatch[1].str().front()))
                                {
                                    std::ostringstream t_msg{""};
                                    t_msg << "Syntax error line " << t_count << ". Constant name starts with number."
                                          << std::endl;
                                    throw AssemblerException(t_msg.str(), 1027);
                                }

                            if (is_number(t_LineMatch[2].str()))
                                {
                                    t_value = std::stoi(t_LineMatch[2].str(), nullptr, 0);
                                }
                            else
                                {
                                    std::ostringstream t_msg{""};
                                    t_msg << "Syntax error line " << t_count << ". Constant value is not an integer."
                                          << std::endl;
                                    throw AssemblerException(t_msg.str(), 1026);
                                }

                            auto t_pcPtr = new ParseObjectConst(t_LineMatch[1].str(), t_value, Level::getCurrentLevel(),
                                                                t_LineMatch[0].str(), t_count);

                            // At parse object to current level
                            Level::getCurrentLevel()->addParseObj(t_pcPtr);

                            // Show properties of variable for debugging
                            std::cout << *t_pcPtr << "\n";

                            ++t_count;
                        }
                    else if (boost::regex_search(t_str, t_LineMatch, c_eCommand))
                        {
                            // Temporary variable
                            boost::smatch t_commandMatch{};
                            /*
                             * Sometimes the direct usage of t_LineMatch[0].str() did not work.
                             * Thus, the parameter for regex search is directly copied into a string constant
                             * to overcome this issue.
                             */
                            const std::string t_match = t_LineMatch[0].str();

                            if (boost::regex_search(t_match, t_commandMatch, c_eThree))
                                {
                                    std::string t_command = t_commandMatch[1].str();
                                    std::array<std::string, 3> t_Ops{t_commandMatch[2].str(), t_commandMatch[3].str(),
                                                                     t_commandMatch[4].str()};

                                    for (const auto &vec : t_commandMap.find("ThreeOperator")->second)
                                        {

                                            if (vec.first == t_command)
                                                {
                                                    as::ParseObjBase *t_first{nullptr};
                                                    as::ParseObjBase *t_second{nullptr};
                                                    as::ParseObjBase *t_third{nullptr};
                                                    uint8_t op_cnt{0};

                                                    for (const auto &op : t_Ops)
                                                        {
                                                            auto t_op = as::Level::getCurrentLevel()->findParseObj(op);

                                                            if (!t_op)
                                                                {
                                                                    if (std::find_if(op.begin(), op.end(),
                                                                                     [](unsigned char c) {
                                                                                         return !std::isdigit(c);
                                                                                     }) != op.end())
                                                                        {
                                                                            std::ostringstream t_msg{""};
                                                                            t_msg << "Syntax error line " << t_count
                                                                                  << ". Unknown variable." << std::endl;
                                                                            throw AssemblerException(t_msg.str(), 1051);
                                                                        }
                                                                    else
                                                                        {
                                                                            auto t_parseObj = new as::ParseObjectConst(
                                                                                op, std::stoi(op, nullptr, 0),
                                                                                as::Level::getCurrentLevel(),
                                                                                t_commandMatch[0].str(), t_count);
                                                                            as::Level::getCurrentLevel()->addParseObj(
                                                                                t_parseObj);

                                                                            t_op = t_parseObj;
                                                                        }
                                                                }

                                                            if (op_cnt == 0)
                                                                {
                                                                    t_first = t_op;
                                                                    ++op_cnt;
                                                                }
                                                            else if (op_cnt == 1)
                                                                {
                                                                    t_second = t_op;
                                                                    ++op_cnt;
                                                                }
                                                            else
                                                                {
                                                                    t_third = t_op;
                                                                }
                                                        }

                                                    auto t_parseObj = new as::ThreeOperand(
                                                        as::Level::getCurrentLevel(), t_match, t_count, t_first,
                                                        t_second, t_third, vec.second);

                                                    // At parse object to current level
                                                    as::Level::getCurrentLevel()->addParseObj(t_parseObj);

                                                    // Show properties of variable for debugging
                                                    std::cout << *t_parseObj << "\n";

                                                    break;
                                                }
                                        }
                                }
                            else if (boost::regex_search(t_match, t_commandMatch, c_eTwo))
                                {

                                    std::string t_command = t_commandMatch[1].str();
                                    std::array<std::string, 2> t_Ops{t_commandMatch[2].str(), t_commandMatch[3].str()};
                                    bool found{false};

                                    for (const auto &op_type : {"TwoOperator", "ArithOperator"})
                                        {
                                            if (!found)
                                                {
                                                    for (const auto &vec : t_commandMap.find(op_type)->second)
                                                        {

                                                            if (vec.first == t_command)
                                                                {

                                                                    createTwoOpParseObjParam_t t_param{
                                                                        .Ops = t_Ops,
                                                                        .count = t_count,
                                                                        .command = t_command,
                                                                        .match = t_match,
                                                                        .machineId = vec.second,
                                                                        .op_type = op_type,
                                                                        .commandMatch = t_commandMatch,
                                                                    };

                                                                    as::ParseObjBase *t_parseObj =
                                                                        createTwoOpParseObj(t_param);

                                                                    // At parse object to current level
                                                                    as::Level::getCurrentLevel()->addParseObj(
                                                                        t_parseObj);

                                                                    found = true;

                                                                    break; // Stop loop over commands for operater class
                                                                }
                                                        }
                                                }
                                            else
                                                {
                                                    break; // Stop loop over operator classes
                                                }
                                        }
                                }
                            else if (boost::regex_search(t_match, t_commandMatch, c_eOne))
                                {
                                    std::string t_command = t_commandMatch[1].str();
                                    std::string t_value = t_commandMatch[2].str();

                                    for (const auto &vec : t_commandMap.find("OneOperator")->second)
                                        {
                                            if (vec.first == t_command)
                                                {
                                                    auto t_first = as::Level::getCurrentLevel()->findParseObj(t_value);

                                                    if (!t_first)
                                                        {
                                                            if (std::find_if(t_value.begin(), t_value.end(),
                                                                             [](unsigned char c) {
                                                                                 return !std::isdigit(c);
                                                                             }) != t_value.end())
                                                                {
                                                                    std::ostringstream t_msg{""};
                                                                    t_msg << "Syntax error line " << t_count
                                                                          << ". Unknown variable." << std::endl;
                                                                    throw AssemblerException(t_msg.str(), 1049);
                                                                }
                                                            else
                                                                {
                                                                    auto t_parseObj = new as::ParseObjectConst(
                                                                        t_value, std::stoi(t_value, nullptr, 0),
                                                                        as::Level::getCurrentLevel(),
                                                                        t_commandMatch[0].str(), t_count);
                                                                    as::Level::getCurrentLevel()->addParseObj(
                                                                        t_parseObj);
                                                                    t_first = t_parseObj;
                                                                }
                                                        }

                                                    auto t_parseObj =
                                                        new as::OneOperand(as::Level::getCurrentLevel(), t_match,
                                                                           t_count, t_first, vec.second);

                                                    // At parse object to current level
                                                    as::Level::getCurrentLevel()->addParseObj(t_parseObj);

                                                    // Show properties of variable for debugging
                                                    std::cout << *t_parseObj << "\n";

                                                    break;
                                                }
                                        }
                                }
                            else
                                {
                                    for (const auto &vec : t_commandMap.find("NoOperator")->second)
                                        {
                                            if (vec.first == t_match)
                                                {
                                                    auto t_parseObj = new NoOperand(Level::getCurrentLevel(), t_match,
                                                                                    t_count, vec.second);

                                                    // At parse object to current level
                                                    Level::getCurrentLevel()->addParseObj(t_parseObj);

                                                    // Show properties of variable for debugging
                                                    std::cout << *t_parseObj << "\n";

                                                    break;
                                                }
                                        }
                                }

                            ++t_count;
                        }
                    else
                        {
                            std::ostringstream t_msg{""};
                            t_msg << "Syntax error line " << t_count << ". Unknown command code type." << std::endl;
                            throw AssemblerException(t_msg.str(), 1002);
                        }
                }
            while (!t_is.eof());
        }
    else
        throw as::AssemblerException("Error while opening assembler source file for parsing", 1202);

    m_log << "Parsing of assembler input file successfully finished." << std::endl;

    return;
}

void Assembler::assemble(void)
{
    m_log << "\nStart assembling code" << std::endl;
    m_log << "---------------------" << std::endl;

    // Opening file to store machine code.
    std::filebuf t_fb;

    if (t_fb.open(m_outPath.c_str(), std::ios::out))
        {
            std::ostream t_codeFile(&t_fb);

            t_codeFile << "#ifndef " << std::uppercase << m_outFileName.stem().string() << "_H_\n";
            t_codeFile << "#define " << std::uppercase << m_outFileName.stem().string() << "_H_\n\n\n";

            t_codeFile << "#include <vector>\n\n";
            t_codeFile << "namespace cgra \n{\n\n";
            t_codeFile << "std::vector<cgra::TopLevel::assembler_type_t> assembly{\n";

            uint64_t lvlId{0};

            for (auto po : m_firstLevel->getParseObjList())
                {
                    switch (po->getCommandClass())
                        {
                        case as::COMMANDCLASS::ARITHMETIC:
                            static_cast<as::IArithmetic *>(po)->processOperation();
                            break;
                        case as::COMMANDCLASS::NOOPERAND:
                            t_codeFile << static_cast<as::NoOperand *>(po)->assemble(m_config);
                            t_codeFile << ",";
                            t_codeFile << " //" << po->getReadCmdLine() << std::endl;
                            break;
                        case as::COMMANDCLASS::ONEOPERAND:
                            t_codeFile << static_cast<as::OneOperand *>(po)->assemble(m_config);
                            t_codeFile << ",";
                            t_codeFile << " //" << po->getReadCmdLine() << std::endl;
                            break;
                        case as::COMMANDCLASS::TWOOPERAND:
                            t_codeFile << static_cast<as::TwoOperand *>(po)->assemble(m_config);
                            t_codeFile << ",";
                            t_codeFile << " //" << po->getReadCmdLine() << std::endl;
                            break;
                        case as::COMMANDCLASS::THREEOPERAND:
                            t_codeFile << static_cast<as::ThreeOperand *>(po)->assemble(m_config);
                            t_codeFile << ",";
                            t_codeFile << " //" << po->getReadCmdLine() << std::endl;
                            break;
                        case as::COMMANDCLASS::LOOP:
                            static_cast<Loop *>(m_firstLevel->at(lvlId++))->assemble(m_config, t_codeFile);
                            break;
                        case as::COMMANDCLASS::RESETVAR:
                            static_cast<ResetVariable *>(po)->resetVariable();
                            break;
                        case as::COMMANDCLASS::CONSTANT:
                        case as::COMMANDCLASS::VARIABLE:
                        default:
                            break;
                        }
                }

            t_codeFile << "};\n\n";
            t_codeFile << "} //End namespace cgra\n\n";
            t_codeFile << "#endif //" << std::uppercase << m_outFileName.stem().string() << "_H_\n";
            t_fb.close();
        }
    else
        {
            throw AssemblerException("Error while opening output file.", 4500);
        }

    m_log << "Machine operation code successfully stored at " << m_outPath << std::endl;

    return;
}

// void Assembler::writeVmcFile()
// {
//
//     //Opening file to store machine code.
//     std::filebuf t_fb;
//     if(t_fb.open(m_outPath.c_str(), std::ios::out))
//     {
//         std::ostream t_codeFile(&t_fb);
//         t_codeFile << m_mc << std::endl;
//         t_fb.close();
//     }
//     else
//     {
//         throw AssemblerException("Error while opening output file.", 4500);
//     }
//
//     m_log << "Machine operation code successfully stored at " << m_outPath << std::endl;
//
//     return;
//
// }
//
} // End namespace as
