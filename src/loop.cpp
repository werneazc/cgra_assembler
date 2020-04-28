/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2019  andrewerner <andre.werner-w2m@ruhr-uni-bochum.de>
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

#include "loop.h"
#include "iarithmetic.h"
#include "myException.h"
#include "nooperand.h"
#include "oneoperand.h"
#include "parseobjectconst.h"
#include "parseobjectvariable.h"
#include "resetvariable.h"
#include "threeoperand.h"
#include "twooperand.h"
#include <utility>

namespace
{

/**
 * @brief Helper function to return variable or constant value.
 *
 * @param[in] objA  Pointer to parse object.
 */
int32_t getValue(const as::ParseObjBase *objA)
{

    if (objA->getCommandClass() == as::COMMANDCLASS::CONSTANT)
        {
            return static_cast<const as::ParseObjectConst *>(objA)->getConstValue();
        }
    else if (objA->getCommandClass() == as::COMMANDCLASS::VARIABLE)
        {
            return static_cast<const as::ParseObjectVariable *>(objA)->getVariableValue();
        }
    else
        {
            throw as::AssemblerException("Error: Invalid type for parse object. It's neither a constant no a variable",
                                         8019);
        }
}

} // end of anonymous namespace

namespace as
{

Loop::Loop(Level *const parentLvlA, const uint32_t fileLineA, ParseObjBase *startValueA, ParseObjBase *endValueA,
           ParseObjBase *stepwidthA, const std::string readCmdA)
    : Level{parentLvlA}, m_readCommandLine{readCmdA}, m_stepWidth{stepwidthA}, m_fileLine{fileLineA}
{
    int32_t t_stepwidth = getValue(m_stepWidth);
    int32_t t_startValue = getValue(startValueA);
    int32_t t_endValue = getValue(endValueA);

    if (t_stepwidth == 0)
        throw AssemblerException("Error: Stepwidth of zero effects endless loop", 8511);
    else if (t_stepwidth < 0 && t_startValue < t_endValue)
        throw AssemblerException(
            "Error: Start value smaller then end value by negative stepwidth used for loop conditions.", 8511);
    else
        {
            if (t_startValue < t_endValue)
                {
                    m_startValue = startValueA;
                    m_endValue = endValueA;
                    m_currentValue = t_startValue;
                }
            else
                {
                    m_startValue = endValueA;
                    m_endValue = startValueA;
                    m_currentValue = t_endValue;
                }
        }
}

Loop::Loop(Loop &&rhsA) : Level{std::move(rhsA)}
{
    m_currentValue = rhsA.m_currentValue;
    rhsA.m_currentValue = INT32_MIN;
    m_endValue = rhsA.m_endValue;
    rhsA.m_endValue = nullptr;
    m_fileLine = rhsA.m_fileLine;
    rhsA.m_fileLine = UINT32_MAX;
    m_readCommandLine = rhsA.m_readCommandLine;
    rhsA.m_readCommandLine.clear();
    m_startValue = rhsA.m_startValue;
    rhsA.m_startValue = nullptr;
    m_stepWidth = rhsA.m_stepWidth;
    rhsA.m_stepWidth = nullptr;

    return;
}

Loop &Loop::operator=(Loop &&rhsA)
{
    *(static_cast<Level *>(this)) = std::move(rhsA);

    m_currentValue = rhsA.m_currentValue;
    rhsA.m_currentValue = INT32_MIN;
    m_endValue = rhsA.m_endValue;
    rhsA.m_endValue = nullptr;
    m_fileLine = rhsA.m_fileLine;
    rhsA.m_fileLine = UINT32_MAX;
    m_readCommandLine = rhsA.m_readCommandLine;
    rhsA.m_readCommandLine.clear();
    m_startValue = rhsA.m_startValue;
    rhsA.m_startValue = nullptr;
    m_stepWidth = rhsA.m_stepWidth;
    rhsA.m_stepWidth = nullptr;

    return *this;
}

bool operator==(const Loop &lhsA, const Loop &rhsA)
{
    // same object
    if (&lhsA == &rhsA)
        return true;

    if (!((static_cast<const Level &>(lhsA)) == (static_cast<const Level &>(rhsA))))
        return false;

    if (lhsA.m_currentValue != rhsA.m_currentValue || lhsA.m_endValue != rhsA.m_endValue ||
        lhsA.m_fileLine != rhsA.m_fileLine || lhsA.m_startValue != rhsA.m_startValue ||
        lhsA.m_stepWidth != rhsA.m_stepWidth)
        return false;

    if (lhsA.m_readCommandLine == rhsA.m_readCommandLine)
        return true;
    else
        return false;
}

bool Loop::updateLoopIndex(void)
{

    int32_t t_stepwidth = getValue(m_stepWidth);
    int32_t t_endVal = getValue(m_endValue);

    m_currentValue += t_stepwidth;

    if (0 > t_stepwidth) // negative stepwidth
        {
            if (t_endVal >= m_currentValue)
                return false;
            else
                return true;
        }
    else
        {
            if (t_endVal <= m_currentValue)
                return false;
            else
                return true;
        }
}

std::ostream &Loop::assemble(const boost::property_tree::ptree &ptreeA, std::ostream &osA)
{

    uint64_t lvlId{0};

    do
        {
            for (auto po : this->getParseObjList())
                {
                    switch (po->getCommandClass())
                        {
                        case as::COMMANDCLASS::ARITHMETIC:
                            static_cast<as::IArithmetic *>(po)->processOperation();
                            break;
                        case as::COMMANDCLASS::NOOPERAND:
                            osA << static_cast<as::NoOperand *>(po)->assemble(ptreeA);
                            osA << ",";
                            osA << " //" << po->getReadCmdLine() << std::endl;
                            break;
                        case as::COMMANDCLASS::ONEOPERAND:
                            osA << static_cast<as::OneOperand *>(po)->assemble(ptreeA);
                            osA << ",";
                            osA << " //" << po->getReadCmdLine() << std::endl;
                            break;
                        case as::COMMANDCLASS::TWOOPERAND:
                            osA << static_cast<as::TwoOperand *>(po)->assemble(ptreeA);
                            osA << ",";
                            osA << " //" << po->getReadCmdLine() << std::endl;
                            break;
                        case as::COMMANDCLASS::THREEOPERAND:
                            osA << static_cast<as::ThreeOperand *>(po)->assemble(ptreeA);
                            osA << ",";
                            osA << " //" << po->getReadCmdLine() << std::endl;
                            break;
                        case as::COMMANDCLASS::LOOP:
                            static_cast<Loop *>(this->at(lvlId++))->assemble(ptreeA, osA);
                            break;
                        case as::COMMANDCLASS::RESETVAR:
                            static_cast<ResetVariable *>(po)->resetVariable();
                            break;
                        case as::COMMANDCLASS::CONSTANT:
                        case as::COMMANDCLASS::VARIABLE:
                        default:
                            break;
                        }
                }

            lvlId = 0;
        }
    while (updateLoopIndex());

    m_currentValue = getValue(m_startValue);

    return osA;
}

} /* End namespace as */
