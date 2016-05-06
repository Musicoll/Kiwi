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
        
        //! @brief The Patcher manages a set of Object and Link
        class Patcher : public flip::Object
        {
        public:
            
            using object_array_t = flip::Array<model::Object>;
            using link_array_t = flip::Array<model::Link>;
                        
            //! @brief Default constructor.
            Patcher();
            
            //! @brief Destructor.
            ~Patcher();
            
            //! @brief Creates and adds a "plus" object to the Patcher.
            model::Object& addPlus();
            
            //! @brief Creates and adds a "print" object to the Patcher.
            model::Object& addPrint();
            
            //! @brief Constructs and add a Link to the Patcher.
            //! @details Constructs a Link with given origin and destination Object
            //! and IO indexes then adds it in the Patcher.
            //! @param from     The origin Object.
            //! @param outlet   The origin outlet index.
            //! @param to       The destination Object.
            //! @param inlet    The destination inlet index.
            void addLink(model::Object const& from,
                         const uint32_t outlet,
                         model::Object const& to,
                         const uint32_t inlet);
            
            //! @brief Removes an object from the Patcher.
            //! @details This will also remove all links connected to this object.
            //! @param object The Object to remove.
            void removeObject(model::Object const& object);
            
            //! @brief Removes a link from the Patcher.
            //! @param link The Link to remove.
            void removeLink(model::Link const& link);
            
            //! @brief Returns true if an Object has been added, removed or changed.
            inline bool objectsChanged() const noexcept { return m_objects.changed(); }
            
            //! @brief Returns true if a Link has been added, removed or changed.
            inline bool linksChanged() const noexcept   { return m_links.changed(); }
            
            //! @brief Get the first Object.
            object_array_t::const_iterator getFirstObject() const noexcept  { return m_objects.cbegin(); }
            
            //! @brief Get the first Object.
            object_array_t::iterator getFirstObject() noexcept              { return m_objects.begin(); }
            
            //! @brief Get the last Object.
            object_array_t::const_iterator getLastObject() const noexcept   { return m_objects.cend(); }
            
            //! @brief Get the last Object.
            object_array_t::iterator getLastObject() noexcept               { return m_objects.end(); }
            
            //! @brief Get the first Link.
            link_array_t::const_iterator getFirstLink() const noexcept      { return m_links.cbegin(); }
            
            //! @brief Get the first Link.
            link_array_t::iterator getFirstLink() noexcept                  { return m_links.begin(); }
            
            //! @brief Get the last Link.
            link_array_t::const_iterator getLastLink() const noexcept       { return m_links.cend(); }
            
            //! @brief Get the last Link.
            link_array_t::iterator getLastLink() noexcept                   { return m_links.end(); }
            
            //! @brief Get the objects.
            flip::Array<model::Object> const& getObjects() const noexcept   { return m_objects; }
            
            //! @brief Get the links.
            flip::Array<model::Link> const& getLinks() const noexcept       { return m_links; }
            
            //! @internal flip static declare method
            template<class TModel> static void declare();
            
        private:
            
            bool canConnect(model::Object const& from, const uint32_t outlet,
                            model::Object const& to, const uint32_t inlet) const;
        
            object_array_t::const_iterator findObject(model::Object const& object) const;
            object_array_t::iterator findObject(model::Object const& object);
            
            link_array_t::const_iterator findLink(model::Link const& object) const;
            link_array_t::iterator findLink(model::Link const& object);
            
        private:
            
            //! objects and links are stored in a flip::Array to maintain a graphical z-order.
            flip::Array<model::Object>   m_objects;
            flip::Array<model::Link>     m_links;
        };
        
        // ================================================================================ //
        //                                  PATCHER::declare                                //
        // ================================================================================ //
        
        template<class TModel>
        void Patcher::declare()
        {
            assert(! TModel::template has<Patcher>());

            TModel::template declare<Patcher>()
            .name("cicm.kiwi.Patcher")
            .template member<flip::Array<model::Object>, &Patcher::m_objects> ("objects")
            .template member<flip::Array<model::Link>, &Patcher::m_links> ("links");
        }
    }
}


#endif // KIWI_MODEL_PATCHER_HPP_INCLUDED
