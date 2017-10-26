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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Basic/KiwiModel_ErrorBox.h>

#include "KiwiModel_Factory.h"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      FACTORY                                     //
        // ================================================================================ //
        
        std::unique_ptr<model::Object> Factory::create(std::vector<tool::Atom> const& atoms)
        {
            std::unique_ptr<model::Object> object;
            
            auto * object_class = !atoms.empty() ? getClassByName(atoms[0].getString()) : getClassByName("newbox");
            
            if (object_class == nullptr)
            {
                object_class = getClassByName("errorbox");
                
                object = object_class->create(std::vector<tool::Atom>());
                dynamic_cast<ErrorBox*>(object.get())->setError("object \"" + atoms[0].getString() + "\" not found");
            }
            else
            {
                try
                {
                    std::vector<tool::Atom> args(atoms.empty() ? atoms.begin() : atoms.begin() + 1, atoms.end());
                    object = object_class->create(args);
                }
                catch(std::runtime_error & e)
                {
                    object_class = getClassByName("errorbox");
                    
                    object = object_class->create(std::vector<tool::Atom>());
                    dynamic_cast<ErrorBox*>(object.get())->setError(e.what());
                }
            }
            
            object->m_name = object_class->getName();
            object->m_text = tool::AtomHelper::toString(atoms);
            
            return object;
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
            std::string model_name = "cicm.kiwi.object.";
            
            static const std::string valid_chars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_.");
            
            for(auto const& c : name)
            {
                if(c == '~')
                {
                    model_name.append("_tilde");
                    continue;
                }
                
                model_name += (valid_chars.find(c) != std::string::npos) ? c : '_';
            }
            
            return model_name;
        }
        
        auto Factory::getClasses() -> object_classes_t&
        {
            static object_classes_t static_object_classes;
            return static_object_classes;
        }
    }
}
