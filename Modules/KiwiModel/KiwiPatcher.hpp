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

#include "KiwiLink.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      PATCHER                                     //
        // ================================================================================ //
        
        //! @brief The patcher manages a set of objects, links and attributes
        class Patcher : public Attribute::Manager
        {
        public:
            
            //! @brief Default constructor.
            Patcher();
            
            //! @internal flip Default constructor.
            Patcher(flip::Default&) {}
            
            //! @brief Destructor.
            ~Patcher();
            
            //! @brief initialize the Patcher model
            void init();
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                TModel::template declare<Patcher>()
                .name("cicm.kiwi.Patcher")
                .template inherit<Attribute::Manager>()
                .template member<decltype(Patcher::m_objects),   &Patcher::m_objects>   ("objects")
                .template member<decltype(Patcher::m_links),     &Patcher::m_links>     ("links");
            }
            
            //! Get the objects.
            /** The function retrieves the objects from the patcher.
             @return A vector with the objects.
             */
            flip::Array<flip::Object>::difference_type getNumberOfObjects() const
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                return std::count_if(m_objects.begin(), m_objects.end(), [](flip::Object const&){return true;});
            }
            
            //! Get the objects.
            /** The function retrieves the objects from the patcher.
             @return A vector with the objects.
             */
            flip::Array<model::Object> const& getObjects() noexcept
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                return m_objects;
            }
            
            //! Get the objects.
            /** The function retrieves the objects from the patcher.
             @return A vector with the objects.
             */
            flip::Collection<Link> const& getLinks() noexcept
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                return m_links;
            }
            
            //! @brief Adds an object to the Patcher.
            model::Object* addObject(std::string const& name, std::string const& text = "");
            
            //! @brief Constructs and add a Link to the Patcher.
            //! @details Constructs a Link with given origin and destination Object pointers
            //! and IO indexes then adds it in the Patcher.
            //! @param from     The origin Object pointer.
            //! @param outlet   The origin outlet index.
            //! @param to       The destination Object pointer.
            //! @param inlet    The destination inlet index.
            Link* addLink(model::Object* from, const uint8_t outlet, model::Object* to, const uint8_t inlet);
            
            //! @brief Removes an object from the Patcher.
            //! @param obj The pointer to the object to remove.
            void remove(model::Object* obj);
            
            //! @brief Returns true if an Object has been added, removed or changed.
            inline bool objectsChanged() const noexcept { return m_objects.changed(); }
            
            //! @brief Returns true if a Link has been added, removed or changed.
            inline bool linksChanged() const noexcept   { return m_links.changed(); }
            
        private:
            
            //! object are stored in a flip::Array to maintain a z-order.
            flip::Array<model::Object>  m_objects;
            flip::Collection<Link>      m_links;
            mutable std::mutex          m_mutex;        // lock-free (grahams) ??
        };
    }
}


#endif // KIWI_MODEL_PATCHER_HPP_INCLUDED
