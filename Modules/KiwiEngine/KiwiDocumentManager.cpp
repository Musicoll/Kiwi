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


#include <cassert>
#include <mutex>
#include <condition_variable>

#include <KiwiModel/KiwiPatcher.hpp>

#include "KiwiDocumentManager.hpp"
#include "KiwiInstance.hpp"

namespace kiwi
{
    DocumentManager::DocumentManager(flip::DocumentBase& document, engine::Instance const& instance) :
    m_instance(instance),
    m_document(document),
    m_history(document),
    m_socket(m_document, "", 0),
    m_loaded(false)
    {
        m_socket.listenConnected(std::bind(&DocumentManager::onConnected,
                                           this));
        m_socket.listenDisconnected(std::bind(&DocumentManager::onDisconnected,
                                              this));
        m_socket.listenLoaded(std::bind(&DocumentManager::onLoaded,
                                        this));
        
        m_timer = m_instance.createTimer(std::bind(&DocumentManager::tick, this));
    }
    
    DocumentManager::~DocumentManager()
    {
        disconnect();
    }
    
    void DocumentManager::commit(flip::Type& type, std::string action)
    {
        auto& patcher = type.ancestor<model::Patcher>();
        patcher.entity().use<DocumentManager>().commit(action);
    }
    
    void DocumentManager::connect(flip::Type& type, const std::string host, uint16_t port)
    {
        model::Patcher& patcher = type.ancestor<model::Patcher>();
        patcher.entity().use<DocumentManager>().connect(host, port);
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
    
    void DocumentManager::connect(std::string const host, uint16_t port)
    {
        disconnect();
        
        m_socket.connect(host, port);
        
        std::chrono::steady_clock::time_point init_time = std::chrono::steady_clock::now();
        std::chrono::duration<int> time_out(2);
        
        while(!m_socket.isConnected() && std::chrono::steady_clock::now() - init_time < time_out)
        {
            m_socket.process();
        }
        
        if (m_socket.isConnected())
        {
            while(!m_loaded.load())
            {
                m_socket.process();
            }
            
            pull();
            
            m_socket.startProcess();
        }
        else
        {
            throw std::runtime_error("Connexion failed");
        }
    }
    
    void DocumentManager::disconnect()
    {
        m_loaded.store(false);
        m_socket.disconnect();
    }
    
    bool DocumentManager::isConnected()
    {
        return m_socket.isConnected();
    }
    
    void DocumentManager::startPulling()
    {
        if(m_timer)
        {
            m_timer->start(20);
        }
    }
    
    void DocumentManager::tick()
    {
        pull();
    }
    
    void DocumentManager::stopPulling()
    {
        if(m_timer)
        {
            m_timer->stop();
        }
    }
    
    void DocumentManager::onConnected()
    {
        //Console::post("Connected"); -> To check issue with multiThreading
        startPulling();
    }
    
    void DocumentManager::onDisconnected()
    {
        //Console::post("Disconnected"); -> To check Issue with multiThreading
        stopPulling();
    }
    
    void DocumentManager::onLoaded()
    {
        m_loaded.store(true);
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
}
