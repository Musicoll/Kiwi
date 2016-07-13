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

#ifndef KIWI_JINSTANCE_HPP_INCLUDED
#define KIWI_JINSTANCE_HPP_INCLUDED

#include <string>

#include <KiwiEngine/KiwiEngineInstance.hpp>

#include "flip/Document.h"

#include "jConsole.hpp"
#include "DocumentExplorer.hpp"
#include "jPatcherManager.hpp"
#include "KiwiApp_StoredSettings.hpp"
#include "BeaconDispatcher.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                     jINSTANCE                                    //
    // ================================================================================ //

    //! @brief The jInstance
    class jInstance
    {
    public:
        
        //! @brief Constructor
        jInstance();
        
        //! @brief Destructor
        ~jInstance();
        
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
        
        //! @brief Attempt to close the given window asking user to save file if needed.
        bool closeWindow(jWindow& window);
        
        //! @brief Attempt to close all document, after asking user to save them if needed.
        //! @return True if all document have been closed, false if the user cancel the action.
        bool closeAllWindows();
        
        //! @brief Attempt to create a new patcher with given host and port parameters.
        void openRemotePatcher(std::string& host, uint16_t& port);
        
        //! @brief Opens the settings
        void openSettings();
        
        //! @brief Brings the Console to front.
        void showConsoleWindow();
        
        //! @brief Brings the DocumentExplorerWindow to front.
        void showDocumentExplorerWindow();
        
        //! @brief Brings the BeaconDispatcherWindow to front.
        void showBeaconDispatcherWindow();
        
        //! @brief Get Patcher clipboard data.
        std::vector<uint8_t>& getPatcherClipboardData();
        
    private: // methods
        
        using jPatcherManagers = std::vector<std::unique_ptr<jPatcherManager>>;
        
        //! @internal get the given patcher manager iterator.
        jPatcherManagers::const_iterator getPatcherManager(jPatcherManager const& manager) const;
        
        //! @brief Returns the next untitled number based on current documents
        size_t getNextUntitledNumberAndIncrement();
        
        //! @brief Sets the instance user id
        void setUserId(uint64_t user_id);
        
    private: // members
        
        uint64_t m_user_id;
        
        std::unique_ptr<engine::Instance>           m_instance;
        
        jPatcherManagers                            m_patcher_managers;
        
        sConsoleHistory                             m_console_history;
        std::unique_ptr<jConsoleWindow>             m_console_window;
        
        std::unique_ptr<DocumentExplorer>           m_document_explorer;
        std::unique_ptr<DocumentExplorerWindow>     m_document_explorer_window;
        
        std::unique_ptr<BeaconDispatcherWindow>     m_beacon_dispatcher_window;
        
        std::vector<uint8_t>                        m_patcher_clipboard;
        
        static size_t                               m_untitled_patcher_index;
        juce::File                                  m_last_opened_file;
    };
}

#endif // KIWI_JINSTANCE_HPP_INCLUDED
