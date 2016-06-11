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

#ifndef KIWI_JINSTANCE_HPP_INCLUDED
#define KIWI_JINSTANCE_HPP_INCLUDED

#include <string>

#include <KiwiEngine/KiwiInstance.hpp>
#include <KiwiCore/KiwiFile.hpp>

#include "flip/Document.h"

#include "jConsole.hpp"
#include "DocumentExplorer.hpp"
#include "jPatcherManager.hpp"

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
        engine::Instance& getEngineInstance();
        
        //! @brief Returns the engine::Instance
        engine::Instance const& getEngineInstance() const;
        
        //! @brief Open a File.
        void openFile(kiwi::FilePath const& file);
        
        //! @brief Open a patcher from file
        void openPatcher();
        
        //! @brief Attempt to close all document, after asking user to save them if needed.
        //! @return True if all document have been closed, false if the user cancel the action.
        bool closeAllWindows();
        
        //! @brief Create a new remote patcher
        void openRemotePatcher();
        
        //! @brief Opens the settings
        void openSettings();
        
        //! @brief Brings the Console to front.
        void showConsoleWindow();
        
        //! @brief Get Patcher clipboard data.
        std::vector<uint8_t>& getPatcherClipboardData();
        
    private: // methods
        
        //! @brief Returns the next untitled number based on current documents
        size_t getNextUntitledNumberAndIncrement();
        
        //! @brief Open a dialog box used to enter server infos
        void openRemoteDialogBox(std::string & host, uint16_t & port);
        
        //! @brief Sets the instance user id
        void setUserId(uint64_t user_id);
        
    private: // members
        
        using jPatcherManagers = std::vector<std::unique_ptr<jPatcherManager>>;
        
        uint64_t m_user_id;
        
        std::unique_ptr<engine::Instance>           m_instance;
        
        jPatcherManagers                            m_patcher_managers;
        
        std::unique_ptr<jConsoleWindow>             m_console_window;
        
        std::unique_ptr<DocumentExplorer>           m_document_explorer;
        std::unique_ptr<DocumentExplorerWindow>     m_document_explorer_window;
        
        std::vector<uint8_t>                        m_patcher_clipboard;
        
        static size_t                               m_untitled_patcher_index;
    };
}

#endif // KIWI_JINSTANCE_HPP_INCLUDED
