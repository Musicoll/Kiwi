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

#ifndef KIWI_MODEL_FACTORY_HPP_INCLUDED
#define KIWI_MODEL_FACTORY_HPP_INCLUDED

#include "KiwiModel_Object.hpp"

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
            //! - constructible with flip::Default.
            //! - constructible with a string and a vector of atom.
            template<typename TModel> struct isValidObject
            {
                enum
                {
                    value = std::is_base_of<model::Object, TModel>::value
                    && !std::is_abstract<TModel>::value
                    && std::is_constructible<TModel, std::string, std::vector<Atom>>::value
                    && std::is_constructible<TModel, flip::Default &>::value
                };
            };
            
            //! @brief Adds an object model into the Factory.
            //! @details This function adds a new object model to the factory.
            //! If the name of the object already exists, the function does nothing,
            //! otherwise the object is added to the factory.
            //! @param name The name of the object.
            template<class TModel>
            static void add(std::string const& name,
                            std::set<std::string> aliases = {}, bool internal = false)
            {
                static_assert(isValidObject<TModel>::value, "Not a valid Object");
                
                if(name.empty())
                    return; // abort
                    
                auto& creators = getCreators();
                
                // check if the name match the name of another object in the factory.
                if(creators.count(name) != 0)
                {
                    throw std::runtime_error("The \"" + name + "\" object is already in the factory");
                }
                
                // check if an alias name match the name of another object in the factory.
                if(!aliases.empty())
                {
                    for(const auto& alias : aliases)
                    {
                        if(creators.count(alias) != 0)
                        {
                            throw std::runtime_error("Duplicate name alias : \"" + alias + "\"");
                        }
                    }
                }
                
                const auto infos_sptr = std::make_shared<const object_infos>(name,
                                                                             getCtor<TModel>(name),
                                                                             getMoldMaker<TModel>(),
                                                                             getMoldCaster<TModel>(),
                                                                             internal);
                
                // store infos for class name
                creators.emplace(std::make_pair(name, infos_sptr));
                
                // store infos for aliases
                for(const auto& alias : aliases)
                {
                    creators.emplace(std::make_pair(alias, infos_sptr));
                }
            }
            
            //! @brief Creates a new Object with a text.
            //! @param name The name of the Object.
            //! @return An object (if the name matches a registered Object name).
            static std::unique_ptr<model::Object> create(std::string const& name,
                                                         std::vector<Atom> const& args);
            
            //! @brief Creates a new Object with from a flip::Mold.
            //! @param name The name of the Object to create.
            //! @return An object (if the name matches a registered Object name).
            static std::unique_ptr<model::Object> create(std::string const& name,
                                                         flip::Mold const& mold);
            
            //! @brief Make a mold of this object.
            static void copy(model::Object const& object, flip::Mold& mold);
            
            //! @brief Returns true if a given string match a registered Object model name.
            //! @param name The name of the object model to find.
            //! @return true if the object has been added, otherwise false.
            static bool has(std::string const& name);
            
            //! @brief Gets the names of the objects that has been added to the Factory.
            //! @param ignore_aliases Default false, you may pass true to exclude them.
            //! @param ignore_internals Default true, you may pass false to include them.
            //! @return A vector of Object names.
            static std::vector<std::string> getNames(const bool ignore_aliases = false,
                                                     const bool ignore_internals = true);
            
        private: // methods
            
            using ctor_fn_t = std::function<std::unique_ptr<model::Object>(std::vector<Atom>)>;
            using mold_maker_fn_t = std::function<void(model::Object const&, flip::Mold& mold)>;
            using mold_caster_fn_t = std::function<std::unique_ptr<model::Object>(flip::Mold const& mold)>;
            
            //! @internal Returns a constructor function.
            template<class TModel>
            static ctor_fn_t getCtor(std::string const& name)
            {
                return [name](std::vector<Atom> const& args) -> std::unique_ptr<model::Object>
                {
                    return std::make_unique<TModel>(name, args);
                };
            }
            
            //! @internal Returns a mold maker function.
            template<class TModel>
            static mold_maker_fn_t getMoldMaker()
            {
                return [](model::Object const& object, flip::Mold& mold) -> void
                {
                    // make a mold with container_flag active
                    mold.make(static_cast<TModel const&>(object), false);
                };
            }
            
            //! @internal Returns a mold caster function.
            template<class TModel>
            static mold_caster_fn_t getMoldCaster()
            {
                return [](flip::Mold const& mold)
                {
                    flip::Default d;
                    auto object_uptr = std::make_unique<TModel>(d);
                    mold.cast<TModel>(static_cast<TModel&>(*(object_uptr.get())));
                    return object_uptr;
                };
            }
            
            struct object_infos
            {
                object_infos(const std::string class_name_,
                             const ctor_fn_t ctor_,
                             const mold_maker_fn_t mold_maker_,
                             const mold_caster_fn_t  mold_caster_,
                             const bool internal_)
                :
                class_name(class_name_),
                ctor(ctor_),
                mold_maker(mold_maker_),
                mold_caster(mold_caster_),
                internal(internal_) {}
                
                const std::string       class_name {};
                const ctor_fn_t         ctor {};
                const mold_maker_fn_t   mold_maker {};
                const mold_caster_fn_t  mold_caster {};
                const bool internal = true;
            };
            
            using creator_map_t = std::map<std::string, const std::shared_ptr<const object_infos>>;
            
            //! @internal Returns the static map of creators.
            static creator_map_t& getCreators();
            
        private: // deleted methods
            
            Factory() = delete;
            ~Factory() = delete;
        };
    }
}

#endif // KIWI_MODEL_FACTORY_HPP_INCLUDED
