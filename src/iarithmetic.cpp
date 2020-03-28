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

#include "iarithmetic.h"
#include "level.h"
#include <cstdint>

namespace as
{

IArithmetic::IArithmetic() : IArithmetic{nullptr, "", UINT32_MAX, nullptr, nullptr}
{
    return;
}

IArithmetic::IArithmetic(Level *lvlA, const std::string &cmdLineA, const uint32_t &lineNumberA,
                         ParseObjBase *const firstA, ParseObjBase *const secondA)
    : ParseObjBase(lvlA, COMMANDCLASS::ARITHMETIC, cmdLineA, lineNumberA), m_first{firstA}, m_second{secondA}
{
    return;
}

IArithmetic &IArithmetic::operator=(IArithmetic &&src)
{

    static_cast<ParseObjBase>(*this) = std::move(static_cast<ParseObjBase>(src));

    m_first = src.m_first;
    m_first = nullptr;
    m_second = src.m_second;
    m_second = nullptr;

    return *this;
}

IArithmetic &IArithmetic::operator=(const IArithmetic &src)
{

    static_cast<ParseObjBase>(*this) = static_cast<ParseObjBase>(src);

    m_first = src.m_first;
    m_second = src.m_second;

    return *this;
}

IArithmetic::IArithmetic(IArithmetic &&src)
    : ParseObjBase(src.getLevel(), COMMANDCLASS::ARITHMETIC, src.getReadCmdLine(), src.getFileLineNumber()),
      m_first(src.getFirst()), m_second(src.getSecond())
{
    return;
}

IArithmetic::IArithmetic(const as::IArithmetic &src)
    : ParseObjBase(src.getLevel(), COMMANDCLASS::ARITHMETIC, src.getReadCmdLine(), src.getFileLineNumber()),
      m_first(src.getFirst()), m_second(src.getSecond())
{
    *this = src;

    return;
}

ParseObjBase *const IArithmetic::getFirst() const
{
    return m_first;
}

ParseObjBase *const IArithmetic::getSecond() const
{
    return m_second;
}

ParseObjBase *const IArithmetic::setFirst(as::ParseObjBase *const firstA)
{
    ParseObjBase *t_Ptr = nullptr;

    if (firstA != m_first)
        {
            t_Ptr = m_first;
            m_first = firstA;
        }

    return t_Ptr;
}

ParseObjBase *const IArithmetic::setSecond(as::ParseObjBase *const secondA)
{

    ParseObjBase *t_Ptr = nullptr;

    if (secondA != m_second)
        {
            t_Ptr = m_second;
            m_second = secondA;
        }

    return t_Ptr;
}

IArithmetic::operator_type_t IArithmetic::getOperandPair()
{
    return std::make_pair(m_first, m_second);
}

IArithmetic::operator_type_t IArithmetic::setOperandPair(ParseObjBase *const firstA, ParseObjBase *const secondA)
{
    auto t_pair = std::make_pair(m_first, m_second);

    if (m_first != firstA && m_second != secondA)
        {
            m_first = firstA;
            m_second = secondA;
        }
    else
        {
            t_pair.first = nullptr;
            t_pair.second = nullptr;
        }

    return t_pair;
}

IArithmetic::operator_type_t IArithmetic::setOperandPair(const IArithmetic::operator_type_t &operandsA)
{
    return this->setOperandPair(operandsA.first, operandsA.second);
}

} // namespace as
