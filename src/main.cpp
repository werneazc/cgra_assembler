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
#include <cstdlib>
#include "../header/assembler.h"

/**
* \brief ...
* 
* \param[in] argc Number of command line arguments.
* \param[in] argv Vector containing command line arguments. 
* \return int
*/
int main(int argc, char **argv) {
    namespace po = boost::program_options;
    //!< \brief Abbreviation for boost program option library.
    namespace fs = boost::filesystem;
    //!< \brief Abbreviation for boost file system library.
    
    
    /* Define command line options for cmd-tool.
       help: Shows cmd-tool options
       file: Path to assembler file which shall be processed.
       config: Program configuration file search path. (default=./config.cfg)
     */
    po::options_description desc("Usable options");
    desc.add_options()
        ("help,h", "Show command line options and usability.")
        ("file,f", po::value<std::string>()->required(), "File path to assembler file.")
        ("config,c", po::value<std::string>()->default_value("./config.cfg"), "Assembler configuration file.");
    
    /* Parse cmd-line arguments and store them in variables map.*/
    po::variables_map vm;
    //!< \brief Variable map to store command line options.
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    
    //If help is within vm, show description.
    if(vm.count("help"))
    {
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }
    
    /* Create file system path variable to validate assembler input file.*/
    fs::path filePtr{vm["file"].as<std::string>().c_str()};
    //!< \brief Handle path to assembler file.
    if(!fs::exists(filePtr))
    {
        std::cout << "Assembler file is missing" << std::endl;
        return EXIT_FAILURE;
    }
    else if(!fs::is_regular_file(filePtr))
    {
        std::cout << "Path " << filePtr.string() << " is not a regular file." << std::endl;
        return EXIT_FAILURE;
    }
    else if(filePtr.extension() != ".asm")
    {
        std::cout << "File " << filePtr.filename() << " has wrong file extension." << std::endl;
        return EXIT_FAILURE;
    }
    else if(fs::is_empty(filePtr))
    {
        std::cout << "File " << filePtr.filename() << " is empty." << std::endl;
        return EXIT_FAILURE;
    }
    
    /* Create file system path variable to validate configuration file.*/
    fs::path configPtr{vm["config"].as<std::string>().c_str()};
    //!< \brief Handle path to configuration file.
    as::configMap_type_t configOptions;
    //!< \brief Store options from configuration file for further usage. 
    if(!fs::exists(configPtr))
    {
        std::cout << "Program configuration file is missing" << std::endl;
        return EXIT_FAILURE;
    }
    else if(!fs::is_regular_file(configPtr))
    {
        std::cout << "Path " << configPtr.string() << " is not a regular file." << std::endl;
        return EXIT_FAILURE;
    }
    else if(configPtr.extension() != ".cfg")
    {
        std::cout << "Configuration file " << configPtr.filename() << " has wrong file extension." << std::endl;
        return EXIT_FAILURE;
    }
    else if(fs::is_empty(configPtr))
    {
        std::cout << "Configuration file " << configPtr.filename() << " is empty." << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        /*Load configuration file and parse.*/
        std::filebuf fb;
        if(fb.open(configPtr.c_str(), std::ios::in))
        {
            std::istream is(&fb);
            auto parsed_options = po::parse_config_file(is, desc, true);
            po::store(parsed_options, vm);
            fb.close();
            
            
            for (const auto& o : parsed_options.options)
                if (o.unregistered)
                    configOptions.emplace(o.string_key, o.value.front());
        }
        else
        {
            std::cout << "Error while loading configuration file." << std::endl;
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}
