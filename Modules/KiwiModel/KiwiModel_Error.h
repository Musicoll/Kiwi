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

namespace kiwi { namespace model {

    // ==================================================================================== //
    //                                          ERROR                                       //
    // ==================================================================================== //
    
    //! @brief A generic exception for engine failures.
    class Error : public std::runtime_error
    {
    public:
        
        //! @brief The std::string constructor.
        explicit Error(std::string const& message)
        : std::runtime_error(message)
        {}
        
        //! @brief The const char* constructor.
        explicit Error(const char* message)
        : std::runtime_error(std::string(message))
        {}
        
        //! @brief The destructor.
        virtual inline ~Error() noexcept = default;
    };
    
}}
