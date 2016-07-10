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

#ifndef KIWI_ENGINE_OBJECT_HPP_INCLUDED
#define KIWI_ENGINE_OBJECT_HPP_INCLUDED

#include "KiwiEngineDef.hpp"

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

            //! @brief Gets the name of the Object.
            std::string getName() const;
            
            //! @brief Gets the number of inlets of the Object.
            size_t getNumberOfInlets() const;
            
            //! @brief Gets the number of outlets of the Object.
            size_t getNumberOfOutlets() const;
            
            //! @brief Receives a set of arguments via an inlet.
            //! @details This method must be overriden by object's subclasses.
            //! @todo see if the method must be noexcept.
            virtual void receive(size_t index, std::vector<Atom> const& args) = 0;
            
            //! @internal Appends a new Link to an outlet.
            void addOutputLink(Link const& link);
            
            //! @internal Removes a Link from an outlet.
            void removeOutputLink(Link const& link);
            
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
            //                                      BEACON                                      //
            // ================================================================================ //
            
            //! @brief Gets or creates a Beacon with a given name.
            Beacon& getBeacon(std::string const& name) const;
            
            // ================================================================================ //
            //                                       SEND                                       //
            // ================================================================================ //
            
            //! @brief Sends a vector of Atom via an outlet.
            //! @todo Improve the stack overflow system.
            //! @todo See if the method must be noexcept.
            void send(const size_t index, std::vector<Atom> const& args);
            
        private: // members
            
            using  Outlet = std::set<Link const*>;

            model::Object const&    m_model;
            Patcher&                m_patcher;
            std::vector<Outlet>     m_outlets;
            size_t                  m_stack_count = 0ul;
            
        private: // deleted methods
            
            Object(Object const&) = delete;
            Object(Object&&) = delete;
            Object& operator=(Object const&) = delete;
            Object& operator=(Object&&) = delete;
        };
        
        typedef std::shared_ptr<Object> sObject;
    }
}

#endif // KIWI_ENGINE_OBJECT_HPP_INCLUDED
