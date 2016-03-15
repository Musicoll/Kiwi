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

#include "KiwiPatcher.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      FACTORY                                     //
        // ================================================================================ //
        
        //! @brief The Object's factory
        class Factory
        {
        public:
            
            using creator_function_t = std::function<Object*(std::string)>;
            using creator_map_t = std::map<std::string, creator_function_t>;
            
            //! @brief Add an object to the Factory.
            //! @details This function adds a new object to the factory. If the name of the object already exists,
            //! the function doesn't do anything otherwise the object is added to the factory.
            //! @param name An alias name of the object or nothing if you want yo use the default object name.
            template <class T>
            static void add(std::string name)
            {
                static_assert(std::is_base_of<model::Object, T>::value, "The class must inherit from object.");
                static_assert(!std::is_abstract<T>::value, "The class must not be abstract.");
                
                if(!name.empty())
                {
                    std::lock_guard<std::mutex> guard(getMutex());
                    
                    creator_map_t& creators = getCreators();

                    if(creators.find(name) != creators.end())
                    {
                        //Console::error("The object " + name->getName() + " already exist !");
                    }
                    else
                    {
                        creator_function_t func = [name](std::string text) -> Object*
                        {
                            return new T(name, text);
                        };
                        
                        creators[name] = func;
                    }
                }
            }
            
            //! @brief Creates a new Object.
            //! @param name The name of the Object.
            //! @param text The text of the Object.
            //! @return An object (if the name matches a registered Object name).
            static std::unique_ptr<Object> create(std::string const& name, std::string const& text);
            
            //! @brief Returns true if a given string match a registered Object name.
            //! @param name The name of the object.
            //! @return true if the object has been registered, otherwise false.
            static bool has(std::string const& name);
            
            //! @brief Removes an object from the factory.
            //! @param name The name of the object to be removed.
            static void remove(std::string const& name);
            
            //! @brief Returns all the registered Object names.
            //! @return A vector of Object names.
            static std::vector<std::string> getNames();
            
        private:
            
            //! @brief Returns the static map of creators.
            //! @return The static map of creators.
            static creator_map_t& getCreators() noexcept
            {
                static creator_map_t static_creators;
                return static_creators;
            }
            
            //! @brief Returns the static mutex.
            //! @return The static mutex.
            static inline std::mutex& getMutex() noexcept
            {
                static std::mutex static_mutex;
                return static_mutex;
            }
        };
    }
}


#endif // KIWI_MODEL_FACTORY_HPP_INCLUDED
