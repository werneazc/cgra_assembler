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

#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <cstdint>
#include "../header/assembler.h"
#include "../header/myException.h"

namespace {
const boost::regex c_eLine("^([_A-Z]+)((\\s\\d+){0,3})\\s*$");
//!< \brief Regular expression to parse assembler file line 
const boost::regex c_eOne("(\\s\\d+)");
const boost::regex c_eTwo("(\\s\\d+)(\\s\\d+)");
const boost::regex c_eThree("(\\s\\d+)(\\s\\d+)(\\s\\d+)");
//!< \brief Regular expression to parse address line and place.


/**
* \brief Detect and set command type.
* 
* \details
* The assembler has four different kinds: NoOperator, OneOperator,
* TwoOperator, ThreeOperator. Which operation are in which type
* is specified in SW configuration file. The information are 
* extracted from configA. cmdA is used to search within the 
* extracted information. If the cmdA is found in the extracted 
* type the operation type is set.
* 
* \param[in] cmdA Command string from assembler file.
* \param[out] pObjA Parse object to store command string and code type.
* \param[in] configA Configuration file parameters.
*/
void setCodeType(const std::string& cmdA, as::ParseObject& pObjA, as::configMap_type_t& configA)
{
    
    //Temporary variables
    std::unordered_map<std::string, as::ParseObject::CODE_TYPE> t_codeMap;
    constexpr std::array<as::ParseObject::CODE_TYPE, 4> c_avialableCodeType {{
        as::ParseObject::CODE_TYPE::NO_OP,
        as::ParseObject::CODE_TYPE::ONE_OP,
        as::ParseObject::CODE_TYPE::TWO_OP,
        as::ParseObject::CODE_TYPE::THREE_OP
    }};
    
    //Check that configuration file contains needed parameters
    {
        uint8_t idx{0u};
        for(const auto& property : {"Assembler_Property.NoOperator", "Assembler_Property.OneOperator",\
            "Assembler_Property.TwoOperator", "Assembler_Property.ThreeOperator"})
            {
                if(configA.count(property))
                {
                    auto t_str = configA[property];
                    {
                        boost::tokenizer<boost::escaped_list_separator<char>> t_tok(t_str);
                        for(auto& tok : t_tok)
                            t_codeMap.emplace(tok, c_avialableCodeType[idx]);
                    }
                }
                else
                {
                    std::ostringstream t_msg;
                    t_msg << "Option \"" << property << "\" missing in configuration file";
                    throw as::AssemblerException (t_msg.str(), 2000 + idx);
                }
                ++idx;
            }
    }
    
    //Detect if actual command is available and set parameters in parse object.
    if(t_codeMap.count(cmdA))
    {
        pObjA.cmd = cmdA;
        pObjA.codeType = t_codeMap.at(cmdA);
    }
    else
    {
        std::ostringstream t_msg;
        t_msg << "Command \"" << cmdA << "\" unknown";
        throw as::AssemblerException (t_msg.str(), 2010);
    }
    
    return;
}


} //End anonymous namespace

namespace as {

Assembler::Assembler(fs::path& filePathA, as::configMap_type_t& configA, std::ostream& logA) :
    m_filePath(filePathA), m_config(configA), m_log(logA)
{
    
    //Validate output file option from configuration:
    if(!m_config.count("General.Output"))
        throw as::AssemblerException ("Option \"General.Output\" missing in configuration file", 1000);
    
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
    }
    
    //Store filename to variable
    m_outFileName = m_outPath.filename();
    //Validate file extension.
    if(m_outFileName.extension() != ".vmc")
        throw as::AssemblerException ("Output file has wrong file extension. Expected extension \".vmc\"", 1001);
    
    return;
    
}

