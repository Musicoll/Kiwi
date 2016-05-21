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

#ifndef KIWI_JPATCHER_MANAGER_HPP_INCLUDED
#define KIWI_JPATCHER_MANAGER_HPP_INCLUDED

#include "flip/Document.h"

#include <KiwiModel/KiwiPatcher.hpp>
#include <KiwiEngine/KiwiInstance.hpp>

#include "jWindow.hpp"
#include "jPatcher.hpp"

namespace kiwi
{
    class jInstance;
    
    //! @brief The main DocumentObserver.
    //! @details The jInstance dispatch changes to all other DocumentObserver objects
    class jPatcherManager : public flip::DocumentObserver<model::Patcher>
    {
    public:
        
        //! @brief Constructor.
        jPatcherManager(jInstance& instance);
        
        //! @brief Construct and connect to remote server
        jPatcherManager(jInstance & instance, const std::string host, uint16_t port);
        
        //! @brief Destructor.
        ~jPatcherManager();
        
        //! @brief create a new patcher view window.
        void newView();
        
    private:
        
        //! @brief Returns the Patcher model
        model::Patcher& getPatcher();
        
        //! @brief Initializes a hardcoded patcher
        void populatePatcher();
        
        //! @internal flip::DocumentObserver<model::Patcher>::document_changed
        void document_changed(model::Patcher& patcher) override final;
        
        //! @internal Notify and create jPatchers.
        void notifyPatcherViews(model::Patcher& patcher);
        
        //! @internal React to the fact that a View has just been added to the document.
        //! @details create a jPatcherWindow.
        void createPatcherWindow(model::Patcher& patcher,
                                 model::Patcher::User& user,
                                 model::Patcher::View& view);
        
        //! @internal View is resident and internal value changed.
        void notifyPatcherView(model::Patcher& patcher,
                               model::Patcher::User& user,
                               model::Patcher::View& view);
        
        //! @internal Vser will be removed from the document.
        void removePatcherWindow(model::Patcher& patcher,
                                 model::Patcher::User& user,
                                 model::Patcher::View& view);

    private: // members
        
        jInstance&                          m_instance;
        flip::Document                      m_document;
    };
}

#endif // KIWI_JPATCHER_MANAGER_HPP_INCLUDED
