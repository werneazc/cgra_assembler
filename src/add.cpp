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

#include "add.h"
#include "myException.h"
#include "parseobjectconst.h"
#include "parseobjectvariable.h"
#include <array>
#include <utility>

namespace as
{
Add::Add(Level *lvlA, const std::string &cmdLineA, const uint32_t lineNumberA, ParseObjBase *const firstA,
         ParseObjBase *const secondA)
    : IArithmetic{lvlA, cmdLineA, lineNumberA, firstA, secondA}
{
    return;
}

Add::Add(const Add &srcA)
    : IArithmetic{srcA.getLevel(), srcA.getReadCmdLine(), srcA.getFileLineNumber(), srcA.getFirst(), srcA.getSecond()}
{
    return;
}

Add::Add(Add &&srcA)
    : IArithmetic{srcA.getLevel(), srcA.getReadCmdLine(), srcA.getFileLineNumber(), srcA.getFirst(), srcA.getSecond()}
{
    srcA.clearMembers();
    return;
}

Add &Add::operator=(const Add &rhsA)
{
    *(static_cast<ParseObjBase *>(this)) = static_cast<const ParseObjBase &>(rhsA);
    this->setFirst(rhsA.getFirst());
    this->setSecond(rhsA.getSecond());

    return *this;
}

Add &Add::operator=(Add &&rhsA)
{
    *(static_cast<ParseObjBase *>(this)) = std::move(static_cast<ParseObjBase &>(rhsA));
    this->setFirst(rhsA.getFirst());
    this->setSecond(rhsA.getSecond());

    rhsA.clearMembers();

    return *this;
}

void Add::clearMembers(void)
{
    static_cast<ParseObjBase *>(this)->clearMembers();
    this->setFirst(nullptr);
    this->setSecond(nullptr);

    return;
}

bool Add::processOperation(void)
{
    auto t_first = dynamic_cast<ParseObjectVariable *const>(this->getFirst());
    auto t_cmdClass = this->getSecond()->getCommandClass();

    if (!t_first)
        throw AssemblerException("Error Addition: One of the operands is not a variable type", 8525);
    else
        {
            if (t_cmdClass == as::COMMANDCLASS::CONSTANT)
                {
                    auto t_second = dynamic_cast<as::ParseObjectConst *const>(this->getSecond());
                    t_first->setVariableValue(t_first->getVariableValue() + t_second->getConstValue());
                }
            else if (t_cmdClass == as::COMMANDCLASS::VARIABLE)
                {
                    auto t_second = dynamic_cast<as::ParseObjectVariable *const>(this->getSecond());
                    t_first->setVariableValue(t_first->getVariableValue() + t_second->getVariableValue());
                }
            else
                {
                    throw AssemblerException("Error Addition: One of the operands is not a variable type", 8525);
                }
        }
    return true;
}

} /* End namespace as */

std::ostream &operator<<(std::ostream &osA, const as::Add &opA)
{
    osA << static_cast<const as::ParseObjBase &>(opA) << "; ";

    std::array<const as::ParseObjBase *, 2> t_op{opA.getFirst(), opA.getSecond()};

    bool first{true};

    for (auto op : t_op)
        {
            if (first)
                {
                    osA << "first: ";
                    first = false;
                }
            else
                {
                    osA << "second: ";
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