void Assembler::parse(void)
{
    
    m_log << "Start parsing assembler file" << std::endl;
    m_log << "----------------------------" << std::endl;
    
    //Open file
    std::filebuf t_fb;
    if(t_fb.open(m_filePath.c_str(), std::ios::in))
    {
        //Temporary variables to handle lines of file
        std::istream t_is(&t_fb);
        std::string t_str;
        uint16_t t_count{1};
        boost::smatch t_LineMatch;
        
        //Iterate over file lines:
        do{
            std::getline(t_is, t_str);
            
            m_log << "Parsed Assembler line " << t_count << ": " << t_str << std::endl;
            
            if(t_str.front() == '#' || t_str.empty())
            {
                ++t_count;
                continue;
            }
            else
            {
                if(boost::regex_match(t_str, t_LineMatch, c_eLine))
                {
                    //Temporary variables
                    ParseObject t_po;
                    auto t_cmd = t_LineMatch[1].str();
                    
                    //Set command code type for 
                    try 
                    {
                        setCodeType(t_cmd, t_po, m_config);
                    }
                    catch(as::AssemblerException& e){
                        m_log << e.what() << std::endl;
                        //Re-Throw message
                        throw;
                    }
                    
                    //Parse additional address and/or line and/or place.
                    bool t_match{false};
                    boost::smatch t_NumbersMatch;
                    switch (t_po.codeType)
                    {
                        case as::ParseObject::CODE_TYPE::NO_OP:
                            break;
                        case as::ParseObject::CODE_TYPE::ONE_OP:
                            t_match = boost::regex_match(t_LineMatch[2].str(), t_NumbersMatch, c_eOne);
                            break;
                        case as::ParseObject::CODE_TYPE::TWO_OP:
                            t_match = boost::regex_match(t_LineMatch[2].str(), t_NumbersMatch, c_eTwo);
                            break;
                        case as::ParseObject::CODE_TYPE::THREE_OP:
                            t_match = boost::regex_match(t_LineMatch[2].str(), t_NumbersMatch, c_eThree);
                            break;
                        default:
                            break;
                    };
             
                    if(t_match)
                    {
                        switch(t_NumbersMatch.size())
                        {
                            case 4: //ThreeOperator
                                t_po.addr = std::stoi(t_NumbersMatch[1].str());
                                t_po.cacheLine = std::stoi(t_NumbersMatch[2].str());
                                t_po.place = std::stoi(t_NumbersMatch[3].str());
                                break;
                            case 3://TwoOperator
                                t_po.addr = std::stoi(t_NumbersMatch[1].str());
                                t_po.cacheLine = std::stoi(t_NumbersMatch[2].str());
                                t_po.place = 127;
                                break;
                            case 2://OneOperator
                                t_po.cacheLine = std::stoi(t_NumbersMatch[1].str());
                                break;
                            case 1://NoOperator
                                break;
                            default://Unknown
                            {
                                std::ostringstream t_msg{""};
                                t_msg << "Syntax error line " << t_count << ". Unknown command code type." << std::endl;
                                throw AssemblerException(t_msg.str(), 1002);
                            }
                        };
                        
                    }
                    
                    //Store parsed information from assembler file for further processing.
                    m_fileContent.push_back(t_po);
                        
                }
                else
                {
                    std::ostringstream t_msg{""};
                    t_msg << "Syntax error line " << t_count << ". Please correct assembler file and repeat." << std::endl;
                    throw AssemblerException(t_msg.str(), 1002);
                }
                
                ++t_count;
            }
        }while(!t_is.eof());
    }
    else
        throw as::AssemblerException("Error while opening assembler source file for parsing", 1002);
    
    
    m_log << "Parsing of assembler input file successfully finished." << std::endl;
    
    return;
}

void Assembler::assemble(void)
{
    
    m_log << "Starting assembling of file content." << std::endl;
    m_log << "------------------------------------" << std::endl;
    
    //Temporary variables:
    boost::format t_fmtStr("0x%1$04X%2$04X");
    // Format string to write machine code for assembler line.
    std::ostringstream t_mc;
    // Collect generated machine code from assembler
    
    //Validate, that necessary config file options available
    uint8_t idx{0};
    for(const auto& property : {"Assembler_Property.LineSize","Assembler_Property.PlaceSize",
        "Assembler_Property.OpCodeSize", "VCGRA_Property.Available_Memory", "VCGRA_Property.Num_Dic_Lines",
        "VCGRA_Property.Num_Dic_Places", "VCGRA_Property.Num_Doc_Lines", "VCGRA_Property.Num_Doc_Places",
        "VCGRA_Property.Num_PC_Lines", "VCGRA_Property.Num_CC_Lines" })
    {
        if(!m_config.count(property))
        {
            std::ostringstream t_msg;
            t_msg << "Option \"" << property << "\" missing in configuration file";
            throw as::AssemblerException (t_msg.str(), 3000 + idx);
        }
        ++idx;
    }
    
    //Properties for code assembling loaded from configuration file.
    auto t_MemorySize = std::stoi(m_config["VCGRA_Property.Available_Memory"]);
    auto t_placeSize = std::stoi(m_config["Assembler_Property.PlaceSize"]);
    auto t_opCodeSize = std::stoi(m_config["Assembler_Property.OpCodeSize"]);
    uint16_t t_as;  //Local variable to build machine operation code.
    
    for(auto& po : m_fileContent)
    {
        
        if(po.addr > t_MemorySize)
        {
            std::ostringstream t_msg;
            t_msg << "In assembler file line " << po.fileLine << ": Addressed Memory " << po.addr << " out of available address space." << std::endl;
            throw as::AssemblerException (t_msg.str(), 3010);
        }
        
        std::ostringstream t_property;
        t_property << "Assembler_Property." << po.cmd;
        try 
        {
            auto t_opCode = std::stoi(m_config[t_property.str()]);
            t_as = 0;
            t_as |= po.cacheLine << (t_placeSize + t_opCodeSize);
            t_as |= po.place << t_opCodeSize;
            t_as |= t_opCode;
        }
        catch( const std::exception& e)
        {
            std::ostringstream t_msg;
            t_msg << "Error in generating machine code: " << e.what();
            throw AssemblerException (t_msg.str(), 4011);
        }

        
        t_mc << "\"" << t_fmtStr % po.addr % t_as << "\"," << std::endl;
    }
    
    m_mc = t_mc.str().substr(0, t_mc.str().length() - 2);
    
    m_log << "Machine code successfully finished." << std::endl;
    
    return;
}

void Assembler::writeVmcFile()
{
    
    //Opening file to store machine code.  
    std::filebuf t_fb;
    if(t_fb.open(m_outPath.c_str(), std::ios::out))
    {
        std::ostream t_codeFile(&t_fb);
        t_codeFile << m_mc << std::endl;
        t_fb.close();
    }
    else
    {
        throw AssemblerException("Error while opening output file.", 4500);
    }
    
    m_log << "Machine operation code successfully stored at " << m_outPath << std::endl;
    
    return;
    
}

} //End namespace as
