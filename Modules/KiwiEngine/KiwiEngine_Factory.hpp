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

#ifndef KIWI_ENGINE_FACTORY_HPP_INCLUDED
#define KIWI_ENGINE_FACTORY_HPP_INCLUDED

#include "KiwiEngine_Def.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                   OBJECT FACTORY                                 //
        // ================================================================================ //
        
        //! @brief The engine Object's factory
        class Factory
        {
        public: // methods
            
            //! @brief Adds an object engine into the Factory.
            //! @details This function adds a new object engine to the factory.
            //! If the name of the object already exists, the function does nothing,
            //! otherwise the object is added to the factory.
            //! @param name The name of the object.
            template <class TEngine,
            typename std::enable_if<std::is_base_of<Object, TEngine>::value,
            Object>::type* = nullptr>
            static void add(std::string const& name)
            {
                static_assert(!std::is_abstract<TEngine>::value,
                              "The engine object must not be abstract.");
            
                static_assert(std::is_constructible<TEngine,
                              model::Object const&, Patcher&, std::vector<Atom> const&>::value,
                              "The engine object must have a valid constructor.");
                
                assert(!name.empty());
                assert(modelHasObject(name) && "The model counterpart does not exist");
                
                auto& creators = getCreators();
                assert(creators.count(name) == 0 && "The object already exists");
                
                creators[name] = [](model::Object const& model,
                                    Patcher& patcher,
                                    std::vector<Atom> const& args) -> TEngine*
                {
                    return new TEngine(model, patcher, args);
                };
            }
            
            //! @brief Creates a new engine Object.
            //! @param model The model::Object.
            //! @return An engine object.
            static std::unique_ptr<Object> create(Patcher& patcher, model::Object const& model);
            
            //! @brief Returns true if a given string match a registered Object name.
            //! @param name The name of the object engine to find.
            //! @return true if the object has been added, otherwise false.
            static bool has(std::string const& name);
            
            //! @brief Gets the names of the objects that has been added to the Factory.
            //! @return A vector of Object names.
            static std::vector<std::string> getNames();
            
        private: // methods
            
            static bool modelHasObject(std::string const& name);
            
            using ctor_fn_t = std::function<Object*(model::Object const& model,
                                                    Patcher& patcher,
                                                    std::vector<Atom> const&)>;
            
            using creator_map_t = std::map<std::string, ctor_fn_t>;
        
            //! @internal Returns the static map of creators.
            static creator_map_t& getCreators();
            
        private: // deleted methods
            
            Factory() = delete;
            ~Factory() = delete;
        };
    }
}

#endif // KIWI_ENGINE_FACTORY_HPP_INCLUDED
