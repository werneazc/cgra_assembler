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

ParseObjectConst::ParseObjectConst(int32_t valueA, Level* lvlA, const std::string& cmdLineA, const uint32_t lineNumberA) :
    as::ParseObjBase{lvlA, COMMANDCLASS::CONSTANT, cmdLineA, lineNumberA}, m_value{valueA}
{ return; }

ParseObjectConst::ParseObjectConst(const ParseObjectConst& srcA) :
    ParseObjBase{srcA.getLevel(), COMMANDCLASS::CONSTANT, srcA.getReadCmdLine(), srcA.getFileLineNumber()},
    m_value{srcA.getConstValue()}
{ return; }

ParseObjectConst::ParseObjectConst(ParseObjectConst&& srcA) :
    ParseObjBase{srcA.getLevel(), COMMANDCLASS::CONSTANT, srcA.getReadCmdLine(), srcA.getFileLineNumber()},
    m_value{srcA.getConstValue()}
{
    srcA.clearMembers();
    
    return;
}

ParseObjectConst& ParseObjectConst::operator=(const ParseObjectConst& srcA)
{
    
    *(dynamic_cast<ParseObjBase*>(this)) = dynamic_cast<const ParseObjBase&>(srcA);
    this->m_value = srcA.getConstValue();
    
    return *this;
}


ParseObjectConst& ParseObjectConst::operator=(ParseObjectConst&& srcA)
{
    
    
    *(dynamic_cast<ParseObjBase*>(this)) = std::move(dynamic_cast<ParseObjBase&>(srcA));
    this->m_value = srcA.getConstValue();
    
    srcA.clearMembers();
    
    return *this;
}

void ParseObjectConst::clearMembers(void)
{
    dynamic_cast<ParseObjBase*>(this)->clearMembers();
    m_value = INT32_MAX;
}

} // End namespace as
