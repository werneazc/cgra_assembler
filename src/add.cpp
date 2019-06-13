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
#include "add.h"
#include "parseobjectvariable.h"
#include "myException.h"

namespace as 
{
Add::Add(Level* lvlA, const std::string& cmdLineA, const uint32_t lineNumberA,
    ParseObjBase* const firstA, ParseObjBase* const secondA) : 
    ParseObjBase{lvlA, COMMANDCLASS::ARITHMETIC, cmdLineA, lineNumberA}, IArithmetic{firstA, secondA}
{ return; }

Add::Add(const Add& srcA) :
    ParseObjBase{srcA.getLevel(), srcA.getCommandClass(), srcA.getReadCmdLine(),
        srcA.getFileLineNumber()}, IArithmetic{srcA.getFirst(), srcA.getSecond()}
{ return; }

Add::Add(Add&& srcA) :
    ParseObjBase{srcA.getLevel(), srcA.getCommandClass(), srcA.getReadCmdLine(),
        srcA.getFileLineNumber()}, IArithmetic{srcA.getFirst(), srcA.getSecond()}
{ 
    srcA.clearMembers();
    return; 
}

Add& Add::operator=(const Add& rhsA)
{
    *(static_cast<ParseObjBase*>(this)) = static_cast<const ParseObjBase&>(rhsA);
    this->setFirst(rhsA.getFirst());
    this->setSecond(rhsA.getSecond());
    
    return *this;
}

Add& Add::operator=(Add&& rhsA)
{
    *(static_cast<ParseObjBase*>(this)) = std::move(static_cast<ParseObjBase&>(rhsA));
    this->setFirst(rhsA.getFirst());
    this->setSecond(rhsA.getSecond());
    
    rhsA.clearMembers();
    
    return *this;
}

void Add::clearMembers(void)
{
    static_cast<ParseObjBase*>(this)->clearMembers();
    this->setFirst(nullptr);
    this->setSecond(nullptr);
    
    return;
}

bool Add::processOperation(void)
{
    auto t_first = dynamic_cast<ParseObjectVariable* const>(this->getFirst());
    auto t_second = dynamic_cast<ParseObjectVariable* const>(this->getSecond());
    
    if(!(t_first && t_second))
        throw AssemblerException("Error Addition: One of the operands is not a variable type", 8525);
    else
        t_first->setVariableValue(t_first->getVariableValue() + t_second->getVariableValue());
    
    return true;
}

} /* End namespace as */