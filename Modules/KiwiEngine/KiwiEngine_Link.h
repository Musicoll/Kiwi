/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
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

#include "KiwiEngine_Def.h"

namespace kiwi
{    
    namespace engine
    {
        // ================================================================================ //
        //                                      LINK                                        //
        // ================================================================================ //
        
        //! @brief The Link represents the connection between the outlet of an Object to the inlet of another.
        class Link
        {
        public: // methods
            
            //! @brief Constructor.
            Link(Object & receiver, size_t index);
            
            //! @brief Copy constructor
            Link(Link const& other) = default;
            
            //! @brief Move constructor
            Link(Link && other) = default;
            
            //! @brief Destructor.
            ~Link() = default;
            
            //! @brief Comparison operator
            bool operator<(Link const& other) const;
            
            //! @brief Gets the Object that receives messages.
            Object & getReceiver() const;
            
            //! @brief Gets the index of the inlet of the Link.
            size_t getReceiverIndex() const;
            
        private: // members
            
            engine::Object & m_receiver;
            size_t           m_index;
            
        private: // deleted methods
            
            Link& operator=(Link const&) = delete;
            Link& operator=(Link&&) = delete;
        };
    }
}
