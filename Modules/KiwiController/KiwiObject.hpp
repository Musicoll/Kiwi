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
        
        //! @brief The model::Object controller.
        class Object
        {
        public:
            friend class controller::Patcher;
            
            using IoType = model::Object::IoType;
            
            class Iolet;
            class Inlet;
            class Outlet;
            
            //! @brief Constructor.
            Object(model::Object& model) noexcept;
            
            //! @brief Destructor.
            virtual ~Object() noexcept;
            
            //! @brief Get the Object model
            inline model::Object& getModel()                    { return m_model; }
            
            //! @brief Get the Object model
            inline model::Object const& getModel() const        { return m_model; }
            
            //! @brief Returns the patcher in which this Object is instantiated.
            inline controller::Patcher* getPatcher() const      { return m_patcher; }
            
            //! @brief Returns the name of the Object.
            inline std::string getName() const noexcept         { return m_model.getName(); }
            
            //! @brief Returns the text of the object.
            inline std::string getText() const noexcept         { return m_model.getText(); }
            
            //! @brief Get the number of inlets of the Object.
            inline uint32_t getNumberOfInlets() const noexcept  { return m_model.getNumberOfInlets(); }
            
            //! @brief Get the number of inlets of the object.
            inline uint32_t getNumberOfOutlets() const noexcept { return m_model.getNumberOfOutlets(); }
            
            //! @brief Returns the Inlet at a given index.
            Object::Inlet* getInlet(const uint32_t index) noexcept;
            
            //! @brief Returns the Outlet at a given index.
            Object::Outlet* getOutlet(const uint32_t index) noexcept;
            
            //! @brief The receive method.
            //! @details This method must be overriden by object's subclasses.
            virtual void receive(uint32_t index, std::vector<Atom> args) = 0;
            
        protected:
            
            //! @brief Send a message through a given outlet index.
            void send(const uint32_t index, std::vector<Atom> args);
            
        private:
            model::Object&          m_model;
            controller::Patcher*    m_patcher;
            
            std::vector<Outlet>     m_outlets;
            std::vector<Inlet>      m_inlets;
            std::atomic_ushort      m_stack_count;
        };
        
        // ================================================================================ //
        //                                  Object::Iolet                                   //
        // ================================================================================ //
        
        //! @brief The base class of Inlet and Outlet.
        //! @details The Iolet can be of a given IoType
        //! and owns a vector of connections between inlets and outlets.
        class Object::Iolet
        {
        public:
            
            using connections_t  = std::vector<std::pair<Object*, uint32_t>>;
            
            //! @brief Constructor.
            inline Iolet(IoType type) noexcept : m_type(type) {}
            
            //! @brief Destructor.
            virtual ~Iolet() noexcept
            {
                m_connections.clear();
            }
            
            //! @brief Returns the IoType of the iolet.
            inline IoType getType() const noexcept { return m_type; }
            
            //! @brief Get the number of connections.
            //! @return The number of connections.
            inline uint32_t getNumberOfConnections() const noexcept
            {
                return static_cast<uint32_t>(m_connections.size());
            }
            
            //! @brief Get the object of a connection at a given index.
            //! @param index The index of the connection.
            //! @return The object of a connection.
            Object const* getObject(const uint32_t index) const noexcept;
            
            //! @brief Get the object of a connection at a given index.
            //! @param index The index of the connection.
            //! @return The object of a connection.
            Object* getObject(const uint32_t index) noexcept;
            
            //! @brief Get the iolet's index of a connection at a given index.
            //! @param index The index of the connection.
            //! @return The iolet's index of a connection.
            uint32_t getIndex(const uint32_t index) const noexcept;
            
            //! @brief Check if a connection is in the iolet.
            //! @param object A pointer to the Object to which the iolet belongs.
            //! @param index the iolet's index.
            //! @return true if the connection is in the iolet, otherwise false.
            bool has(Object const* const object, const uint32_t index) const noexcept;
            
            //! @brief Append a new connection to the iolet.
            //! @param object A pointer to the Object to which the iolet belongs.
            //! @param index the iolet's index.
            //! @return true if the connection has been added, otherwise false.
            bool append(Object* object, const uint32_t index);
            
            //! @brief Remove a connection from the iolet.
            //! @param object A pointer to the Object to which the iolet belongs.
            //! @param index the iolet's index.
            //! @return true if the connection has been removed, otherwise false.
            bool erase(Object* object, const uint32_t index);
            
        protected:
            
            connections_t   m_connections;
            const IoType    m_type;
        };
        
        // ================================================================================ //
        //                                   Object::INLET                                  //
        // ================================================================================ //
        
        //! @brief The Inlet.
        class Object::Inlet : public Iolet
        {
        public:
            
            //! @brief Constructor.
            //! @param type The IoType of the inlet.
            Inlet(IoType type) noexcept;
            
            //! @brief Destructor.
            ~Inlet();
        };
        
        // ================================================================================ //
        //                                  Object::OUTLET                                  //
        // ================================================================================ //
        
        //! @brief The outlet is a specific Iolet that can send messages to Inlet.
        class Object::Outlet : public Iolet
        {
        public:
            //! Constructor.
            //! @param type The type of the outlet.
            Outlet(IoType type) noexcept;
            
            //! @brief Destructor.
            ~Outlet();
            
            //! @brief Sends a vector of atoms to the connected inlets.
            //! @param atoms The vector of atoms.
            void send(std::vector<Atom> const& atoms) const noexcept;
        };
    }
}

#endif // KIWI_CONTROLLER_OBJECT_HPP_INCLUDED
