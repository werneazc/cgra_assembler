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

#ifndef LEVEL_H
#define LEVEL_H

#include <cstdint>
#include <vector>
#include "parseobjbase.h"

namespace as {

/**
* @class Level
* 
* @brief Level in a VCGRA assembler file
* 
* @details
* A level is used to define visibility of variables and to design control flow.
* A LOOP for instance opens a new Level and can interact with variables from its
* parent Level or from itself. A Level includes parse objects and child levels.
* 
*/
class Level
{
public:
    
    /**
    * @brief Empty constructor
    * 
    */
    Level();
    
    /**
    * @brief Standard constructor to add a new child level
    * 
    * @param[in] parentLvlA parentLvlA: Adds new child level to actual level.
    */
    Level(Level* parentLvlA);
    
    /**
    * @brief Move constructor
    * 
    * @param[in] src src: Source from where to move content. Members of source going to be deleted.
    */
    Level(Level&& src);
    
    /**
    * @brief Move assignment
    * 
    * @param[in] src src: Source from where to move content. Members of source going to be deleted.
    * @return as::Level& New assigned level element.
    */
    Level& operator=(Level&& src);
    
    /**
    * @brief Desctrutor
    * 
    * @details
    * Delete function is called on all items in m_parsedObjVec and m_childLvlVec.
    * 
    */
    virtual ~Level();
    
    /**
    * @brief Compare to levels
    * 
    * @param lhsA left hand side
    * @param rhsA right hand side
    * @return true is member variables are identically
    */
    friend bool operator==(const Level& lhsA, const Level& rhsA);
    
    //Actions with parsed objects
    /**
    * @brief Add parse object to current level.
    * 
    * @param[in] pObjA pObjA: Object pointer of parsed object to add.
    * @return uint8_t 0=success, 1=failure.    
    */
    virtual uint8_t addParseObj(ParseObjBase* pObjA) final;
    
    /**
    * @brief Get parsed object from m_parsedObjVec.
    * 
    * @param[in] idxA idxA: Element index of m_parsedObjVec vector.
    * @return ParseObjBase* nullptr, if element not found or idxA out of range
    */
    virtual ParseObjBase* getParseObj(uint32_t idxA) const final;
    
    /**
    * @brief Delete parsed object from m_parsedObjVec.
    * 
    * @param[in] idxA idxA: Index of item in m_parsedObjVec going to be deleted.
    * @return as::ParseObjBase* nullptr if idxA out of range, otherwise pointer to deleted item.
    */
    virtual ParseObjBase* deleteParseObj(uint32_t idxA) final;
    
    /**
    * @brief Find parsed object by name in current level and parent level.
    * 
    * @param[in] nameA nameA: name of variable to find.
    * @return as::ParseObjBase* nullptr, if variable is not found by its name, else pointer to parse object.
    */
    virtual ParseObjBase* findParseObj(const std::string& nameA) final;
    
    /**
    * @brief Add parse object to current level.
    * 
    * @param[in] lvlA Level where the parsed object should be added
    * @param[in] parseObjA Parsed object to add.
    * @return as::Level& Level where the parsed object currently added.
    */
    friend Level& operator<<(Level& lvlA, ParseObjBase* parseObjA);
    
    //Level adapting class methods
    /**
    * @brief Clear m_parsedObjVec. All items are deleted from head.
    * 
    * @return uint8_t 0=success, 1=failure.
    */
    virtual uint8_t clearParseObjList() final;
    
    /**
    * @brief Get access to m_parsedObjVec
    * 
    * @return const std::vector<ParseObjBase*>& Reference to m_parsedObjVec variable. 
    */
    virtual const std::vector<ParseObjBase*>& getParseObjList() const final;
    
    /**
    * @brief Leave current level and set parent level as active level.
    */
    virtual void leave(void);
   
    //Static class methods:
    /**
    * @brief Set current active level activeLvl.
    * 
    * @param lvlA lvlA: Pointer to next active level.
    */
    static void setCurrentLevel(Level* lvlA);
    
    /**
    * @brief Return static class member activeLvl
    * 
    * @return Level*
    */
    static Level* getCurrentLevel(void);
    
private:
    //Forbidden constructors
    Level(const Level& src) = delete;
    Level& operator=(const Level& src) = delete;
    
    //Class Members
    std::vector<Level*> m_childLvlVec{};
    //!< @brief Store child levels
    Level* m_parentLvl;
    //!< @brief Store parent level
    std::vector<ParseObjBase*> m_parsedObjVec{};
    //!< @brief Store parsed objects of actual level.
    //Class static members
    static Level* activeLvl;
    //!< @brief Currently active level for adding new parsed objects.
};


} /* End namespace as */

#endif // LEVEL_H
