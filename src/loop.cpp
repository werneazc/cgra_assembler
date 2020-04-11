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
#include "resetvariable.h"
#include "threeoperand.h"
#include "twooperand.h"
#include <utility>

namespace as
{

Loop::Loop(Level *const parentLvlA, const uint32_t fileLineA, uint32_t startValueA, uint32_t endValueA,
           int32_t stepwidthA, const std::string readCmdA)
    : Level{parentLvlA}, m_readCommandLine{readCmdA}, m_stepWidth{stepwidthA}, m_fileLine{fileLineA}
{
    if (m_stepWidth == 0)
        throw AssemblerException("Error: Stepwidth of zero effects endless loop", 8511);
    else if (m_stepWidth < 0 && startValueA < endValueA)
        throw AssemblerException(
            "Error: Start value smaller then end value by negative stepwidth used for loop conditions.", 8511);
    else
        {
            m_startValue = startValueA;
            m_endValue = endValueA;
            m_currentValue = startValueA;
        }
}

Loop::Loop(Loop &&rhsA) : Level{std::move(rhsA)}
{
    m_currentValue = rhsA.m_currentValue;
    rhsA.m_currentValue = INT32_MIN;
    m_endValue = rhsA.m_endValue;
    rhsA.m_endValue = UINT32_MAX;
    m_fileLine = rhsA.m_fileLine;
    rhsA.m_fileLine = UINT32_MAX;
    m_readCommandLine = rhsA.m_readCommandLine;
    rhsA.m_readCommandLine.clear();
    m_startValue = rhsA.m_startValue;
    rhsA.m_startValue = UINT32_MAX;
    m_stepWidth = rhsA.m_stepWidth;
    rhsA.m_stepWidth = INT32_MIN;

    return;
}

Loop &Loop::operator=(Loop &&rhsA)
{
    *(static_cast<Level *>(this)) = std::move(rhsA);

    m_currentValue = rhsA.m_currentValue;
    rhsA.m_currentValue = INT32_MIN;
    m_endValue = rhsA.m_endValue;
    rhsA.m_endValue = UINT32_MAX;
    m_fileLine = rhsA.m_fileLine;
    rhsA.m_fileLine = UINT32_MAX;
    m_readCommandLine = rhsA.m_readCommandLine;
    rhsA.m_readCommandLine.clear();
    m_startValue = rhsA.m_startValue;
    rhsA.m_startValue = UINT32_MAX;
    m_stepWidth = rhsA.m_stepWidth;
    rhsA.m_stepWidth = INT32_MIN;

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
    m_currentValue += m_stepWidth;

    if (0 > m_stepWidth) // negative stepwidth
        {
            if (m_endValue >= m_currentValue)
                return false;
            else
                return true;
        }
    else
        {
            if (m_endValue <= m_currentValue)
                return false;
            else
                return true;
        }
}

std::ostream &Loop::assemble(const boost::property_tree::ptree &ptreeA, std::ostream &osA)
{

    uint64_t lvlId{0};

    while (updateLoopIndex())
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
                            osA << "," << std::endl;
                            break;
                        case as::COMMANDCLASS::ONEOPERAND:
                            osA << static_cast<as::OneOperand *>(po)->assemble(ptreeA);
                            osA << "," << std::endl;
                            break;
                        case as::COMMANDCLASS::TWOOPERAND:
                            osA << static_cast<as::TwoOperand *>(po)->assemble(ptreeA);
                            osA << "," << std::endl;
                            break;
                        case as::COMMANDCLASS::THREEOPERAND:
                            osA << static_cast<as::ThreeOperand *>(po)->assemble(ptreeA);
                            osA << "," << std::endl;
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

    return osA;
}

} /* End namespace as */
