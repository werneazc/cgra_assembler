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

#ifndef PARSEOBJECTCONST_H
#define PARSEOBJECTCONST_H

#include <cstdint>
#include "parseobjbase.h"

namespace as {

/**
* @class ParseObjectConst
* 
* @brief Parsed object o a constant value in assembler file.
*/
class ParseObjectConst : public as::ParseObjBase
{
public:
    
    /**
    * @brief General constructor
    * 
    * @param[in] valueA Constant integer value
    * @param[in] lvlA Level pointer where constant exists
    * @param[in] cmdLineA Parsed command line in assembler file
    * @param[in] lineNumberA Line number in assembler file
    */
    ParseObjectConst(int32_t valueA, Level* lvlA, const std::string& cmdLineA, 
                     uint32_t lineNumberA);
    
    /**
    * @brief Copy constructor
    * 
    * @param[in] srcA Source for constant copy.
    */
    ParseObjectConst(const ParseObjectConst& srcA);
    
    /**
    * @brief Move constructor
    * 
    * @param[in] srcA Source for new constant. Members of source are deleted.
    */
    ParseObjectConst(ParseObjectConst&& srcA);
    
    /**
    * @brief Copy assignment
    * 
    * @param[in] srcA Source where copy is based on.
    * @return New parsed constant with members of source.
    */
    ParseObjectConst& operator=(const ParseObjectConst& srcA);
    
    /**
    * @brief Move assignment
    * 
    * @param[in] srcA Source to create new parsed object. Members of source are deleted.
    * @return New parsed object with members of source.
    */
    ParseObjectConst& operator=(ParseObjectConst&& srcA);
    
    /**
    * @brief Return value of constant.
    * 
    * @return Value of constant.
    */
    const int32_t getConstValue(void) const;
    
    /**
    * @brief Delete all class members.
    * 
    */
    virtual void clearMembers(void) override;
    
    
    /**
    * @brief Destructor
    */
    ~ParseObjectConst() = default;
    
private:
    
    //Forbidden Construtors
    ParseObjectConst() = delete;
    
    int32_t m_value;
    //!< @brief Value of constant
    
};

} /* End namespace as */

#endif // PARSEOBJECTCONST_H
