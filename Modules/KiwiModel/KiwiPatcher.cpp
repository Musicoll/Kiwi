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

#include "KiwiPatcher.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                   PATCHER MODEL                                  //
        // ================================================================================ //
        
        Patcher::Patcher()
        {
            ;
        }
        
        Patcher::~Patcher()
        {
            m_objects.clear();
            m_links.clear();
        }
        
        std::unique_ptr<model::Object> Patcher::createObject(std::string const& name, std::string const& text)
        {
            if(name == "plus" || name == "+")
            {
                return std::unique_ptr<ObjectPlus>(new ObjectPlus(name, text));
            }
            else if(name == "print")
            {
                //return std::unique_ptr<ObjectPrint>(new ObjectPrint(name, text));
            }
            
            return nullptr;
        }
        
        model::Object* Patcher::addObject(std::string const& name, std::string const& text)
        {
            auto object = createObject(name, text);
            
            if(object)
            {
                const auto it = m_objects.insert(m_objects.end(), std::move(object));
                return it.operator->();
            }
            
            return nullptr;
        }
        
        Link* Patcher::addLink(model::Object* from, const uint8_t outlet, model::Object* to, const uint8_t inlet)
        {
            std::unique_ptr<Link> link = Link::create(from, outlet, to, inlet);

            if(link)
            {
                const auto it = m_links.insert(std::move(link));
                return it.operator->();
            }
            
            return nullptr;
        }
        
        void Patcher::remove(model::Object* object)
        {
            if(object)
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                
                auto predicate = [object](Object const& obj)
                {
                    return &obj == object;
                };
                
                auto it = find_if(m_objects.begin(), m_objects.end(), predicate);
                if(it != m_objects.end())
                {
                    m_objects.erase(it);
                }
            }
        }
    }
    
}
