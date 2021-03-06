/*
 * <one line to give the program's name and a brief idea of what it does.>
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

#ifndef LOOP_H
#define LOOP_H

#include "level.h"
#include <boost/property_tree/ptree.hpp>
#include <cstdint>
#include <iostream>

namespace as
{

/**
 * @class Loop
 *
 * @brief Special level for the usability of loops in assembler files.
 *
 * @details
 * Loops store their internal parsed objects as a new level instance.
 */
class Loop : public Level
{
  public:
    /**
     * @brief General constructor
     *
     * @throws AssemblerException if stepwith is zero or if range conditions are wrong.
     *
     *
     * @param[in] parentLvlA Ptr. to parent level of Loop.
     * @param[in] fileLineA File line number of assembler file.
     * @param[in] startValueA Start value of Loop index.
     * @param[in] endValueA End value of Loop index.
     * @param[in] stepWidthA Stepwidth to adapt Loop index for each iteration.
     * @param[in] readCmdA Source code line of assembler file (human readable).
     */
    Loop(Level *const parentLvlA, const uint32_t fileLineA, ParseObjBase *startValueA, ParseObjBase *endValueA,
         ParseObjBase *stepWidthA, const std::string readCmdA);

    /**
     * @brief Move constructor
     *
     * @param[in] srcA Base to construct new Loop object. Members of source are deleted.
     */
    Loop(Loop &&srcA);

    /**
     * @brief Loop assigment operator
     *
     * @param[in] rhsA Base to construct new Loop object. Members of rhsA are deleted.
     * @return New Loop object with content of rhsA.
     */
    Loop &operator=(Loop &&rhsA);

    /**
     * @brief Destuctor
     */
    virtual ~Loop(void) = default;

    /**
     * @brief Comparison operator (Equal)
     *
     * @param lhsA
     * @param rhsA
     * @return True, if all members of lhsA and rhsA are identically.
     */
    friend bool operator==(const Loop &lhsA, const Loop &rhsA);

    /**
     * @brief Update loop index by stepwidth and check range conditions
     *
     * @return True while end of loop is not reached, false otherwise.
     */
    bool updateLoopIndex(void);

    /**
     * @brief Get current index value of loop
     *
     * @return Copy on current index value
     */
    inline int32_t getCurrentIndexValue(void)
    {
        return m_currentValue;
    }

    /**
     * @brief Interface function to implement assemble functionality.
     *
     * @param ptreeA Assembler object containing VCGRA configuration parameter
     */
    std::ostream &assemble(const boost::property_tree::ptree &ptreeA, std::ostream &osA = std::cout);

  private:
    // Forbidden Constructors
    Loop(void) = delete;
    Loop(const Loop &srcA) = delete;
    Loop &operator=(const Loop &rhsA) = delete;

    // Private Members
    std::string m_readCommandLine;
    //!< @brief Line in assembler file to declare Loop conditions.
    uint32_t m_fileLine;
    //!< @brief File line number in assembler file where loop is declared.
    int32_t m_currentValue;
    //!< @brief Current value of loop index.
    ParseObjBase *m_stepWidth;
    //!< @brief Stepwidth for loop index update.
    ParseObjBase *m_startValue;
    //!< @brief Start value for index loop of range.
    ParseObjBase *m_endValue;
    //!< @brief End value for index loop of range.
};

} /* End namespace as */

#endif // LOOP_H
