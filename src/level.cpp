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

#include <stdexcept>
#include "level.h"
#include "../header/myException.h"
#include "../header/parseobjectvariable.h"

as::Level& operator<<(as::Level& lelA, as::ParseObjBase* parseObjA)
{
    if(lelA.addParseObj(parseObjA))
        throw as::AssemblerException("Error while adding parse object to current level.", 4000);
    
    return lelA;
}

namespace as {


Level* Level::activeLvl = nullptr;

void Level::setCurrentLevel(Level* lvlA)
{
    Level::activeLvl = lvlA;
    return;
}

Level* Level::getCurrentLevel()
{
    return Level::activeLvl;
}


Level::Level() : m_parentLvl{nullptr}
{ }

Level::Level(Level* parantLvlA)
{
    if(parantLvlA)
        m_parentLvl = parantLvlA;
    else
        throw std::invalid_argument("Parent Level cannot be empty.");
    
    return;
}

Level& Level::operator=(Level&& src)
{
    this->m_parentLvl = src.m_parentLvl;
    src.m_parentLvl = nullptr;
    this->m_childLvlVec = src.m_childLvlVec;
    src.m_childLvlVec.clear();
    this->m_parsedObjVec = src.m_parsedObjVec;
    src.m_parsedObjVec.clear();
    
    return *this;
}

Level::Level(Level&& src)
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
    for(auto* item : m_childLvlVec)
        delete item;
    
    clearParseObjList(); 
    
    return;
}

uint8_t Level::addParseObj(ParseObjBase* pObjA)
{
    if(pObjA)
    {
        m_parsedObjVec.push_back(pObjA);
        return 0;
    }
    else
        return 1;
}


const std::vector<ParseObjBase*>& Level::getParseObjList() const
{
    return m_parsedObjVec;
}

ParseObjBase* Level::getParseObj(uint32_t idxA) const
{
    try
    {
       return m_parsedObjVec.at(idxA);
    }
    catch(std::out_of_range& e)
    {
        return nullptr;
    }
    catch(std::exception& e)
    {
        throw e;
    }
}

uint8_t Level::clearParseObjList()
{
    try {
        for(auto* item : m_parsedObjVec)
            delete item;
        m_parsedObjVec.clear();
        return 0;
    }
    catch(std::exception& e)
    {
        return 1;
    }

}

ParseObjBase* Level::deleteParseObj(uint32_t idxA)
{
    try
    {
        ParseObjBase* tPtr;
        tPtr = m_parsedObjVec.at(idxA);
        m_parsedObjVec.erase(m_parsedObjVec.begin()+idxA);
        return tPtr;
    }
    catch(std::out_of_range& e)
    {
        return nullptr;
    }
    catch(std::exception& e)
    {
        throw e;
    }
}

ParseObjBase* Level::findParseObj(const std::string& nameA)
{
    
    ParseObjBase* t_parseObj=nullptr;
    
    //Search for variable in current level
    for(auto* parseObj : m_parsedObjVec)
    {
        if(parseObj->getCommandClass() == COMMANDCLASS::VARIABLE)
        {
            if(*(static_cast<ParseObjectVariable*>(parseObj)) == nameA)
            {
                t_parseObj=parseObj;
                break;
            }
        }
        else
            continue;
    }
    
    //Look for variable in parent level
    t_parseObj = m_parentLvl->findParseObj(nameA);
        
    return t_parseObj;
}

void Level::leave(void)
{
    Level::setCurrentLevel(m_parentLvl);
    return;
}

} /* End namespace as */
