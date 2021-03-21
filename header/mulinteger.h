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

#ifndef MULINTEGER_H
#define MULINTEGER_H

#include "iarithmetic.h"
#include "parseobjbase.h"

namespace as
{

/**
 * @class MulInteger
 *
 * @brief arithmetic operation to multiply a constand value with a variable.
 */
class MulInteger : public IArithmetic
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
    MulInteger(Level *lvlA, const std::string &cmdLineA, const uint32_t &lineNumberA, ParseObjBase *firstA = nullptr,
               ParseObjBase *secondA = nullptr);

    /**
     * @brief Copy constructor
     *
     * @param srcA Source for generated copy.
     */
    MulInteger(const MulInteger &srcA);

    /**
     * @brief Copy assignment
     *
     * @param rhsA Source for new object. Members of source are deleted.
     * @return New parsed object
     */
    MulInteger &operator=(const MulInteger &rhsA);

    /**
     * @brief Move constructor
     *
     * @param srcA Members of srcA are deleted after copying.
     */
    MulInteger(MulInteger &&srcA);

    /**
     * @brief Move assignment
     *
     * @param rhsA Members of lhsA are deleted after copying.
     * @return Object containing values of rhsA.
     */
    MulInteger &operator=(MulInteger &&rhsA);

    /**
     * @brief Destructor
     */
    virtual ~MulInteger() = default;

    /**
     * @brief Multiply m_first and m_second. Result stored at m_first.
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
    MulInteger() = delete;
};

} /* End namespace as */

/**
 * @brief Dump information about add-operand instance
 *
 * @param[in] osA   Output stream to write to
 * @param[in] opA   Reference to add-integer operand that shall be dumped
 */
std::ostream &operator<<(std::ostream &osA, const as::MulInteger &opA);

#endif // MULINTEGER_H
