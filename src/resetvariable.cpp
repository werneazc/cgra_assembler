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
#include "parseobjectconst.h"
#include "parseobjectvariable.h"
#include <sstream>
#include <utility>

as::ResetVariable::ResetVariable(as::Level *const lvlA, const std::string &cmdLineA, const uint32_t lineNumberA,
                                 as::ParseObjBase *const valHandleA, as::ParseObjBase *const varhandleA)
    : as::ParseObjBase{lvlA, as::COMMANDCLASS::RESETVAR, cmdLineA, lineNumberA}, m_varHandle(varhandleA),
      m_valHandle(valHandleA)
{
    return;
}

as::ResetVariable::ResetVariable(const as::ResetVariable &srcA)
    : as::ParseObjBase{srcA.getLevel(), srcA.getCommandClass(), srcA.getReadCmdLine(), srcA.getFileLineNumber()}
{
    this->m_valHandle = srcA.m_valHandle;
    this->m_varHandle = srcA.m_varHandle;
    return;
}

as::ResetVariable::ResetVariable(as::ResetVariable &&srcA)
    : as::ParseObjBase{srcA.getLevel(), srcA.getCommandClass(), srcA.getReadCmdLine(), srcA.getFileLineNumber()}
{
    this->m_valHandle = srcA.m_valHandle;
    this->m_varHandle = srcA.m_varHandle;
    srcA.clearMembers();

    return;
}

as::ResetVariable &as::ResetVariable::operator=(as::ResetVariable &&rhsA)
{
    *(static_cast<ParseObjBase *>(this)) = std::move(static_cast<ParseObjBase &>(rhsA));
    this->m_valHandle = rhsA.m_valHandle;
    this->m_varHandle = rhsA.m_varHandle;

    rhsA.clearMembers();

    return *this;
}

as::ResetVariable &as::ResetVariable::operator=(const as::ResetVariable &rhsA)
{
    *(static_cast<ParseObjBase *>(this)) = static_cast<const ParseObjBase &>(rhsA);
    this->m_valHandle = rhsA.m_valHandle;
    this->m_varHandle = rhsA.m_varHandle;

    return *this;
}

void as::ResetVariable::clearMembers(void)
{
    static_cast<ParseObjBase *const>(this)->clearMembers();
    m_valHandle = nullptr;
    m_varHandle = nullptr;

    return;
}

void as::ResetVariable::resetVariable()
{
    if (m_valHandle && m_varHandle)
        {
            if (m_valHandle->getCommandClass() == as::COMMANDCLASS::CONSTANT)
                {
                    auto t_val = static_cast<as::ParseObjectConst *>(m_valHandle)->getConstValue();
                    static_cast<as::ParseObjectVariable *>(m_varHandle)->setVariableValue(t_val);
                }
            else if (m_valHandle->getCommandClass() == as::COMMANDCLASS::VARIABLE)
                {
                    auto t_val = static_cast<as::ParseObjectVariable *>(m_valHandle)->getVariableValue();
                    static_cast<as::ParseObjectVariable *>(m_varHandle)->setVariableValue(t_val);
                }
        }

    return;
}

std::ostream &operator<<(std::ostream &osA, const as::ResetVariable &opA)
{
    osA << static_cast<const as::ParseObjBase &>(opA) << "; ";
    return osA;
}