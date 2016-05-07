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
#include "Objects/KiwiObjects.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      INSTANCE                                    //
        // ================================================================================ //
        
        Instance::Instance(uint64_t user_id, std::string const& name) noexcept :
        m_user_id(user_id),
        m_name(name)
        {
            model::Model::init("v0.0.1");
            registerObjects();
        }
        
        Instance::~Instance()
        {
            ;
        }
        
        void Instance::document_changed(model::Patcher& patcher)
        {
            if(patcher.added())
            {
                patcher.entity().emplace<Patcher>(*this);
            }
            
            patcher.entity().use<Patcher>().document_changed(patcher);
            
            if(patcher.removed())
            {
                patcher.entity().erase<Patcher>();
            }
        }
    }
}
