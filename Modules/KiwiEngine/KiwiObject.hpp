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

#include "flip/SignalConnection.h"

#include <KiwiModel/KiwiConsole.hpp>
#include <KiwiModel/KiwiObject.hpp>

namespace kiwi
{
    namespace engine
    {
        class Patcher;
        class Instance;
        class Link;
        
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        //! @brief The object reacts and interacts with other objects by sending and receiving messages via its inlets and outlets.
        class Object
        {
        public: // methods
            
            //! @brief Constructor.
            Object() noexcept;
            
            //! @brief Destructor.
            virtual ~Object() noexcept;

            //! @brief Returns the name of the Object.
            std::string getName() const noexcept;
            
            //! @brief Returns the number of inlets of the Object.
            size_t getNumberOfInlets() const noexcept;
            
            //! @brief Returns the number of inlets of the Object.
            size_t getNumberOfOutlets() const noexcept;
            
            //! @brief Receives a set of arguments via an inlet.
            //! @details This method must be overriden by object's subclasses.
            //! @todo see if the method must be noexcept.
            virtual void receive(size_t index, std::vector<Atom> const& args) = 0;
            
        protected: // methods
            
            //! @brief Sends a vector of Atom via an outlet.
            //! @todo Improve the stack overflow system.
            //! @todo see if the method must be noexcept.
            void send(const size_t index, std::vector<Atom> const& args);
            
            //! @brief Called when the signalTrigger method is fired.
            virtual void signalTriggerCalled() {};
        
        private: // methods
            
            //! @internal Append a new link to an outlet.
            void addOutputLink(Link* link);
            
            //! @internal Remove a link from an outlet.
            void removeOutputLink(Link* link);
            
            //! @internal Model change notification.
            void objectChanged(model::Object& object_m);
            
            //! @internal signalTriggerCallback.
            void internal_signalTriggerCalled();
            
            friend class engine::Patcher;
            
        private: // members
            
            typedef std::set<Link*> Outlet;

            model::Object*          m_model;
            std::vector<Outlet>     m_outlets;
            size_t                  m_stack_count = 0ul;
            
            flip::SignalConnection  m_signal_cnx;
            
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
