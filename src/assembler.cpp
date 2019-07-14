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

#include <iostream>
#include <cstdint>
#include <locale>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/exceptions.hpp>
#include "assembler.h"
#include "loop.h"
#include "myException.h"
#include "parseobjectvariable.h"
#include "parseobjectconst.h"
#include "nooperand.h"
#include "oneoperand.h"
#include "twooperand.h"
#include "threeoperand.h"


namespace {
    
const boost::regex c_eVariable("VAR\\s+(\\w+)\\s+(\\w+)\\s*$");
//!< \brief Regular expression to parse a variable line
const boost::regex c_eConstant("CONST\\s+(\\w+)\\s+(\\d+)\\s*$");
//!< \brief Regular expression to parse a constant line
const boost::regex c_eLoop("LOOP\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s*$");
//!< \brief Regular expression to parse a start line of a loop construct
const boost::regex c_ePool("POOL");
//!< \brief Regular expression to parse a end line of a loop construct
const boost::regex c_eCommand("([\\u_]+)(\\s+\\w+){0,3}\\s*$");
//!< \brief Regular expression to parse an assembler command
const boost::regex c_eOne("\\s(\\w+)");
//!< \brief Regular expression to parse one argument
const boost::regex c_eTwo("\\s(\\w+)\\s(\\w+)");
//!< \brief Regular expression to parse two arguments
const boost::regex c_eThree("\\s(\\w+)\\s(\\w+)\\s(\\w+)");
//!< \brief Regular expression to parse three arguments


/**
* @brief Check that string is a number.
* 
* @param strA Constant reference to search string.
* @return True if all characters in search string are digits
*/
bool is_number(const std::string& strA)
{
    bool t_status{true};
    for(auto it = strA.cbegin(); it != strA.cend(); ++it)
    {
        if(!std::isdigit(*it))
        {
            t_status = false;
            break;
        }
    }
    
    return t_status;
    
}

} //End anonymous namespace 

