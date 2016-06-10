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
    
    // ================================================================================ //
    //                                  JPATCHER WINDOW                                 //
    // ================================================================================ //
    
    class jPatcherWindow : public jWindow
    {
    public:
        jPatcherWindow(jPatcherManager& manager, jPatcher& jpatcher);
        void closeButtonPressed() override;
        
    private:
        jPatcherManager& m_manager;
        jPatcher& m_jpatcher;
    };
    
    // ================================================================================ //
    //                                  JPATCHER MANAGER                                //
    // ================================================================================ //
    
    //! @brief The main DocumentObserver.
    //! @details The jInstance dispatch changes to all other DocumentObserver objects
    class jPatcherManager : public flip::DocumentObserver<model::Patcher>
    {
    public:
        
        //! @brief Constructor.
        jPatcherManager(jInstance& instance);
        
        //! @brief Constructs and load patcher from file
        jPatcherManager(jInstance& instance, kiwi::FilePath const& file);

        //! @brief Construct and connect to remote server
        jPatcherManager(jInstance & instance, const std::string host, uint16_t port);
        
        //! @brief Destructor.
        ~jPatcherManager();
        
        //! @brief Returns the Patcher model
        model::Patcher& getPatcher();
        
        //! @brief Returns the Patcher model
        model::Patcher const& getPatcher() const;
        
        //! @brief Initialize the patcher manager by creating a new document.
        //! @return The newly created the Patcher model.
        model::Patcher& init();
        
        //! @brief Returns the number of patcher views.
        size_t getNumberOfView();
        
        //! @brief create a new patcher view window.
        void newView();
        
        //! @brief Force all windows to close without asking user to save document.
        void forceCloseAllWindows();
        
        //! @brief Attempt to close all document windows, after asking user to save them if needed.
        //! @return True if all document have been closed, false if the user cancel the action.
        bool askAllWindowsToClose();
        
        //! @brief Close the window that contains a given jpatcher.
        //! @details if it's the last patcher view, it will ask the user the save the document before closing
        bool closePatcherViewWindow(jPatcher& jpatcher);
        
        //! @brief Save the document.
        bool saveDocument();
        
        //! @brief Returns true if the patcher needs to be saved.
        bool needsSaving() const;
        
    private:
        
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
        
        //! @internal Save document if needed and if user agrees.
        FileBasedDocument::SaveResult saveIfNeededAndUserAgrees();

    private: // members
        
        jInstance&      m_instance;
        flip::Document  m_document;
        bool            m_need_saving_flag;
    };
}

#endif // KIWI_JPATCHER_MANAGER_HPP_INCLUDED
