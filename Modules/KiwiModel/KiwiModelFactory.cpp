/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiModelFactory.hpp"
#include "KiwiModelObjects.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      FACTORY                                     //
        // ================================================================================ //
        
        std::unique_ptr<model::Object> Factory::create(std::string const& text)
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
                    
                    const ctor_fn_t& model_ctor = it->second;
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
            
            assert(true && "typed object creation fail"); // should never appear
            return nullptr;
        }
        
        bool Factory::has(std::string const& name)
        {
            const auto& creators = getCreators();
            return (creators.find(name) != creators.end());
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
    }
}
