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

#ifndef NOOPERAND_H
#define NOOPERAND_H

#include <cstdint>
#include "parseobjbase.h"
#include "iassemble.h"

namespace as
{


//Forward declaration
class Level;

    
/**
* @class NoOperand
* 
* @brief Parse object for assembler operations with no operand.
* 
* @details
* Operands of this kind generate machine codes for VCGRA. Examples are START,
* WAIT_RAIDY or FINISH. The IDs for machine code are stored in private Member.
*/
class NoOperand : public ParseObjBase, public IAssemble
{
public:
    /**
    * @brief General constructor
    * 
    * @param lvlA Level where the parse object exists.
    * @param cmdLineA Assembler file line with command.
    * @param lineNumberA Assembler file line number.
    * @param machineIdA Machine Code ID for decode at VCGRA.
    */
    NoOperand(Level* const lvlA, const std::string& cmdLineA, const uint32_t lineNumberA,
        const uint32_t machineIdA);
    
    /**
    * @brief Copy constructor
    * 
    * @param srcA Source for members of new object.
    */
    NoOperand(const NoOperand& srcA);
    
    /**
    * @brief Move constructor
    * 
    * @param srcA Source for members of new object. Members of source are deleted.
    */
    NoOperand(NoOperand&& srcA);
    
    /**
    * @brief Copy assignment
    * 
    * @param rhsA Source for members of assigned object.
    * @return Object with member values of rhsA.
    */
    NoOperand& operator=(const NoOperand& rhsA);
    
    /**
    * @brief Move assignment
    * 
    * @param rhsA Source for members of new object. Members of source are deleted.
    * @return Object with member values of rhsA.
    */
    NoOperand& operator=(NoOperand&& rhsA);
    
    /**
    * @brief Destructor
    */
    virtual ~NoOperand(void) = default;
    
    /**
    * @brief Delete all member variables.
    */
    virtual void clearMembers(void) override;
    
    /**
    * @brief Get machine code ID.
    * 
    * @return Current machine code ID.
    */
    uint32_t getMachineCodeId(void) const;
    
    /**
    * @brief Set new machine code ID.
    * 
    * @param machineIdA New machine code ID.
    * @return Former machine code ID.
    */
    uint32_t setMachineCodeId(const uint32_t machineIdA);
    
    /**
    * @brief Create machine code from assembler command.
    * 
    * @param asA Assembler object containing VCGRA configuration parameter
    * @return Machine code line for VCGRA instance.
    */
    virtual std::string assemble(const Assembler& asA) override final;
    
private:
    //Forbidden constructor
    NoOperand() = delete;
    
    //Private member variables
    uint32_t m_machineCodeID;
    //!< @brief Machine code Identification code for command fetching and decode.
};

} /* End namespace as */
#endif // NOOPERAND_H
