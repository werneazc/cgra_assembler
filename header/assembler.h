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

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <unordered_map>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cstdint>

/**
* \namespace as
* 
* \brief Namespace of VCGRA assembler program.
*/
namespace as {

namespace fs = boost::filesystem;
//!< \brief Abbreviation for boost file system library.
namespace po = boost::program_options;
//!< \brief Abbreviation for boost program option library.

typedef std::unordered_map<std::string, std::string> configMap_type_t;
//!< \brief Type definition for options from configuration file.

/**
* \struct ParseObject
* 
* \brief Store information of parsed assembler file line
* 
* \details
* #TODO: Add description.
* 
*/
struct ParseObject {
    uint16_t fileLine{0};
    //!< \brief Line in assembler file where the command is parsed
    std::string cmd;
    //!< \brief Assembler command
    uint16_t addr{0};
    //!< \brief Shared memory address
    uint8_t cacheLine{0};
    //!< \brief Cache line of target cache
    uint8_t place{0};
    //!< \brief Place in selected cache line
    enum CODE_TYPE : uint8_t {
        UNKNOWN,    //!< \brief Unknown code type
        NO_OP,      //!< \brief Operation with no operator
        ONE_OP,     //!< \brief Operation with one operator
        TWO_OP,     //!< \brief Operation with two operator
        THREE_OP    //!< \brief Operation with three operator
    } codeType{UNKNOWN};
    //!< \brief Code type of assembler operation
};


/**
* \class Assembler
* 
* \brief Handle and assemble VCGRA assembler to machine code.
* 
* \details
* #TODO: Add description.
* 
*/
class Assembler
{
public:
    //Constructor
    /**
    * \brief General constructor.
    * 
    * \param[in] filePathA Path to assembler file
    * \param[in] configA Map of parameters from program configuration file.
    * \param[out] logA Logging stream (default = std::cout)
    */
    Assembler ( boost::filesystem::path& filePathA, configMap_type_t& configA, \
                std::ostream& logA = std::cout );
    
    //Destructor
    /**
    * \brief Destructor
    */
    virtual ~Assembler() = default;
    
    //Member functions
    /**
    * \brief Parse assembler file for further processing
    */
    void parse(void);
    
    /**
    * \brief Create machine code from parsed assembly file
    */
    void assemble(void);
    
    /**
    * \brief Write VCGRA machine code to output file.
    */
    void writeVmcFile(void);
    
private:
    //Member
    fs::path& m_filePath;
    //!< \brief Reference to assembler file.
    as::configMap_type_t m_config;
    //!< \brief Reference to configuration map type.
    fs::path m_outPath;
    //!< \brief Path to output file.
    fs::path m_outFileName;
    //!< \brief Output file name from configuration file.
    std::ostream& m_log;
    //!< \brief Logging string stream (default=std::cout)
    std::vector<ParseObject> m_fileContent;
    //!< \brief Assembler file content without comments.
    std::string m_mc{""};
    //!< \brief Machine code string which should be written to outfile.
    
    //Forbidden Constructor
    Assembler& operator=(const Assembler& src) = delete;
    Assembler(const Assembler& src) = delete;
    Assembler(Assembler&& src) = delete;
    Assembler& operator=(Assembler&& src) = delete;
    
};
} //End namespace as
#endif // ASSEMBLER_H
