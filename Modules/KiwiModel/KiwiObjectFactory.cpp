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

#include "KiwiObjectFactory.hpp"
#include "KiwiTypedObjects.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      FACTORY                                     //
    // ================================================================================ //
    
    std::unique_ptr<model::Object> ObjectFactory::createModel(std::string const& text)
    {
        std::vector<Atom> atoms = AtomHelper::parse(text);
        
        if(atoms.size() > 0)
        {
            const std::string name = atoms[0].getString();
            
            const auto& creators = getCreators();
            const auto it = creators.find(name);
            if(it != creators.end())
            {
                std::vector<Atom> args {atoms.begin() + 1, atoms.end()};
                
                const model_ctor_t model_ctor = it->second.model_ctor_fn;
                auto object_uptr = std::unique_ptr<model::Object>(model_ctor(args));
                
                object_uptr->m_name = name;
                
                // parse text again to clean input text
                object_uptr->m_text = AtomHelper::toString(atoms);
                
                return object_uptr;
            }
            else
            {
                Console::error("Object \"" + name + "\" not found");
                
                std::vector<Atom> args {atoms.begin() + 1, atoms.end()};
                
                auto object_uptr = std::unique_ptr<model::Object>(new model::ErrorBox("errorbox", args));
                
                object_uptr->m_name = "errorbox";
                
                // parse text again to clean input text
                object_uptr->m_text = AtomHelper::toString(atoms);
                
                return object_uptr;
            }
        }
        else
        {
            auto object_uptr = std::unique_ptr<model::Object>(new model::NewBox("newbox", {}));
            object_uptr->m_name = "newbox";
            object_uptr->m_text = "";
            
            return object_uptr;
        }
        
        assert(true && "typed object creation fail"); // sould never appear
        return nullptr;
    }
    
    bool ObjectFactory::hasModel(std::string const& name)
    {
        const auto& creators = getCreators();
        return (creators.find(name) != creators.end());
    }
    
    bool ObjectFactory::hasEngine(std::string const& name)
    {
        const auto& creators = getCreators();
        const auto it = creators.find(name);
        if(it != creators.end())
        {
            return (it->second.engine_ctor_fn != nullptr);
        }
        
        return false;
    }
    
    std::vector<std::string> ObjectFactory::getNames()
    {
        std::vector<std::string> names;
        for(const auto& creator : getCreators())
        {
            names.emplace_back(creator.first);
        }
        
        return names;
    }
    
    auto ObjectFactory::getCreators() -> creator_map_t&
    {
        static creator_map_t static_creators;
        return static_creators;
    }
}
