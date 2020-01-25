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

#ifndef TWOOPERAND_H
#define TWOOPERAND_H

#include "iassemble.h"
#include "parseobjbase.h"
#include <utility>

namespace as
{

/**
 * @class TwoOperand
 *
 * @brief Parse object for VCGRA assembler commands with two operands.
 *
 * @details
 * This parse object handles assembler commands with two operands like LOADPC
 * or LOADCC. It stores the machine code ID for the VCGRA command parser as
 * well as two pointers to the operands.
 */
class TwoOperand : public ParseObjBase, public IAssemble
{
  public:
    typedef std::pair<ParseObjBase *const, ParseObjBase *const> operands_type_t;
    //!< @brief Pair type for both operands of parse object.

    /**
     * @brief Standard constructor
     *
     * @param lvlA Pointer to actual level, where command exists.
     * @param cmdLineA Assembler file line of command.
     * @param lineNumberA Assembler file line number of command.
     * @param firstA Pointer to first operand.
     * @param secondA Pointer to second operand.
     * @param machineIdA VCGRA machine code ID for command parser at VCGRA.
     */
    TwoOperand(Level *const lvlA, const std::string &cmdLineA, const uint32_t lineNumberA, ParseObjBase *const firstA,
               ParseObjBase *const secondA, const uint32_t machineIdA);

    /**
     * @brief Copy constructor
     *
     * @param srcA Source to initialize members of new object.
     */
    TwoOperand(const TwoOperand &srcA);

    /**
     * @brief Move constructor
     *
     * @param srcA Source to initialize members of new object. Members of source are deleted.
     */
    TwoOperand(TwoOperand &&srcA);

    /**
     * @brief Destructor
     */
    virtual ~TwoOperand(void) = default;

    /**
     * @brief Copy assignment
     *
     * @param rhsA Source for object members to assign.
     * @return Object with member values of rhsA.
     */
    TwoOperand &operator=(const TwoOperand &rhsA);

    /**
     * @brief Move assignment
     *
     * @param rhsA Source for object members to assign. Member values of rhsA are deleted.
     * @return Object with member values of rhsA.
     */
    TwoOperand &operator=(TwoOperand &&rhsA);

    /**
     * @brief Get object handle for first operand.
     *
     * @return Constant pointer to first operand (base class).
     */
    ParseObjBase *const getFirst(void) const;

    /**
     * @brief Set object handle for first operand.
     *
     * @param firstA Constant pointer to new first operand.
     * @return Constant pointer to previous first operand.
     */
    ParseObjBase *const setFirst(ParseObjBase *const firstA);

    /**
     * @brief Get object handle for second operand.
     *
     * @return Constant pointer to second operand (base class)
     */
    ParseObjBase *const getSecond(void) const;

    /**
     * @brief Set object handle for second operand.
     *
     * @param secondA Constant pointer to new second operand.
     * @return Constant pointer to previous second operand.
     */
    ParseObjBase *const setSecond(ParseObjBase *const secondA);

    /**
     * @brief Get a pair of object handles for both, first and second operand.
     *
     * @return A pair of constant pointers for first and second operand.
     */
    operands_type_t getOperands(void) const;

    /**
     * @brief Set both operands for parse object.
     *
     * @param operandsA A pair of constant pointers for both operands.
     * @return A pair of constant pointers to both previous operands.
     */
    operands_type_t setOperands(operands_type_t operandsA);

    /**
     * @brief Set both operands for parse object.
     *
     * @param firstA Handle for first operand.
     * @param secondA Handle for second operand.
     * @return A pair of constant pointers to both previous operands.
     */
    operands_type_t setOperands(ParseObjBase *const firstA, ParseObjBase *const secondA);

    /**
     * @brief Get ID for machine code of VCGRA command parser.
     *
     * @return Copy of stored machine code ID.
     */
    uint32_t getMachineCodeId(void) const;

    /**
     * @brief Set new machine code ID.
     *
     * @param machineIdA New machine code ID to store.
     * @return Previous machine code ID.
     */
    uint32_t setMachinenCodeId(const uint32_t machineIdA);

    /**
     * @brief Delete all members of parse object.
     */
    virtual void clearMembers() override;

    /**
     * @brief Create machine code from assembler command.
     *
     * @param ptreeA Property tree with configuration values from SW configuration file
     * @return Machine code line for VCGRA instance.
     */
    virtual std::string assemble(const boost::property_tree::ptree &ptreeA) override final;

  private:
    // Forbidden constructor
    TwoOperand() = delete;

    // Private member
    ParseObjBase *m_first;
    //!< @brief Handle to first operand.
    ParseObjBase *m_second;
    //!< @brief Handle to second operand.
    uint32_t m_machineCodeId;
    //!< @brief Machine code ID for VCGRA command parser.
};

} /* End namespace as */

/**
 * @brief Dump information about two-operand instance
 *
 * @param[in] osA   Output stream to write to
 * @param[in] opA   Reference to two operand that shall be dumped
 * @return std::ostream&
 */
std::ostream &operator<<(std::ostream &osA, const as::TwoOperand &opA);

#endif // TWOOPERAND_H
