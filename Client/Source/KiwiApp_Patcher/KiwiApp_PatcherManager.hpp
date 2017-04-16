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

#ifndef KIWI_APP_PATCHER_MANAGER_HPP_INCLUDED
#define KIWI_APP_PATCHER_MANAGER_HPP_INCLUDED

#include <juce_gui_extra/juce_gui_extra.h>

#include "flip/Document.h"

#include <KiwiModel/KiwiModel_PatcherUser.hpp>
#include <KiwiModel/KiwiModel_PatcherValidator.hpp>
#include <KiwiEngine/KiwiEngine_Instance.hpp>

#include "../KiwiApp_Window.hpp"
#include "KiwiApp_PatcherView.hpp"

#include "../KiwiApp_Network/KiwiApp_DocumentBrowser.hpp"

#include "KiwiApp_PatcherComponent.hpp"

namespace kiwi
{
    class Instance;
    
    // ================================================================================ //
    //                                  PATCHER MANAGER                                 //
    // ================================================================================ //
    
    //! @brief The main DocumentObserver.
    //! @details The Instance dispatch changes to all other DocumentObserver objects
    class PatcherManager :  public flip::DocumentObserver<model::Patcher>,
                            public DocumentBrowser::Drive::Listener
    {
    public: // nested classes
        
        struct Listener;
        
    public: // methods
        
        //! @brief Constructor.
        PatcherManager(Instance& instance);
        
        //! @brief Destructor.
        ~PatcherManager();
        
        //! @brief Try to connect this patcher to a remote server.
        void connect(DocumentBrowser::Drive::DocumentSession& session);
        
        //! @brief Load patcher datas from file.
        void loadFromFile(juce::File const& file);
        
        //! @brief Returns the Patcher model
        model::Patcher& getPatcher();
        
        //! @brief Returns the Patcher model
        model::Patcher const& getPatcher() const;
        
        //! @brief Returns true if the this is a remotely connected document.
        bool isRemote() const noexcept;
        
        //! @brief Returns the session ID of the document.
        //! @details This function returns 0 if the document is loaded from disk or memory.
        //! @see isRemote
        uint64_t getSessionId() const noexcept;
        
        //! @brief Returns the name of the document.
        //! @details This function returns 0 if the document is loaded from disk or memory.
        //! @see isRemote
        std::string getDocumentName() const;
        
        //! @brief Returns the number of patcher views.
        size_t getNumberOfView();
        
        //! @brief create a new patcher view window.
        void newView();
        
        //! @brief Brings the first patcher view to front.
        void bringsFirstViewToFront();
        
        //! @brief Force all windows to close without asking user to save document.
        void forceCloseAllWindows();
        
        //! @brief Attempt to close all document windows, after asking user to save them if needed.
        //! @return True if all document have been closed, false if the user cancel the action.
        bool askAllWindowsToClose();
        
        //! @brief Close the window that contains a given patcherview.
        //! @details if it's the last patcher view, it will ask the user the save the document before closing if needed.
        bool closePatcherViewWindow(PatcherView& patcherview);
        
        //! @brief Save the document.
        bool saveDocument();
        
        //! @brief Returns true if the patcher needs to be saved.
        bool needsSaving() const noexcept;
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief remove a listener.
        void removeListener(Listener& listener);
        
        //! @brief Called when a document session has been added.
        void documentAdded(DocumentBrowser::Drive::DocumentSession& doc) override;
        
        //! @brief Called when a document session changed.
        void documentChanged(DocumentBrowser::Drive::DocumentSession& doc) override;
        
        //! @brief Called when a document session has been removed.
        void documentRemoved(DocumentBrowser::Drive::DocumentSession& doc) override;
        
    private:
        
        //! @internal flip::DocumentObserver<model::Patcher>::document_changed
        void document_changed(model::Patcher& patcher) override final;
        
        //! @internal Notify and create PatcherViews.
        void notifyPatcherViews(model::Patcher& patcher);
        
        //! @internal React to the fact that a View has just been added to the document.
        //! @details create a PatcherViewWindow.
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
        juce::FileBasedDocument::SaveResult saveIfNeededAndUserAgrees();

    private: // members
        
        Instance&                                   m_instance;
        model::PatcherValidator                     m_validator;
        flip::Document                              m_document;
        bool                                        m_need_saving_flag;
        bool                                        m_is_remote;
        DocumentBrowser::Drive::DocumentSession*    m_session {nullptr};
    };
    
    // ================================================================================ //
    //                                PATCHER VIEW WINDOW                               //
    // ================================================================================ //
    
    class PatcherViewWindow : public Window
    {
    public:
        
        PatcherViewWindow(PatcherManager& manager, PatcherView& patcherview);
        
        void closeButtonPressed() override;
        
        //! @brief returns the patcher manager.
        PatcherManager& getPatcherManager() const;
        
        //! @brief returns the PatcherView.
        PatcherView& getPatcherView();
        
    private: // variables
        
        PatcherManager&     m_patcher_manager;
        PatcherComponent    m_patcher_component;
    };
}

#endif // KIWI_APP_PATCHER_MANAGER_HPP_INCLUDED
