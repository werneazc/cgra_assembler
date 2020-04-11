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

#include "threeoperand.h"
#include "myException.h"
#include "parseobjectconst.h"
#include "parseobjectvariable.h"
#include <boost/tokenizer.hpp>
#include <sstream>
#include <utility>

as::ThreeOperand::ThreeOperand(as::Level *const lvlA, const std::string &cmdLineA, const uint32_t lineNumberA,
                               as::ParseObjBase *const firstA, as::ParseObjBase *const secondA,
                               as::ParseObjBase *const thridA, const uint32_t machienIdA)
    : as::ParseObjBase{lvlA, as::COMMANDCLASS::THREEOPERAND, cmdLineA, lineNumberA}, m_first{firstA}, m_second{secondA},
      m_third{thridA}, m_machineCodeId{machienIdA}
{
    return;
}

as::ThreeOperand::ThreeOperand(const as::ThreeOperand &srcA)
    : as::ParseObjBase{srcA.getLevel(), srcA.getCommandClass(), srcA.getReadCmdLine(), srcA.getFileLineNumber()},
      m_first{srcA.m_first}, m_second{srcA.m_second}, m_third{srcA.m_third}, m_machineCodeId{srcA.m_machineCodeId}
{
    return;
}

as::ThreeOperand::ThreeOperand(as::ThreeOperand &&srcA)
    : as::ParseObjBase{srcA.getLevel(), srcA.getCommandClass(), srcA.getReadCmdLine(), srcA.getFileLineNumber()},
      m_first{srcA.m_first}, m_second{srcA.m_second}, m_third{srcA.m_third}, m_machineCodeId{srcA.m_machineCodeId}
{
    this->clearMembers();
    return;
}

as::ThreeOperand &as::ThreeOperand::operator=(const as::ThreeOperand &rhsA)
{
    *(static_cast<as::ParseObjBase *>(this)) = static_cast<const ParseObjBase &>(rhsA);
    m_first = rhsA.m_first;
    m_second = rhsA.m_second;
    m_third = rhsA.m_third;
    m_machineCodeId = rhsA.m_machineCodeId;

    return *this;
}

as::ThreeOperand &as::ThreeOperand::operator=(as::ThreeOperand &&rhsA)
{
    *(static_cast<as::ParseObjBase *>(this)) = std::move(static_cast<ParseObjBase &>(rhsA));
    m_first = rhsA.m_first;
    m_second = rhsA.m_second;
    m_third = rhsA.m_third;
    m_machineCodeId = rhsA.m_machineCodeId;

    rhsA.clearMembers();

    return *this;
}

void as::ThreeOperand::clearMembers()
{
    static_cast<as::ParseObjBase *>(this)->clearMembers();
    m_first = nullptr;
    m_second = nullptr;
    m_third = nullptr;
    m_machineCodeId = UINT32_MAX;

    return;
}

uint32_t as::ThreeOperand::getMachineCodeId() const
{
    return m_machineCodeId;
}

uint32_t as::ThreeOperand::setMachineCodeId(const uint32_t machineIdA)
{
    auto t_id = this->m_machineCodeId;
    this->m_machineCodeId = machineIdA;

    return t_id;
}

as::ParseObjBase *const as::ThreeOperand::getFirst() const
{
    return m_first;
}

as::ParseObjBase *const as::ThreeOperand::getSecond() const
{
    return m_second;
}

as::ParseObjBase *const as::ThreeOperand::getThird() const
{
    return m_third;
}

as::ParseObjBase *const as::ThreeOperand::setFirst(as::ParseObjBase *const firstA)
{
    auto t_ptr = this->m_first;
    this->m_first = firstA;

    return t_ptr;
}

as::ParseObjBase *const as::ThreeOperand::setSecond(as::ParseObjBase *const secondA)
{
    auto t_ptr = this->m_second;
    this->m_second = secondA;

    return t_ptr;
}

as::ParseObjBase *const as::ThreeOperand::setThird(as::ParseObjBase *const thirdA)
{
    auto t_ptr = this->m_third;
    this->m_third = thirdA;

    return t_ptr;
}

as::ThreeOperand::operands_type_t as::ThreeOperand::getOperands() const
{
    return std::make_tuple(m_first, m_second, m_third);
}

as::ThreeOperand::operands_type_t as::ThreeOperand::setOperands(as::ParseObjBase *const firstA,
                                                                as::ParseObjBase *const secondA,
                                                                as::ParseObjBase *const thirdA)
{
    return this->setOperands(std::make_tuple(firstA, secondA, thirdA));
}

as::ThreeOperand::operands_type_t as::ThreeOperand::setOperands(const as::ThreeOperand::operands_type_t &operandsA)
{

    as::ThreeOperand::operands_type_t t_ops = std::make_tuple(this->m_first, this->m_second, this->m_third);

    this->m_first = std::get<0>(operandsA);
    this->m_second = std::get<1>(operandsA);
    this->m_third = std::get<2>(operandsA);

    return t_ops;
}

