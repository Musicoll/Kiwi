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

#ifndef KIWI_MODEL_PATCHER_HPP_INCLUDED
#define KIWI_MODEL_PATCHER_HPP_INCLUDED

#include "Objects/KiwiObjects.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      PATCHER                                     //
        // ================================================================================ //
        
        //! @brief The patcher manages a set of objects, links and attributes
        class Patcher : public flip::Object
        {
        public:
            
            using objects_t = flip::Array<model::Object>;
            using links_t   = flip::Array<model::Link>;
            
            //! @brief Default constructor.
            Patcher();
            
            //! @brief Destructor.
            ~Patcher();
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                TModel::template declare<Patcher>()
                .name("cicm.kiwi.Patcher")
                .template member<objects_t,   &Patcher::m_objects>   ("objects")
                .template member<links_t,     &Patcher::m_links>     ("links");
            }
            
            //! @brief Get the objects.
            objects_t const& getObjects() noexcept      { return m_objects; }
            
            //! @brief Get the links.
            links_t const& getLinks() const noexcept    { return m_links; }
            
            //! @brief Creates and adds an object to the Patcher.
            ObjectId addObject(std::string const& name, std::string const& text = "");
            
            //! @brief Constructs and add a Link to the Patcher.
            //! @details Constructs a Link with given origin and destination Object pointers
            //! and IO indexes then adds it in the Patcher.
            //! @param from     The origin Object pointer.
            //! @param outlet   The origin outlet index.
            //! @param to       The destination Object pointer.
            //! @param inlet    The destination inlet index.
            LinkId addLink(ObjectId const& from,
                           const uint32_t outlet,
                           ObjectId const& to,
                           const uint32_t inlet);
            
            //! @brief Removes an object from the Patcher.
            //! @details This will also remove all links connected to this object.
            //! @param id The ID of the object to remove.
            //! @return true if success, otherwise false.
            bool removeObject(ObjectId const& id);
            
            //! @brief Removes a link from the Patcher.
            //! @param id The ID of the link to remove.
            //! @return true if success, otherwise false.
            bool removeLink(LinkId const& id);
            
            //! @brief Returns true if an Object has been added, removed or changed.
            inline bool objectsChanged() const noexcept { return m_objects.changed(); }
            
            //! @brief Returns true if a Link has been added, removed or changed.
            inline bool linksChanged() const noexcept   { return m_links.changed(); }
            
        private:
            
            bool canConnect(ObjectId const& from,
                            const uint32_t outlet,
                            ObjectId const& to,
                            const uint32_t inlet) const;
            
            //! @brief Adds an object to the Patcher.
            //! @param object A unique pointer of object.
            ObjectId addObject(std::unique_ptr<model::Object> object);
            
            //! @brief Adds a Link to the Patcher.
            //! @param link A unique pointer of Link.
            LinkId addLink(std::unique_ptr<model::Link> link);
            
            //! objects and links are stored in a flip::Array to maintain a graphical z-order.
            objects_t   m_objects;
            links_t     m_links;
        };
    }
}


#endif // KIWI_MODEL_PATCHER_HPP_INCLUDED
