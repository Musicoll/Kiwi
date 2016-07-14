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

#include <flip/BackEndIR.h>
#include <flip/BackEndBinary.h>
#include <flip/contrib/DataConsumerFile.h>
#include <flip/contrib/DataProviderFile.h>

#include <KiwiModel/KiwiModelPatcherUser.hpp>

#include "KiwiApp_DocumentManager.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                   DOCUMENT MANAGER                               //
    // ================================================================================ //
    
    DocumentManager::DocumentManager(flip::DocumentBase& document) :
    m_document(document),
    m_history(document),
    m_socket(m_document, "", 0),
    m_file_handler(m_document),
    m_loaded(false)
    {
        m_socket.listenConnected(std::bind(&DocumentManager::onConnected, this));
        m_socket.listenDisconnected(std::bind(&DocumentManager::onDisconnected, this));
        m_socket.listenLoaded(std::bind(&DocumentManager::onLoaded, this));
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
    
    void DocumentManager::save(flip::Type& type, juce::File const& file)
    {
        model::Patcher& patcher = type.ancestor<model::Patcher>();
        patcher.entity().use<DocumentManager>().save(file);
    }
    
    void DocumentManager::load(flip::Type& type, juce::File const& file)
    {
        model::Patcher& patcher = type.ancestor<model::Patcher>();
        patcher.entity().use<DocumentManager>().load(file);
    }
    
    juce::File const& DocumentManager::getSelectedFile(flip::Type& type)
    {
        model::Patcher& patcher = type.ancestor<model::Patcher>();
        return patcher.entity().use<DocumentManager>().getSelectedFile();
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
        startTimer(20);
    }
    
    void DocumentManager::timerCallback()
    {
        pull();
    }
    
    void DocumentManager::stopPulling()
    {
        stopTimer();
    }
    
    void DocumentManager::onConnected()
    {
        //KiwiApp::post("Connected"); -> To check issue with multiThreading
        startPulling();
    }
    
    void DocumentManager::onDisconnected()
    {
        //KiwiApp::post("Disconnected"); -> To check Issue with multiThreading
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
    
    void DocumentManager::save(juce::File const& file)
    {
        m_file_handler.save(file);
    }
    
    void DocumentManager::load(juce::File const& file)
    {
        m_file_handler.load(file);
    }
    
    juce::File const& DocumentManager::getSelectedFile() const
    {
        return m_file_handler.getFile();
    }
    
    // ================================================================================ //
    //                                    FILE HANDLER                                  //
    // ================================================================================ //
    
    FileHandler::FileHandler(flip::DocumentBase & document):
    m_document(document),
    m_file()
    {
    }
    
    juce::File const& FileHandler::getFile() const
    {
        return m_file;
    }
    
    void FileHandler::setFile(juce::File const& file)
    {
        m_file = file;
    }
    
    bool FileHandler::hasValidExtension(juce::File const& file)
    {
        return file.hasFileExtension("kiwi");
    }
    
    void FileHandler::load(juce::File const& file)
    {
        if (hasValidExtension(file))
        {
            setFile(file);
            load();
        }
    }
    
    void FileHandler::load()
    {
        flip::DataProviderFile provider(m_file.getFullPathName().toStdString().c_str());
        flip::BackEndIR back_end;
        
        back_end.register_backend<flip::BackEndBinary>();
        back_end.read(provider);
        
        m_document.read(back_end);
    }
    
    void FileHandler::save(juce::File const& file)
    {
        if (hasValidExtension(file))
        {
            setFile(file);
            save();
        }
    }
    
    void FileHandler::save()
    {
        flip::DataConsumerFile consumer(m_file.getFullPathName().toStdString().c_str());
        
        flip::BackEndIR back_end =  m_document.write();
        back_end.write<flip::BackEndBinary>(consumer);
    }
}