std::string as::ThreeOperand::assemble(const boost::property_tree::ptree &ptreeA)
{
    // Temporary variables
    std::ostringstream t_os{""};                                               // Formated machine code command
    auto t_OpcodeSize = ptreeA.get<uint32_t>("Assembler_Property.OpCodeSize"); // Opcode size in machine code signal
    auto t_PlaceSize = ptreeA.get<uint32_t>("Assembler_Property.PlaceSize");   // Places size in machine code signal
    auto t_MemorySize =
        ptreeA.get<uint32_t>("VCGRA_Property.Available_Memory"); // Available memory space for shared memory
    uint32_t t_numOfAvailableLines{0}; // Number of available cache lines for range check of argument of command
    uint32_t t_numOfPlaces{0};         // Number of available cache lines for range check of argument of command
    uint32_t t_countVal{0};            // Counter to select write property for available cache lines
    uint32_t t_valFirst{0}, t_valSecond{0}, t_valThird{0}; // Parameter value of command

    /*#TODO: This separates the values of configuration file parameter "TwoOperator" in a list.
     * It seems, that the ordering of the values depends on the order in the configuration file.
     * Thus, reading out the write properties for cache line size needs to be updated for an independent order.
     */
    auto t_opt = ptreeA.get_child("Assembler_Property.ThreeOperator");

    // Search for parameter of available cache lines for actual command
    for (const auto &opt : t_opt)
        {
            if (opt.second.get<uint32_t>("MachineId") == m_machineCodeId)
                {
                    switch (t_countVal)
                        {
                        case 0:
                            t_numOfAvailableLines = ptreeA.get<uint32_t>("VCGRA_Property.Num_Dic_Lines");
                            t_numOfPlaces = ptreeA.get<uint32_t>("VCGRA_Property.Num_Dic_Places");
                            break;
                        case 1:
                            t_numOfAvailableLines = ptreeA.get<uint32_t>("VCGRA_Property.Num_Doc_Lines");
                            t_numOfPlaces = ptreeA.get<uint32_t>("VCGRA_Property.Num_Doc_Places");
                            break;
                        default:
                            throw as::AssemblerException(
                                "Unknown three-operation-command. Cannot select number of cache lines.", 8812);
                            break;
                        }
                    break;
                }
            else
                ++t_countVal;
        }

    // Get value of first operator (address)
    if (m_first->getCommandClass() == as::COMMANDCLASS::CONSTANT)
        t_valFirst = static_cast<as::ParseObjectConst *>(this->m_first)->getConstValue();
    else if (m_first->getCommandClass() == as::COMMANDCLASS::VARIABLE)
        t_valFirst = static_cast<as::ParseObjectVariable *>(this->m_first)->getVariableValue();
    else
        throw as::AssemblerException("Wrong parameter format for three-operation-command.\n \
        Expected constant integer value or integer variable for first operand",
                                     8813);
    // Get value of second parameter (cache line)
    if (m_second->getCommandClass() == as::COMMANDCLASS::CONSTANT)
        t_valSecond = static_cast<as::ParseObjectConst *>(this->m_second)->getConstValue();
    else if (m_second->getCommandClass() == as::COMMANDCLASS::VARIABLE)
        t_valSecond = static_cast<as::ParseObjectVariable *>(this->m_second)->getVariableValue();
    else
        throw as::AssemblerException("Wrong parameter format for three-operation-command.\n \
        Expected constant integer value or integer variable for second operator",
                                     8814);
    // Get value of third parameter (place)
    if (m_third->getCommandClass() == as::COMMANDCLASS::CONSTANT)
        t_valThird = static_cast<as::ParseObjectConst *>(this->m_third)->getConstValue();
    else if (m_third->getCommandClass() == as::COMMANDCLASS::VARIABLE)
        t_valThird = static_cast<as::ParseObjectVariable *>(this->m_third)->getVariableValue();
    else
        throw as::AssemblerException("Wrong parameter format for three-operation-command.\n \
        Expected constant integer value or integer variable for third operator",
                                     8814);

    /*Check ranges of parameters.
     * #TODO: For address in shared memory, control of memory size needs to be extended by
     * address + size of a cache line.
     */
    if (t_valSecond < t_numOfAvailableLines && t_valFirst <= t_MemorySize && t_valThird < t_numOfPlaces)
        {
            t_valSecond <<= (t_OpcodeSize + t_PlaceSize);
            t_valSecond |= t_valThird << t_OpcodeSize;
            t_valSecond |= m_machineCodeId;
        }
    else
        throw as::AssemblerException("Selected cache line is not available or address is out of memory.", 8815);

    t_os << "\"" << m_fmtStr % t_valFirst % t_valSecond << "\"";

    return t_os.str();
}

std::ostream &operator<<(std::ostream &osA, const as::ThreeOperand &opA)
{
    osA << static_cast<const as::ParseObjBase &>(opA) << "; ";

    std::array<const as::ParseObjBase *, 3> t_op{opA.getFirst(), opA.getSecond(), opA.getThird()};

    uint8_t t_opcnt{0};

    for (auto op : t_op)
        {
            if (t_opcnt == 0)
                {
                    osA << "first: ";
                    ++t_opcnt;
                }
            else if (t_opcnt == 1)
                {
                    osA << "second: ";
                    ++t_opcnt;
                }
            else
                {
                    osA << "thrird: ";
                }

            if (op->getCommandClass() == as::COMMANDCLASS::CONSTANT)
                {
                    osA << *(static_cast<const as::ParseObjectConst *>(op));
                }
            else if (op->getCommandClass() == as::COMMANDCLASS::VARIABLE)
                {
                    osA << *(static_cast<const as::ParseObjectVariable *>(op));
                }
            else
                {
                    osA << *op;
                }

            osA << "; ";
        }

    return osA;
}