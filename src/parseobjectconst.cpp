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

#include "parseobjectconst.h"

namespace as {

ParseObjectConst::ParseObjectConst(const std::string& nameA, int32_t valueA, Level* lvlA, 
                                   const std::string& cmdLineA, const uint32_t lineNumberA) :
    as::ParseObjBase{lvlA, COMMANDCLASS::CONSTANT, cmdLineA, lineNumberA}, m_value{valueA}, m_name{nameA}
{ return; }

ParseObjectConst::ParseObjectConst(const ParseObjectConst& srcA) :
    ParseObjBase{srcA.getLevel(), COMMANDCLASS::CONSTANT, srcA.getReadCmdLine(), srcA.getFileLineNumber()},
    m_value{srcA.getConstValue()}, m_name{srcA.getConstName()}
{ return; }

ParseObjectConst::ParseObjectConst(ParseObjectConst&& srcA) :
    ParseObjBase{srcA.getLevel(), COMMANDCLASS::CONSTANT, srcA.getReadCmdLine(), srcA.getFileLineNumber()},
    m_value{srcA.getConstValue()}, m_name{srcA.getConstName()}
{
    srcA.clearMembers();
    
    return;
}

ParseObjectConst& ParseObjectConst::operator=(const ParseObjectConst& srcA)
{
    
    *(static_cast<ParseObjBase*>(this)) = static_cast<const ParseObjBase&>(srcA);
    this->m_value = srcA.getConstValue();
    this->m_name =srcA.getConstName();
    
    return *this;
}


ParseObjectConst& ParseObjectConst::operator=(ParseObjectConst&& srcA)
{
    
    
    *(static_cast<ParseObjBase*>(this)) = std::move(static_cast<ParseObjBase&>(srcA));
    this->m_value = srcA.getConstValue();
    this->m_name =srcA.getConstName();
    
    srcA.clearMembers();
    
    return *this;
}

void ParseObjectConst::clearMembers(void)
{
    ParseObjBase::clearMembers();
    m_value = INT32_MAX;
    m_name.clear();
}

bool operator==(const ParseObjectConst& lhsA, const std::string& nameA)
{
    return (lhsA.getConstName() == nameA) ? true : false;
}

const std::string& ParseObjectConst::getConstName() const
{ return m_name; }


const int32_t ParseObjectConst::getConstValue() const
{ return m_value; }

} // End namespace as

std::ostream& operator<<(std::ostream& osA, const as::ParseObjectConst& objA)
{
    osA << static_cast<const as::ParseObjBase&>(objA) << "; ";
    osA << objA.getConstName() << "; " << objA.getConstValue();

    return osA;
}

