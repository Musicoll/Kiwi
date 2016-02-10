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

#include "KiwiPatcher.h"
#include "KiwiInstance.h"
#include "KiwiFactory.h"

namespace kiwi
{    
    // ================================================================================ //
    //                                      PAGE                                        //
    // ================================================================================ //
    
    //Patcher::Patcher(sInstance instance, PatcherModel& model) noexcept : m_instance(instance), m_model(model)
    Patcher::Patcher(Instance* instance) noexcept : m_instance(instance)
    {
        if(m_instance != nullptr)
        {
            // Set up a document
            m_document = std::make_shared<flip::Document>(Model::use(), *this, m_instance->getUserId(), 'cicm', 'kpat');
            
            // Set up an history for this document
            m_history = std::make_shared<flip::History<flip::HistoryStoreMemory>>(*m_document.get());
            
            // Get our document's root model
            m_model = &m_document->root<PatcherModel>();
            
            Dico dico;
            m_model->init(dico);
            m_document->commit();
        }
    }
	
    Patcher::~Patcher()
    {
        m_objects.clear();
        m_links.clear();
    }
    
    std::unique_ptr<Patcher> Patcher::create(Instance* instance)
    {
        std::unique_ptr<Patcher> patcher(new Patcher(instance));
        return patcher;
    }
    
    void Patcher::document_changed(PatcherModel& patcher)
    {
        std::cout << "Patcher : document_changed fn" << '\n';
        
        //if (patcher.attributeChanged())
        if (true)
        {
            std::cout << "\tpatcher attribute changed" << '\n';
            
            const Atom sigcolor = patcher.getAttributeValue(Tags::sigcolor);
            std::cout << "\t\t- sigcolor : " << sigcolor << '\n';
            
            const Atom color = patcher.getAttributeValue(Tags::unlocked_bgcolor);
            std::cout << "\t\t- unlocked_bgcolor : " << color << '\n';
            
            int64_t gridsize = patcher.getGridSize();
            std::cout << "\t\t- gridSize : " << gridsize << '\n';
            
            auto val = patcher.getAttributeValue(Tag::create("attr_bool"));
            std::cout << "\t\t- attr_bool : " << val << '\n';
            
            auto tag = patcher.getAttributeValue(Tag::create("attr_tag"));
            std::cout << "\t\t- attr_tag : " << tag << '\n';
            
            auto tagArray = patcher.getAttributeValue(Tag::create("attr_tagArray"));
            std::cout << "\t\t- attr_tag_array : " << tagArray << '\n';
            
            std::cout << "\t\t- atom : " << patcher.m_atom << '\n';
            std::cout << "\t\t-- is undefined : " << patcher.m_atom.isUndefined() << '\n';
            std::cout << "\t\t-- is bool : " << patcher.m_atom.isBool() << '\n';
            std::cout << "\t\t-- is number : " << patcher.m_atom.isNumber() << '\n';
            std::cout << "\t\t-- is tag : " << patcher.m_atom.isTag() << '\n';
        }
    }
    
    void Patcher::createObject(Dico& dico)
    {
        if(dico.count(Tags::name))
        {
            sTag objectName = dico[Tags::name];
            if(Factory::has(objectName))
            {
                //@todo no need to cast here
                const auto infos = Infos(static_cast<int64_t>(dico[Tags::id]),
                                         sTag(dico[Tags::name]),
                                         sTag(dico[Tags::text])->getName(),
                                         dico, dico[Tags::arguments]);
                
                const Object* object = Factory::create(objectName, infos);
                
                if(object)
                {
                    //m_objects.insert(m_objects.end(), *object);
                    
                    //m_listeners.call(&Listener::objectCreated, getShared(), object);
                    //object->loaded();
                }
            }
        }
    }
    
    void Patcher::add(Dico const& dico)
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
                /*
                const ulong n_id = m_free_ids.empty() ? getNumberOfObjects() + 1 : m_free_ids[0];
                if(!m_free_ids.empty())
                {
                    m_free_ids.erase(m_free_ids.begin());
                }
                objdico[Tags::id] = (long)n_id;
                
                createObject(objdico);
                */
            }
        }
    }
    
    void Patcher::remove(sObject object)
    {
        ;
    }
}




