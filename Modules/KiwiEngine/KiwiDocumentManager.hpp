/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright(c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2(or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_ENGINE_DOCUMENT_MANAGER_HPP_INCLUDED
#define KIWI_ENGINE_DOCUMENT_MANAGER_HPP_INCLUDED

#include "flip/History.h"
#include "flip/HistoryStoreMemory.h"

namespace flip
{
    class DocumentBase;
    class Object;
}

namespace kiwi
{
    class DocumentManager
    {
    public:
        
        DocumentManager(flip::DocumentBase & document);
        virtual ~DocumentManager() = default;
        
        //! @brief Commit and push.
        //! @see startCommitGesture, endCommitGesture.
        static void commit(flip::Type& type, std::string action = std::string());
        
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
        
        void commit(std::string action);
        void startCommitGesture();
        void commitGesture(std::string action);
        void endCommitGesture();
        
    private:
        
        flip::DocumentBase&                     m_document;
        flip::History<flip::HistoryStoreMemory> m_history;
        
        bool                                    m_gesture_flag = false;
        size_t                                  m_gesture_cnt = 0;
        
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

#endif // KIWI_ENGINE_DOCUMENT_MANAGER_HPP_INCLUDED
