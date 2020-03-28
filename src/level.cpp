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

#include "level.h"
#include "myException.h"
#include "parseobjectconst.h"
#include "parseobjectvariable.h"
#include <algorithm>
#include <stdexcept>

as::Level &operator<<(as::Level &lvlA, as::ParseObjBase *parseObjA)
{
    if (lvlA.addParseObj(parseObjA))
        throw as::AssemblerException("Error while adding parse object to current level.", 4000);

    return lvlA;
}

namespace as
{

Level *Level::activeLvl = nullptr;

void Level::setCurrentLevel(Level *lvlA)
{
    Level::activeLvl = lvlA;
    return;
}

Level *Level::getCurrentLevel()
{
    return Level::activeLvl;
}

Level::Level() : m_parentLvl{nullptr} {}

Level::Level(Level *parantLvlA)
{
    if (parantLvlA)
        m_parentLvl = parantLvlA;
    else
        throw std::invalid_argument("Parent Level cannot be empty.");

    return;
}

Level &Level::operator=(Level &&src)
{
    this->m_parentLvl = src.m_parentLvl;
    src.m_parentLvl = nullptr;
    this->m_childLvlVec = src.m_childLvlVec;
    src.m_childLvlVec.clear();
    this->m_parsedObjVec = src.m_parsedObjVec;
    src.m_parsedObjVec.clear();

    return *this;
}

Level::Level(Level &&src)
{
    this->m_parentLvl = src.m_parentLvl;
    src.m_parentLvl = nullptr;
    this->m_childLvlVec = src.m_childLvlVec;
    src.m_childLvlVec.clear();
    this->m_parsedObjVec = src.m_parsedObjVec;
    src.m_parsedObjVec.clear();

    return;
}

Level::~Level()
{
    for (auto *item : m_childLvlVec)
        delete item;

    clearParseObjList();

    return;
}

uint8_t Level::addParseObj(ParseObjBase *pObjA)
{
    if (pObjA)
        {
            m_parsedObjVec.push_back(pObjA);
            return 0;
        }
    else
        return 1;
}

const std::vector<ParseObjBase *> &Level::getParseObjList() const
{
    return m_parsedObjVec;
}

ParseObjBase *Level::getParseObj(uint32_t idxA) const
{
    try
        {
            return m_parsedObjVec.at(idxA);
        }
    catch (std::out_of_range &e)
        {
            return nullptr;
        }
    catch (std::exception &e)
        {
            throw e;
        }
}

uint8_t Level::clearParseObjList()
{
    try
        {
            for (auto *item : m_parsedObjVec)
                delete item;
            m_parsedObjVec.clear();
            return 0;
        }
    catch (std::exception &e)
        {
            return 1;
        }
}

ParseObjBase *Level::deleteParseObj(uint32_t idxA)
{
    try
        {
            ParseObjBase *tPtr;
            tPtr = m_parsedObjVec.at(idxA);
            m_parsedObjVec.erase(m_parsedObjVec.begin() + idxA);
            return tPtr;
        }
    catch (std::out_of_range &e)
        {
            return nullptr;
        }
    catch (std::exception &e)
        {
            throw e;
        }
}

ParseObjBase *Level::findParseObj(const std::string &nameA)
{

    ParseObjBase *t_parseObj = nullptr;

    // Search for variable in current level
    for (auto *parseObj : m_parsedObjVec)
        {
            if (parseObj->getCommandClass() == COMMANDCLASS::VARIABLE)
                {
                    if (*(static_cast<ParseObjectVariable *>(parseObj)) == nameA)
                        {
                            t_parseObj = parseObj;
                            break;
                        }
                }
            else if (parseObj->getCommandClass() == COMMANDCLASS::CONSTANT)
                {
                    if (*(static_cast<ParseObjectConst *>(parseObj)) == nameA)
                        {
                            t_parseObj = parseObj;
                            break;
                        }
                }
            else
                continue;
        }

    // Look for variable in parent level
    if (!t_parseObj)
        {
            if (m_parentLvl)
                t_parseObj = m_parentLvl->findParseObj(nameA);
            else
                t_parseObj = nullptr;
        }

    return t_parseObj;
}

void Level::leave(void)
{
    Level::setCurrentLevel(m_parentLvl);
    return;
}

bool operator==(const Level &lhsA, const Level &rhsA)
{
    // same object
    if (&lhsA == &rhsA)
        return true;

    // Compare parent level member
    if (lhsA.m_parentLvl != rhsA.m_parentLvl)
        return false;

    // Compare child level entries
    if (lhsA.m_childLvlVec.size() != rhsA.m_childLvlVec.size())
        return false;
    else
        {
            auto t_res = std::equal(lhsA.m_childLvlVec.begin(), lhsA.m_childLvlVec.end(), rhsA.m_childLvlVec.begin());

            if (!t_res)
                return false;
        }

    if (lhsA.m_parsedObjVec.size() != rhsA.m_parsedObjVec.size())
        return false;
    else
        {

            auto t_res =
                std::equal(lhsA.m_parsedObjVec.begin(), lhsA.m_parsedObjVec.end(), rhsA.m_parsedObjVec.begin());

            if (!t_res)
                return t_res;
        }

    return true;
}

void Level::addChildLevel(Level *const levelA)
{
    if (levelA)
        m_childLvlVec.push_back(levelA);
    else
        throw AssemblerException("Invalid level pointer for child level.", 4444);
}

std::vector<Level *>::iterator Level::begin()
{
    return m_childLvlVec.begin();
}

std::vector<Level *>::iterator Level::end()
{
    return m_childLvlVec.end();
}

std::vector<Level *>::const_iterator Level::cbegin() const
{
    return m_childLvlVec.cbegin();
}

std::vector<Level *>::const_iterator Level::cend() const
{
    return m_childLvlVec.cend();
}

} /* End namespace as */
