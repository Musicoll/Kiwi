/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiModel/KiwiModel_Factory.h>

#include "KiwiEngine_Factory.h"
#include "KiwiEngine_Objects.h"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      FACTORY                                     //
        // ================================================================================ //
        
        std::unique_ptr<Object> Factory::create(Patcher& patcher, model::Object const& model)
        {
            std::vector<Atom> atoms = AtomHelper::parse(model.getText());
            assert(!atoms.empty() && "The model object isn't valid.");
            
            auto& creators = getCreators();
            assert(creators.count(model.getName()) != 0 && "The object has not been registered.");
            return std::unique_ptr<Object>(creators[model.getName()](model, patcher, std::vector<Atom>(atoms.begin() + 1, atoms.end())));
        }
        
        bool Factory::has(std::string const& name)
        {
            return static_cast<bool>(getCreators().count(name));
        }
        
        auto Factory::getCreators() -> creator_map_t&
        {
            static creator_map_t static_creators;
            return static_creators;
        }
        
        bool Factory::modelHasObject(std::string const& name)
        {
            return model::Factory::has(name);
        }
    }
}
