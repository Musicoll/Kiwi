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

#include <functional>
#include <set>
#include <string>
#include <atomic>

#include <flip/Ref.h>

#include <KiwiEngine/KiwiEngine_Def.h>

#include <KiwiTool/KiwiTool_Scheduler.h>
#include <KiwiTool/KiwiTool_ConcurrentQueue.h>

#include <KiwiEngine/KiwiEngine_Patcher.h>

#include <KiwiDsp/KiwiDsp_Processor.h>

#include <KiwiModel/KiwiModel_Object.h>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        //! @brief The Object reacts and interacts with other ones by sending and receiving messages via its inlets and outlets.
        class Object : public model::Object::Listener
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
            
            //! @brief Called when a parameter has changed.
            void modelParameterChanged(std::string const& name, tool::Parameter const& parameter) override final;
            
            //! @brief Called when an attribute has changed.
            void modelAttributeChanged(std::string const& name, tool::Parameter const& parameter) override final;
            
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
            
            //! @biref Returns the main scheduler.
            tool::Scheduler<> & getMainScheduler() const;
            
            //! @brief Defers a task on the engine thread.
            //! @details The task is automatically unscheduled when object is destroyed.
            void defer(std::function<void()> call_back);
            
            //! @brief Defers a task on the main thread.
            //! @details The tasks is automatically unscheduled when object is destroyed.
            void deferMain(std::function<void()> call_back);
            
            //! @brief Schedules a task on the engine thread.
            //! @details The tasks is automatically unscheduled when object is destroyed.
            void schedule(std::function<void()> call_back, tool::Scheduler<>::duration_t delay);
            
            //! @brief Schedules a task on the main thread.
            //! @details The tasks is automatically unscheduled when object is destroyed.
            void scheduleMain(std::function<void()> call_back, tool::Scheduler<>::duration_t delay);
            
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
            
            //! @brief Changes one of the data model's attributes.
            //! @details For thread safety actual model's modification is called on the main thread.
            void setAttribute(std::string const& name, tool::Parameter const& parameter);
            
            //! @brief Changes one of the data model's parameter.
            //! @details For thread safety actual model's modification is called on the main thread.
            void setParameter(std::string const& name, tool::Parameter const& parameter);
            
        private: // methods
            
            //! @brief Returns the object's data model.
            model::Object & getObjectModel();
            
            //! @brief Called once the data model's parameters has changed.
            //! @details Automatically called on the engine's thread.
            virtual void parameterChanged(std::string const& param_name, tool::Parameter const& param);
            
            //! @brief Called once one of the data model's attributes has changed.
            //! @brief Automatically called on the engine's thread.
            virtual void attributeChanged(std::string const& name, tool::Parameter const& attribute);
            
        private: // members
            
            using Outlet = std::set<Link>;

            Patcher&                        m_patcher;
            flip::Ref const                 m_ref;
            size_t                          m_inlets;
            std::vector<Outlet>             m_outlets;
            size_t                          m_stack_count;
            std::shared_ptr<Object>         m_master;
            
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
