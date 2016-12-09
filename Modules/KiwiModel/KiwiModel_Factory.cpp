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

#include "KiwiModel_Factory.hpp"
#include "KiwiModel_Objects.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      FACTORY                                     //
        // ================================================================================ //
        
        std::unique_ptr<model::Object> Factory::create(std::string const& name,
                                                       std::vector<Atom> const& args)
        {
            const auto& creators = getCreators();
            const auto it = creators.find(name);
            if(it != creators.end())
            {
                auto object_uptr = it->second->ctor(args);
                object_uptr->m_name = it->second->class_name;
                object_uptr->m_text = args.empty() ? name : name + " " + AtomHelper::toString(args);
                return object_uptr;
            }
            else
            {
                throw std::runtime_error("Factory can't create object");
            }
        }
        
        void Factory::copy(model::Object const& object, flip::Mold& mold)
        {
            const auto& creators = getCreators();
            const auto object_name = object.getName();
            const auto it = creators.find(object_name);
            
            if(it != creators.cend())
            {
                it->second->mold_maker(object, mold);
            }
            else
            {
                throw std::runtime_error("can't copy object " + object_name);
            }
        }
        
        std::unique_ptr<model::Object> Factory::create(std::string const& name, flip::Mold const& mold)
        {
            const auto& creators = getCreators();
            const auto it = creators.find(name);
            if(it != creators.end())
            {
                const mold_caster_fn_t& mold_caster = it->second->mold_caster;
                return mold_caster(mold);
            }
            else
            {
                throw std::runtime_error("Factory can't create object");
            }
        }
        
        bool Factory::has(std::string const& name)
        {
            const auto& creators = getCreators();
            return (creators.find(name) != creators.cend());
        }
        
        std::vector<std::string> Factory::getNames(const bool ignore_aliases, const bool ignore_internals)
        {
            const auto& creators = getCreators();
            std::vector<std::string> names;
            names.reserve(creators.size());
            
            for(const auto& creator : creators)
            {
                if((!creator.second->internal || !ignore_internals) &&
                   (!ignore_aliases || (creator.second->class_name == creator.first)))
                {
                    names.emplace_back(creator.first);
                }
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
