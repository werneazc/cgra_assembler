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

#ifndef ONEOPERAND_H
#define ONEOPERAND_H

#include "parseobjbase.h"
#include "iassemble.h"

namespace as 
{

//Forward declaration
class Level;
    
/**
* @class OneOperand
* 
* @brief Parse object for assembler commands with one operand.
*
* @details
* This parse object is for assembler commands with only one operand.
* The operand is stored as a pointer to the base class and needs to be 
* interpreted as the necessary parse object class. Moreover, the parse object
* stored the machine code ID. 
*/
class OneOperand : public ParseObjBase, public IAssemble
{
public:
    /**
    * @brief General constructor
    * 
    * @param lvlA Level where ome-operand command exists.
    * @param cmdLineA Command line from assembler file.
    * @param lineNumberA Line number of assembler file.
    * @param firstA Pointer to operand of assembler command.
    * @param machineIdA Machine code ID for assembler command.
    */
    OneOperand(Level* const lvlA, const std::string& cmdLineA, uint32_t lineNumberA,
        ParseObjBase* const firstA, const uint32_t machineIdA);
    
    /**
    * @brief Copy constructor
    * 
    * @param srcA Source for initialization of members for new object.
    */
    OneOperand(const OneOperand& srcA);
    
    /**
    * @brief Copy assignment
    * 
    * @param rhsA Source for member variables to assign.
    * @return Object with a copy of member values of rhsA.
    */
    OneOperand& operator=(const OneOperand& rhsA);
    
    /**
    * @brief Move constructor
    * 
    * @param srcA Source to initialize members of new object. Members of srcA are deleted.
    */
    OneOperand(OneOperand&& srcA);
    
    /**
    * @brief Move assignment
    * 
    * @param rhsA Source for member variables to assign. Members of rhsA are deleted.
    * @return Object with assigned member values of rhsA.
    */
    OneOperand& operator=(OneOperand&& rhsA);
    
    /**
    * @brief Destructor
    */
    ~OneOperand() = default;
    
    /**
    * @brief Delete values of private members
    */
    virtual void clearMembers(void) override;
    
    /**
    * @brief Get operand of assembler command.
    * 
    * @return as::ParseObjBase* const
    */
    ParseObjBase* const getFirst(void) const;
    
    /**
    * @brief Set new operand for assembler command.
    * 
    * @param firstA Pointer to new operand of assembler command.
    * @return Pointer to previous operand of assembler command.
    */
    ParseObjBase* const setFirst(ParseObjBase* const firstA);
    
    /**
    * @brief Create machine code from assembler command.
    * 
    * @param ptreeA Property tree with configuration values from SW configuration file
    * @return Machine code line for VCGRA instance.
    */
    virtual std::string assemble(const boost::property_tree::ptree& ptreeA) override final;
    
private:
    //Forbidden constructors
    OneOperand(void) = delete;
    
    //Private member
    uint32_t m_machnineCodeID;
    //!< @brief Machine code ID for assembler command
    ParseObjBase* m_first;
    //!< @brief Pointer to operand of assembler command
    
};

} /* End namespace as */

#endif // ONEOPERAND_H
