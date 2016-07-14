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

#ifndef KIWI_ENGINE_LINK_HPP_INCLUDED
#define KIWI_ENGINE_LINK_HPP_INCLUDED

#include "KiwiEngine_Def.hpp"

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
            Link(model::Link const& model);
            
            //! @brief Destructor.
            ~Link() = default;
            
            //! @brief Gets the Object that sends messages.
            Object& getSenderObject() const;
            
            //! @brief Gets the Object that receives messages.
            Object& getReceiverObject() const;
            
            //! @brief Gets the index of the outlet of the Link.
            size_t getSenderIndex() const;
            
            //! @brief Gets the index of the inlet of the Link.
            size_t getReceiverIndex() const;
            
        private: // members
            
            model::Link const& m_model;
            
        private: // deleted methods
            
            Link(Link const&) = delete;
            Link(Link&&) = delete;
            Link& operator=(Link const&) = delete;
            Link& operator=(Link&&) = delete;
        };
    }
}

#endif // KIWI_ENGINE_LINK_HPP_INCLUDED
