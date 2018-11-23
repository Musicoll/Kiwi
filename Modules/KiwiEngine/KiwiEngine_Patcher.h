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

#pragma once

#include <map>
#include <flip/Ref.h>

#include <KiwiTool/KiwiTool_Beacon.h>

#include "KiwiEngine_Def.h"
#include "KiwiEngine_AudioControler.h"

#include <KiwiDsp/KiwiDsp_Chain.h>

#include <KiwiModel/KiwiModel_PatcherUser.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                      PATCHER                                     //
    // ================================================================================ //
    
    //! @brief The Patcher manages a set of Object and Link.
    class Patcher
    {
    private: // classes
        
        class CallBack;
        
    public: // methods
        
        //! @brief Constructor.
        Patcher(Instance& instance, model::Patcher & patcher_model) noexcept;
        
        //! @brief Destructor.
        ~Patcher();
        
        //! @internal The model changed.
        void modelChanged(model::Patcher const& model);
        
        //! @brief Adds a link to the current stack overflow list (or create a new list if there is no).
        //! @internal Only the Object should use this method.
        void signalStackOverflow(flip::Ref ref);
        
        //! @brief Gets the lists of stack overflow.
        bool stackOverflowDetected() const;
        
        //! @brief Clears the stack overflow.
        void clearStackOverflow();
        
        //! @brief Returns the audio controler held by the patcher's instance.
        AudioControler& getAudioControler() const;
        
        //! @internal Call the loadbang method of all objects.
        void sendLoadbang();
        
        //! @brief Returns the patcher's data model.
        model::Patcher & getPatcherModel();
        
        // ================================================================================ //
        //                                      CONSOLE                                     //
        // ================================================================================ //
        
        //! @brief post a log message in the Console.
        void log(std::string const& text) const;
        
        //! @brief post a message in the Console.
        void post(std::string const& text) const;
        
        //! @brief post a warning message in the Console.
        void warning(std::string const& text) const;
        
        //! @brief post an error message in the Console.
        void error(std::string const& text) const;
        
        // ================================================================================ //
        //                                      SCHEDULER                                   //
        // ================================================================================ //
        
        //! @brief Returns the engine's scheduler.
        tool::Scheduler<> & getScheduler() const;
        
        //! @brief Returns the main scheduler
        tool::Scheduler<> & getMainScheduler() const;
        
        // ================================================================================ //
        //                                      BEACON                                      //
        // ================================================================================ //
        
        //! @brief Gets or creates a Beacon with a given name.
        tool::Beacon& getBeacon(std::string const& name) const;
        
    private: // methods
        
        //! @brief Called when the stack-overflow is cleared.
        void onStackOverflowCleared();
        
        //! @brief Updates the dsp chain held by the engine patcher
        void updateChain();
        
        //! @internal Update the patcher's graph according to datamodel.
        void updateGraph(model::Patcher const& patcher_model);
        
        //! @internal Updates obects' parameters according to there data model.
        void updateAttributes(model::Patcher const& patcher_model);
        
        //! @internal Object model has just been added to the document.
        void objectAdded(model::Object const& object);
        
        //! @internal Object model has changed.
        void objectChanged(model::Object const& object);
        
        //! @internal Object model will be removed from the document.
        void objectRemoved(model::Object const& object);
        
        //! @internal Link model has just been added to the document.
        void linkAdded(model::Link const& link);
        
        //! @internal Link model has changed.
        void linkChanged(model::Link const& link_m);
        
        //! @internal Link model will be removed from the document.
        void linkRemoved(model::Link const& link_m);
        
    private: // members
        
        Instance&                                       m_instance;
        std::map<flip::Ref, std::shared_ptr<Object>>    m_objects {};
        dsp::Chain                                      m_chain {};
        model::Patcher&                                 m_patcher_model;
        bool                                            m_has_stack_overflow = false;
        bool                                            m_dsp_chain_need_update = false;
        
        flip::SignalConnection                          m_stack_overflow_cleared_signal_cnx;
        
    private: // deleted methods
        
        Patcher(Patcher const&) = delete;
        Patcher(Patcher&&) = delete;
        Patcher& operator=(Patcher const&) = delete;
        Patcher& operator=(Patcher&&) = delete;
    };
    
}}
