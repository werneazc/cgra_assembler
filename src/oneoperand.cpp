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
#include "oneoperand.h"
#include "parseobjectconst.h"
#include "parseobjectvariable.h"

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

std::string as::OneOperand::assemble(const boost::property_tree::ptree& ptreeA)
{
    //Tempoarary variables
    std::ostringstream t_os{""}; //Formated machine code command
    auto t_OpcodeSize = ptreeA.get<uint32_t>("Assembler_Property.OpCodeSize"); //Opcode size in machine code signal
    auto t_PlaceSize = ptreeA.get<uint32_t>("Assembler_Property.PlaceSize"); //Places size in machine code signal
    uint32_t t_numOfAvailableLines{0}; //Number of available cache lines for range check of argument of command
    uint32_t t_countVal{0}; //Counter to select write property for available cache lines
    uint32_t t_val{0}; //Parameter value of command
    
    /*#TODO: This separates the values of configuration file parameter "OneOperator" is a list.
     * It seems, that the ordering of the values depends on the order in the configuration file.
     * Thus, reading out the write properties for cache line size needs to be updated for an independent order.
     */
    auto t_opt = boost::tokenizer<boost::escaped_list_separator<char>>(ptreeA.get<std::string>("Assembler_Property.OneOperator"));
    
    //Search for parameter of available cache lines for actual command
    for(const auto& opt : t_opt)
        {
            if(ptreeA.get<uint32_t>(opt) == m_machnineCodeID)
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
                        throw as::AssemblerException("Unknown one-operation-command. Cannot select number of cache lines.", 8612);
                        break;
                }
                break;
            }
            else
                ++t_countVal;
                
        }
    
    if(m_first->getCommandClass() == as::COMMANDCLASS::CONSTANT)
            t_val = static_cast<as::ParseObjectConst*>(this->m_first)->getConstValue();
    else if(m_first->getCommandClass() == as::COMMANDCLASS::VARIABLE)
            t_val = static_cast<as::ParseObjectVariable*>(this->m_first)->getVariableValue();
    else
        throw as::AssemblerException("Wrong parameter format for one-operation-command.\n \
        Expected constant integer value or integer variable", 8613);
    
    if(t_val <= t_numOfAvailableLines)
    {
        t_val <<= (t_OpcodeSize + t_PlaceSize);
        t_val |= m_machnineCodeID;
    }
    else
        throw as::AssemblerException("Selected cache line is not available.", 8614);
    
    t_os << "\"" << m_fmtStr % 0u % t_val << "\"";
    
    return t_os.str();
}


std::ostream& operator<<(std::ostream& osA, const as::OneOperand& opA)
{
    osA << static_cast<const as::ParseObjBase&>(opA) << "; operand: ";
    
    const auto* t_first = opA.getFirst();
    
    if(t_first->getCommandClass() == as::COMMANDCLASS::CONSTANT) {
        osA << *(static_cast<const as::ParseObjectConst*>(t_first));
    }
    else if(t_first->getCommandClass() == as::COMMANDCLASS::VARIABLE) {
        osA << *(static_cast<const as::ParseObjectVariable*>(t_first));
    }
    else{
        osA << *t_first;
    }
    
    return osA;
}

