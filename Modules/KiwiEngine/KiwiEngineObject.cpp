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

#include "KiwiEngineObject.hpp"
#include "KiwiEngineLink.hpp"
#include "KiwiEnginePatcher.hpp"

#include <KiwiModel/KiwiModelObject.hpp>

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        Object::Object(model::Object const& model, Patcher& patcher) noexcept :
        m_model(model),
        m_patcher(patcher),
        m_outlets(model.getNumberOfOutlets()),
        m_stack_count(0ul)
        {
            ;
        }
        
        Object::~Object() noexcept
        {
            m_outlets.clear();
        }
        
        std::string Object::getName() const
        {
            return m_model.getName();
        }
        
        size_t Object::getNumberOfInlets() const
        {
            return m_model.getNumberOfInlets();
        }
        
        size_t Object::getNumberOfOutlets() const
        {
            return m_model.getNumberOfOutlets();
        }
        
        void Object::addOutputLink(Link const& link)
        {
            m_outlets[link.getSenderIndex()].insert(&link);
        }
        
        void Object::removeOutputLink(Link const& link)
        {
            m_outlets[link.getSenderIndex()].erase(&link);
        }
        
#define KIWI_ENGINE_STACKOVERFLOW_MAX 256
        
        void Object::send(const size_t index, std::vector<Atom> const& args)
        {
            const auto idx = static_cast<std::vector<Outlet>::size_type>(index);
            
            if(idx < m_outlets.size())
            {
                for(auto const* link : m_outlets[idx])
                {
                    Object& receiver = link->getReceiverObject();
                    if(++(receiver.m_stack_count) < KIWI_ENGINE_STACKOVERFLOW_MAX)
                    {
                        receiver.receive(link->getReceiverIndex(), args);
                    }
                    else if(++(receiver.m_stack_count) == KIWI_ENGINE_STACKOVERFLOW_MAX)
                    {
                        m_patcher.addStackOverflow(*link);
                        receiver.receive(link->getReceiverIndex(), args);
                    }
                    else
                    {
                        m_patcher.endStackOverflow();
                    }
                    --(receiver.m_stack_count);
                }
            
            }
        }

#undef KIWI_ENGINE_STACKOVERFLOW_MAX
        
    }
}


