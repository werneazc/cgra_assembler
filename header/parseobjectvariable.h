/*
 * <one line to give the program's name and a brief idea of what it does.>
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

#ifndef PARSEOBJECTVARIABLE_H
#define PARSEOBJECTVARIABLE_H

#include <cstdint>
#include "parseobjbase.h"

class Level;

namespace as {
    
class ParseObjectVariable : public as::ParseObjBase {
public:
    
    //Constructors
    /**
    * @brief Default constructor
    * 
    * @param[in] nameA Name of the current variable.
    * @param[in] valueA Value of the current variable.
    * @param[in] lvlA Level in which the variable is created.
    * @param[in] cmdLineA Command string in assembler file.
    * @param[in] lineNumberA Line number in assembler file.
    */
    ParseObjectVariable(const std::string& nameA, const int32_t valueA,
        Level* lvlA, const std::string& cmdLineA, uint32_t lineNumberA);
    
    /**
    * @brief Copy constructor
    * 
    * @param src Source from which to create a copy.
    */
    ParseObjectVariable(const ParseObjectVariable& src);
    
    /**
    * @brief Move constructor
    * 
    * @param src Source from which to create the new object. Members of source are deleted.
    */
    ParseObjectVariable(ParseObjectVariable&& src);
    
    /**
    * @brief Copy assignment
    * 
    * @param src Source from which to create a copy.
    * @return Copy of source object.
    */
    ParseObjectVariable& operator=(const ParseObjectVariable& src);
    
    /**
    * @brief Move assignment
    * 
    * @param src Source from which to create new object. Members of source are deleted.
    * @return New object with values of source.
    */
    ParseObjectVariable& operator=(ParseObjectVariable&& src);
    
    /**
    * @brief Destructor
    */
    virtual ~ParseObjectVariable(void) = default;
    
    /**
    * @brief Return variable name of parsed object.
    * 
    * @return Variable name of parsed object.
    */
    const std::string& getVariableName(void) const;
    
    /**
    * @brief Return variable value of parsed object.
    * 
    * @return Value of parsed object.
    */
    int32_t getVariableValue(void) const;
    
    /**
    * @brief Set a new value for parsed object.
    * 
    * @param valueA New value to store in parsed object.
    */
    void setVariableValue(int32_t valueA);
    
    /**
    * @brief Clear members of variable parse object.
    */
    virtual void clearMembers() override;
    
    
private:
    //Forbidden constructors
    ParseObjectVariable() = delete;
    
    
    //class members
    std::string m_name;
    //!< @brief name of parsed object variable
    int32_t m_value;
    //!< @brief stored value of parsed object variable
};

/**
* @brief Comparison operator
* 
* @param lhsA Parsed object to compare it's name
* @param nameA Name as a string to compare with parsed object name.
* @return If search name and name of parsed object identically return true, else return fail.
*/
bool operator==(const ParseObjectVariable& lhsA, const std::string& nameA);

} /* End namespace as */

#endif // PARSEOBJECTVARIABLE_H
