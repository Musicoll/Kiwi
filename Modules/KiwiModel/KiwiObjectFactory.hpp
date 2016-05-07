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

#include "KiwiObject.hpp"

namespace kiwi
{
    namespace engine
    {
        class Object;
    }
    
    // ================================================================================ //
    //                                      FACTORY                                     //
    // ================================================================================ //
    
    //! @brief The Object's factory
    class ObjectFactory
    {
    public:
        
        //! @brief Add an object to the ObjectFactory.
        //! @details This function adds a new object to the factory. If the name of the object already exists,
        //! the function doesn't do anything otherwise the object is added to the factory.
        //! @param name An alias name of the object or nothing if you want yo use the default object name.
        template <class T,
        typename std::enable_if<std::is_base_of<model::Object, T>::value, model::Object>::type* = nullptr>
        static void registerModel(std::string name)
        {
            static_assert(!std::is_abstract<T>::value, "The class must not be abstract.");
            
            static_assert(std::is_constructible<T, std::string, std::vector<Atom>>::value, "Bad model object constructor");
            
            if(!name.empty())
            {
                std::lock_guard<std::mutex> guard(getMutex());
                
                std::map<std::string, CreatorBundle>& creators = getCreators();
                
                if(creators.find(name) == creators.end())
                {
                    CreatorBundle creator;
                    
                    creator.model_ctor_fn = [name](std::vector<Atom> args) -> T*
                    {
                        return new T(name, args);
                    };
                    
                    creators[name] = std::move(creator);
                }
                else
                {
                    assert(false && "The object already exist");
                    //Console::error("The object " + name->getName() + " already exist !");
                }
            }
        }
        
        template <class TModel, class TEngine, typename
        std::enable_if<std::is_base_of<engine::Object, TEngine>::value,
        model::Object>::type* = nullptr>
        static void registerEngine(std::string name)
        {
            static_assert(!std::is_abstract<TEngine>::value, "The class must not be abstract.");
            
            static_assert(std::is_constructible<TEngine, std::vector<Atom>>::value, "Bad engine object constructor");
            
            if(!name.empty())
            {
                std::lock_guard<std::mutex> guard(getMutex());
                
                std::map<std::string, CreatorBundle>& creators = getCreators();
                
                if(creators.find(name) != creators.end())
                {
                    CreatorBundle& creator = creators[name];
                    
                    creator.engine_ctor_fn = [](std::vector<Atom> args) -> TEngine*
                    {
                        return new TEngine(args);
                    };
                }
                else
                {
                    //Console::error("Object model not declared");
                    assert(false && "The Object model is not registered");
                }
            }
        }
        
        //! @brief Creates a new Object with a text.
        //! @param text The text of the Object.
        //! @example "plus 42" or "print zozo", or just "plus".
        //! @return An object (if the name matches a registered Object name).
        static std::unique_ptr<model::Object> createModel(std::string const& text);
        
        //! @brief Creates a new engine Object with a given text.
        //! @param text The text of the Object.
        //! @return An object (if the name matches a registered engine Object name).
        template <class TEngineObject, typename
        std::enable_if<std::is_base_of<engine::Object, TEngineObject>::value,
        engine::Object>::type* = nullptr>
        static std::unique_ptr<TEngineObject> createEngine(model::Object const& model)
        {
            std::vector<Atom> atoms = AtomHelper::parse(model.getText());
            
            if(atoms.size() > 0 && atoms[0].isString())
            {
                std::lock_guard<std::mutex> guard(getMutex());
                
                const std::string name = atoms[0].getString();
                
                const auto& creators = getCreators();
                const auto it = creators.find(name);
                if(it != creators.end())
                {
                    std::vector<Atom> args {atoms.begin() + 1, atoms.end()};
                    
                    const auto& engine_ctor = it->second.engine_ctor_fn;
                    
                    if(!engine_ctor)
                    {
                        assert(false && "the engine object has not been registered");
                    }
                    
                    return std::unique_ptr<TEngineObject>(engine_ctor(args));
                }
            }
        
            return nullptr;
        }
        
        //! @brief Returns true if a given string match a registered Object name.
        //! @param name The name of the object.
        //! @return true if the object has been registered, otherwise false.
        static bool hasModel(std::string const& name);
        
        //! @brief Returns true if a given string match a registered Object name.
        //! @param name The name of the object.
        //! @return true if the object has been registered, otherwise false.
        static bool hasEngine(std::string const& name);
        
        //! @brief Returns all the registered Object names.
        //! @return A vector of Object names.
        static std::vector<std::string> getNames();
        
    private:
        
        ObjectFactory() = delete;
        ~ObjectFactory() = delete;
        
        using model_ctor_t = std::function<model::Object*(std::vector<Atom>)>;
        using engine_ctor_t = std::function<engine::Object*(std::vector<Atom>)>;
        
        struct CreatorBundle
        {
            model_ctor_t model_ctor_fn = nullptr;
            engine_ctor_t engine_ctor_fn = nullptr;
        };
        
        //! @brief Returns the static map of creators.
        //! @return The static map of creators.
        static std::map<std::string, CreatorBundle>& getCreators() noexcept
        {
            static std::map<std::string, CreatorBundle> static_creators;
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


#endif // KIWI_MODEL_FACTORY_HPP_INCLUDED
