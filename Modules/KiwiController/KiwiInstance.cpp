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

#include "KiwiInstance.hpp"

namespace kiwi
{
    namespace controller
    {
        // ================================================================================ //
        //                                      INSTANCE                                    //
        // ================================================================================ //
        
        bool Instance::m_declared_flag = false;
        
        Instance::Instance(uint64_t user_id, std::string const& name) noexcept : m_user_id(user_id), m_name(name)
        {
            ;
        }
        
        Instance::~Instance()
        {
            m_patchers.clear();
        }
        
        std::unique_ptr<Instance> Instance::create(uint64_t user_id, std::string const& name)
        {
            declare();
            
            //@todo assert unique instance's name
            return std::unique_ptr<Instance>(new Instance(user_id, name));
        }
        
        void Instance::declare()
        {
            if(m_declared_flag) return;
            
            // define current model version :
            model::Model::init("v0.0.1");
            
            m_declared_flag = true;
        }
        
        Patcher& Instance::createPatcher()
        {
            auto it = m_patchers.emplace(m_patchers.cend(), Patcher::create(*this));
            return *it->get();
        }
        
        void Instance::removePatcher(Patcher* patcher)
        {
            bool success = false;
            {
                //success = m_patchers.erase(patcher);
            }
        }
        
        auto Instance::getPatchers() -> patcher_container_t const&
        {
            return m_patchers;
        }
    }
}
