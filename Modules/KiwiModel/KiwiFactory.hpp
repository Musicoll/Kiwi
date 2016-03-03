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

#ifndef __DEF_KIWI_MODELS_FACTORY__
#define __DEF_KIWI_MODELS_FACTORY__

#include "KiwiPatcher.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      FACTORY                                     //
    // ================================================================================ //
    
    //! The factory
    /** The factory is the kiwi's counterpart of Andy Warhol's factory.
     */
    class Factory
    {
    private:
        
        //! The full virtual object's creator.
        class Creator
        {
        public:
            virtual ~Creator() {};
            virtual Object* create(std::string const& name, std::string const& text) = 0;
        };
        
        typedef std::shared_ptr<Creator> sCreator;
        
        //! The full virtual object's creator.
        template <class T> class CreatorTyped : public Creator
        {
        public:
            inline Object* create(std::string const& name, std::string const& text) override
            {
                return new T(name, text);
            }
        };
        
        //! Retrieves the static map of creators.
        /** This function retrieves  the static map of creators.
         @return The map of creators.
         */
        static std::map<std::string const, std::shared_ptr<Creator>>& getCreators() noexcept
        {
            static std::map<std::string const, sCreator> static_creators;
            return static_creators;
        }
        
        //! Retrieves the static mutex.
        /** This function retrieves the static mutex.
         @return The mutex.
         */
        static inline std::mutex& getMutex() noexcept
        {
            static std::mutex static_mutex;
            return static_mutex;
        }
        
    public:
        
        //! Add an object to the factory.
        /** This function adds a new object to the factory. If the name of the object already exists, the function doesn't do anything otherwise the object is added to the factory.
         @name  An alias name of the object or nothing if you want yo use the default object name.
         */
        template <class T>
        static void add(std::string name = "")
        {
            static_assert(std::is_base_of<Object, T>::value, "The class must inherit from object.");
            static_assert(!std::is_abstract<T>::value, "The class must not be abstract.");
            //static_assert(std::is_constructible<T, Infos const&>::value, "The class must be constructible with Infos.");
            /*
            std::shared_ptr<Object> object = std::make_shared<T>(Infos());
            if(object)
            {
                std::lock_guard<std::mutex> guard(getMutex());
                
                auto creators = getCreators();
                
                if(name == Tags::_empty) {name = Tag::create(object->getName());}
                if(creators.find(name) != creators.end())
                {
                    //Console::error("The object " + name->getName() + " already exist !");
                }
                else
                {
                    creators[name] = std::make_shared<CreatorTyped<T>>();
                }
            }
            else
            {
                //Console::error("The prototype of an object has a bad ctor !");
            }
            */
        }
        
        //! Create an object.
        /** This function creates an object.
         @param name The name of the object.
         @param infos The informations to initialize the object.
         @return An object.
         */
        static Object* create(std::string const& name, std::string const& text);
        
        //! Retrieves if an object exist.
        /** This function retrieves if an object exist.
         @param name The name of the object.
         @return true if the object exist, otherwise false.
         */
        static bool has(std::string const& name);
        
        //! Removes an object from the factory.
        /** This function remoes an object from the factory.
         @param name The name of the object.
         */
        static void remove(std::string const& name);
        
        //! Retrieves all the names of the objects.
        /** This function retrieves all the names of the object.
         @return A vector of tags with the names.
         */
        static std::vector<std::string const> names();
    };
}


#endif


