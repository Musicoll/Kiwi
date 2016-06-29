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

#include <KiwiModel/KiwiModelDataModel.hpp>

#include "KiwiEngineFactory.hpp"
#include "KiwiTypedObjects.hpp"
#include "KiwiEngineInstance.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                           PATCHER MODEL DECLARATOR                               //
        // ================================================================================ //
        
        //! @brief The Patcher Model class declarator
        class Instance::DataModelDeclarator : public model::DataModel
        {
        public:
            
            void endOfModelDeclaration() final override
            {
                engine::Factory::add<NewBox>("newbox");
                engine::Factory::add<NewBox>("errorbox");
                engine::Factory::add<ObjectPlus>("plus");
                engine::Factory::add<ObjectPrint>("print");
            }
        };
        
        // ================================================================================ //
        //                                      INSTANCE                                    //
        // ================================================================================ //
        
        Instance::Instance(uint64_t user_id) :
        m_user_id(user_id)
        {
            DataModelDeclarator model;
            model.init("v0.0.2");
        }
        
        Instance::~Instance()
        {
            ;
        }
        
        uint64_t Instance::getUserId() const noexcept
        {
            return m_user_id;
        }
    }
}
