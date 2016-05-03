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

#include "flip/Optional.h"
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
            
            class Client;
            class View;
            
            //! @brief Default constructor.
            Patcher();
            
            //! @brief Destructor.
            ~Patcher();
            
            //! @internal flip static declare method
            template<class TModel> static void declare();
            
            //! @brief Get the first Object.
            flip::Array<model::Object>::const_iterator getFirstObject() const noexcept
            {
                return m_objects.cbegin();
            }
            
            //! @brief Get the last Object.
            flip::Array<model::Object>::const_iterator getLastObject() const noexcept
            {
                return m_objects.cend();
            }
            
            //! @brief Get the first Link.
            flip::Array<model::Link>::const_iterator getFirstLink() const noexcept
            {
                return m_links.cbegin();
            }
            
            //! @brief Get the last Link.
            flip::Array<model::Link>::const_iterator getLastLink() const noexcept
            {
                return m_links.cend();
            }
            
            //! @brief Get the objects.
            flip::Array<model::Object> const& getObjects() const noexcept  { return m_objects; }
            
            //! @brief Get the links.
            flip::Array<model::Link> const& getLinks() const noexcept    { return m_links; }
            
            //! @brief Creates and adds a "plus" object to the Patcher.
            void addPlus();
            
            //! @brief Creates and adds a "print" object to the Patcher.
            void addPrint();
            
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
            
            void addView();
            
            void removeView();
            
            View& getView();
            
            bool viewChanged() const noexcept { return m_view.changed(); }
            
            bool hasView() const noexcept { return ! m_view.empty(); }
            
        private:
            
            bool canConnect(model::Object const& from, const uint32_t outlet,
                            model::Object const& to, const uint32_t inlet) const;
            
            flip::Array<model::Object>::const_iterator findObject(model::Object const& object) const;
            flip::Array<model::Object>::iterator findObject(model::Object const& object);
            
            flip::Array<model::Link>::const_iterator findLink(model::Link const& object) const;
            flip::Array<model::Link>::iterator findLink(model::Link const& object);
            
            //! objects and links are stored in a flip::Array to maintain a graphical z-order.
            flip::Array<model::Object>   m_objects;
            flip::Array<model::Link>     m_links;
            
            flip::Optional<View>         m_view;
        };
        
        // ================================================================================ //
        //                                   PATCHER VIEW                                   //
        // ================================================================================ //
        
        class Patcher::View : public flip::Object
        {
        public:
            
            View() = default;
            View(model::Patcher& patcher) : m_patcher(&patcher) {}
            ~View() = default;
            
            model::Patcher& getPatcher() {return *m_patcher;}
            
        public:
            
            template<class TModel>
            static void declare()
            {
                assert(! TModel::template has<Patcher::View>());
                
                TModel::template declare<Patcher::View>()
                .name("cicm.kiwi.Patcher.View")
                .template member<flip::ObjectRef<Patcher>, &View::m_patcher> ("patcher");
            }
            
        private:
            
            flip::ObjectRef<Patcher> m_patcher;
        };
        
        // ================================================================================ //
        //                                  PATCHER::declare                                //
        // ================================================================================ //
        
        template<class TModel>
        void Patcher::declare()
        {
            assert(! TModel::template has<Patcher>());
            
            Patcher::View::declare<TModel>();

            TModel::template declare<Patcher>()
            .name("cicm.kiwi.Patcher")
            .template member<flip::Array<model::Object>, &Patcher::m_objects> ("objects")
            .template member<flip::Array<model::Link>, &Patcher::m_links> ("links")
            .template member<flip::Optional<Patcher::View>, &Patcher::m_view> ("view");
        }
    }
}


#endif // KIWI_MODEL_PATCHER_HPP_INCLUDED
