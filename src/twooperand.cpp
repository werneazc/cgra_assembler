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
#include <boost/tokenizer.hpp>
#include "myException.h"
#include "twooperand.h"
#include "parseobjectconst.h"
#include "parseobjectvariable.h"


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

std::string as::TwoOperand::assemble(const boost::property_tree::ptree& ptreeA)
{
    //Temporary variables
    std::ostringstream t_os{""}; //Formated machine code command
    auto t_OpcodeSize = ptreeA.get<uint32_t>("Assembler_Property.OpCodeSize"); //Opcode size in machine code signal
    auto t_PlaceSize = ptreeA.get<uint32_t>("Assembler_Property.PlaceSize"); //Places size in machine code signal
    auto t_MemorySize = ptreeA.get<uint32_t>("VCGRA_Property.Available_Memory"); //Available memory space for shared memory
    uint32_t t_numOfAvailableLines{0}; //Number of available cache lines for range check of argument of command
    uint32_t t_countVal{0}; //Counter to select write property for available cache lines
    uint32_t t_valFirst{0}, t_valSecond{0}; //Parameter value of command
    
    /*#TODO: This separates the values of configuration file parameter "TwoOperator" is a list.
     * It seems, that the ordering of the values depends on the order in the configuration file.
     * Thus, reading out the write properties for cache line size needs to be updated for an independent order.
     */
    auto t_opt = boost::tokenizer<boost::escaped_list_separator<char>>(ptreeA.get<std::string>("Assembler_Property.TwoOperator"));
    
    //Search for parameter of available cache lines for actual command
    for(const auto& opt : t_opt)
        {
            if(ptreeA.get<uint32_t>(opt) == m_machineCodeId)
            {
                switch(t_countVal)
                {
                    case 0:
                        t_numOfAvailableLines = ptreeA.get<uint32_t>("VCGRA_Property.Num_Dic_Lines");
                        break;
                    case 1:
                        t_numOfAvailableLines = ptreeA.get<uint32_t>("VCGRA_Property.Num_Doc_Lines");
                        break;
                    case 2:
                        t_numOfAvailableLines = ptreeA.get<uint32_t>("VCGRA_Property.Num_PC_Lines");
                        break;
                    case 3:
                        t_numOfAvailableLines = ptreeA.get<uint32_t>("VCGRA_Property.Num_CC_Lines");
                        break;
                    default:
                        throw as::AssemblerException("Unknown two-operation-command. Cannot select number of cache lines.", 8712);
                        break;
                }
                break;
            }
            else
                ++t_countVal;
                
        }
    
    //Get value of first operator (address)
    if(m_first->getCommandClass() == as::COMMANDCLASS::CONSTANT)
            t_valFirst = static_cast<as::ParseObjectConst*>(this->m_first)->getConstValue();
    else if(m_first->getCommandClass() == as::COMMANDCLASS::VARIABLE)
            t_valFirst = static_cast<as::ParseObjectVariable*>(this->m_first)->getVariableValue();
    else
        throw as::AssemblerException("Wrong parameter format for two-operation-command.\n \
        Expected constant integer value or integer variable for first operand", 8713);
    //Get value of second parameter (cache line)
    if(m_second->getCommandClass() == as::COMMANDCLASS::CONSTANT)
            t_valSecond = static_cast<as::ParseObjectConst*>(this->m_second)->getConstValue();
    else if(m_second->getCommandClass() == as::COMMANDCLASS::VARIABLE)
            t_valSecond = static_cast<as::ParseObjectVariable*>(this->m_second)->getVariableValue();
    else
        throw as::AssemblerException("Wrong parameter format for two-operation-command.\n \
        Expected constant integer value or integer variable for second operator", 8714);
    
    /*Check ranges of parameters.
     * #TODO: For address in shared memory, control of memory size needs to be extended by
     * address + size of a cache line.
     */
    if(t_valSecond < t_numOfAvailableLines && t_valFirst < t_MemorySize)
    {
        t_valSecond <<= (t_OpcodeSize + t_PlaceSize);
        t_valSecond |= 127 << t_OpcodeSize; 
        t_valSecond |= m_machineCodeId;
    }
    else
        throw as::AssemblerException("Selected cache line is not available.", 8715);
    
    t_os << "\"" << m_fmtStr % t_valFirst % t_valSecond << "\"";
    
    return t_os.str();
}

void as::TwoOperand::clearMembers()
{
    static_cast<ParseObjBase*>(this)->clearMembers();
    this->m_first = nullptr;
    this->m_second = nullptr;
    this->m_machineCodeId = UINT32_MAX;
    
    return;
}
