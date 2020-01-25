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

#include "parseobjectvariable.h"

namespace as
{

ParseObjectVariable::ParseObjectVariable(const std::string &nameA, const int32_t valueA, Level *lvlA,
                                         const std::string &cmdLineA, uint32_t lineNumberA)
    : ParseObjBase{lvlA, COMMANDCLASS::VARIABLE, cmdLineA, lineNumberA}, m_name{nameA}, m_value{valueA}
{
}

ParseObjectVariable::ParseObjectVariable(const ParseObjectVariable &src)
    : ParseObjBase{src.getLevel(), src.getCommandClass(), src.getReadCmdLine(), src.getFileLineNumber()}
{
    m_name = src.m_name;
    m_value = src.m_value;

    return;
}

ParseObjectVariable &ParseObjectVariable::operator=(const ParseObjectVariable &src)
{
    ParseObjBase t_pObj{src.getLevel(), src.getCommandClass(), src.getReadCmdLine(), src.getFileLineNumber()};

    *(static_cast<ParseObjBase *>(this)) = std::move(t_pObj);

    m_name = src.m_name;
    m_value = src.m_value;

    return *this;
}

ParseObjectVariable::ParseObjectVariable(ParseObjectVariable &&src)
    : ParseObjBase{src.getLevel(), src.getCommandClass(), src.getReadCmdLine(), src.getFileLineNumber()}
{
    m_name = src.m_name;
    m_value = src.m_value;

    src.clearMembers();

    return;
}

ParseObjectVariable &ParseObjectVariable::operator=(ParseObjectVariable &&src)
{

    ParseObjBase t_pObj{src.getLevel(), src.getCommandClass(), src.getReadCmdLine(), src.getFileLineNumber()};

    *(static_cast<ParseObjBase *>(this)) = std::move(t_pObj);

    m_name = src.m_name;
    m_value = src.m_value;

    src.clearMembers();

    return *this;
}

const std::string &ParseObjectVariable::getVariableName() const
{
    return m_name;
}

int32_t ParseObjectVariable::getVariableValue() const
{
    return m_value;
}

void ParseObjectVariable::setVariableValue(const int32_t valueA)
{
    m_value = valueA;
}

void ParseObjectVariable::clearMembers()
{
    ParseObjBase::clearMembers();
    m_name.clear();
    m_value = 0;

    return;
}

bool operator==(const ParseObjectVariable &lhsA, const std::string &nameA)
{
    return (lhsA.getVariableName() == nameA) ? true : false;
}

} /* End namespace as */

std::ostream &operator<<(std::ostream &osA, const as::ParseObjectVariable &objA)
{
    osA << static_cast<const as::ParseObjBase &>(objA) << "; ";
    osA << objA.getVariableName() << "; " << objA.getVariableValue();

    return osA;
}