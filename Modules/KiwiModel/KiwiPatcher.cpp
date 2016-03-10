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
#include "KiwiFactory.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                   PATCHER MODEL                                  //
        // ================================================================================ //
        
        Patcher::~Patcher()
        {
            m_objects.clear();
            m_links.clear();
            m_free_ids.clear();
        }
        
        void Patcher::init()
        {
            flip::Class<Patcher>::Members members = get_class().members();
            
            flip::ClassBase& attr_base_class = flip::Class<Attribute>::use();
            
            std::string name_to_find = "bgcolor";
            
            for(const auto& member : members)
            {
                const bool isAttr = (member._base_ptr->inherit_from(attr_base_class));
                
                std::cout << member._name_0 << "\n";
                std::cout << "is Attr : ";
                std::cout << (isAttr ? "true" : "false") << '\n';
                
                
                //Attribute* attr = member;
            }
            
            addAttr(&m_bgcolor, "bgcolor", {0., 0., 0., 1.});
            addAttr(&m_gridsize, "gridsize", {20});
            //addAttr(&m_attr_tag, "attr_tag", "test tag");
            
            /*
             auto it = dico.find(Tags::patcher);
             if(it != dico.end())
             {
             add(it->second);
             }
             */
        }
        
        void Patcher::createObject(std::string const& name, std::string const& text)
        {
            if(Factory::has(name))
            {
                const model::Object* object = Factory::create(name, text);
                
                if(object)
                {
                    m_objects.insert(m_objects.end(), *object);
                }
            }
        }
        
        void Patcher::add(std::map<const std::string, Atom> const& /*dico*/)
        {
            /*
             Vector objects;
             const auto it = dico.find(Tags::objects);
             if(it != dico.end())
             {
             objects = it->second;
             }
             
             std::lock_guard<std::mutex> guard(m_mutex);
             
             for(Dico objdico : objects)
             {
             if(!objdico.empty())
             {
             //const ulong r_id = objdico[Tags::id];
             const ulong n_id = m_free_ids.empty() ? getNumberOfObjects() + 1 : m_free_ids[0];
             if(!m_free_ids.empty())
             {
             m_free_ids.erase(m_free_ids.begin());
             }
             //@todo : no need to cast here
             objdico[Tags::id] = static_cast<int64_t>(n_id);
             
             createObject(objdico);
             }
             }
             */
        }
        
        void Patcher::remove(model::Object* object)
        {
            if(object)
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                
                auto predicate = [object](const Object& obj) {
                    return &obj == object;
                };
                
                auto it = find_if(m_objects.begin(), m_objects.end(), predicate);
                if(it != m_objects.end())
                {
                    //m_listeners.call(&Listener::objectRemoved, getShared(), object);
                    m_objects.erase(it);
                    m_free_ids.push_back(static_cast<uint64_t>(object->getId()));
                }
            }
        }
    }
    
}
