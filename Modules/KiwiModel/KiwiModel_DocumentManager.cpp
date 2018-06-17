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
    
    DocumentManager::DocumentManager(flip::DocumentBase& document)
    : m_document(document)
    , m_history(m_document)
    {}
    
    DocumentManager::~DocumentManager()
    {}
    
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
    
    void DocumentManager::endCommitGesture(flip::Type& type)
    {
        auto& patcher = type.ancestor<model::Patcher>();
        patcher.entity().use<DocumentManager>().endCommitGesture();
    }
    
    bool DocumentManager::isInCommitGesture(flip::Type& type)
    {
        auto & patcher = type.ancestor<model::Patcher>();
        return(patcher.entity().use<DocumentManager>().m_session != nullptr);
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
        if(m_session)
        {
            m_session->revert();
        }
        else
        {
            m_session.reset(new Session(m_document));
        }
        
        m_session->start();
    }
    
    void DocumentManager::commitGesture(std::string action)
    {
        assert(m_session && "call startCommitGesture before");
        
        m_session->commit(action);
    }
    
    void DocumentManager::endCommitGesture()
    {
        assert(m_session && "call startCommitGesture before");
        
        m_session->end(&m_history);
        m_document.push();
        m_session.reset();
    }
    
    // ================================================================================ //
    //                                      SESSION                                     //
    // ================================================================================ //
    
    DocumentManager::Session::Session(flip::DocumentBase& document)
    : m_document(document)
    , m_history(document)
    {}
    
    DocumentManager::Session::~Session()
    {}
    
    void DocumentManager::Session::start()
    {
        if(m_tx.empty())
            return;
        
        m_document.execute_backward(m_tx);
    }
    
    void DocumentManager::Session::commit()
    {
        bool first_flag = m_tx.empty();
        auto tx = m_document.commit();
        
        m_document.execute_backward(tx);
        m_document.execute_backward(m_tx);
        
        flip::Transaction tx_abs;
        m_document.root<model::Patcher>().make(tx_abs);
        tx_abs.invert_direction();
        
        m_tx = tx_abs;
        
        if(tx.has_metadata(flip::Transaction::key_label))
        {
            m_tx.impl_use_metadata_map()[flip::Transaction::key_label] = tx.label();
        }
        
        if(first_flag)
        {
            m_history.add_undo_step(m_tx);
        }
        else if(m_tx.empty())
        {
            m_history.erase(m_history.last_undo());
        }
        else
        {
            *m_history.last_undo() = m_tx;
        }
        
        m_document.revert();
    }
    
    void DocumentManager::Session::commit(std::string label)
    {
        if(!label.empty())
        {
            m_document.set_label(label);
        }
        
        commit();
    }
    
    void DocumentManager::Session::end(flip::History<flip::HistoryStoreMemory>* master_history)
    {
        if(m_tx.empty()) return;
        
        if(master_history != nullptr)
        {
            // copy session transaction into the master history.
            master_history->add_undo_step(m_tx);
        }
        
        m_tx.clear();
        m_history.clear();
    }
    
    void DocumentManager::Session::revert()
    {
        if(m_tx.empty()) return;
        
        m_document.execute_backward(m_tx);
        m_tx.clear();
        
        m_document.commit();
        
        m_history.erase(m_history.last_undo());
    }
}}
