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

#ifndef IASSEMBLE_H
#define IASSEMBLE_H

#include <string>

namespace as {
    

/**
* @interface IAssemble
* 
* @brief Interface to enable assembling functionality where necessary. 
*/
class IAssemble
{
public:
    /**
    * @brief Interface function to implement assemble functionality.
    * 
    * @return std::string VCGRA machine code string.
    */
    virtual std::string assemble(void) = 0;
    
private:
    
    //Forbidden constructors
    IAssemble() = delete;
    IAssemble(const IAssemble& src) = delete;
    IAssemble(IAssemble&& src) = delete;
    IAssemble& operator=(const IAssemble& src) = delete;
    IAssemble& operator=(IAssemble&& src) = delete;
};

} /*End namespace as */

#endif // IASSEMBLE_H
