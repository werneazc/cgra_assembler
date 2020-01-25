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

#include "nooperand.h"
#include <sstream>
#include <utility>

as::NoOperand::NoOperand(as::Level *const lvlA, const std::string &cmdLineA, const uint32_t lineNumberA,
                         const uint32_t machineIdA)
    : as::ParseObjBase{lvlA, as::COMMANDCLASS::NOOPERANT, cmdLineA, lineNumberA}, m_machineCodeID{machineIdA}
{
    return;
}

as::NoOperand::NoOperand(const as::NoOperand &srcA)
    : as::ParseObjBase{srcA.getLevel(), srcA.getCommandClass(), srcA.getReadCmdLine(), srcA.getFileLineNumber()}
{
    this->m_machineCodeID = srcA.m_machineCodeID;
    return;
}

as::NoOperand::NoOperand(as::NoOperand &&srcA)
    : as::ParseObjBase{srcA.getLevel(), srcA.getCommandClass(), srcA.getReadCmdLine(), srcA.getFileLineNumber()}
{
    this->m_machineCodeID = srcA.m_machineCodeID;

    srcA.clearMembers();

    return;
}

as::NoOperand &as::NoOperand::operator=(as::NoOperand &&rhsA)
{
    *(static_cast<ParseObjBase *>(this)) = std::move(static_cast<ParseObjBase &>(rhsA));
    this->m_machineCodeID = rhsA.m_machineCodeID;

    rhsA.clearMembers();

    return *this;
}

as::NoOperand &as::NoOperand::operator=(const as::NoOperand &rhsA)
{
    *(static_cast<ParseObjBase *>(this)) = static_cast<const ParseObjBase &>(rhsA);
    this->m_machineCodeID = rhsA.m_machineCodeID;

    return *this;
}

void as::NoOperand::clearMembers(void)
{
    static_cast<ParseObjBase *const>(this)->clearMembers();
    m_machineCodeID = UINT32_MAX;

    return;
}

uint32_t as::NoOperand::getMachineCodeId() const
{
    return m_machineCodeID;
}

uint32_t as::NoOperand::setMachineCodeId(const uint32_t machineIdA)
{
    auto t_val = m_machineCodeID;
    m_machineCodeID = machineIdA;

    return t_val;
}

std::string as::NoOperand::assemble(const boost::property_tree::ptree &ptreeA)
{
    // Temporary variables
    std::ostringstream t_os{""};

    t_os << "\"" << m_fmtStr % 0u % this->getMachineCodeId() << "\"";

    return t_os.str();
}

std::ostream &operator<<(std::ostream &osA, const as::NoOperand &opA)
{
    osA << static_cast<const as::ParseObjBase &>(opA) << "; ";
    osA << opA.getMachineCodeId();

    return osA;
}