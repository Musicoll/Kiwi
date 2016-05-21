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

#include "KiwiDocumentManager.hpp"

#include <KiwiModel/KiwiPatcher.hpp>


namespace kiwi
{
    DocumentManager::DocumentManager(flip::DocumentBase& document) :
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
        std::chrono::duration<int> time_out(1);
        
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
        startTimer();
    }
    
    void DocumentManager::tick()
    {
        pull();
    }
    
    void DocumentManager::stopPulling()
    {
        stopTimer();
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
    
    //=============================================================================
    // CarrierSocket
    //=============================================================================
    
    CarrierSocket::CarrierSocket(flip::DocumentBase & document, std::string const& host, uint16_t port):
    m_transport_socket(document, host, port),
    m_transport_loop(),
    m_transport_running(false)
    {
        bindCallBacks();
    }
    
    void CarrierSocket::listenStateTransition(flip::CarrierBase::Transition state, flip::CarrierBase::Error error)
    {
        switch(state)
        {
            case flip::CarrierBase::Transition::Disconnected:
            {
                if (m_transport_socket.is_connected())
                {
                    stopProcess();
                    m_func_disonnected();
                }
                break;
            }
            case flip::CarrierBase::Transition::Connecting:
            {
                break;
            }
            case flip::CarrierBase::Transition::Connected:
            {
                m_func_connected();
                break;
            }
        }
    }
    
    void CarrierSocket::listenTransferBackEnd(size_t cur, size_t total)
    {
        if (cur == total)
        {
            m_func_loaded();
        }
    }
    
    void CarrierSocket::listenTransferTransaction(size_t cur, size_t total)
    {
    }
    
    void CarrierSocket::listenTransferSignal(size_t cur, size_t total)
    {
    }
    
    void CarrierSocket::bindCallBacks()
    {
        m_transport_socket.listen_state_transition(std::bind(&CarrierSocket::listenStateTransition,
                                                             this,
                                                             std::placeholders::_1,
                                                             std::placeholders::_2));
        
        m_transport_socket.listen_transfer_backend(std::bind(&CarrierSocket::listenTransferBackEnd,
                                                             this,
                                                             std::placeholders::_1,
                                                             std::placeholders::_2));
        m_transport_socket.listen_transfer_transaction(std::bind(&CarrierSocket::listenTransferTransaction,
                                                                 this,
                                                                 std::placeholders::_1,
                                                                 std::placeholders::_2));
        m_transport_socket.listen_transfer_signal(std::bind(&CarrierSocket::listenTransferSignal,
                                                            this,
                                                            std::placeholders::_1,
                                                            std::placeholders::_2));
    }
    
    void CarrierSocket::listenDisconnected(std::function<void ()> func)
    {
        m_func_disonnected = func;
    }
    
    void CarrierSocket::listenConnected(std::function<void ()> func)
    {
        m_func_connected = func;
    }
    
    void CarrierSocket::listenLoaded(std::function<void ()> func)
    {
        m_func_loaded = func;
    }
    
    void CarrierSocket::startProcess()
    {
        if (!m_transport_running.load())
        {
            m_transport_running.store(true);
            m_transport_loop = std::move(std::thread(&CarrierSocket::runProcess, this));
            m_transport_loop.detach();
        }
    }
    
    void CarrierSocket::process()
    {
        if (!m_transport_running.load())
        {
            m_transport_socket.process();
        }
    }
    
    void CarrierSocket::runProcess()
    {
        while(m_transport_running.load())
        {
            m_transport_socket.process();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    void CarrierSocket::stopProcess()
    {
        if (m_transport_running.load())
        {
            m_transport_running.store(false);
        }
    }
    
    void CarrierSocket::disconnect()
    {
        stopProcess();
        m_transport_socket.rebind("", 0);
    }
    
    bool CarrierSocket::isConnected() const
    {
        return m_transport_socket.is_connected();
    }
    
    void CarrierSocket::connect(std::string const& host, uint16_t port)
    {
        stopProcess();
        m_transport_socket.rebind(host, port);
    }
    
    CarrierSocket::~CarrierSocket()
    {
        stopProcess();
    }
}
