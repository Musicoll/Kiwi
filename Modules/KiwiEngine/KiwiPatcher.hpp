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

#ifndef KIWI_ENGINE_PATCHER_HPP_INCLUDED
#define KIWI_ENGINE_PATCHER_HPP_INCLUDED

#include "KiwiLink.hpp"

#include "flip/DocumentObserver.h"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      PATCHER                                     //
        // ================================================================================ //
        
        //! @brief The Patcher manages object and link controllers.
        //! @details The patcher engine observes the Patcher mode for changes.
        class Patcher : public flip::DocumentObserver<model::Patcher>
        {
        public:
            
            //! @brief Constructor.
            //! @details use the create method instead.
            Patcher(Instance& instance) noexcept;
            
            //! @brief Destructor.
            ~Patcher();
            
            //! @brief Creates and adds a "plus" object to the Patcher.
            void addPlus();
            
            //! @brief Creates and adds a "print" object to the Patcher.
            void addPrint();
            
            //! @brief Constructs and add a Link to the Patcher.
            //! @details Constructs a Link with given origin and destination Object pointers
            //! and IO indexes then adds it in the Patcher.
            //! @param from     The origin Object pointer.
            //! @param outlet   The origin outlet index.
            //! @param to       The destination Object pointer.
            //! @param inlet    The destination inlet index.
            void addLink(Object const& from, const uint32_t outlet, Object const& to, const uint32_t inlet);
            
            //! @brief Removes an Object from the Patcher.
            //! @param id The ID of the object to be removed.
            void removeObject(Object const& object);
            
            //! @brief Removes an Object from the Patcher.
            //! @param object The pointer to the object to be removed.
            void removeLink(Link const& link);
            
            //! @brief Returns the objects.
            std::vector<engine::Object const*> getObjects() const;
            
            //! @brief Returns the objects.
            std::vector<engine::Object*> getObjects();
            
            //! @brief Returns the objects.
            std::vector<engine::Link const*> getLinks() const;
            
            // Rebouger et refaire
            //! @brief Send a message to an object
            void sendToObject(Object& object, uint32_t inlet, std::vector<Atom> args);
            
        private:
            
            //! @internal flip::DocumentObserver<model::Patcher>::document_changed
            void document_changed(model::Patcher& patcher) final;
            
            //! @internal Object has just been added to the document.
            void objectHasBeenAdded(model::Object& object);
            
            //! @internal Object is resident and internal value changed.
            void objectChanged(model::Object& object);
            
            //! @internal Object will be removed from the document.
            void objectWillBeRemoved(model::Object& object);
            
            //! @internal Link has just been added to the document.
            void linkHasBeenAdded(model::Link& link);
            
            //! @internal Link is resident and internal value changed.
            void linkChanged(model::Link& link);
            
            //! @internal Link will be removed from the document.
            void linkWillBeRemoved(model::Link& link);
            
            // -----------------------------
            
            friend class Instance; // for document_changed access
            
            Instance&                       m_instance;
            model::Patcher*                 m_model;
        };
    }
}


#endif // KIWI_ENGINE_PATCHER_HPP_INCLUDED
