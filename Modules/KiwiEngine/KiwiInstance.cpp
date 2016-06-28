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

#include <KiwiModel/KiwiPatcherModel.hpp>

#include "KiwiEngineFactory.hpp"
#include "KiwiTypedObjects.hpp"
#include "KiwiInstance.hpp"
#include "KiwiDocumentManager.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                           PATCHER MODEL DECLARATOR                               //
        // ================================================================================ //
        
        //! @brief The Patcher Model class declarator
        class Instance::PatcherModelDeclarator : public model::PatcherModel
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
        
        Instance::Instance(uint64_t user_id, std::unique_ptr<GuiDevice> gui_device) :
        m_user_id(user_id),
        m_gui_device(std::move(gui_device))
        {
            PatcherModelDeclarator model;
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
        
        std::unique_ptr<Timer> Instance::createTimer(std::function<void()> callback) const
        {
            if(m_gui_device)
            {
                return std::unique_ptr<Timer>(new Timer(m_gui_device->createTimer(callback)));
            }
            
            return nullptr;
        }
    }
}
