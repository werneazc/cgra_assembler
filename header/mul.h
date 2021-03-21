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

#ifndef MUL_H
#define MUL_H

#include "iarithmetic.h"
#include "parseobjbase.h"

namespace as
{

/**
 * @class Mul
 *
 * @brief Multiply two variables and store result in m_first.
 *
 */
class Mul : public IArithmetic
{
  public:
    /**
     * @brief General constructor
     *
     * @param lvlA Level pointer where the add command exists in assembler file.
     * @param cmdLineA Command line in assembler file.
     * @param lineNumberA Line number in assembler file where the command is placed.
     * @param firstA Pointer to first variable for multiplication.
     * @param secondA Pointer to second variable for multiplication.
     */
    Mul(Level *lvlA, const std::string &cmdLineA, const uint32_t lineNumberA, ParseObjBase *const firstA = nullptr,
        ParseObjBase *const secondA = nullptr);

    /**
     * @brief Copy constructor
     *
     * @param srcA Source to initialize members of new object.
     */
    Mul(const Mul &srcA);

    /**
     * @brief Move constructor
     *
     * @param srcA Source to initialize members of new object. Members of source are deleted.
     */
    Mul(Mul &&srcA);

    /**
     * @brief Copy assignment
     *
     * @param rhsA Source for assignment to new object.
     * @return Object with values of rhsA.
     */
    Mul &operator=(const Mul &rhsA);

    /**
     * @brief Move assignment
     *
     * @param rhsA Source for assignment of member variables. Members of lhs are deleted.
     * @return Object with values of rhsA.
     */
    Mul &operator=(Mul &&rhsA);

    /**
     * @brief Destructor
     */
    virtual ~Mul() = default;

    /**
     * @brief Delete all members of object.
     */
    virtual void clearMembers() override;

    /**
     * @brief Multiply two variables and store result m_first.
     *
     * @throws AssemblerException if one of the operands is not a variable object.
     *
     * @return Always true (unused)
     */
    virtual bool processOperation() override;

  private:
    // Forbidden constructor
    Mul() = delete;
};

} /* End namespace as */

/**
 * @brief Dump information about add-operand instance
 *
 * @param[in] osA   Output stream to write to
 * @param[in] opA   Reference to add operand that shall be dumped
 */
std::ostream &operator<<(std::ostream &osA, const as::Mul &opA);

#endif // MUL_H
