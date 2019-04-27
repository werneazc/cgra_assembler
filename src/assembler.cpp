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

#include <boost/regex.hpp>
#include <iostream>
#include "../header/assembler.h"
#include "../header/myException.h"

namespace {
const boost::regex c_eLine("^([A-Z_]+)((\s\d+){0,3})\s*$");
//!< \brief Regular expression to parse assembler file line 
const boost::regex c_eNumbers("(\s\d+)(\s\d+)?(\s\d+)?");
//!< \brief Regular expression to parse assembler file line 
} //End anonymous namespace

namespace as {

Assembler::Assembler(fs::path& filePathA, as::configMap_type_t& configA, std::ostream& logA) :
    m_filePath(filePathA), m_config(configA), m_log(logA)
{
    
    //Validate output file option from configuration:
    if(!m_config.count("General.Output"))
        throw as::AssemblerException ("Option \"General.Output\" missing", 1000);
    
    //Initialize output file path variable and validate.
    m_outPath = m_config["General.Output"];
    
    if(fs::exists(m_outPath))
        m_log << "Warning: File" << m_outPath.filename() << " will be replaced." \
        << std::endl;
    else 
    {
        if(m_outPath.parent_path().string() != ".")
        {
            if(fs::create_directories(m_outPath.parent_path()))
                m_log << "Info: Create output file directory." << std::endl;
        }
        m_outFileName = m_outPath.filename();
    }
    
    if(m_outFileName.extension() != ".vmc")
        throw as::AssemblerException ("Output file has wrong file extension. Expected extension \".vcm\"", 1001);
    
    return;
    
}

void Assembler::parse(void)
{
    //Open file
    std::filebuf t_fb;
    if(t_fb.open(m_filePath.c_str(), std::ios::in))
    {
        //Temporary variables to handle lines of file
        std::istream t_is(&t_fb);
        std::string t_str;
        
        //Iterate over file lines:
        do{
            std::getline(t_is, t_str); 
            std::cout << t_str << std::endl;
        }while(!t_is.eof());
    }
    else
        throw as::AssemblerException("Error while opening assembler source file for parsing", 1002);
    
    return;
}


} //End namespace as
