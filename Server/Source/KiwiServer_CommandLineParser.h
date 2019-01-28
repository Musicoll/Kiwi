/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#pragma once

#include <string>

namespace kiwi
{
    //! @brief A simple command line parser
    //! @details based on http://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c/868894#868894
    class CommandLineParser
    {
    public:
        
        //! @brief Constructor.
        //! @param argc Argument count.
        //! @param argv Arguments.
        CommandLineParser(int argc, char const* const* const argv)
        {
            for(int i=1; i < argc; ++i)
            {
                tokens.emplace_back(argv[i]);
            }
        }
        
        //! @brief Destructor.
        ~CommandLineParser() = default;
        
        //! @brief Get a command option or an empty string if the option is not found.
        std::string const& getOption(std::string const& option) const
        {
            auto it = std::find(tokens.begin(), tokens.end(), option);
            
            if (it != tokens.end() && ++it != tokens.end())
            {
                return *it;
            }
            
            return empty_string;
        }
        
        //! @brief Returns true if the command option exists.
        bool hasOption(const std::string& option) const
        {
            return (std::find(tokens.begin(), tokens.end(), option) != this->tokens.end());
        }
        
    private: // variables
        
        static const std::string empty_string;
        std::vector<std::string> tokens;
    };
    
    const std::string CommandLineParser::empty_string = {""};
}
