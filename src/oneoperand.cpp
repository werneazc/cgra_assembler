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

#include <utility>
#include "oneoperand.h"

as::OneOperand::OneOperand(Level* const lvlA, const std::string& cmdLineA,
    const uint32_t lineNumberA, ParseObjBase* const firstA, const uint32_t machineIdA) :
    as::ParseObjBase{lvlA, as::COMMANDCLASS::ONEOPERANT, cmdLineA, lineNumberA}, 
    m_machnineCodeID{machineIdA}, m_first{firstA}
{ return; }

as::OneOperand::OneOperand(const as::OneOperand& srcA) :
    as::OneOperand{srcA.getLevel(), srcA.getReadCmdLine(),
        srcA.getFileLineNumber(), srcA.m_first, srcA.m_machnineCodeID}
{ return; }

as::OneOperand::OneOperand(as::OneOperand && srcA) :
    as::OneOperand{srcA.getLevel(), srcA.getReadCmdLine(),
        srcA.getFileLineNumber(), srcA.m_first, srcA.m_machnineCodeID}
{
    srcA.clearMembers();
    
    return;
}

as::OneOperand & as::OneOperand::operator=(const as::OneOperand& rhsA)
{
    *(static_cast<ParseObjBase*>(this)) = static_cast<const ParseObjBase&>(rhsA);
    this->m_first = rhsA.m_first;
    this->m_machnineCodeID = rhsA.m_machnineCodeID;
    
    return *this;
}

as::OneOperand & as::OneOperand::operator=(as::OneOperand && rhsA)
{
    *(static_cast<ParseObjBase*>(this)) = std::move(static_cast<ParseObjBase&>(rhsA));
    this->m_first = rhsA.m_first;
    this->m_machnineCodeID = rhsA.m_machnineCodeID;
    
    rhsA.clearMembers();
    
    return *this;
}

as::ParseObjBase *const as::OneOperand::setFirst(as::ParseObjBase *const firstA)
{
    auto t_first = m_first;
    m_first = firstA;
    
    return t_first;
}

as::ParseObjBase *const as::OneOperand::getFirst() const
{ return m_first; }

void as::OneOperand::clearMembers(void)
{
    static_cast<ParseObjBase*>(this)->clearMembers();
    m_first = nullptr;
    m_machnineCodeID = UINT32_MAX;
    
    return;
}


