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

#include "KiwiObject.hpp"
#include "KiwiPatcher.hpp"
#include "KiwiInstance.hpp"

namespace kiwi
{
    namespace controller
    {
        // ================================================================================ //
        //                                      IOLET                                       //
        // ================================================================================ //
        
        Object const* Object::Iolet::getObject(const uint32_t index) const noexcept
        {
            const auto idx = static_cast<connection_vec_t::size_type>(index);
            
            if(idx < m_connections.size())
            {
                return m_connections[idx].first;
            }
            
            return nullptr;
        }
        
        Object* Object::Iolet::getObject(const uint32_t index) noexcept
        {
            const auto idx = static_cast<connection_vec_t::size_type>(index);
            
            if(idx < m_connections.size())
            {
                return m_connections[idx].first;
            }
            
            return nullptr;
        }
        
        
        uint32_t Object::Iolet::getIndex(const uint32_t index) const noexcept
        {
            const auto idx = static_cast<connection_vec_t::size_type>(index);
            
            if(idx < m_connections.size())
            {
                return m_connections[idx].second;
            }
            
            return 0;
        }
        
        bool Object::Iolet::has(Object const* const object, const uint32_t index) const noexcept
        {
            if(object)
            {
                const auto idx = static_cast<connection_vec_t::size_type>(index);
                
                const auto it = std::find_if(m_connections.begin(), m_connections.end(), [&idx, object](connection_t const& con)
                {
                    return (idx == con.second && object == con.first);
                });
                
                return (it != m_connections.end());
            }
            
            return false;
        }
        
        bool Object::Iolet::append(Object* object, const uint32_t index) noexcept
        {
            if(object && !has(object, index))
            {
                m_connections.emplace_back(object, index);
                std::cout << "append iolet" << '\n';
                return true;
            }
            
            return false;
        }
        
        bool Object::Iolet::erase(Object* object, uint32_t index) noexcept
        {
            if(object)
            {
                const auto idx = static_cast<connection_vec_t::size_type>(index);
                
                const auto connection_exists = [&idx, object](connection_t const& con)
                {
                    return (idx == con.second && object == con.first);
                };
                
                const auto it = std::find_if(m_connections.begin(), m_connections.end(), connection_exists);
                
                if(it != m_connections.end())
                {
                    m_connections.erase(it);
                    return true;
                }
            }
            return false;
        }
        
        // ================================================================================ //
        //                                      OUTLET                                      //
        // ================================================================================ //
        
        Object::Outlet::Outlet(IoType type) noexcept :
        Iolet(type)
        {
            ;
        }
        
        Object::Outlet::~Outlet()
        {
            ;
        }
        
        void Object::Outlet::send(std::vector<Atom> const& atoms) const noexcept
        {
            for(auto& connection : m_connections)
            {
                Object* const receiver = connection.first;
                
                if(receiver)
                {
                    if(++receiver->m_stack_count < 256)
                    {
                        receiver->receive(connection.second, atoms);
                    }
                    else if(receiver->m_stack_count == 256)
                    {
                        receiver->receive(connection.second, atoms);
                    }
                    else
                    {
                        // commented because of an xcode f*c*i*g indentation bug
                        //std::cout << "object " << receiver->getName() << "Stack overflow";
                    }
                    
                    receiver->m_stack_count--;
                }
            }
        }
        
        // ================================================================================ //
        //                                      INLET                                       //
        // ================================================================================ //
        
        Object::Inlet::Inlet(IoType type) noexcept :
        Iolet(type)
        {
            ;
        }
        
        Object::Inlet::~Inlet()
        {
            ;
        }
        
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        Object::Object(model::Object& model) noexcept : m_model(model)
        {
            // populate inlets from model
            for(auto const& inlet : m_model.getInlets())
            {
                m_inlets.emplace_back(inlet.getType());
            }
            
            // populate outlets from model
            for(auto const& outlet : m_model.getOutlets())
            {
                m_outlets.emplace_back(outlet.getType());
            }
        }
        
        Object::~Object() noexcept
        {
            ;
        }
        
        Object::Inlet* Object::getInlet(const uint32_t index) noexcept
        {
            const auto idx = static_cast<std::vector<Inlet>::size_type>(index);
            
            if(idx < m_inlets.size())
            {
                return &m_inlets[idx];
            }
            else
            {
                return nullptr;
            }
        }
        
        Object::Outlet* Object::getOutlet(const uint32_t index) noexcept
        {
            const auto idx = static_cast<std::vector<Outlet>::size_type>(index);
            
            if(idx < m_outlets.size())
            {
                return &m_outlets[idx];
            }
            else
            {
                return nullptr;
            }
        }
        
        void Object::send(const uint32_t index, std::vector<Atom> args)
        {
            std::cout << "Object " << getName() << " send msg : {"
            << AtomHelper::toString(args)
            << "} through outlet " << index << '\n';
            
            const auto idx = static_cast<std::vector<Outlet>::size_type>(index);
            
            if(idx < m_outlets.size())
            {
                m_outlets[idx].send(args);
            }
        }
    }
}
