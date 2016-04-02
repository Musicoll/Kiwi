/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#ifndef KIWI_CONTROLLER_OBJECT_HPP_INCLUDED
#define KIWI_CONTROLLER_OBJECT_HPP_INCLUDED

#include "../KiwiModel/KiwiModel.hpp"
#include <utility>

namespace kiwi
{
    namespace controller
    {
        class Patcher;
        class Instance;
        
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        //! The object is a graphical object.
        /**
         The object is a graphical class that aims to be instantiated in a patcher.
         */
        class Object
        {
        public:
            friend class controller::Patcher;
            
            struct Io
            {
                enum class Type : unsigned char
                {
                    Message = 0u,
                    Signal  = 1u,
                    Both    = 2u
                };
            };
            
            class Iolet;
            class Inlet;
            class Outlet;
            
            //! Constructor.
            Object(model::Object& model) noexcept;
            
            //! Destructor.
            virtual ~Object() noexcept;
            
            //! Retrieve the patcher that manages the object.
            /** The function retrieves the patcher that manages the object.
             @return The patcher that manages the object.
             */
            inline controller::Patcher* getPatcher() const  { return m_patcher; }
            
            //! Retrieve the name of the object.
            /** The function retrieves the name of the object as a tag.
             @return The name of the object as a tag.
             */
            inline std::string getName() const noexcept     { return m_model.getName(); }
            
            //! Retrieve the text of the object.
            /** The function retrieves the text of the object.
             @return The text of the object.
             */
            inline std::string getText() const noexcept     { return m_model.getText(); }
            
            //! @brief Get the number of inlets of the object.
            //! @return The text of the object.
            inline uint32_t getNumberOfInlets() const noexcept      { return m_model.getNumberOfInlets(); }
            
            //! @brief Get the number of inlets of the object.
            //! @return The text of the object.
            inline uint32_t getNumberOfOutlets() const noexcept     { return m_model.getNumberOfOutlets(); }
            
            //! @brief The receive method.
            virtual void receive(uint32_t index, std::vector<Atom> args) = 0;
            
        protected:
            
            //! @brief Send a message to at a given outlet index.
            void send(const uint32_t index, std::vector<Atom> args);
            
        private:
            model::Object&          m_model;
            controller::Patcher*    m_patcher;
            
            std::vector<Outlet>     m_outlets;
            std::vector<Inlet>      m_inlets;
            std::atomic_ushort      m_stack_count;
        };
        
        //! The outlet owns a set of links.
        /**
         The outlet owns a set of links that are used to manage links in a object. It also have a type and a description.
         */
        class Object::Iolet
        {
        protected:
            using connection_t      = std::pair<Object*, uint32_t>;
            using connection_vec_t  = std::vector<connection_t>;
            
            connection_vec_t    m_connections;
            const Io::Type      m_type;
            
        public:
            //! Check if a connection is in the iolet.
            /** The functions checks if a connection is in the iolet.
             @param object The object.
             @param index the iolet's index.
             @return true if the connection is in the iolet, otherwise false.
             */
            bool has(Object const* const object, const uint32_t index) const noexcept;
            
            //! Append a new connection to the iolet.
            /** The functions appends a new connection to the iolet.
             @param object The object.
             @param index the iolet's index.
             @return true if the connection has been added, otherwise false.
             */
            bool append(Object* object, const uint32_t index) noexcept;
            
            //! Remove a connection from the iolet.
            /** The functions removes a connection from the iolet.
             @param object The object.
             @param index the iolet's index.
             @return true if the connection has been removed, otherwise false.
             */
            bool erase(Object* object, const uint32_t index) noexcept;
            
            //! Constructor.
            inline Iolet(Io::Type type) noexcept : m_type(type) {}
            
            //! Destructor.
            inline ~Iolet() noexcept
            {
                m_connections.clear();
            }
            
            //! Retrieve the type of the iolet.
            /** The functions retrieves the type of the iolet.
             @return The type of the iolet.
             */
            inline Io::Type getType() const noexcept
            {
                return m_type;
            }
            
            //! Retrieve the number of connections.
            /** The functions retrieves the number of connections of the iolet.
             @return The number of connections.
             */
            inline uint32_t getNumberOfConnections() const noexcept
            {
                return static_cast<uint32_t>(m_connections.size());
            }
            
            //! Retrieve the object of a connection.
            /** The functions retrieves the object of a connection.
             @param index The index of the connection.
             @return The object of a connection.
             */
            Object const* getObject(const uint32_t index) const noexcept;
            
            //! Retrieve the object of a connection.
            /** The functions retrieves the object of a connection.
             @param index The index of the connection.
             @return The object of a connection.
             */
            Object* getObject(const uint32_t index) noexcept;
            
            //! Retrieve the iolet's index of a connection.
            /** The functions retrieves the iolet's index of a connection.
             @param index The index of the connection.
             @return The iolet's index of a connection.
             */
            uint32_t getIndex(const uint32_t index) const noexcept;
        };
        
        // ================================================================================ //
        //                                      INLET                                       //
        // ================================================================================ //
        
        //! The inlet owns a set of links.
        /**
         The inlet owns a set of links that are used to manage links in a object. It also have a type and a description.
         */
        class Object::Inlet : public Iolet
        {
        public:
            
            //! Constructor.
            /** You should never call this method except if you really know what you're doing.
             */
            Inlet(Io::Type type) noexcept;
            
            //! Destructor.
            /** You should never call this method except if you really know what you're doing.
             */
            ~Inlet();
        };
        
        // ================================================================================ //
        //                                      OUTLET                                      //
        // ================================================================================ //
        
        //! The outlet owns a set of links.
        /**
         The outlet owns a set of links that are used to manage links in a object. It also have a type and a description.
         */
        class Object::Outlet : public Iolet
        {
        public:
            //! Constructor.
            /** You should never call this method except if you really know what you're doing.
             @param type        The type of the outlet.
             */
            Outlet(Io::Type type) noexcept;
            
            //! Destructor.
            /** You should never call this method except if you really know what you're doing.
             */
            ~Outlet();
            
            //! Send a vector of atoms to the connected inlets.
            /** The function sends of atoms to the connected inlets.
             @param atoms The vector of atoms.
             */
            void send(std::vector<Atom> const& atoms) const noexcept;
        };
    }
}

#endif // KIWI_CONTROLLER_OBJECT_HPP_INCLUDED
