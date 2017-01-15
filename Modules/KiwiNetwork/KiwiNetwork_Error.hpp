/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_NETWORK_ERROR_HPP_INCLUDED
#define KIWI_NETWORK_ERROR_HPP_INCLUDED

#include <stdexcept>

namespace kiwi
{
    namespace network
    {
        // ==================================================================================== //
        //                                          ERROR                                       //
        // ==================================================================================== //
        
        //! @brief The exception.
        //! @details The class defines a generic execption thrown by the network layer.
        
        class Error : public std::runtime_error
        {
        public:
            explicit Error(const std::string& message) :
            std::runtime_error(std::string("kiwi::network:") + message) {}
            
            explicit Error(const char* message) :
            std::runtime_error(std::string("kiwi::network:") + std::string(message)) {}
            
            virtual inline ~Error() noexcept = default;
        };
    }
}


#endif // KIWI_NETWORK_ERROR_HPP_INCLUDED
