/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <functional>

#include <KiwiApp_Patcher/KiwiApp_Factory.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ClassicView.h>

namespace kiwi
{
    std::map<std::string, Factory::ctor_fn_t> Factory::m_creators;
    
    std::unique_ptr<ObjectView> Factory::createObjectView(model::Object & object_model)
    {
        std::string object_name = object_model.getName();
        if (m_creators.find(object_name) != m_creators.end())
        {
            return m_creators[object_name](object_model);
        }
        else
        {
            return std::make_unique<ClassicView>(object_model);
        }

        return std::unique_ptr<ObjectView>();
    }
}
