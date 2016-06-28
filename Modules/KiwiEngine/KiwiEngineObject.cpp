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

#include "KiwiEngineObject.hpp"
#include "KiwiEngineLink.hpp"

#include <KiwiModel/KiwiConsole.hpp>
#include <KiwiModel/KiwiObject.hpp>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        Object::Object(model::Object const& object) noexcept :
        m_model(object),
        m_outlets(object.getNumberOfOutlets()),
        m_stack_count(0ul)
        {
            ;
        }
        
        Object::~Object() noexcept
        {
            m_outlets.clear();
        }
        
        std::string Object::getName() const noexcept
        {
            return m_model.getName();
        }
        
        size_t Object::getNumberOfInlets() const noexcept
        {
            return m_model.getNumberOfInlets();
        }
        
        size_t Object::getNumberOfOutlets() const noexcept
        {
            return m_model.getNumberOfOutlets();
        }
        
        void Object::addOutputLink(Link const* link)
        {
            m_outlets[link->getSenderIndex()].insert(link);
        }
        
        void Object::removeOutputLink(Link const* link)
        {
            m_outlets[link->getSenderIndex()].erase(link);
        }
        
        void Object::send(const size_t index, std::vector<Atom> const& args)
        {
            const auto idx = static_cast<std::vector<Outlet>::size_type>(index);
            
            if(idx < m_outlets.size())
            {
                for(auto const* link : m_outlets[idx])
                {
                    std::shared_ptr<Object> receiver = link->getReceiverObject();
                    
                    if(++(receiver->m_stack_count) < 256)
                    {
                        receiver->receive(link->getReceiverIndex(), args);
                    }
                    else
                    {
                        Console::error("object " + getName() + " => Stack overflow !");
                    }
                    
                    --(receiver->m_stack_count);
                }
            
            }
        }
    }
}