namespace as {

Assembler::Assembler(boost::filesystem::path& filePathA, 
                     boost::property_tree::ptree& configA, 
                     std::ostream& logA) :
    m_filePath(filePathA), m_config(configA), m_log(logA), m_firstLevel{new Level()}
{
    
    try
    {
        //Initialize output file path variable and validate.
        m_outPath = m_config.get<std::string>("General.Output");
    }
    catch(boost::property_tree::ptree_error& e)
    {
        throw as::AssemblerException ("Option \"General.Output\" missing in configuration file", 1000);
    }
    
    if(boost::filesystem::exists(m_outPath))
        m_log << "Warning: File" << m_outPath.filename() << " will be replaced." \
        << std::endl;
    else 
    {
        if(m_outPath.parent_path().string() != ".")
        {
            if(boost::filesystem::create_directories(m_outPath.parent_path()))
                m_log << "Info: Create output file directory." << std::endl;
        }
    }
    
    //Store filename to variable
    m_outFileName = m_outPath.filename();
    //Validate file extension.
    if(m_outFileName.extension() != ".vmc")
        throw as::AssemblerException ("Output file has wrong file extension. Expected extension \".vmc\"", 1001);
    
    Level::setCurrentLevel(m_firstLevel);
    
    return;
    
}

Assembler::~Assembler()
{
    delete m_firstLevel;
}

void Assembler::parse(void)
{
    m_log << "Start parsing assembler file" << std::endl;
    m_log << "----------------------------" << std::endl;
    
    //Load list of available operations and there machine ID from configuration file
    std::unordered_map<std::string, std::vector<std::pair<std::string, uint8_t> > > t_commandMap{};
    
    try
    {
        //Temporary create vector of available operations and there machine Id of an command class.
        std::vector<std::pair<std::string, uint8_t> > t_vec;
        
        //Iterate over available command classes
        for(const std::string& iter : {"Assembler_Property.NoOperator",
            "Assembler_Property.OneOperator",
            "Assembler_Property.TwoOperator",
            "Assembler_Property.ThreeOperator",
            "Assembler_Property.ArithOperator"})
        {
            for(const auto& op : m_config.get_child(iter))
                t_vec.emplace_back(op.second.get<std::string>("Name"), op.second.get<uint8_t>("MachineId", UINT8_MAX));
            
            t_commandMap.emplace(iter.substr(iter.find_first_of('.') + 1), std::move(t_vec));
        }
        
        m_log << "Available commands: " << '\n';
        m_log << "====================" << '\n';
            
        for(const auto& iter : t_commandMap)
            for(const auto& com : iter.second)
                m_log << com.first << ",";
            
        m_log << "\n\n\n" << std::endl;
    }
    catch(boost::property_tree::ptree_error& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
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
            else if(boost::regex_search(t_str, t_LineMatch, c_eLoop))
            {
                //Temporary variables to store loop parameters from parsed command string
                int32_t t_start{0};
                int32_t t_end{0};
                int32_t t_step{0};
                uint8_t t_countval{1};
                    
                //Get start, end and step value value for Loop
                for(int32_t* const val : {&t_start, &t_end, &t_step})
                {
                    
                    if(is_number(t_LineMatch[t_countval].str()))
                    {
                        *val = std::stoi(t_LineMatch[t_countval].str());
                    }
                    else
                    {
                        auto t_searchResult = Level::getCurrentLevel()->findParseObj(t_LineMatch[t_countval].str());
                        if(t_searchResult)
                        {
                            if(t_searchResult->getCommandClass() == COMMANDCLASS::VARIABLE)
                                *val = static_cast<ParseObjectVariable*>(t_searchResult)->getVariableValue();
                            else if(t_searchResult->getCommandClass() == COMMANDCLASS::CONSTANT)
                                *val = static_cast<ParseObjectConst*>(t_searchResult)->getConstValue();
                            else
                            {
                                std::ostringstream t_msg{""};
                                t_msg << "Syntax error line " << t_count << \
                                    ". Arguments for LOOP command invalid." << std::endl;
                                throw AssemblerException(t_msg.str(), 1020);
                            }
                        }
                    }
                    
                    ++t_countval;
                }
                
                //Create new level as a loop
                Loop* t_loopPtr = new Loop(Loop::getCurrentLevel(), t_count, t_start, t_end, t_step,t_LineMatch[0].str());
                Level::getCurrentLevel()->addChildLevel(static_cast<Level*>(t_loopPtr));
                
                //Set actual level to new created loop
                Level::setCurrentLevel(static_cast<Level*>(t_loopPtr));
                
                ++t_count;
                
            }
            else if(boost::regex_search(t_str, t_LineMatch, c_ePool))
            {
                Level::getCurrentLevel()->leave();
                ++t_count;
                
            }
            else if(boost::regex_search(t_str, t_LineMatch, c_eVariable))
            {
                //Temporary variable to store value of assembler variable
                int32_t t_value{0};
                
                if(is_number(t_LineMatch[2].str()))
                {
                    t_value = std::stoi(t_LineMatch[2]);
                }
                else
                {
                    
                    auto t_valPtr = Level::getCurrentLevel()->findParseObj(t_LineMatch[2].str());
                    if(t_valPtr)
                    {
                        if(t_valPtr->getCommandClass() == COMMANDCLASS::VARIABLE)
                            t_value = static_cast<ParseObjectVariable*>(t_valPtr)->getVariableValue();
                        else if(t_valPtr->getCommandClass() == COMMANDCLASS::CONSTANT)
                            t_value = static_cast<ParseObjectConst*>(t_valPtr)->getConstValue();
                        else
                        {
                            std::ostringstream t_msg{""};
                            t_msg << "Syntax error line " << t_count << \
                                ". Arguments for VARIABLE command invalid." << std::endl;
                            throw AssemblerException(t_msg.str(), 1030);
                        }
                        
                    }
                    else
                    {
                        std::ostringstream t_msg{""};
                        t_msg << "Syntax error line " << t_count << ". Variable value is invalid." << std::endl;
                        throw AssemblerException(t_msg.str(), 1023);
                    }
                    
                }
                
                if(std::isdigit(t_LineMatch[1].str().front()))
                {
                    std::ostringstream t_msg{""};
                    t_msg << "Syntax error line " << t_count << ". Variable name starts with number." << std::endl;
                    throw AssemblerException(t_msg.str(), 1024);
                }
                
                auto t_pvPtr = new ParseObjectVariable(t_LineMatch[1].str(),
                    t_value, Level::getCurrentLevel() ,t_LineMatch[0].str(),
                    t_count);
                
                //At parse object to current level
                Level::getCurrentLevel()->addParseObj(t_pvPtr);
                
                ++t_count;
                
            }
            else if(boost::regex_search(t_str, t_LineMatch, c_eConstant))
            {
                //Temporary variable to store value of assembler variable
                int32_t t_value{0};
                
                if(is_number(t_LineMatch[2].str()))
                {
                    t_value = std::stoi(t_LineMatch[2]);
                }
                else
                {
                    std::ostringstream t_msg{""};
                    t_msg << "Syntax error line " << t_count << ". Constant value is not an integer." << std::endl;
                    throw AssemblerException(t_msg.str(), 1026);
                }
                
                if(std::isdigit(t_LineMatch[1].str().front()))
                {
                    std::ostringstream t_msg{""};
                    t_msg << "Syntax error line " << t_count << ". Constant name starts with number." << std::endl;
                    throw AssemblerException(t_msg.str(), 1027);
                }
                
                auto t_pcPtr = new ParseObjectConst(t_LineMatch[1].str(),
                    t_value, Level::getCurrentLevel() ,t_LineMatch[0].str(),
                    t_count);
                
                //At parse object to current level
                Level::getCurrentLevel()->addParseObj(t_pcPtr);
                
                ++t_count;
                
            }
            else if(boost::regex_search(t_str, t_LineMatch, c_eCommand))
            {
                //Temporary variable
                boost::smatch t_commandMatch{};
                
                if(boost::regex_search(t_LineMatch[0].str(), t_commandMatch, c_eThree))
                {
                    
                    
                }
                else if(boost::regex_search(t_LineMatch[0].str(), t_commandMatch, c_eTwo))
                {
                    
                    
                }
                else if(boost::regex_search(t_LineMatch[0].str(), t_commandMatch, c_eOne))
                {
                }
                else
                {
                    for(const auto& vec : t_commandMap.find("NoOperator")->second)
                    {
                        if(vec.first == t_LineMatch[0].str())
                        {
                            auto t_parseObj = new NoOperand(Level::getCurrentLevel(),t_LineMatch[0], t_count, vec.second);
                            m_firstLevel->addParseObj(t_parseObj);
                            break;
                        }
                    }
                }
                
                ++t_count;
            }
            else
            {
                std::ostringstream t_msg{""};
                t_msg << "Syntax error line " << t_count << ". Unknown command code type." << std::endl;
                throw AssemblerException(t_msg.str(), 1002);
            }
            
            
        }while(!t_is.eof());
    }
    else
        throw as::AssemblerException("Error while opening assembler source file for parsing", 1202);
    
    
    m_log << "Parsing of assembler input file successfully finished." << std::endl;
    
    return;
}

// void Assembler::assemble(void)
// {
//     return;
// }
// 
// void Assembler::writeVmcFile()
// {
//     
//     //Opening file to store machine code.  
//     std::filebuf t_fb;
//     if(t_fb.open(m_outPath.c_str(), std::ios::out))
//     {
//         std::ostream t_codeFile(&t_fb);
//         t_codeFile << m_mc << std::endl;
//         t_fb.close();
//     }
//     else
//     {
//         throw AssemblerException("Error while opening output file.", 4500);
//     }
//     
//     m_log << "Machine operation code successfully stored at " << m_outPath << std::endl;
//     
//     return;
//     
// }
// 
} //End namespace as
