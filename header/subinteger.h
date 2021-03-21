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

#ifndef SUBINTEGER_H
#define SUBINTEGER_H

#include "iarithmetic.h"
#include "parseobjbase.h"

namespace as
{

/**
 * @class SubInteger
 *
 * @brief arithmetic operation to add a constand value to a variable.
 */
class SubInteger : public IArithmetic
{
  public:
    /**
     * @brief General constructor
     *
     * @param lvlA Level where the parsed object is defined.
     * @param cmdLineA Assembler command line as a string.
     * @param lineNumberA Line number of assembler file where the parse object is defined.
     * @param firstA Pointer to first arithmetic operand (default=nullptr).
     * @param secondA Pointer to second arithmetic operand (default=nullptr).
     */
    SubInteger(Level *lvlA, const std::string &cmdLineA, const uint32_t &lineNumberA, ParseObjBase *firstA = nullptr,
               ParseObjBase *secondA = nullptr);

    /**
     * @brief Copy constructor
     *
     * @param srcA Source for generated copy.
     */
    SubInteger(const SubInteger &srcA);

    /**
     * @brief Copy assignment
     *
     * @param rhsA Source for new object. Members of source are deleted.
     * @return New parsed object
     */
    SubInteger &operator=(const SubInteger &rhsA);

    /**
     * @brief Move constructor
     *
     * @param srcA Members of srcA are deleted after copying.
     */
    SubInteger(SubInteger &&srcA);

    /**
     * @brief Move assignment
     *
     * @param rhsA Members of lhsA are deleted after copying.
     * @return Object containing values of rhsA.
     */
    SubInteger &operator=(SubInteger &&rhsA);

    /**
     * @brief Destructor
     */
    virtual ~SubInteger() = default;

    /**
     * @brief Add m_first and m_second. Result stored at m_first.
     *
     * @throws AssemblerException if dynamic_cast failed.
     *
     * @return always true (return unused).
     */
    virtual bool processOperation(void) override;

    /**
     * @brief Clear members of integer addition instance.
     *
     */
    virtual void clearMembers(void) override;

  private:
    // Forbidden constructor
    SubInteger() = delete;
};

} /* End namespace as */

/**
 * @brief Dump information about add-operand instance
 *
 * @param[in] osA   Output stream to write to
 * @param[in] opA   Reference to add-integer operand that shall be dumped
 */
std::ostream &operator<<(std::ostream &osA, const as::SubInteger &opA);

#endif // SUBINTEGER_H
