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

#include "KiwiEngineDef.hpp"
#include <KiwiCore/KiwiCoreConsole.hpp>

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
        public:
            
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
                              "The class must not be abstract.");
                
                // The engine Object must be constructible with a model and vector of Atom
                static_assert(std::is_constructible<TEngine, model::Object const&, std::vector<Atom>>::value,
                              "Bad engine object constructor");
                
                assert(!name.empty());
                
                if(!name.empty())
                {
                    auto& creators = getCreators();
                    
                    if(!modelHasObject(name))
                    {
                        Console::error("The model::Object " + name + " counterpart does not exist");
                        return;
                    }
                    
                    if(creators.find(name) == creators.end())
                    {
                        creators[name] = [](model::Object const& model, std::vector<Atom> const& args) -> TEngine*
                        {
                            return new TEngine(model, args);
                        };
                    }
                    else
                    {
                        Console::error("The Object " + name + " already exists");
                    }
                }
            }
            
            //! @brief Creates a new engine Object.
            //! @param model The model::Object.
            //! @return An engine object.
            static std::unique_ptr<Object> create(model::Object const& model);
            
            //! @brief Returns true if a given string match a registered Object name.
            //! @param name The name of the object engine to find.
            //! @return true if the object has been added, otherwise false.
            static bool has(std::string const& name);
            
            //! @brief Gets the names of the objects that has been added to the Factory.
            //! @return A vector of Object names.
            static std::vector<std::string> getNames();
            
        private:
            
            static bool modelHasObject(std::string const& name);
            
            Factory() = delete;
            ~Factory() = delete;
            
            using ctor_fn_t = std::function<Object*(model::Object const& model, std::vector<Atom>)>;
            using creator_map_t = std::map<std::string, ctor_fn_t>;
            
            //! @internal Returns the static map of creators.
            static creator_map_t& getCreators();
        };
    }
}

#endif // KIWI_ENGINE_FACTORY_HPP_INCLUDED
