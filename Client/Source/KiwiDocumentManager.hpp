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

#ifndef KIWI_DOCUMENT_MANAGER_HPP_INCLUDED
#define KIWI_DOCUMENT_MANAGER_HPP_INCLUDED

#include "flip/History.h"
#include "flip/HistoryStoreMemory.h"

#include "../JuceLibraryCode/JuceHeader.h"

#include "KiwiFilePath.hpp"
#include "KiwiCarrierSocket.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                    FILE HANDLER                                  //
    // ================================================================================ //
 
    //! @brief Class that enable saving and loading the document from a kiwi file
    class FileHandler final
    {
    public:
        
        //! @brief Constructs the FileHandler referencing document and pointing to a non-existing file.
        FileHandler(flip::DocumentBase & document);
        
        //! @brief Destructor.
        ~FileHandler() = default;
        
        //! @biref Loads the document from file and sets the pointed file.
        void load(FilePath const& file);
        
        //! @brief Saves the document from file and sets the pointed file.
        void save(FilePath const& file);
        
        //! @brief Get the pointed file.
        FilePath const& getFile() const;
        
    private:
        void setFile(FilePath const& file);
        void load();
        void save();
        
    private:
        flip::DocumentBase& m_document;
        FilePath m_file;
        
    private:
        FileHandler(FilePath const& other) = delete;
        FileHandler(FilePath && other) = delete;
        FileHandler& operator=(FilePath const& other) = delete;
        FileHandler& operator=(FilePath && other) = delete;
    };
    
    // ================================================================================ //
    //                                   DOCUMENT MANAGER                               //
    // ================================================================================ //
    
    class DocumentManager : public juce::Timer
    {
    public:
        
        DocumentManager(flip::DocumentBase & document);
        
        //! @brief Destructor.
        ~DocumentManager();
        
        //! @brief Commit and push.
        //! @see startCommitGesture, endCommitGesture.
        static void commit(flip::Type& type, std::string action = std::string());
        
        //! @brief  Connect the DocumentManager to a remote server
        static void connect(flip::Type& type, const std::string host, uint16_t port);
        
        //! @brief Pull changes from remote server
        static void pull(flip::Type& type);
        
        //! @brief Starts a commit gesture.
        //! @details Each call to this function must be followed by a call to endCommitGesture.
        //! @see endCommitGesture, commitGesture
        static void startCommitGesture(flip::Type& type);
        
        //! @brief Commit a gesture.
        //! @param label The label of the current gesture.
        //! @see startCommitGesture, endCommitGesture.
        static void commitGesture(flip::Type& type, std::string label);
        
        //! @brief Ends a commit gesture.
        //! @details Each call to this function must be preceded by a call to startCommitGesture.
        //! @see startCommitGesture.
        static void endCommitGesture(flip::Type& type);

        //! @brief Returns true if the document is currently commiting a gesture.
        static bool isInCommitGesture(flip::Type& type);
        
        //! @brief Saves the patch into the designated file
        //! @details Doesn't save if not kiwi file. Sets the currently pointed file.
        static void save(flip::Type& type, FilePath const& file);
        
        //! @brief Loads the patch from the designated file
        //! @details Doesn't load if not kiwi file. Sets the currently pointed file.
        static void load(flip::Type& type, FilePath const& file);
        
        //! @brief Returns the file that is currently pointed to by the DocumentManager.
        //! @details If neither save or load was called return a non existing file (file.exist() == false).
        static FilePath const& getSelectedFile(flip::Type& type);
        
        //! @brief Returns true if there is an action to undo.
        bool canUndo();
        
        //! @brief Returns the label of the last undo action.
        std::string getUndoLabel();
        
        //! @brief Undo the last action.
        void undo();
        
        //! @brief Returns true if there is an action to redo.
        bool canRedo();
        
        //! @brief Returns the label of the next redo action.
        std::string getRedoLabel();
        
        //! @brief Redo the next action.
        void redo();
        
        //! @brief Returns the object's pointer or nullptr if not found in document.
        template<class T> T* get(flip::Ref const& ref)
        {
            return m_document.object_ptr<T>(ref);
        }
        
    private:
        
        //! @brief Commmits and pushes a transaction
        void commit(std::string action);
        
        //! @brief Pulls transactions stacked by a socket's process
        void pull();
        
        //! @brief Pushes a trasactions stacked by a socket's process
        void push();
        
        //! @brief Connects the document manager and download the patcher's initial state
        void connect(std::string const host, uint16_t port);
        
        //! @brief Returns true if the document manager is connected false otherwise
        bool isConnected();
        
        //! @brief Disconnects the document manager from the server
        void disconnect();
        
        //! @brief Starts a timer callback on the message thread that that pulls
        void startPulling();
        
        //! @brief Stops the pulling callback
        void stopPulling();
        
        //! @brief Called once document manager is connected and starts pulling
        void onConnected();
        
        //! @brief Called once document manager is disconnected and stops pulling
        void onDisconnected();
        
        //! @brief Called once the initial load happened
        void onLoaded();
        
        //! @brief Called at a regular frequency to pull document
        void timerCallback() override;
        
        void startCommitGesture();
        void commitGesture(std::string action);
        void endCommitGesture();
        void save(FilePath const& file);
        void load(FilePath const& file);
        FilePath const& getSelectedFile() const;
        
    private:
        
        flip::DocumentBase&                     m_document;
        flip::History<flip::HistoryStoreMemory> m_history;
        CarrierSocket                           m_socket;
        FileHandler                             m_file_handler;
        
        bool                                    m_gesture_flag = false;
        size_t                                  m_gesture_cnt = 0;
        std::atomic_bool                        m_loaded;
        
    private:
        
        DocumentManager() = delete;
        DocumentManager(const DocumentManager& rhs) = delete;
        DocumentManager(DocumentManager&& rhs) = delete;
        DocumentManager& operator =(const DocumentManager& rhs) = delete;
        DocumentManager& operator =(DocumentManager&& rhs) = delete;
        bool operator ==(DocumentManager const& rhs) const = delete;
        bool operator !=(DocumentManager const& rhs) const = delete;
    };
}

#endif // KIWI_DOCUMENT_MANAGER_HPP_INCLUDED
