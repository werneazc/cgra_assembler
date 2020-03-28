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

#ifndef IARITHMETIC_H
#define IARITHMETIC_H

#include "parseobjbase.h"
#include <utility>

namespace as
{

/**
 * @interface IArithmetic
 *
 * @brief Defines interface of arithmetic assembler operations.
 *
 * @details
 * Pure virtual function 'processOperation' needs to be defined in arithmetic
 * operation classes for a specific operation.
 */
class IArithmetic : public ParseObjBase
{
  public:
    typedef std::pair<ParseObjBase *const, ParseObjBase *const> operator_type_t;
    //!< @brief Return type for the pair of operands.

    /**
     * @brief Empty constructor
     */
    IArithmetic();

    /**
     * @brief General constructor
     *
     * @param[in,out] lvlA Level where the parsed object is defined.
     * @param[in] cmdLineA Assembler command line as a string.
     * @param[in] lineNumberA Line number of assembler file where the parse object is defined.
     * @param[in] firstA Ptr. to first element of arithmetic operation; stores also result of operation.
     * @param[in] secondA Ptr. to second element of arithmetic operation.
     */

    IArithmetic(Level *lvlA, const std::string &cmdLineA, const uint32_t &lineNumberA, ParseObjBase *const firstA,
                ParseObjBase *const secondA);
    /**
     * @brief Copy constructor
     *
     * @param[in] src Initial values for members of new object.
     */
    IArithmetic(const IArithmetic &src);

    /**
     * @brief Move constructor
     *
     * @param src Initial values for members of new element. Members of source are deleted.
     */
    IArithmetic(IArithmetic &&src);

    /**
     * @brief Copy assignment
     *
     * @param[in] src Initial values for members of new object.
     * @return new object
     */
    IArithmetic &operator=(const IArithmetic &src);

    /**
     * @brief Move assignment
     *
     * @param src Initial values for members of new element. Members of source are deleted.
     * @return new object
     */
    IArithmetic &operator=(IArithmetic &&src);

    /**
     * @brief Destructor
     */
    virtual ~IArithmetic(void) = default;

    /**
     * @brief Performs an arithmetic operation. Result is stored at m_first.
     *
     * @return True indicates a successful operation, false a failure.
     */
    virtual bool processOperation() = 0;

    /**
     * @brief Get a constant pointer to first operand.
     *
     * @return ParseObjBase* const
     */
    ParseObjBase *const getFirst(void) const;

    /**
     * @brief Set new pointer to first operand.
     *
     * @param firstA Pointer to new first operand.
     * @return Pointer to previous first operand.
     */
    ParseObjBase *const setFirst(ParseObjBase *const firstA);

    /**
     * @brief Get a constant pointer to second operand.
     *
     * @return ParseObjBase* const
     */
    ParseObjBase *const getSecond(void) const;

    /**
     * @brief Set new pointer to second operand.
     *
     * @param secondA Pointer to new second operand.
     * @return Pointer to previous second operand.
     */
    ParseObjBase *const setSecond(ParseObjBase *const secondA);

    /**
     * @brief Return operand pair with pointer to both operands.
     *
     * @return as::IArithmetic::operator_type_t
     */
    operator_type_t getOperandPair(void);

    /**
     * @brief Set new pointers for both operands.
     *
     * @param operandsA Pair of pointers to new operands (first, second).
     * @return A pair pointer to the old operands.
     */
    operator_type_t setOperandPair(const operator_type_t &operandsA);

    /**
     * @brief Set new pointers for both operands
     *
     * @param firstA Pointer to new first operand.
     * @param secondA Pointer to new second operand.
     * @return A pair pointer to the old operands.
     */
    operator_type_t setOperandPair(ParseObjBase *const firstA, ParseObjBase *const secondA);

  private:
    // Class member
    ParseObjBase *m_first;
    //!< @brief First operand of arithmetic operation which also stores the result.
    ParseObjBase *m_second;
    //!< @brief Second operand of arithmetic operation.
};

} /* End namespace as */

#endif // IARITHMETIC_H
