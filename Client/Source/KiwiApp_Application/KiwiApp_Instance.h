/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#pragma once

#include <KiwiEngine/KiwiEngine_Instance.h>
#include <KiwiTool/KiwiTool_Scheduler.h>

#include <juce_events/juce_events.h>

#include "flip/Document.h"

#include "KiwiApp_Console.h"
#include "KiwiApp_SettingsPanel.h"
#include "KiwiApp_DocumentBrowserView.h"
#include "KiwiApp_BeaconDispatcher.h"

#include "../KiwiApp_Patcher/KiwiApp_PatcherManager.h"
#include "../KiwiApp_Audio/KiwiApp_DspDeviceManager.h"

#include "../KiwiApp_Auth/KiwiApp_AuthPanel.h"

namespace kiwi
{
    class PatcherViewWindow;
    
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //

    //! @brief The Application Instance
    class Instance
    {
    public:
        
        //! @brief Constructor
        Instance();
        
        //! @brief Destructor
        ~Instance();
        
        //! @brief Tick the instance regularly to pull remote document data.
        void tick();
        
        //! @brief Get the user ID of the Instance.
        uint64_t getUserId() const noexcept;
        
        //! @brief Enables the document browser view.
        void login();
    
        //! @brief create a new patcher window.
        void newPatcher();
        
        //! @brief Returns the engine::Instance
        engine::Instance& useEngineInstance();
        
        //! @brief Returns the engine::Instance
        engine::Instance const& useEngineInstance() const;
        
        //! @brief Open a File.
        bool openFile(juce::File const& file);
        
        //! @brief Open a patcher from file
        void askUserToOpenPatcherDocument();

        //! @brief Removes the view of a certain patcher.
        void removePatcherWindow(PatcherViewWindow& patcher_window);
        
        //! @brief Attempt to close the given window asking user to save file if needed.
        void closeWindow(Window& window);
        
        //! @brief Attempt to close the window with the given id, asking user to save file if needed.
        void closeWindowWithId(WindowId window_id);
        
        //! @brief Attempt to close all document, after asking user to save them if needed.
        //! @return True if all document have been closed, false if the user cancel the action.
        bool closeAllPatcherWindows();
        
        //! @brief Force close all patcher windows.
        void forceCloseAllPatcherWindows();
        
        //! @brief Attempt to create a new patcher with document Session informations.
        void openRemotePatcher(DocumentBrowser::Drive::DocumentSession& session);
        
        //! @brief Brings the Application settings window to front.
        void showAppSettingsWindow();
        
        //! @brief Opens a juce native audio setting pannel.
        void showAudioSettingsWindow();
        
        //! @brief Brings the Console to front.
        void showConsoleWindow();
        
        //! @brief Brings the Auth form window to front.
        void showAuthWindow(AuthPanel::FormType type);
        
        //! @brief Brings the "About Kiwi" window to front.
        void showAboutKiwiWindow();
        
        //! @brief Brings the DocumentBrowserWindow to front.
        void showDocumentBrowserWindow();
        
        //! @brief Brings the BeaconDispatcherWindow to front.
        void showBeaconDispatcherWindow();
        
        //! @brief Get Patcher clipboard data.
        std::vector<uint8_t>& getPatcherClipboardData();
        
        //! @internal Handle connection lost.
        //! @todo refactor this to handle this event by a callback instead
        void handleConnectionLost();
        
    private: // methods
        
        //! @internal pull all documents
        //! @brief currently used by the Instance::tick method
        void pullRemoteDocuments();
        
        //! @brief Ask the user if he wants to continue to edit document offline
        //! @details This could happen if the user logged out or if the connection was lost.
        bool askUserToContinueEditingDocumentOffline(PatcherManager& manager,
                                                     std::string const& reason) const;
        
        using PatcherManagers = std::vector<std::unique_ptr<PatcherManager>>;
        
        //! @internal get the given patcher manager iterator.
        PatcherManagers::iterator getPatcherManager(PatcherManager const& manager);
        
        //! @internal get the given patcher manager iterator.
        PatcherManagers::iterator getPatcherManagerForSession(DocumentBrowser::Drive::DocumentSession& session);
        
        //! @internal gets patcher manager currently associated to file.
        Instance::PatcherManagers::iterator getPatcherManagerForFile(juce::File const& file);
        
        //! @internal Returns the next untitled number based on current documents
        size_t getNextUntitledNumberAndIncrement();
        
        //! @internal Creates the window if needed then the brings it to front.
        //! @param create_fn The window factory function.
        void showWindowWithId(WindowId id, std::function<std::unique_ptr<Window>()> create_fn);
        
    private: // variables
        
        engine::Instance                            m_instance;
        
        DocumentBrowser                             m_browser;
        
        PatcherManagers                             m_patcher_managers;
        
        sConsoleHistory                             m_console_history;

        std::vector<std::unique_ptr<Window>>        m_windows;
    
        std::vector<uint8_t>                        m_patcher_clipboard;
        
        static size_t                               m_untitled_patcher_index;
        juce::File                                  m_last_opened_file;
    };
}
