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

#include "flip/DocumentObserver.h"

#include "KiwiLink.hpp"

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
            void document_changed(model::Patcher& patcher) final override;
            
            //! @internal Object model has just been added to the document.
            void objectModelAdded(model::Object& object);
            
            //! @internal Object model has changed.
            void objectModelChanged(model::Object& object);
            
            //! @internal Object model will be removed from the document.
            void objectModelRemoved(model::Object& object);
            
            //! @internal Link model has just been added to the document.
            void linkModelAdded(model::Link& link);
            
            //! @internal Link model has changed.
            void linkModelChanged(model::Link& link);
            
            //! @internal Link model will be removed from the document.
            void linkModelRemoved(model::Link& link);
            
            // -----------------------------
            
            friend class Instance; // for document_changed access
            
            Instance&                       m_instance;
            model::Patcher*                 m_model;
        };
    }
}


#endif // KIWI_ENGINE_PATCHER_HPP_INCLUDED
