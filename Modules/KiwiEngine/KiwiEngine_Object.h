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

#include "KiwiEngine_Def.h"

#include <KiwiTool/KiwiTool_Scheduler.h>

#include <KiwiEngine/KiwiEngine_Patcher.h>

#include <KiwiDsp/KiwiDsp_Processor.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        //! @brief The Object reacts and interacts with other ones by sending and receiving messages via its inlets and outlets.
        class Object
        {
        public: // methods
            
            //! @brief Constructor.
            Object(model::Object const& model, Patcher& patcher) noexcept;
            
            //! @brief Destructor.
            virtual ~Object() noexcept;
            
            //! @brief Receives a set of arguments via an inlet.
            //! @details This method must be overriden by object's subclasses.
            //! @todo see if the method must be noexcept.
            virtual void receive(size_t index, std::vector<tool::Atom> const& args) = 0;
            
            //! @brief Called when the Patcher is loaded.
            virtual void loadbang() {};
            
            //! @internal Appends a new Link to an outlet.
            void addOutputLink(size_t outlet_index, Object & receiver, size_t inlet_index);
            
            //! @internal Removes a Link from an outlet.
            void removeOutputLink(size_t outlet_index, Object & receiver, size_t inlet_index);
            
        protected: // methods
            
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
            
            //! @biref Returns the engine's scheduler.
            tool::Scheduler<> & getScheduler() const;
            
            // ================================================================================ //
            //                                      BEACON                                      //
            // ================================================================================ //
            
            //! @brief Gets or creates a Beacon with a given name.
            tool::Beacon& getBeacon(std::string const& name) const;
            
            // ================================================================================ //
            //                                       SEND                                       //
            // ================================================================================ //
            
            //! @brief Sends a vector of Atom via an outlet.
            //! @todo Improve the stack overflow system.
            //! @todo See if the method must be noexcept.
            void send(const size_t index, std::vector<tool::Atom> const& args);
            
        private: // members
            
            using Outlet = std::set<Link>;

            Patcher&                m_patcher;
            size_t                  m_inlets;
            std::vector<Outlet>     m_outlets;
            size_t                  m_stack_count = 0ul;
            
        private: // deleted methods
            
            Object(Object const&) = delete;
            Object(Object&&) = delete;
            Object& operator=(Object const&) = delete;
            Object& operator=(Object&&) = delete;
        };
        
        typedef std::shared_ptr<Object> sObject;
        
        // ================================================================================ //
        //                                    AUDIOOBJECT                                   //
        // ================================================================================ //
        
        //! @brief A pure interface that audio object must implement.
        //! @brief audio objects will be held and triggered by both the engine and the dsp chain.
        class AudioObject : public engine::Object, public dsp::Processor
        {
        public: // methods
            
            //! @brief Constructor.
            AudioObject(model::Object const& model, Patcher& patcher) noexcept;
            
            //! @brief Destructor.
            virtual ~AudioObject() = default;
        };
    }
}
