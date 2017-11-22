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

#pragma once

#include <typeinfo>
#include <vector>
#include <memory>
#include <algorithm>

#include <KiwiModel/KiwiModel_ObjectClass.h>
#include <KiwiModel/KiwiModel_Object.h>
#include <KiwiModel/KiwiModel_DataModel.h>

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                   OBJECT FACTORY                                 //
        // ================================================================================ //
        
        //! @brief The model Object's factory
        class Factory
        {
        public: // methods
            
            //! @brief isValidObject type traits
            //! @details To be considered as valid an object must:
            //! - not be abstract.
            //! - inherit from model::Object.
            //! - be constructible with flip::Default.
            //! - be constructible with a string and a vector of atoms.
            template<typename TModel> struct isValidObject
            {
                enum
                {
                    value = std::is_base_of<model::Object, TModel>::value
                    && !std::is_abstract<TModel>::value
                    && std::is_constructible<TModel, flip::Default &>::value
                };
            };
            
            //! @brief Adds an object model into the Factory.
            //! @details The function throw if the object has already been added.
            //! The name you pass in parameter will be used and stored in the DataModel,
            //! thus if you pass a different name later, this will imply a breaking change in the DataModel.
            //! @param name The name of the object (must not be empty and not already used by another object or alias name in the Factory).
            template<class TModel>
            static void add(std::unique_ptr<ObjectClass> object_class,
                            flip::Class<TModel> & data_model)
            {
                static_assert(isValidObject<TModel>::value, "Not a valid Object");
                
                std::string object_name = object_class->getName();
                
                assert(std::string(data_model.name()) == object_class->getModelName());
                assert(!object_name.empty() && "Registring object with invalid name");
                assert(DataModel::has<TModel>() && "Registering object without declaring its data model");
                
                // check if the name match the name of another object in the factory.
                if(has(object_name))
                {
                    throw std::runtime_error("The \"" + object_name + "\" object is already in the factory");
                }
                
                // check aliases duplicates
                std::set<std::string> const& aliases = object_class->getAliases();
                
                auto duplicate_aliases = std::find_if(aliases.begin(),
                                                      aliases.end(),
                                                      [](std::string const& alias)
                {
                    return Factory::has(alias);
                });
                
                if (duplicate_aliases != aliases.end())
                {
                    throw std::runtime_error(object_name + "Adding twice the same alias");
                }
                
                ObjectClass::mold_maker_t mold_maker = [](model::Object const& object, flip::Mold& mold)
                {
                    mold.make(static_cast<TModel const&>(object), false);
                };
                
                object_class->setMoldMaker(mold_maker);
                
                ObjectClass::mold_caster_t mold_caster = [](flip::Mold const& mold)
                {
                    flip::Default d;
                    auto object_uptr = std::make_unique<TModel>(d);
                    mold.cast<TModel>(static_cast<TModel&>(*(object_uptr.get())));
                    return object_uptr;
                };
                
                object_class->setMoldCaster(mold_caster);
                
                object_class->setTypeId(typeid(TModel).hash_code());

                m_object_classes.emplace_back(std::move(object_class));
            }
            
            //! @brief Creates a new Object with a name and arguments.
            //! @details This function will throw if the object name does not exist.
            //! @param name The name of the Object.
            //! @param args A list of arguments as a vector of Atom.
            //! @return A ptr to a model::Object.
            static std::unique_ptr<model::Object> create(std::vector<tool::Atom> const& args);
            
            //! @brief Creates a new Object from a flip::Mold.
            //! @details This function will throw if the object name does not exist.
            //! @param name The name of the Object to create.
            //! @param mold The flip::Mold from which to create the object.
            //! @return A ptr to a model::Object.
            static std::unique_ptr<model::Object> create(std::string const& name,
                                                         flip::Mold const& mold);
            
            //! @brief Make a mold of this object.
            //! @details This function will throw if the object does not exist.
            //! @param object The Object to copy.
            //! @param mold The flip::Mold from which to create the object.
            //! @return A ptr to a model::Object.
            static void copy(model::Object const& object, flip::Mold& mold);
            
            //! @brief Returns a ptr to an object class thas has this name or alias name.
            //! @details This method returns nullptr if the name is not found.
            //! @param name The name or an alias name of the class.
            //! @param ignore_aliases Default to false, pass true to ignore aliases.
            //! @return A ptr to an ObjectClassBase.
            static ObjectClass const* getClassByName(std::string const& name,
                                               const bool ignore_aliases = false);
            
            //! @brief Returns the object's class filtering by type id's hash code.
            static ObjectClass const* getClassByTypeId(size_t type_id);
            
            //! @brief Returns true if a given string match a registered object class name.
            //! @param name The name of the object class to find.
            //! @return true if the object class has been added, otherwise false.
            static bool has(std::string const& name);
            
            //! @brief Gets the names of the objects that has been added to the Factory.
            //! @param ignore_aliases Default to false, you may pass true to exclude them.
            //! @param ignore_internals Default to true, you may pass false to include them.
            //! @return A vector of object class names.
            static std::vector<std::string> getNames(const bool ignore_aliases = false,
                                                     const bool ignore_internals = true);
            
            //! @internal Returns a sanitized version of the string.
            static std::string sanitizeName(std::string const& name);
            
        private: // members
            
            static std::vector<std::unique_ptr<ObjectClass>> m_object_classes;
            
        private: // deleted methods
            
            Factory() = delete;
            ~Factory() = delete;
        };
    }
}
