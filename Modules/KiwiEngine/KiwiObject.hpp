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

//#include "flip/Signal.h"
#include "flip/SignalConnection.h"

#include "../KiwiModel/KiwiModel.hpp"

#include <utility>

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
        
        //! @brief The model::Object engine.
        class Object
        {
        public:
            
            //! @brief Constructor.
            Object(model::Object& model) noexcept;
            
            //! @brief Destructor.
            virtual ~Object() noexcept;

            //! @brief Returns the name of the Object.
            inline std::string getName() const noexcept         { return m_model.getName(); }
            
            //! @brief Get the number of inlets of the Object.
            inline uint32_t getNumberOfInlets() const noexcept  { return m_model.getNumberOfInlets(); }
            
            //! @brief Get the number of inlets of the object.
            inline uint32_t getNumberOfOutlets() const noexcept { return m_model.getNumberOfOutlets(); }
            
            //! @brief The receive method.
            //! @details This method must be overriden by object's subclasses.
            virtual void receive(uint32_t index, std::vector<Atom> args) = 0;
            
        protected:
            
            //! @brief Send a message through a given outlet index.
            void send(const uint32_t index, std::vector<Atom> args);
            
            //! @brief Called when the signalTrigger method is fired.
            virtual void signalTriggerCalled() {};
        
        private:
            
            //! @internal signalTriggerCallback.
            void internal_signalTriggerCalled();
            
            //! @internal Append a new link to an outlet.
            void addOutputLink(Link* link);
            
            //! @internal Remove a link from an outlet.
            void removeOutputLink(Link* link);
            
            friend class engine::Patcher;
            
        private:
            
            typedef std::set<Link*> Outlet;

            model::Object const&    m_model;
            std::vector<Outlet>     m_outlets;
            uint32_t                m_stack_count = 0ul;
            
            flip::SignalConnection  m_signal_cnx;
        };
    }
}

#endif // KIWI_ENGINE_OBJECT_HPP_INCLUDED
