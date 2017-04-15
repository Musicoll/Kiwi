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

#ifndef KIWI_APP_INSTANCE_HPP_INCLUDED
#define KIWI_APP_INSTANCE_HPP_INCLUDED

#include <KiwiEngine/KiwiEngine_Instance.hpp>

#include "flip/Document.h"

#include "KiwiApp_Console.hpp"
#include "KiwiApp_Components/KiwiApp_SettingsPanel.hpp"
#include "KiwiApp_DocumentBrowserView.hpp"
#include "KiwiApp_PatcherManager.hpp"
#include "KiwiApp_StoredSettings.hpp"
#include "KiwiApp_BeaconDispatcher.hpp"
#include "KiwiApp_DspDeviceManager.hpp"

namespace kiwi
{
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
        
        //! @brief Get the user ID of the Instance.
        uint64_t getUserId() const noexcept;
        
        //! @brief create a new patcher window.
        void newPatcher();
        
        //! @brief Returns the engine::Instance
        engine::Instance& useEngineInstance();
        
        //! @brief Returns the engine::Instance
        engine::Instance const& getEngineInstance() const;
        
        //! @brief Open a File.
        bool openFile(juce::File const& file);
        
        //! @brief Open a patcher from file
        void askUserToOpenPatcherDocument();

        //! @brief Removes the view of a certain patcher.
        void removePatcherWindow(PatcherViewWindow & patcher_window);
        
        //! @brief Attempt to close the given window asking user to save file if needed.
        void closeWindow(Window& window);
        
        //! @brief Attempt to close all document, after asking user to save them if needed.
        //! @return True if all document have been closed, false if the user cancel the action.
        bool closeAllPatcherWindows();
        
        //! @brief Attempt to create a new patcher with document Session informations.
        PatcherManager* openRemotePatcher(DocumentBrowser::Drive::DocumentSession& session);
        
        //! @brief Brings the Application settings window to front.
        void showAppSettingsWindow();
        
        //! @brief Removes a patcher from cach.
        void removePatcher(PatcherManager const& patcher_manager);
        
        //! @brief Opens a juce native audio setting pannel.
        void showAudioSettingsWindow();
        
        //! @brief Brings the Console to front.
        void showConsoleWindow();
        
        //! @brief Brings the "About Kiwi" window to front.
        void showAboutKiwiWindow();
        
        //! @brief Brings the DocumentBrowserWindow to front.
        void showDocumentBrowserWindow();
        
        //! @brief Brings the BeaconDispatcherWindow to front.
        void showBeaconDispatcherWindow();
        
        //! @brief Get Patcher clipboard data.
        std::vector<uint8_t>& getPatcherClipboardData();
        
    private: // methods
        
        using PatcherManagers = std::vector<std::unique_ptr<PatcherManager>>;
        
        //! @internal get the given patcher manager iterator.
        PatcherManagers::iterator getPatcherManager(PatcherManager const& manager);
        
        //! @internal get the given patcher manager iterator.
        PatcherManagers::iterator getPatcherManagerForSession(DocumentBrowser::Drive::DocumentSession& session);
        
        //! @internal Returns the next untitled number based on current documents
        size_t getNextUntitledNumberAndIncrement();
        
        //! @internal Creates the window if needed then the brings it to front.
        //! @param create_fn The window factory function.
        void showWindowWithId(WindowId id, std::function<std::unique_ptr<Window>()> create_fn);
        
    private: // variables
        
        engine::Instance                            m_instance;
        
        uint64_t                                    m_user_id;
        
        DocumentBrowser                             m_browser;
        
        PatcherManagers                             m_patcher_managers;
        
        sConsoleHistory                             m_console_history;

        std::vector<std::unique_ptr<Window>>        m_windows;
    
        std::vector<uint8_t>                        m_patcher_clipboard;
        
        static size_t                               m_untitled_patcher_index;
        juce::File                                  m_last_opened_file;
    };
}

#endif // KIWI_APP_INSTANCE_HPP_INCLUDED
