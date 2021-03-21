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
#include "assembler.h"
#include "myException.h"
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <cstdlib>
#include <iostream>

// String variable to create error message in exception.
std::string as::AssemblerException::m_os;

/**
 * \brief ...
 *
 * \param[in] argc Number of command line arguments.
 * \param[in] argv Vector containing command line arguments.
 * \return int
 */
int main(int argc, char **argv)
{
    namespace po = boost::program_options;
    //!< @brief Abbreviation for boost program option library.
    namespace fs = boost::filesystem;
    //!< @brief Abbreviation for boost file system library.
    namespace pt = boost::property_tree;
    //!< @brief Abbreviation for boost file system library.
    pt::ptree parsed_options{};
    //!< @brief Property map with configuration options from config file.

    /* Define command line options for cmd-tool.
       help: Shows cmd-tool options
       file: Path to assembler file which shall be processed.
       config: Program configuration file search path. (default=./config.cfg)
     */
    po::options_description desc("Usable options");
    desc.add_options()("help,", "Show command line options and usability.")(
        "file,", po::value<std::string>()->required(), "File path to assembler file.")(
        "config,", po::value<std::string>()->default_value("./config.cfg"),
        "Assembler configuration file.")("log,", po::value<std::string>(), "Log file path.");

    /* Parse cmd-line arguments and store them in variables map.*/
    po::variables_map vm;
    //!< \brief Variable map to store command line options.
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // If help is within vm, show description.
    if (vm.count("help") != 0U)
        {
            std::cout << desc << std::endl;
            return EXIT_SUCCESS;
        }

    if (vm.count("log") != 0U)
        {
            /* Create file system path variable for log file.*/
            fs::path logPtr{vm["log"].as<std::string>().c_str()};
            //!< \brief Handle path to assembler file.
            if (!fs::exists(logPtr))
                {
                    if (logPtr.parent_path().string() != ".")
                        {
                            fs::create_directories(logPtr.parent_path());
                        }
                }
            else
                {
                    std::cout << "Warning: Logfile exists and will be overwritten." << std::endl;
                }
        }

    /* Create file system path variable to validate assembler input file.*/
    fs::path filePtr{vm["file"].as<std::string>().c_str()};
    //!< \brief Handle path to assembler file.
    if (!fs::exists(filePtr))
        {
            std::cout << "Assembler file is missing" << std::endl;
            return EXIT_FAILURE;
        }

    if (!fs::is_regular_file(filePtr))
        {
            std::cout << "Path " << filePtr.string() << " is not a regular file." << std::endl;
            return EXIT_FAILURE;
        }

    if (filePtr.extension() != ".asm")
        {
            std::cout << "File " << filePtr.filename() << " has wrong file extension." << std::endl;
            return EXIT_FAILURE;
        }

    if (fs::is_empty(filePtr))
        {
            std::cout << "File " << filePtr.filename() << " is empty." << std::endl;
            return EXIT_FAILURE;
        }

    /* Create file system path variable to validate configuration file.*/
    fs::path configPtr{vm["config"].as<std::string>().c_str()};
    //!< \brief Handle path to configuration file.
    if (!fs::exists(configPtr))
        {
            std::cout << "Program configuration file is missing" << std::endl;
            return EXIT_FAILURE;
        }

    if (!fs::is_regular_file(configPtr))
        {
            std::cout << "Path " << configPtr.string() << " is not a regular file." << std::endl;
            return EXIT_FAILURE;
        }

    if (configPtr.extension() != ".xml")
        {
            std::cout << "Configuration file " << configPtr.filename() << " has wrong file extension." << std::endl;
            return EXIT_FAILURE;
        }

    if (fs::is_empty(configPtr))
        {
            std::cout << "Configuration file " << configPtr.filename() << " is empty." << std::endl;
            return EXIT_FAILURE;
        }

    /*Load configuration file and parse.*/
    std::filebuf fb;
    if (fb.open(configPtr.c_str(), std::ios::in) != nullptr)
        {
            std::istream is(&fb);
            pt::read_xml(is, parsed_options);
            fb.close();
        }
    else
        {
            std::cout << "Error while loading configuration file." << std::endl;
            return EXIT_FAILURE;
        }

    try
        {
            // Run assembler with log file
            if (vm.count("log") != 0U)
                {

                    /* Create file system path variable for log file.*/
                    fs::path logPtr{vm["log"].as<std::string>().c_str()};

                    std::filebuf fb;
                    if (fb.open(logPtr.c_str(), std::ios::out) != nullptr)
                        {
                            std::ostream log_os(&fb);
                            as::Assembler myAs(filePtr, parsed_options, log_os);
                            myAs.parse();
                            myAs.assemble();
                            // myAs.writeVmcFile();
                            fb.close();
                        }
                }
            else // Run assembler with printing on std cout.
                {
                    as::Assembler myAs(filePtr, parsed_options);
                    myAs.parse();
                    myAs.assemble();
                    // myAs.writeVmcFile();
                }
        }
    catch (const as::AssemblerException &ce)
        {
            std::cout << ce.what() << std::endl;
            return EXIT_FAILURE;
        }
    catch (const std::exception &e)
        {
            std::cout << "Std. error: " << e.what() << std::endl;
            return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;
}
