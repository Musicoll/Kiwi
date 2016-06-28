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

#ifndef KIWI_ENGINE_OBJECT_HPP_INCLUDED
#define KIWI_ENGINE_OBJECT_HPP_INCLUDED

#include "KiwiEngineDef.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        //! @brief The Object reacts and interacts with other ones by sending and receiving messages via its inlets and outlets.
        class Object
        {
        public: // methods
            
            //! @brief Constructor.
            Object(model::Object const& model) noexcept;
            
            //! @brief Destructor.
            virtual ~Object() noexcept;

            //! @brief Gets the name of the Object.
            std::string getName() const noexcept;
            
            //! @brief Gets the number of inlets of the Object.
            size_t getNumberOfInlets() const noexcept;
            
            //! @brief Gets the number of inlets of the Object.
            size_t getNumberOfOutlets() const noexcept;
            
            //! @brief Receives a set of arguments via an inlet.
            //! @details This method must be overriden by object's subclasses.
            //! @todo see if the method must be noexcept.
            virtual void receive(size_t index, std::vector<Atom> const& args) = 0;
            
            //! @internal Appends a new Link to an outlet.
            void addOutputLink(Link const* link);
            
            //! @internal Removes a Link from an outlet.
            void removeOutputLink(Link const* link);
            
        protected: // methods
            
            //! @brief Sends a vector of Atom via an outlet.
            //! @todo Improve the stack overflow system.
            //! @todo See if the method must be noexcept.
            void send(const size_t index, std::vector<Atom> const& args);
            
        private: // members
            
            typedef std::set<Link const*> Outlet;

            model::Object const&    m_model;
            std::vector<Outlet>     m_outlets;
            size_t                  m_stack_count = 0ul;
            
        private: // deleted methods
            
            Object(Object const&) = delete;
            Object(Object&&) = delete;
            Object& operator=(Object const&) = delete;
            Object& operator=(Object&&) = delete;
        };
        
        typedef std::shared_ptr<Object> sObject;
    }
}

#endif // KIWI_ENGINE_OBJECT_HPP_INCLUDED
