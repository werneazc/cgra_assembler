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

#ifndef ADDINTEGER_H
#define ADDINTEGER_H

#include "parseobjbase.h"
#include "iarithmetic.h"

namespace as 
{

/**
* @class AddInteger
* 
* @brief arithmetic operation to add a constand value to a variable.
*/
class AddInteger : public IArithmetic, public ParseObjBase
{
public:
    
    /**
    * @brief General constructor
    * 
    * @param lvlA Level where the parsed object is defined.
    * @param cmdLineA Assembler command line as a string.
    * @param lineNumberA Line number of assembler file where the parse object is defined.
    * @param firstA Pointer to first arithmetic operand (default=nullptr).
    * @param secondA Pointer to second arithmetic operand (default=nullptr).
    */
    AddInteger(Level* lvlA, const std::string& cmdLineA, const uint32_t& lineNumberA,
        ParseObjBase* firstA = nullptr, ParseObjBase* secondA = nullptr );
    
    /**
    * @brief Copy constructor
    * 
    * @param srcA Source for generated copy.
    */
    AddInteger(const AddInteger& srcA);
    
    /**
    * @brief Copy assignment
    * 
    * @param lhsA Source for new object. Members of source are deleted.
    * @return New parsed object
    */
    AddInteger& operator=(const AddInteger& lhsA);
    
    /**
    * @brief Move constructor
    * 
    * @param srcA Members of srcA are deleted after copying.
    */
    AddInteger(AddInteger&& srcA);
    
    /**
    * @brief Move assignment
    * 
    * @param lhsA Members of lhsA are deleted after copying.
    * @return Object containing values of lhsA.
    */
    AddInteger& operator=(AddInteger&& lhsA);
    
    
    /**
    * @brief Destructor
    */
    virtual ~AddInteger() = default;
    
    /**
    * @brief Add m_first and m_second. Result stored at m_first.
    * 
    * @throws AssemblerException if dynamic_cast failed.
    * 
    * @return always true (return unused).
    */
    virtual bool processOperation(void) override;
    
    /**
    * @brief Clear members of integer addition instance.
    * 
    */
    virtual void clearMembers(void) override;
    
private:
    //Forbidden constructor
    AddInteger() = delete;
};

} /* End namespace as */

#endif // ADDINTEGER_H
