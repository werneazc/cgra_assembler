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

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 * \namespace as
 *
 * \brief Namespace of VCGRA assembler program.
 */
namespace as
{

// Forward declaration
class Level;

/**
 * \class Assembler
 *
 * \brief Handle and assemble VCGRA assembler to machine code.
 *
 * \details
 * \todo: Add description.
 *
 */
class Assembler
{
  public:
    // Constructor
    /**
     * \brief General constructor.
     *
     * \param[in] filePathA Path to assembler file
     * \param[in] configA Map of parameters from program configuration file.
     * \param[out] logA Logging stream (default = std::cout)
     */
    Assembler(boost::filesystem::path &filePathA, boost::property_tree::ptree &configA, std::ostream &logA = std::cout);

    // Destructor
    /**
     * \brief Destructor
     */
    virtual ~Assembler();

    // Member functions
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
    // void writeVmcFile(void);

  private:
    // Member
    boost::filesystem::path &m_filePath;
    //!< \brief Reference to assembler file.
    boost::property_tree::ptree m_config;
    //!< \brief Reference to configuration map type.
    boost::filesystem::path m_outPath;
    //!< \brief Path to output file.
    boost::filesystem::path m_outFileName;
    //!< \brief Output file name from configuration file.
    std::ostream &m_log;
    //!< \brief Logging string stream (default=std::cout)
    Level *m_firstLevel;
    //!< \brief Pointer to start level of parse document

    // Forbidden Constructor
    Assembler &operator=(const Assembler &src) = delete;
    Assembler(const Assembler &src) = delete;
    Assembler(Assembler &&src) = delete;
    Assembler &operator=(Assembler &&src) = delete;
};

} // End namespace as
#endif // ASSEMBLER_H
