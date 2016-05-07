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

namespace kiwi
{
    // ================================================================================ //
    //                                      FACTORY                                     //
    // ================================================================================ //
    
    std::unique_ptr<model::Object> ObjectFactory::createModel(std::string const& text)
    {
        std::vector<Atom> atoms = AtomHelper::parse(text);
        
        if(atoms.size() > 0 && atoms[0].isString())
        {
            std::lock_guard<std::mutex> guard(getMutex());
            
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
        }
        
        return nullptr;
    }
    
    bool ObjectFactory::has(std::string const& name)
    {
        std::lock_guard<std::mutex> guard(getMutex());
        const auto& creators = getCreators();
        return (creators.find(name) != creators.end());
    }
    
    void ObjectFactory::remove(std::string const& name)
    {
        std::lock_guard<std::mutex> guard(getMutex());
        getCreators().erase(name);
    }
    
    std::vector<std::string> ObjectFactory::getNames()
    {
        std::lock_guard<std::mutex> guard(getMutex());
        
        std::vector<std::string> names;
        for(const auto& creator : getCreators())
        {
            names.emplace_back(creator.first);
        }
        
        return names;
    }
}
