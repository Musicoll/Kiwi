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

#include <KiwiModel/KiwiModel_DocumentManager.h>
#include <KiwiModel/KiwiModel_PatcherUser.h>

namespace kiwi { namespace model {

    // ================================================================================ //
    //                                   DOCUMENT MANAGER                               //
    // ================================================================================ //
    
    DocumentManager::DocumentManager(flip::DocumentBase& document) :
    m_document(document),
    m_history(document),
    m_gesture_flag(false),
    m_gesture_cnt(0)
    {
        ;
    }
    
    DocumentManager::~DocumentManager()
    {
    }
    
    void DocumentManager::commit(flip::Type& type, std::string action)
    {
        auto& patcher = type.ancestor<model::Patcher>();
        patcher.entity().use<DocumentManager>().commit(action);
    }
    
    void DocumentManager::pull(flip::Type& type)
    {
        model::Patcher& patcher = type.ancestor<model::Patcher>();
        patcher.entity().use<DocumentManager>().pull();
    }
    
    void DocumentManager::startCommitGesture(flip::Type& type)
    {
        auto& patcher = type.ancestor<model::Patcher>();
        patcher.entity().use<DocumentManager>().startCommitGesture();
    }
    
    void DocumentManager::commitGesture(flip::Type& type, std::string action)
    {
        auto& patcher = type.ancestor<model::Patcher>();
        patcher.entity().use<DocumentManager>().commitGesture(action);
    }
    
    void DocumentManager::endCommitGesture(flip::Type & type)
    {
        auto& patcher = type.ancestor<model::Patcher>();
        patcher.entity().use<DocumentManager>().endCommitGesture();
    }
    
    bool DocumentManager::isInCommitGesture(flip::Type & type)
    {
        auto & patcher = type.ancestor<model::Patcher>();
        return patcher.entity().use<DocumentManager>().m_gesture_flag;
    }
    
    bool DocumentManager::canUndo()
    {
        return m_history.last_undo() != m_history.end();
    }
    
    std::string DocumentManager::getUndoLabel()
    {
        assert(canUndo());
        
        return m_history.last_undo()->label();
    }
    
    void DocumentManager::undo()
    {
        assert(canUndo());
        
        m_history.execute_undo();
    }
    
    bool DocumentManager::canRedo()
    {
        return m_history.first_redo() != m_history.end();
    }
    
    std::string DocumentManager::getRedoLabel()
    {
        assert(canRedo());
        
        return m_history.first_redo()->label();
    }
    
    void DocumentManager::redo()
    {
        assert(canRedo());
        
        m_history.execute_redo();
    }
    
    //=============================================================================
    
    void DocumentManager::commit(std::string action)
    {
        assert(!m_gesture_flag);
        
        if(!action.empty())
        {
            m_document.set_label(action);
        }
        
        auto tx = m_document.commit();
        push();
        
        if(!action.empty())
        {
            m_history.add_undo_step(tx);
        }
    }
    
    void DocumentManager::pull()
    {
        m_document.pull();
    }
    
    void DocumentManager::push()
    {
        m_document.push();
    }
    
    
    void DocumentManager::startCommitGesture()
    {
        assert(!m_gesture_flag);
        m_gesture_flag = true;
        
        m_gesture_cnt = 0;
    }
    
    void DocumentManager::commitGesture(std::string action)
    {
        assert(m_gesture_flag);
        assert(!action.empty());
        
        m_document.set_label(action);
        m_document.commit();
        
        m_document.set_label(action);
        auto tx = m_document.squash();
        
        if(!tx.empty())
        {
            if(m_gesture_cnt == 0)
            {
                m_history.add_undo_step(tx);
            }
            else
            {
                *m_history.last_undo() = tx;
            }
            
            ++m_gesture_cnt;
        }
    }
    
    void DocumentManager::endCommitGesture()
    {
        assert(m_gesture_flag);
        m_gesture_flag = false;
        
        if(m_gesture_cnt > 0)
        {
            m_document.push();
        }
    }
}}
