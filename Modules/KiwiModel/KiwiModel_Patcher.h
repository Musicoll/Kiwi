/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
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

#include "KiwiModel_Link.h"

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

            using Objects = flip::Array<model::Object>;
            using Links = flip::Array<model::Link>;
            using Users = flip::Collection<User>;
            
            //! @brief Default constructor.
            Patcher();
            
            //! @brief Destructor.
            ~Patcher();
            
            //! @brief Adds an object to the patcher model.
            //! @param The object to add in the text.
            //! @return A reference to the object added.
            model::Object& addObject(std::unique_ptr<model::Object> && object);
            
            //! @brief Replaces an object by another one.
            //! @details This function will rewire the new object.
            //! @return A reference to the newly added object.
            model::Object& replaceObject(model::Object const& old_object, std::unique_ptr<model::Object> && object);
            
            //! @brief create an Object from a flip::Mold.
            model::Object& addObject(std::string const& name, flip::Mold const& mold);
            
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
            
            //! @brief Gets the objects.
            Objects const& getObjects() const noexcept;
            
            //! @brief Gets the objects.
            Objects& getObjects() noexcept;
            
            //! @brief Gets the links.
            Links const& getLinks() const noexcept;
            
            //! @brief Gets the links.
            Links& getLinks() noexcept;
            
            //! @brief Gets the users.
            Users const& getUsers() const noexcept;
            
            //! @brief Gets the users.
            Users& getUsers() noexcept;
            
            //! @brief Returns true if current user is already in added to the document.
            bool hasSelfUser() const;
            
            //! @brief Returns the current User.
            //! @details The function will look for a User that match
            //! the current user id of the document, if it's not found, the User will be created,
            //! therefore, do not use this method while observing the model.
            //! @return The current User.
            User& useSelfUser();
            
        public: // signals
            
            enum
            {
                Signal_USER_CONNECT = 0,
                Signal_USER_DISCONNECT,
                Signal_GET_CONNECTED_USERS,
                Signal_RECEIVE_CONNECTED_USERS,
                Signal_STACK_OVERFLOW,
                Signal_STACK_OVERFLOW_CLEAR,
            };
            
            // from server to client
            flip::Signal<uint64_t>              signal_user_connect;
            
            // from server to client
            flip::Signal<uint64_t>              signal_user_disconnect;
            
            // from client to server
            flip::Signal<>                      signal_get_connected_users;
            
            // from server to client
            flip::Signal<std::vector<uint64_t>> signal_receive_connected_users;
            
            // used by the engine when a stack-overflow is detected
            flip::Signal<std::vector<flip::Ref>> signal_stack_overflow;
            
            // used by the view to clear stack-overflow
            flip::Signal<> signal_stack_overflow_clear;
            
        public: // internal methods
            
            //! @internal flip static declare method
            static void declare();
            
        private: // methods
            
            Objects::const_iterator  findObject(model::Object const& object) const;
            Objects::iterator        findObject(model::Object const& object);
            
            Links::const_iterator    findLink(model::Link const& object) const;
            Links::iterator          findLink(model::Link const& object);
            
            //! @internal Returns true if the link can be created.
            bool canConnect(model::Object const& from, const size_t outlet,
                            model::Object const& to, const size_t inlet) const;
            
        private: // members
            
            Objects         m_objects;
            Links           m_links;
            
            Users           m_users;
            
        private: // deleted methods
            
            Patcher(Patcher const&) = delete;
            Patcher(Patcher&&) = delete;
            Patcher& operator=(Patcher const&) = delete;
            Patcher& operator=(Patcher&&) = delete;
        };
    }
}
