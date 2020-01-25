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

#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

#include <exception>
#include <sstream>
#include <string>

namespace as
{

/**
 * \class AssemblerException
 *
 * \brief Exception handler for configuration file failures.
 *
 * \details
 * Constructor contains error ID (default = 0) and error message
 *
 */
class AssemblerException : public std::exception
{
  public:
    /**
     * \brief Standard constructor
     *
     * \param[in] messageA Error description
     * \param[in] errorIdA Error ID (default = 0)
     */
    AssemblerException(const std::string &messageA, const unsigned errorIdA = 0) : m_message{messageA}, m_id{errorIdA}
    {
    }

    /**
     * \brief Return error message.
     *
     * \return Error message format: Error <ID>: <Message>
     */
    virtual const char *what() const throw() override
    {

        std::ostringstream t_os;
        t_os.str("");

        t_os << "Error " << m_id << ": " << m_message << std::endl;

        m_os = t_os.str();

        return m_os.data();
    }

    /**
     * \brief Destructor
     */
    virtual ~AssemblerException() = default;

  private:
    // Member
    std::string m_message;
    //!< \brief Store error message.
    unsigned m_id;
    //!< \brief Store error ID.
    static std::string m_os;
    //!< \brief Static member function to create output message for what variable.

    // Forbidden Constructors
    AssemblerException() = delete;
};

} // End namespace as

#endif // MYEXCEPTION_H
