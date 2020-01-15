/*
 * Copyright (C) 2019  andrewerner <andre.werner-w2m@ruhr-uni-bochum.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *invalid_argument
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../header/parseobjbase.h"
#include <stdexcept>


namespace as {

ParseObjBase::ParseObjBase(Level* levelA, COMMANDCLASS cmdA, const std::string& cmdLineA, const uint32_t lineNumberA) :
    m_class{cmdA}, m_cmdLine{cmdLineA}, m_lineNumber{lineNumberA}
{
    if(levelA)
        m_level = levelA;
    else
        throw std::invalid_argument{"Pointer to current level is invalid."};
    
    return;
}



ParseObjBase::ParseObjBase(const as::ParseObjBase& src)
{
    this->m_level = src.m_level;
    this->m_class = src.m_class;
    this->m_cmdLine = src.m_cmdLine;
    this->m_lineNumber = src.m_lineNumber;
    
    return;
}

ParseObjBase::ParseObjBase(as::ParseObjBase && src)
{
    this->m_level = src.m_level;
    src.m_level = nullptr;
    this->m_cmdLine = src.m_cmdLine;
    src.m_cmdLine.clear();
    this->m_lineNumber = src.m_lineNumber;
    src.m_lineNumber = 0;
    this->m_class = src.m_class;
    src.m_class = COMMANDCLASS::UNKNOWN;
    
    return;
}

ParseObjBase& ParseObjBase::operator=(const ParseObjBase& src)
{
    
    this->m_level = src.m_level;
    this->m_class = src.m_class;
    this->m_cmdLine = src.m_cmdLine;
    this->m_lineNumber = src.m_lineNumber;
    
    return *this;
}

ParseObjBase& ParseObjBase::operator=(as::ParseObjBase && src)
{
    
    this->m_level = src.m_level;
    src.m_level = nullptr;
    this->m_cmdLine = src.m_cmdLine;
    src.m_cmdLine.clear();
    this->m_lineNumber = src.m_lineNumber;
    src.m_lineNumber = 0;
    this->m_class = src.m_class;
    src.m_class = COMMANDCLASS::UNKNOWN;
    
    return *this;
}

std::string ParseObjBase::getReadCmdLine() const
{
    return m_cmdLine;
}

uint32_t ParseObjBase::getFileLineNumber() const
{
    return m_lineNumber;
}

Level* ParseObjBase::getLevel() const
{
    return m_level;
}

COMMANDCLASS ParseObjBase::getCommandClass() const
{
    return m_class;
}

void ParseObjBase::clearMembers()
{
    m_class = COMMANDCLASS::UNKNOWN;
    m_cmdLine.clear();
    m_level = nullptr;
    m_lineNumber = -1;
    
    return;
    
}

std::string ParseObjBase::setCmdReadLine(const std::string& newA)
{
    auto t_str = m_cmdLine;
    m_cmdLine = newA;
    
    return t_str;
    
}

as::COMMANDCLASS ParseObjBase::setCommandClass(as::COMMANDCLASS newA)
{
    auto t_class = m_class;
    m_class = newA;
    
    return t_class;
}

Level* const ParseObjBase::setLevel(const as::Level* newA)
{
    auto* t_lvl = m_level;
    m_level = const_cast<Level*>(newA);
    
    return t_lvl;
}

u_int32_t ParseObjBase::setLineNumber(const u_int32_t newA)
{
    auto t_num = m_lineNumber;
    m_lineNumber = newA;
    
    return t_num;
}


} /* End namespace as */

namespace utils {

std::string commandclassToString(as::COMMANDCLASS classA)
{
    switch (classA)
    {
    case as::COMMANDCLASS::ARITHMETIC:
        return "arithmetic";
    case as::COMMANDCLASS::CONSTANT:
        return "constant";
    case as::COMMANDCLASS::NOOPERANT:
        return "no-operant";
    case as::COMMANDCLASS::ONEOPERANT:
        return "one-operant";
    case as::COMMANDCLASS::THREEOPERANT:
        return "three-operant";
    case as::COMMANDCLASS::TWOOPERANT:
        return "two-operant";
    case as::COMMANDCLASS::VARIABLE:
        return "variable";
    default:
        return "unknown";
    }
}

} // End namespace utils

std::ostream& operator<<(std::ostream& osA, const as::ParseObjBase& objA)
{
    osA << objA.getFileLineNumber() << ": ";
    osA << ::utils::commandclassToString(objA.getCommandClass());

    return osA;
}