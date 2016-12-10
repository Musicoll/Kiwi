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
            auto const* class_ptr = getClassByName(name);
            if(class_ptr != nullptr)
            {
                auto object_uptr = class_ptr->create(args);
                object_uptr->m_name = class_ptr->getName();
                object_uptr->m_text = args.empty() ? name : name + " " + AtomHelper::toString(args);
                return object_uptr;
            }
            else
            {
                throw std::runtime_error("Factory can't create object");
            }
        }
        
        std::unique_ptr<model::Object> Factory::create(std::string const& name, flip::Mold const& mold)
        {
            auto const* class_ptr = getClassByName(name);
            if(class_ptr != nullptr)
            {
                return class_ptr->moldCast(mold);
            }
            else
            {
                throw std::runtime_error("Factory can't create object " + name);
            }
        }
        
        void Factory::copy(model::Object const& object, flip::Mold& mold)
        {
            const auto name = object.getName();
            auto const* class_ptr = getClassByName(name);
            if(class_ptr != nullptr)
            {
                class_ptr->moldMake(object, mold);
            }
            else
            {
                throw std::runtime_error("can't copy object " + name);
            }
        }
        
        bool Factory::has(std::string const& name)
        {
            const auto& object_classes = getClasses();
            for(const auto& object_class : object_classes)
            {
                if(object_class->getName() == name || object_class->hasAlias(name))
                    return true;
            }
            
            return false;
        }
        
        Factory::ObjectClassBase* Factory::getClassByName(std::string const& name,
                                                          const bool ignore_aliases)
        {
            const auto& object_classes = getClasses();
            for(const auto& object_class : object_classes)
            {
                if(object_class->getName() == name || (!ignore_aliases && object_class->hasAlias(name)))
                    return object_class.get();
            }
            
            return nullptr;
        }
        
        std::vector<std::string> Factory::getNames(const bool ignore_aliases, const bool ignore_internals)
        {
            auto const& object_classes = getClasses();
            std::vector<std::string> names;
            names.reserve(object_classes.size());
            
            for(const auto& object_class : object_classes)
            {
                if(!object_class->isInternal() || !ignore_internals)
                {
                    names.emplace_back(object_class->getName());
                    
                    if(!ignore_aliases && object_class->hasAlias())
                    {
                        const auto aliases = object_class->getAliases();
                        names.insert(names.end(), aliases.begin(), aliases.end());
                    }
                }
            }
            
            return names;
        }
        
        std::string Factory::sanitizeName(std::string const& name)
        {
            std::string pretty;
            pretty.reserve(name.size());
            
            static const std::string valid_chars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_.");
            
            for(auto const& c : name)
            {
                if(c == '~')
                {
                    pretty.append("_tilde");
                    continue;
                }
                
                pretty += (valid_chars.find(c) != std::string::npos) ? c : '_';
            }
            
            return name;
        }
        
        auto Factory::getClasses() -> object_classes_t&
        {
            static object_classes_t static_object_classes;
            return static_object_classes;
        }
    }
}
