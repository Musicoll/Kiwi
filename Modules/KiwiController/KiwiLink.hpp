/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#ifndef KIWI_CONTROLLER_LINK_HPP_INCLUDED
#define KIWI_CONTROLLER_LINK_HPP_INCLUDED

#include "KiwiObject.hpp"

namespace kiwi
{
    namespace controller
    {
        // ================================================================================ //
        //                                      LINK                                        //
        // ================================================================================ //
        
        //! @brief The link holds a connection between two objects.
        class Link
        {
        public:
            
            //! @brief Constructor.
            Link(model::Link& model, Object* from, Object* to);
            
            //! @brief Destructor.
            ~Link();
            
            //! @brief Get the Patcher model
            inline model::Link& getModel() { return m_model; }
            
            //! @brief Get the Patcher model
            inline model::Link const& getModel() const { return m_model; }
            
            //! @brief Get the object that send messages.
            inline Object* getObjectFrom() const noexcept   { return m_object_from; }
            
            //! @brief Get the object that receive messages.
            inline Object* getObjectTo() const noexcept     { return m_object_to; }
            
            //! @brief Retrieve the index of the outlet of the link.
            inline uint32_t getOutletIndex() const noexcept { return m_model.getOutletIndex(); }
            
            //! @brief Retrieve the index of the inlet of the link.
            inline uint32_t getInletIndex() const noexcept  { return m_model.getInletIndex(); }
            
        private:
            model::Link&    m_model;
            Object*         m_object_from;
            Object*         m_object_to;
        };
    }
}

#endif // KIWI_CONTROLLER_LINK_HPP_INCLUDED
