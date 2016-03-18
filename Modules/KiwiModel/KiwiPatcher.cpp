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
#include "KiwiObjectFactory.hpp"

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
            addAttr(&m_bgcolor, "bgcolor", {0., 0., 0., 1.});
            addAttr(&m_gridsize, "gridsize", {20});
            
            /*
            flip::Class<Patcher>::Members members = get_class().members();
            
            std::string name_to_find = "bgcolor";
            
            for(const auto& member : members)
            {
                const bool isAttr = (member._base_ptr->inherit_from(flip::Class<Attribute>::use()));
                
                std::cout << member._name_0 << "\n";
                std::cout << "is Attr : ";
                std::cout << (isAttr ? "true" : "false") << '\n';
                
                if (isAttr && std::string(member._name_0) == "bgcolor")
                {
                    flip::Type &type = member._representative.from(*this);
                    Attribute::RGBA& attr = type.ancestor<Attribute::RGBA>();
                    attr.set(FlipRGBA(0.2, 0.4, 0.5, 0.6));
                }
            }
            */
            
            
            //addAttr(&m_attr_tag, "attr_tag", "test tag");
            
            /*
             auto it = dico.find(Tags::patcher);
             if(it != dico.end())
             {
             add(it->second);
             }
             */
        }
        
        model::Object* Patcher::addObject(std::string const& name, std::string const& text)
        {
            if(ObjectFactory::has(name))
            {
                std::unique_ptr<model::Object> object = ObjectFactory::create(name, text);
                
                if(object)
                {
                    const auto it = m_objects.insert(m_objects.begin(), std::move(object));
                    return it.operator->();
                }
            }
            
            return nullptr;
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
