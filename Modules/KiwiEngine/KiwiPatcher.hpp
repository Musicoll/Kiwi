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

#include <KiwiModel/KiwiPatcher.hpp>

#include "flip/DocumentObserver.h"

#include "KiwiLink.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      PATCHER                                     //
        // ================================================================================ //
        
        //! @brief The Patcher manages a set of objects and links.
        //! @details The patcher engine observes the Patcher model for changes.
        class Patcher : public flip::DocumentObserver<model::Patcher>
        {
        public:
            
            //! @brief Constructor.
            Patcher() noexcept;
            
            //! @brief Destructor.
            ~Patcher();
            
            //! @brief Returns the objects.
            std::vector<engine::Object const*> getObjects() const;
            
            //! @brief Returns the objects.
            std::vector<engine::Object*> getObjects();
            
            //! @brief Returns the links.
            std::vector<engine::Link const*> getLinks() const;
            
            //! @internal flip::DocumentObserver<model::Patcher>::document_changed
            void document_changed(model::Patcher& patcher) final override;
            
        private: // methods
            
            //! @internal Object model has just been added to the document.
            void objectAdded(model::Object& object);
            
            //! @internal Object model has changed.
            void objectChanged(model::Object& object);
            
            //! @internal Object model will be removed from the document.
            void objectRemoved(model::Object& object);
            
            //! @internal Link model has just been added to the document.
            void linkAdded(model::Link& link);
            
            //! @internal Link model has changed.
            void linkChanged(model::Link& link);
            
            //! @internal Link model will be removed from the document.
            void linkRemoved(model::Link& link);
        
        private: // members
            
            model::Patcher* m_model;
            
        private: // deleted methods
            
            Patcher(Patcher const&) = delete;
            Patcher(Patcher&&) = delete;
            Patcher& operator=(Patcher const&) = delete;
            Patcher& operator=(Patcher&&) = delete;
        };
    }
}


#endif // KIWI_ENGINE_PATCHER_HPP_INCLUDED
