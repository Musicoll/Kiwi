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
#include "KiwiInstance.hpp"

namespace kiwi
{
    namespace controller
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
                m_document = std::make_shared<flip::Document>(model::Model::use(), *this, m_instance->getUserId(), 'cicm', 'kpat');
                
                // Set up an history for this document
                m_history = std::make_shared<flip::History<flip::HistoryStoreMemory>>(*m_document.get());
                
                // Get our document's root model
                m_model = &m_document->root<model::Patcher>();
                
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
        
        void Patcher::document_changed(model::Patcher& patcher)
        {
            std::cout << "Patcher : document_changed fn" << '\n';
            
            if(patcher.changed())
            {
                if(patcher.objectsChanged())
                {
                    for(auto const& obj : patcher.getObjects())
                    {
                        if(obj.added())
                        {
                            //createControllerForModel(obj);
                        }
                        else if(obj.removed())
                        {
                            //removeControllerForModel(obj);
                        }
                    }
                }
            }
        }
        
        void Patcher::addObject(std::string const& name, std::string const& text)
        {
            std::unique_ptr<model::Object> uptr_object = nullptr;
            
            if(name == "plus" || name == "+")
            {
                uptr_object = std::unique_ptr<model::ObjectPlus>(new model::ObjectPlus(name, text));
            }
            else if(name == "print")
            {
                //uptr_object = std::unique_ptr<ObjectPrint>(new ObjectPrint(name, text));
            }
            
            m_model->addObject(std::move(uptr_object));
        }
        
        void Patcher::remove(sObject object)
        {
            ;
        }
    }
}
