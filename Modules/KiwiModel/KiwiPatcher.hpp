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
                .template member<decltype(Patcher::m_objects),   &Patcher::m_objects>   ("objects")
                .template member<decltype(Patcher::m_links),     &Patcher::m_links>     ("links");
            }
            
            //! @brief Get the objects.
            flip::Array<model::Object> const& getObjects() noexcept { return m_objects; }
            
            //! @brief Get the links.
            flip::Array<Link> const& getLinks() noexcept            { return m_links; }
            
            //! @brief Adds an object to the Patcher.
            //! @param object A unique pointer of object.
            model::Object* addObject(std::unique_ptr<model::Object> && object);
            
            //! @brief Adds a Link to the Patcher.
            //! @param link A unique pointer of Link.
            Link* addLink(std::unique_ptr<model::Link> link);
            
            //! @brief Removes an object from the Patcher.
            //! @param obj A reference to the object to remove.
            void removeObject(model::Object& obj);
            
            //! @brief Removes a link from the Patcher.
            //! @param link A reference to the link to remove.
            void removeLink(model::Link& link);
            
            //! @brief Returns true if an Object has been added, removed or changed.
            inline bool objectsChanged() const noexcept { return m_objects.changed(); }
            
            //! @brief Returns true if a Link has been added, removed or changed.
            inline bool linksChanged() const noexcept   { return m_links.changed(); }
            
        private:
            
            //! objects and links are stored in a flip::Array to maintain a graphical z-order.
            flip::Array<model::Object>  m_objects;
            flip::Array<Link>           m_links;
        };
    }
}


#endif // KIWI_MODEL_PATCHER_HPP_INCLUDED
