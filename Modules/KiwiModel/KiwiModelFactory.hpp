/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#ifndef KIWI_MODEL_FACTORY_HPP_INCLUDED
#define KIWI_MODEL_FACTORY_HPP_INCLUDED

#include <KiwiCore/KiwiCoreConsole.hpp>

#include "KiwiModelObject.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                   OBJECT FACTORY                                 //
        // ================================================================================ //
        
        //! @brief The model::Object factory
        class Factory
        {
        public:
            
            //! @brief Register an object model into the Factory.
            //! @details This function adds a new object model to the factory.
            //! If the name of the object already exists, the function do nothing,
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
                
                if(!name.empty())
                {
                    auto& creators = getCreators();
                    
                    if(creators.find(name) == creators.end())
                    {
                        creators[name] = [name](std::vector<Atom> const& args) -> TModel*
                        {
                            return new TModel(name, args);
                        };
                    }
                    else
                    {
                        Console::error("The object " + name + " already exist !");
                    }
                }
            }
            
            //! @brief Creates a new Object with a text.
            //! @param text The text of the Object.
            //! @example "plus 42" or "print zozo", or just "plus".
            //! @return An object (if the name matches a registered Object name).
            static std::unique_ptr<model::Object> create(std::string const& text);
            
            //! @brief Returns true if a given string match a registered Object model name.
            //! @param name The name of the object model.
            //! @return true if the object has been registered, otherwise false.
            static bool has(std::string const& name);
            
            //! @brief Returns the names of the object that has been added to the Factory.
            //! @return A vector of Object names.
            static std::vector<std::string> getNames();
            
        private:
            
            Factory() = delete;
            ~Factory() = delete;
            
            using ctor_fn_t = std::function<model::Object*(std::vector<Atom>)>;
            using creator_map_t = std::map<std::string, ctor_fn_t>;
            
            //! @internal Returns the static map of creators.
            static creator_map_t& getCreators();
        };        
    }
}

#endif // KIWI_MODEL_FACTORY_HPP_INCLUDED
