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

#ifndef RESETVARIABLE_H
#define RESETVARIABLE_H

#include "iassemble.h"
#include "parseobjbase.h"
#include <cstdint>

namespace as
{

// Forward declaration
class Level;

/**
 * @class ResetVariable
 *
 * @brief Parse object for variables that are set with new value more than once.
 *
 * @details
 * This class is necessary, if a variable is used multiple times and gets a new value
 * by creating several line like VAR variable value
 */
class ResetVariable : public ParseObjBase
{
  public:
    /**
     * @brief General constructor
     *
     * @param lvlA Level where the parse object exists.
     * @param cmdLineA Assembler file line with command.
     * @param lineNumberA Assembler file line number.
     * @param valHandleA Handle for reset value for variable
     * @param varHandleA Handle for variable to control
     */
    ResetVariable(Level *const lvlA, const std::string &cmdLineA, const uint32_t lineNumberA, as::ParseObjBase *const valHandleA,
                  as::ParseObjBase *const varHandleA);

    /**
     * @brief Copy constructor
     *
     * @param srcA Source for members of new object.
     */
    ResetVariable(const ResetVariable &srcA);

    /**
     * @brief Move constructor
     *
     * @param srcA Source for members of new object. Members of source are deleted.
     */
    ResetVariable(ResetVariable &&srcA);

    /**
     * @brief Copy assignment
     *
     * @param rhsA Source for members of assigned object.
     * @return Object with member values of rhsA.
     */
    ResetVariable &operator=(const ResetVariable &rhsA);

    /**
     * @brief Move assignment
     *
     * @param rhsA Source for members of new object. Members of source are deleted.
     * @return Object with member values of rhsA.
     */
    ResetVariable &operator=(ResetVariable &&rhsA);

    /**
     * @brief Destructor
     */
    virtual ~ResetVariable(void) = default;

    /**
     * @brief Delete all member variables.
     */
    virtual void clearMembers(void) override;

    /**
     * @brief Set value for referenced variable
     */
    void resetVariable();

  private:
    // Forbidden constructor
    ResetVariable() = delete;

    // Private member variables
    /** @brief Handle to variable to reset */
    as::ParseObjBase *m_varHandle;
    /** @brief New value to set for handled variable */
    as::ParseObjBase *m_valHandle;
};

} /* End namespace as */

/**
 * @brief Dump information about parsed no operand instance
 *
 * @param[in] osA       Outstream to write to
 * @param[in] opA       Reference to no-operand instance that shall be dumped
 */
std::ostream &operator<<(std::ostream &osA, const as::ResetVariable &opA);

#endif // RESETVARIABLE_H
