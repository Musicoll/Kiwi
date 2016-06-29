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

#include <KiwiModel/KiwiModelFactory.hpp>

#include "KiwiEngineFactory.hpp"
#include "KiwiEngineObjects.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      FACTORY                                     //
        // ================================================================================ //
        
        std::unique_ptr<Object> Factory::create(model::Object const& model)
        {
            const std::string object_name = model.getName();
            std::vector<Atom> atoms = AtomHelper::parse(model.getText());
            
            if(atoms.size() > 0)
            {
                //const std::string name = atoms[0].getString();
                
                const auto& creators = getCreators();
                const auto it = creators.find(object_name);
                if(it != creators.end())
                {
                    std::vector<Atom> args {atoms.begin() + 1, atoms.end()};
                    
                    const auto& engine_ctor = it->second;
                    
                    if(!engine_ctor)
                    {
                        assert(false && "the engine object has not been registered");
                    }
                    
                    return std::unique_ptr<Object>(engine_ctor(model, args));
                }
            }
            
            return nullptr;
        }
        
        bool Factory::has(std::string const& name)
        {
            const auto& creators = getCreators();
            const auto it = creators.find(name);
            if(it != creators.end())
            {
                return (it->second != nullptr);
            }
            
            return false;
        }
        
        std::vector<std::string> Factory::getNames()
        {
            std::vector<std::string> names;
            for(const auto& creator : getCreators())
            {
                names.emplace_back(creator.first);
            }
            
            return names;
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
