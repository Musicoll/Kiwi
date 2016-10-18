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
#include "KiwiModel_DataModel.hpp"

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
            
            //! @brief Adds an object model into the Factory.
            //! @details This function adds a new object model to the factory.
            //! If the name of the object already exists, the function does nothing,
            //! otherwise the object is added to the factory.
            //! @param name The name of the object.
            template <class TModel, typename
            std::enable_if<std::is_base_of<model::Object, TModel>::value,
            model::Object>::type* = nullptr>
            static void add(std::string const& name)
            {
                static_assert(!std::is_abstract<TModel>::value,
                              "The class must not be abstract.");
                
                static_assert(std::is_constructible<TModel, std::string, std::vector<Atom>>::value,
                              "Bad model object constructor");
                
                static_assert(std::is_constructible<TModel, flip::Default &>::value,
                              "Class must be constructible with flip::Default");
                
                if(!name.empty())
                {
                    auto& creators = getCreators();
                    assert(creators.count(name) == 0 && "Object model already added to the factory.");
                    
                    const ctor_fn_t ctor = [name](std::vector<Atom> const& args) -> TModel*
                    {
                        return new TModel(name, args);
                    };
                    
                    const mold_maker_fn_t mold_maker = [](model::Object const& object, flip::Mold& mold)
                    {
                        mold.make(static_cast<TModel const&>(object), true);
                    };
                    
                    const mold_caster_fn_t mold_caster = [](flip::Mold const& mold)
                    {
                        flip::Default d;
                        auto object_uptr = std::unique_ptr<TModel>(new TModel(d));
                        
                        mold.cast<TModel>(static_cast<TModel&>(*(object_uptr.get())));
                        
                        return object_uptr;
                    };
                    
                    object_infos obj_infos { ctor, mold_maker, mold_caster };
                    
                    const auto res = creators.emplace(std::make_pair(name, obj_infos));
                    
                    if(!res.second)
                    {
                        throw std::runtime_error("Adding object to Factory failed");
                    }
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
            //! @return A vector of Object names.
            static std::vector<std::string> getNames();
            
        private: // methods
            
            using ctor_fn_t = std::function<model::Object*(std::vector<Atom>)>;
            using mold_maker_fn_t = std::function<void(model::Object const&, flip::Mold& mold)>;
            using mold_caster_fn_t = std::function<std::unique_ptr<model::Object>(flip::Mold const& mold)>;
            
            struct object_infos
            {
                ctor_fn_t const         ctor;
                mold_maker_fn_t const   mold_maker;
                mold_caster_fn_t const  mold_caster;
            };
            
            using creator_map_t = std::map<std::string, object_infos>;
            
            //! @internal Returns the static map of creators.
            static creator_map_t& getCreators();
            
        private: // deleted methods
            
            Factory() = delete;
            ~Factory() = delete;
        };
    }
}

#endif // KIWI_MODEL_FACTORY_HPP_INCLUDED
