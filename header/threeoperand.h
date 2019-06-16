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

#ifndef THREEOPERAND_H
#define THREEOPERAND_H

#include <tuple>
#include "parseobjbase.h"
#include "iassemble.h"

namespace as 
{

//Forward declaration
class Level;
    
class ThreeOperand : public ParseObjBase, public IAssemble
{
public:
    
    typedef std::tuple<ParseObjBase* const, ParseObjBase* const, ParseObjBase* const> operands_type_t;
    //!< @brief 
    
    ThreeOperand(Level* const lvlA, const std::string& cmdLineA, const uint32_t lineNumber,
        ParseObjBase* const firstA, ParseObjBase* const secondA, ParseObjBase* const thridA, 
        const uint32_t machienIdA);
    
    ThreeOperand(const ThreeOperand& srcA);
    
    ThreeOperand(ThreeOperand&& srcA);
    
    virtual ~ThreeOperand(void) = default;
    
    ThreeOperand& operator=(const ThreeOperand& rhsA);
    
    ThreeOperand& operator=(ThreeOperand&& rhsA);
    
    ParseObjBase* const getFirst(void) const;
    
    ParseObjBase* const setFirst(void);
    
    ParseObjBase* const getSecond(void) const;
    
    ParseObjBase* const setSecond(void);
    
    ParseObjBase* const getThird(void) const;
    
    ParseObjBase* const setThird(void);
    
    operands_type_t getOperands(void) const;
    
    operands_type_t getOperands(const operands_type_t& operandsA);
    
    operands_type_t getOperands(ParseObjBase* const firstA, 
                                ParseObjBase* const secondA,
                                ParseObjBase* const thirdA);
    
    uint32_t getMachineCodeId(void) const;
    
    uint32_t setMachineCodeId(const uint32_t machineIdA);
    
    virtual void clearMembers(void) override;
    
    virtual std::string assemble(const as::Assembler & asA) override final;
    
private:
    ParseObjBase* m_first;
    //!< @brief 
    ParseObjBase* m_second;
    //!< @brief 
    ParseObjBase* m_third;
    //!< @brief 
    uint32_t m_machineCodeId;
    //!< @brief 
};

} /* End namespace as */

#endif // THREEOPERAND_H
