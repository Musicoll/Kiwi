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

#include "KiwiPatcherModel.h"
#include "KiwiFactory.h"

namespace kiwi
{    
    // ================================================================================ //
    //                                   PATCHER MODEL                                  //
    // ================================================================================ //
	
    PatcherModel::~PatcherModel()
    {
        m_objects.clear();
        m_links.clear();
        m_free_ids.clear();
    }
    
    void PatcherModel::init(Dico& dico)
    {
        addAttr(&m_bgcolor, Tags::unlocked_bgcolor, "Unlocked Background Color", "Appearance", FlipRGBA{0., 0., 0., 1.});
        addAttr(&m_gridsize, Tags::gridsize, "Grid Size", "Editing", 20);
        addAttr(&m_attr_bool, Tag::create("attr_bool"), "Test flip bool", "test", false);
        addAttr(&m_attr_tag, Tag::create("attr_tag"), "Test flip tag", "test", Tag::create("test tag"));
        
        auto it = dico.find(Tags::patcher);
        if(it != dico.end())
        {
            add(it->second);
        }
    }
    
    void PatcherModel::declare()
    {
        Model::declare<PatcherModel>()
        .name("cicm.kiwi.PatcherModel")
        .inherit<AttributeBase::Manager>()
        .member<flip::Array<ObjectModel>,   &PatcherModel::m_objects>   ("objects")
        .member<flip::Array<LinkModel>,     &PatcherModel::m_links>     ("links")
        .member<Attribute<FlipRGBA>,        &PatcherModel::m_bgcolor>   ("bgcolor")
        .member<Attribute<flip::Int>,       &PatcherModel::m_gridsize>  ("gridsize")
        .member<Attribute<flip::Bool>,      &PatcherModel::m_attr_bool> ("attr_bool")
        .member<Attribute<FlipTag>,         &PatcherModel::m_attr_tag>  ("attr_tag")
        .member<FlipAtom,                   &PatcherModel::m_atom>      ("atom");
    }
    
    void PatcherModel::createObject(Dico& dico)
    {
        if(dico.count(Tags::name))
        {
            sTag objectName = dico[Tags::name];
            if(FactoryModel::has(objectName))
            {
                const auto infos = Infos(ulong(dico[Tags::id]),
                                         sTag(dico[Tags::name]),
                                         sTag(dico[Tags::text])->getName(),
                                         dico, dico[Tags::arguments]);
                
                const ObjectModel* object = FactoryModel::create(objectName, infos);
                
                if(object)
                {
                    m_objects.insert(m_objects.end(), *object);
                }
            }
        }
    }
    
    void PatcherModel::add(Dico const& dico)
    {
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
                objdico[Tags::id] = (long)n_id;
                
                createObject(objdico);
            }
        }
    }
    
    void PatcherModel::remove(ObjectModel* object)
    {
        if(object)
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            
            auto predicate = [object](const ObjectModel& obj) {
                return &obj == object;
            };
            
            auto it = find_if(m_objects.begin(), m_objects.end(), predicate);
            if(it != m_objects.end())
            {
                //m_listeners.call(&Listener::objectRemoved, getShared(), object);
                m_objects.erase(it);
                m_free_ids.push_back(object->getId());
            }
        }
    }
}




