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

#ifndef THREEOPERAND_H
#define THREEOPERAND_H

#include "iassemble.h"
#include "parseobjbase.h"
#include <tuple>

namespace as
{

// Forward declaration
class Level;

/**
 * @class ThreeOperand
 *
 * @brief Parse object for VCGRA assembler commands with three operands.
 *
 * @details
 * This parse object handles assembler commands with three operands like LOADD
 * or STORED. It stores the machine code ID for the VCGRA command parser as
 * well as three pointers to the operands.
 */
class ThreeOperand : public ParseObjBase, public IAssemble
{
  public:
    typedef std::tuple<ParseObjBase *const, ParseObjBase *const, ParseObjBase *const> operands_type_t;
    //!< @brief Tuple type for the three operands.

    /**
     * @brief General constructor
     *
     * @param lvlA Level where the command is placed in assembler command file.
     * @param cmdLineA String of parsed command line in assembler file.
     * @param lineNumber Line number in assembler file.
     * @param firstA Handle to first argument of parsed command.
     * @param secondA Handle to second argument of parsed command.
     * @param thridA Handle to third argument of parsed command.
     * @param machienIdA Machine code ID for parsed command.
     */
    ThreeOperand(Level *const lvlA, const std::string &cmdLineA, const uint32_t lineNumber, ParseObjBase *const firstA,
                 ParseObjBase *const secondA, ParseObjBase *const thridA, const uint32_t machienIdA);

    /**
     * @brief Copy constructor
     *
     * @param srcA Source for member variables of new object.
     */
    ThreeOperand(const ThreeOperand &srcA);

    /**
     * @brief Move constructor
     *
     * @param srcA Source for member variables of new object. Members of source are deleted.
     */
    ThreeOperand(ThreeOperand &&srcA);

    /**
     * @brief Destructor
     */
    virtual ~ThreeOperand(void) = default;

    /**
     * @brief Copy assignment
     *
     * @param rhsA Source for member variables of assignment object.
     * @return Object as a copy of member variables of rhsA.
     */
    ThreeOperand &operator=(const ThreeOperand &rhsA);

    /**
     * @brief Move assignment
     *
     * @param rhsA Source for member variables of assigned object. Members of rhsA are deleted.
     * @return Object as a copy of member variables of rhsA.
     */
    ThreeOperand &operator=(ThreeOperand &&rhsA);

    /**
     * @brief Get handle of first operand.
     *
     * @return constant handle.
     */
    ParseObjBase *const getFirst(void) const;

    /**
     * @brief Setter for handle of first operand.
     *
     * @param firstA New handle to first operand.
     * @return Handle to previous first operand.
     */
    ParseObjBase *const setFirst(ParseObjBase *const firstA);

    /**
     * @brief Get handle to second operand.
     *
     * @return constant handle.
     */
    ParseObjBase *const getSecond(void) const;

    /**
     * @brief Setter for handle of second operand.
     *
     * @param secondA Handle to new second operand.
     * @return Handle to previous second operand.
     */
    ParseObjBase *const setSecond(ParseObjBase *const secondA);

    /**
     * @brief Get handle to third operand.
     *
     * @return Constant handle.
     */
    ParseObjBase *const getThird(void) const;

    /**
     * @brief Setter for handle of third operand.
     *
     * @param thirdA Handle to new third operand.
     * @return Handle to previous third operand.
     */
    ParseObjBase *const setThird(ParseObjBase *const thirdA);

    /**
     * @brief Get tuple of all three operands
     *
     * @return as::ThreeOperand::operands_type_t
     */
    operands_type_t getOperands(void) const;

    /**
     * @brief Set all operands new.
     *
     * @param operandsA a tuple of operands (first, second, third)
     * @return Tuple of previous three operands.
     */
    operands_type_t setOperands(const operands_type_t &operandsA);

    /**
     * @brief Set all operands.
     *
     * @param firstA Handle to first operand.
     * @param secondA Handle to second new operand.
     * @param thirdA Handle to third new operand.
     * @return A tuple of the previous three operands.
     */
    operands_type_t setOperands(ParseObjBase *const firstA, ParseObjBase *const secondA, ParseObjBase *const thirdA);

    /**
     * @brief Get current machine code ID.
     *
     * @return Machine code ID.
     */
    uint32_t getMachineCodeId(void) const;

    /**
     * @brief Set new machine code ID
     *
     * @param machineIdA New machine code ID.
     * @return Previous machine code ID.
     */
    uint32_t setMachineCodeId(const uint32_t machineIdA);

    /**
     * @brief Reset all object members.
     */
    virtual void clearMembers(void) override;

    /**
     * @brief Create machine code from assembler command.
     *
     * @param ptreeA Property tree with configuration values from SW configuration file
     * @return Machine code line for VCGRA instance.
     */
    virtual std::string assemble(const boost::property_tree::ptree &ptreeA) override final;

  private:
    ParseObjBase *m_first;
    //!< @brief Handle to first operand
    ParseObjBase *m_second;
    //!< @brief Handle to first operand
    ParseObjBase *m_third;
    //!< @brief Handle to first operand
    uint32_t m_machineCodeId;
    //!< @brief Machine code ID for VCGRA command parser.
};

} /* End namespace as */

/**
 * @brief Dump information about two-operand instance
 *
 * @param[in] osA   Output stream to write to
 * @param[in] opA   Reference to three operand that shall be dumped
 * @return std::ostream&
 */
std::ostream &operator<<(std::ostream &osA, const as::ThreeOperand &opA);

#endif // THREEOPERAND_H
