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

#include "resetvariable.h"
#include "parseobjectvariable.h"
#include <sstream>
#include <utility>

as::ResetVariable::ResetVariable(as::Level *const lvlA, const std::string &cmdLineA, const uint32_t lineNumberA,
                                 const int32_t valueA, as::ParseObjBase *const varhandleA)
    : as::ParseObjBase{lvlA, as::COMMANDCLASS::RESETVAR, cmdLineA, lineNumberA}, m_value(valueA),
      m_varHandle(varhandleA)
{
    return;
}

as::ResetVariable::ResetVariable(const as::ResetVariable &srcA)
    : as::ParseObjBase{srcA.getLevel(), srcA.getCommandClass(), srcA.getReadCmdLine(), srcA.getFileLineNumber()}
{
    return;
}

as::ResetVariable::ResetVariable(as::ResetVariable &&srcA)
    : as::ParseObjBase{srcA.getLevel(), srcA.getCommandClass(), srcA.getReadCmdLine(), srcA.getFileLineNumber()}
{
    this->m_value = srcA.m_value;
    this->m_varHandle = srcA.m_varHandle;
    srcA.clearMembers();

    return;
}

as::ResetVariable &as::ResetVariable::operator=(as::ResetVariable &&rhsA)
{
    *(static_cast<ParseObjBase *>(this)) = std::move(static_cast<ParseObjBase &>(rhsA));
    this->m_value = rhsA.m_value;
    this->m_varHandle = rhsA.m_varHandle;

    rhsA.clearMembers();

    return *this;
}

as::ResetVariable &as::ResetVariable::operator=(const as::ResetVariable &rhsA)
{
    *(static_cast<ParseObjBase *>(this)) = static_cast<const ParseObjBase &>(rhsA);
    this->m_value = rhsA.m_value;
    this->m_varHandle = rhsA.m_varHandle;

    return *this;
}

void as::ResetVariable::clearMembers(void)
{
    static_cast<ParseObjBase *const>(this)->clearMembers();
    m_value = UINT32_MAX;
    m_varHandle = nullptr;

    return;
}

void as::ResetVariable::resetVariable()
{
    static_cast<as::ParseObjectVariable *>(m_varHandle)->setVariableValue(m_value);
    return;
}

std::ostream &operator<<(std::ostream &osA, const as::ResetVariable &opA)
{
    osA << static_cast<const as::ParseObjBase &>(opA) << "; ";
    return osA;
}