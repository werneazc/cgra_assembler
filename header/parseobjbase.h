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

#ifndef PARSEOBJBASE_H
#define PARSEOBJBASE_H

#include <cstdint>
#include <iostream>
#include <string>

namespace as
{

// Type declarations
class Level;

/**
 * @enum COMMANDCLASS
 *
 * @brief Types of assembler commands.
 *
 */
enum class COMMANDCLASS : uint8_t
{
    UNKNOWN,      //!< @brief Unknown assembler command (error)
    CONSTANT,     //!< @brief Constant value in assembler command
    VARIABLE,     //!< @brief Variable in assembler command
    NOOPERAND,    //!< @brief Assembler command with no operator
    ONEOPERAND,   //!< @brief Assembler command with one operator
    TWOOPERAND,   //!< @brief Assembler command with two operators
    THREEOPERAND, //!< @brief Assembler command with three operators
    ARITHMETIC,   //!< @brief Assembler command with arithmetic operations
    LOOP,         //!< @brief Identify position of a new loop in assembler sequence
    RESETVAR,     //!< @brief Set a new value for a already defined variable
};

/**
 * @class ParseObjBase
 *
 * @brief Base class for parsed objects of assembler code.
 *
 * @details
 * This class should be used as base class for all valid types of assembler commands.
 *
 */
class ParseObjBase
{
  public:
    /**
     * @brief Standard Constructor
     *
     * @param[in] levelA levelA: Current level, where the parsed object exists.
     * @param[in] cmdA cmdA: Command type of parsed assembler line
     * @param[in] cmdLineA cmdLineA: Copy of assembler line as string
     * @param[in] lineNumberA lineNumberA: Line number in assembler file where the current line exists.
     */
    ParseObjBase(Level *levelA, COMMANDCLASS cmdA, const std::string &cmdLineA, const uint32_t lineNumberA);

    /**
     * @brief Copy Constructor
     *
     * @param[in] src src: Source of the copy.
     */
    ParseObjBase(const ParseObjBase &src);

    /**
     * @brief Move Constructor
     *
     * @param[in] src src: Source of new object. The entries of source object are deleted.
     */
    ParseObjBase(ParseObjBase &&src);

    /**
     * @brief Copy Assignment
     *
     * @param[in] src src: Source of assignment.
     *
     * @return ParseObjBase& new constructed parsed object with values of source.
     */
    ParseObjBase &operator=(const ParseObjBase &src);

    /**
     * @brief Move Assignment
     *
     * @param[in] src src: Source of assignment. Values of source are deleted.
     *
     * @return ParseObjBase& new constructed parsed object with values of source.
     */
    ParseObjBase &operator=(ParseObjBase &&src);

    /**
     * @brief Get string of read assembler file line.
     *
     * @return std::string copy of assembler line.
     */
    virtual std::string getReadCmdLine() const final;

    /**
     * @brief Get line of assembler file.
     *
     * @return uint32_t Assembler file line
     */
    virtual uint32_t getFileLineNumber() const final;

    /**
     * @brief Get level of parse object.
     *
     * @return Level* Pointer to level where parse object was found.
     */
    virtual Level *getLevel() const final;

    /**
     * @brief Return command type of assembler command.
     *
     * @return as::COMMANDCLASS Copy of stored assembler command type
     */
    virtual COMMANDCLASS getCommandClass(void) const final;

    /**
     * @brief Reset all members of class to default values
     */
    virtual void clearMembers(void);

  protected:
    /**
     * @brief Set new read command line
     *
     * @param[in] newA New value for read command line.
     *
     * @return Copy of previous value.
     */
    virtual std::string setCmdReadLine(const std::string &newA) final;

    /**
     * @brief Set new line for parsed command in assembler file.
     *
     * @param[in] newA New value for parsed line
     * @return Copy of previous value.
     */
    virtual u_int32_t setLineNumber(const u_int32_t newA) final;

    /**
     * @brief Set new level pointer.
     *
     * @param[in] newA New level to set.
     * @return Copy of previous level pointer.
     */
    virtual Level *const setLevel(const Level *newA) final;

    /**
    * @brief Set new command class for parsed object
    *
    * @param[in] newA New command class value

    * @return Copy of previous command class value
    */
    virtual COMMANDCLASS setCommandClass(COMMANDCLASS newA) final;

  public:
    /**
     * @brief Destructor
     *
     */
    virtual ~ParseObjBase() = default;

  private:
    // Forbidden Constructors
    ParseObjBase() = delete;

    // Class member
    Level *m_level;
    //!< @brief Stores the pointer to the level, where the parse object is found.
    COMMANDCLASS m_class;
    //!< @brief Defines the type of parsed object
    std::string m_cmdLine;
    //!< @brief Stores the textual line from assembler file
    uint32_t m_lineNumber;
    //!< @brief Stores the line in the assembler file where the parsed object was found.
};

} /* End namespace as */

/** @brief Encapsulate coding utilities */
namespace utils
{
/**
 * @brief Get string representation for command class
 *
 * @param[in] classA            Command class to transform into string
 */
std::string commandclassToString(as::COMMANDCLASS classA);
} // End namespace utils

/**
 * @brief Dump general information of a parse object like command class
 *
 * @param[in] osA               Output stream to write data
 * @param[in] objA              Reference to parse object
 */
std::ostream &operator<<(std::ostream &osA, const as::ParseObjBase &objA);

#endif // PARSEOBJBASE_H
