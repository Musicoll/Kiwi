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

#include <flip/BackEndIR.h>
#include <flip/BackEndBinary.h>
#include <flip/contrib/DataConsumerFile.h>
#include <flip/contrib/DataProviderFile.h>

#include "KiwiDocumentManager.hpp"

#include <KiwiModel/KiwiPatcher.hpp>

#include <cassert>


namespace kiwi
{
    DocumentManager::DocumentManager(flip::DocumentBase& document) :
    m_document(document),
    m_history(document),
    m_file_handler(m_document)
    {
        ;
    }
    
    void DocumentManager::commit(flip::Type& type, std::string action)
    {
        auto& patcher = type.ancestor<model::Patcher>();
        patcher.entity().use<DocumentManager>().commit(action);
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
    
    void DocumentManager::save(flip::Type& type, File const& file)
    {
        model::Patcher& patcher = type.ancestor<model::Patcher>();
        patcher.entity().use<DocumentManager>().save(file);
    }
    
    void DocumentManager::load(flip::Type& type, File const& file)
    {
        model::Patcher& patcher = type.ancestor<model::Patcher>();
        patcher.entity().use<DocumentManager>().load(file);
    }
    
    File const& DocumentManager::getSelectedFile(flip::Type& type)
    {
        model::Patcher& patcher = type.ancestor<model::Patcher>();
        return patcher.entity().use<DocumentManager>().getSelectedFile();
    }
    
    bool DocumentManager::hasUndo()
    {
        return m_history.last_undo() != m_history.end();
    }
    
    std::string DocumentManager::getUndoLabel()
    {
        assert(hasUndo());
        
        return m_history.last_undo()->label();
    }
    
    void DocumentManager::undo()
    {
        m_history.execute_undo();
    }
    
    bool DocumentManager::hasRedo()
    {
        return m_history.first_redo() != m_history.end();
    }
    
    std::string DocumentManager::getRedoLabel()
    {
        assert(hasRedo());
        
        return m_history.first_redo()->label();
    }
    
    void DocumentManager::redo()
    {
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
        m_document.push();
        
        if(!action.empty())
        {
            m_history.add_undo_step(tx);
        }
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
        
        m_document.push();
    }
    
    void DocumentManager::save(File const& file)
    {
        m_file_handler.save(file);
    }
    
    void DocumentManager::load(File const& file)
    {
        m_file_handler.load(file);
    }
    
    File const& DocumentManager::getSelectedFile() const
    {
        return m_file_handler.getFile();
    }
    
    //=============================================================================
    // FileHandler
    //=============================================================================
    
    FileHandler::FileHandler(flip::DocumentBase & document):
    m_document(document),
    m_file()
    {
    }
    
    File const& FileHandler::getFile() const
    {
        return m_file;
    }
    
    void FileHandler::setFile(File const& file)
    {
        m_file = file;
    }
    
    void FileHandler::load(File const& file)
    {
        if (file.isKiwiFile())
        {
            setFile(file);
            load();
        }
    }
    
    void FileHandler::load()
    {
        flip::DataProviderFile provider(m_file.getAbsolutePath().c_str());
        flip::BackEndIR back_end;
        
        back_end.register_backend<flip::BackEndBinary>();
        back_end.read(provider);
        
        m_document.read(back_end);
    }
    
    void FileHandler::save(File const& file)
    {
        if (file.isKiwiFile())
        {
            setFile(file);
            save();
        }
    }
    
    void FileHandler::save()
    {
        flip::DataConsumerFile consumer(m_file.getAbsolutePath().c_str());
        
        flip::BackEndIR back_end =  m_document.write();
        back_end.write<flip::BackEndBinary>(consumer);
    }
}
