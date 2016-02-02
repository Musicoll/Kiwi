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

#include "KiwiInstance.h"

namespace kiwi
{
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //
    
    bool Instance::m_declared_flag = false;
    
    Instance::Instance(uint64_t user_id, sTag name) noexcept : m_user_id(user_id), m_name(name)
    {
		;
    }
    
    Instance::~Instance()
    {
        lock_guard<mutex> guard(m_patchers_mutex);
        m_patchers.clear();
        m_listeners.clear();
    }
    
    unique_ptr<Instance> Instance::create(uint64_t user_id, string const& name)
    {
        declare();
        sTag tagname = Tag::create(name);
        
        //@todo assert unique instance's name
        
        return unique_ptr<Instance>(new Instance(user_id, tagname));
    }
    
    void Instance::declare()
    {
        if(m_declared_flag)
            return;
        
        // define current model version :
        Model::version("1.0_dev");
        
        // basic types declaration :
        FlipTag::declare();
        FlipRGBA::declare();
        FlipAtom::declare();
        
        // attributes declaration :
        AttributeBase::declare();
        Attribute<flip::Bool>::declare("cicm.kiwi.Attribute.Bool");
        Attribute<flip::Int>::declare("cicm.kiwi.Attribute.Int");
        Attribute<flip::Float>::declare("cicm.kiwi.Attribute.Float");
        Attribute<FlipTag>::declare("cicm.kiwi.Attribute.Tag");
        Attribute<FlipRGBA>::declare("cicm.kiwi.Attribute.RGBA");
        
        FlipArray<FlipTag>::declare();
        Attribute<FlipArray<FlipTag>>::declare("cicm.kiwi.Attribute.TagArray");
        
        // patcher elements declaration :
        AttributeBase::Manager::declare();
        
        ObjectModel::declare();
        LinkModel::declare();
        PatcherModel::declare();
        
        m_declared_flag = true;
    }
    
    sPatcher Instance::createPatcher()
    {
        auto success = false;
        sPatcher patcher = Patcher::create(this);
        
        if(patcher)
        {
            lock_guard<mutex> guard(m_patchers_mutex);
            success = m_patchers.insert(patcher).second;
        }
        
        if(success)
        {
            m_listeners.call(&Listener::patcherCreated, this, patcher);
        }
        
        return patcher;
    }

    sPatcher Instance::createPatcher(Dico& dico)
    {
        sPatcher patcher;
        /*
        bool state(false);
        {
            patcher = Patcher::create(getShared());
            lock_guard<mutex> guard(m_patchers_mutex);
            if(patcher && m_patchers.insert(patcher).second)
            {
                state = true;
            }
        }
        if(state)
        {
            m_listeners.call(&Listener::patcherCreated, getShared(), patcher);
        }
        */
        return patcher;
    }
    
    void Instance::removePatcher(sPatcher patcher)
    {
        bool success = false;
        {
            lock_guard<mutex> guard(m_patchers_mutex);
            //success = m_patchers.erase(patcher);
        }
        if(success)
        {
            m_listeners.call(&Listener::patcherRemoved, this, patcher);
        }
    }
    
    vector<sPatcher> Instance::getPatchers()
    {
        lock_guard<mutex> guard(m_patchers_mutex);
        //return vector<sPatcher>(m_patchers.begin(), m_patchers.end());
    }
    
    void Instance::addListener(sListener listener)
    {
        m_listeners.add(listener);
    }
    
    void Instance::removeListener(sListener listener)
    {
        m_listeners.remove(listener);
    }
}

