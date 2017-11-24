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

#include <cassert>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_ErrorBox.h>

#include "KiwiModel_Factory.h"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      FACTORY                                     //
        // ================================================================================ //
        
        std::vector<std::unique_ptr<ObjectClass>> Factory::m_object_classes;
        
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
                catch(model::Object::Error & e)
                {
                    object_class = getClassByName("errorbox");
                    
                    object = object_class->create(std::vector<tool::Atom>());
                    dynamic_cast<ErrorBox*>(object.get())->setError(e.what());
                }
            }
            
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
            for(const auto& object_class : m_object_classes)
            {
                if(object_class->getName() == name || object_class->hasAlias(name))
                    return true;
            }
            
            return false;
        }
        
        ObjectClass const* Factory::getClassByName(std::string const& name,
                                             const bool ignore_aliases)
        {
            for(const auto& object_class : m_object_classes)
            {
                if(object_class->getName() == name || (!ignore_aliases && object_class->hasAlias(name)))
                    return object_class.get();
            }
            
            return nullptr;
        }
        
        ObjectClass const* Factory::getClassByTypeId(size_t type_id)
        {
            auto found_class = std::find_if(m_object_classes.begin(),
                                            m_object_classes.end(),
                                            [type_id](std::unique_ptr<ObjectClass> const& object_class)
            {
                return object_class->m_type_id == type_id;
            });
            
            return found_class != m_object_classes.end() ? found_class->get() : nullptr;
        }
        
        std::vector<std::string> Factory::getNames(const bool ignore_aliases, const bool ignore_internals)
        {
            std::vector<std::string> names;
            names.reserve(m_object_classes.size());
            
            for(const auto& object_class : m_object_classes)
            {
                if(!object_class->hasFlag(ObjectClass::Flag::Internal) || !ignore_internals)
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
        
        std::string Factory::toModelName(std::string const& name)
        {
            std::string model_name = "cicm.kiwi.object.";
            
            for (char const& c : name)
            {
                assert(c != '\\' && "using antislash character in object name");
                
                if (uint8_t (c) < 0x20
                    || uint8_t (c) == 0x20
                    || uint8_t (c) == 0x7f
                    || uint8_t (c) >= 0x80)
                {
                    int i = (int) c;
                    
                    std::string int_string = "\x5C" + std::to_string(i) + "\x5C";
                    
                    model_name.append(int_string);
                }
                else
                {
                    model_name.push_back(c);
                }
            }
            
            return model_name;
        }
        
        std::string Factory::toKiwiName(std::string const& model_name)
        {
            std::string prefix = "cicm.kiwi.object.";
            
            assert(model_name.find(prefix) == 0);
            
            std::string kiwi_name = model_name;
            
            kiwi_name.erase(0, prefix.size());
            
            size_t slash_pos = kiwi_name.find('\\');
            
            while(slash_pos != std::string::npos)
            {
                size_t next_slash = kiwi_name.find('\\', slash_pos + 1);
                
                std::string int_string = kiwi_name.substr(slash_pos + 1, next_slash - slash_pos);
                
                char c = (char) std::stoi(int_string);
                
                kiwi_name.replace(slash_pos, next_slash - slash_pos + 1, 1, c);
                
                slash_pos = kiwi_name.find('\\');
            }
            
            return kiwi_name;
        }
    }
}
