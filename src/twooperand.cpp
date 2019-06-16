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
#include <sstream>
#include "twooperand.h"

as::TwoOperand::TwoOperand(const TwoOperand& srcA) : 
    as::TwoOperand{srcA.getLevel(), srcA.getReadCmdLine(), srcA.getFileLineNumber(), srcA.getFirst(), srcA.getSecond(), srcA.getMachineCodeId()}
{ 
    this->setCommandClass(srcA.getCommandClass());
    
    return; 
}

as::TwoOperand::TwoOperand(as::Level *const lvlA, const std::string& cmdLineA, const uint32_t lineNumberA, as::ParseObjBase *const firstA,
    as::ParseObjBase *const secondA, const uint32_t machineIdA) :
    as::ParseObjBase{lvlA, as::COMMANDCLASS::TWOOPERANT, cmdLineA, lineNumberA}, m_first{firstA}, m_second{secondA}, m_machineCodeId{machineIdA}
{ return; }

as::TwoOperand::TwoOperand(as::TwoOperand && srcA) : 
    as::TwoOperand{srcA.getLevel(), srcA.getReadCmdLine(), srcA.getFileLineNumber(), srcA.getFirst(), srcA.getSecond(), srcA.getMachineCodeId()}
{
    this->setCommandClass(srcA.getCommandClass());
    srcA.clearMembers();
    
    return;
}

as::TwoOperand & as::TwoOperand::operator=(const as::TwoOperand& rhsA)
{
    *(static_cast<ParseObjBase*>(this)) = static_cast<const ParseObjBase&>(rhsA);
    
    this->m_first = rhsA.getFirst();
    this->m_second = rhsA.getSecond();
    this->m_machineCodeId = rhsA.getMachineCodeId();
    
    return *this;
}

as::TwoOperand & as::TwoOperand::operator=(as::TwoOperand && rhsA)
{
    *(static_cast<ParseObjBase*>(this)) = std::move(static_cast<ParseObjBase&>(rhsA));
    
    this->m_first = rhsA.getFirst();
    this->m_second = rhsA.getSecond();
    this->m_machineCodeId = rhsA.getMachineCodeId();
    
    return *this;
}

as::ParseObjBase *const as::TwoOperand::getFirst() const
{ return m_first; }

as::ParseObjBase *const as::TwoOperand::setFirst(as::ParseObjBase *const firstA)
{
    auto t_ptr = this->m_first;
    this->m_first = firstA;
    
    return t_ptr;
}

as::ParseObjBase *const as::TwoOperand::getSecond() const
{ return m_second; }


as::ParseObjBase *const as::TwoOperand::setSecond(as::ParseObjBase *const secondA)
{
    auto t_ptr = this->m_second;
    this->m_second = secondA;
    
    return t_ptr;
}

as::TwoOperand::operands_type_t as::TwoOperand::getOperands() const
{ return  std::make_pair(m_first, m_second); }

as::TwoOperand::operands_type_t as::TwoOperand::setOperands(as::TwoOperand::operands_type_t operandsA)
{ return this->setOperands(operandsA.first, operandsA.second); }

as::TwoOperand::operands_type_t as::TwoOperand::setOperands(as::ParseObjBase *const firstA, as::ParseObjBase *const secondA)
{
    auto t_val = std::make_pair(this->m_first, this->m_second);
    
    this->m_first = firstA;
    this->m_second = secondA;
    
    return t_val;
}

uint32_t as::TwoOperand::getMachineCodeId() const
{ return m_machineCodeId; }

uint32_t as::TwoOperand::setMachinenCodeId(const uint32_t machineIdA)
{
    auto t_val = this->m_machineCodeId;
    this->m_machineCodeId = machineIdA;
    
    return t_val;
}

std::string as::TwoOperand::assemble(const as::Assembler& asA)
{
    std::ostringstream os{""};
    
    return os.str();
}

void as::TwoOperand::clearMembers()
{
    static_cast<ParseObjBase*>(this)->clearMembers();
    this->m_first = nullptr;
    this->m_second = nullptr;
    this->m_machineCodeId = UINT32_MAX;
    
    return;
}
