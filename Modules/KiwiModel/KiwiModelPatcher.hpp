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

#ifndef KIWI_MODEL_PATCHER_HPP_INCLUDED
#define KIWI_MODEL_PATCHER_HPP_INCLUDED

#include "KiwiModelLink.hpp"
#include "KiwiModelObjects.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      PATCHER                                     //
        // ================================================================================ //
        
        //! @brief The Patcher manages a set of Object and Link
        class Patcher : public flip::Object
        {
        public: // methods
            
            class User;
            class View;
            
            //! @brief Default constructor.
            Patcher();
            
            //! @brief Destructor.
            ~Patcher();
            
            //! @brief Try to create an Object with a text.
            //! @details This function will first parse the input string in a vector of atom
            //! to find a registered name object as first atom.
            //! the last atoms are passed to the created object as arguments.
            //! @param text A string composed by the name of the object optionnally followed by a space and a list of argument values (ex : "plus 42")
            //! @return An Object.
            model::Object& addObject(std::string const& name, std::vector<Atom> const& args);
            
            //! @brief create an Object from a flip::Mold.
            model::Object& addObject(flip::Mold const& mold);
            
            //! @brief Constructs and add a Link to the Patcher.
            //! @details Constructs a Link with given origin and destination Object
            //! and IO indexes then adds it in the Patcher.
            //! @param from     The Object that sends messages.
            //! @param outlet   The sending outlet index.
            //! @param to       The Object that receives messages.
            //! @param inlet    The receiving inlet index.
            //! @return A link or nullptr if the link can't be created
            model::Link* addLink(model::Object const& from,
                                 const size_t outlet,
                                 model::Object const& to,
                                 const size_t inlet);
            
            //! @brief Removes an object from the Patcher.
            //! @details This will also remove all links connected to this object.
            //! @param object The Object to remove.
            void removeObject(model::Object const& object, Patcher::View* view = nullptr);
            
            //! @brief Removes a link from the Patcher.
            //! @param link The Link to remove.
            void removeLink(model::Link const& link, Patcher::View* view = nullptr);
            
            //! @brief Returns true if an Object has been added, removed or changed.
            bool objectsChanged() const noexcept;
            
            //! @brief Returns true if a Link has been added, removed or changed.
            bool linksChanged() const noexcept;
            
            //! @brief Returns true if a User has been added, removed or changed.
            bool usersChanged() const noexcept;
            
            //! @brief Returns true if the Patcher name changed.
            bool nameChanged() const noexcept;
            
            //! @brief Returns the Patcher name.
            std::string getName() const;
            
            //! @brief Sets the Patcher name.
            void setName(std::string const& new_name);
            
            //! @brief Set the User.
            //! @param user_id The user unique id.
            //! @return The user.
            User& createUserIfNotAlreadyThere(uint32_t user_id);
            
            //! @brief Get a User by id.
            //! @param user_id The user unique id.
            //! @return The User pointer if found or nullptr.
            User* getUser(uint32_t user_id);
            
            //! @brief Gets the objects.
            flip::Array<model::Object> const& getObjects() const noexcept;
            
            //! @brief Gets the links.
            flip::Array<model::Link> const& getLinks() const noexcept;
            
            //! @brief Gets the users.
            flip::Collection<User> const& getUsers() const noexcept;
        
        public: // internal methods
            
            //! @internal flip static declare method
            static void declare();
            
        private: // methods
            
            using object_array_t = flip::Array<model::Object>;
            using link_array_t = flip::Array<model::Link>;
            
            object_array_t::const_iterator findObject(model::Object const& object) const;
            object_array_t::iterator findObject(model::Object const& object);
            
            link_array_t::const_iterator findLink(model::Link const& object) const;
            link_array_t::iterator findLink(model::Link const& object);
            
            //! @internal Returns true if the link can be created.
            bool canConnect(model::Object const& from, const size_t outlet,
                            model::Object const& to, const size_t inlet) const;
            
        private: // members
            
            //! objects and links are stored in a flip::Array to maintain a graphical z-order.
            flip::Array<model::Object>  m_objects;
            flip::Array<model::Link>    m_links;
            
            flip::Collection<User>      m_users;
            
            flip::String                m_patcher_name;
            
        private: // deleted methods
            
            Patcher(Patcher const&) = delete;
            Patcher(Patcher&&) = delete;
            Patcher& operator=(Patcher const&) = delete;
            Patcher& operator=(Patcher&&) = delete;
        };
    }
}


#endif // KIWI_MODEL_PATCHER_HPP_INCLUDED
