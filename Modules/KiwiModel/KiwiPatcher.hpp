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
        class Patcher : public flip::Object,
                        public Attribute::Manager
        {
        public:
            
            //! @brief Default constructor.
            Patcher() = default;
            
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
                .template member<decltype(Patcher::m_objects),   &Patcher::m_objects>   ("objects")
                .template member<decltype(Patcher::m_links),     &Patcher::m_links>     ("links")
                .template member<decltype(Patcher::m_name),      &Patcher::m_name>      ("name")
                .template member<decltype(Patcher::m_bgcolor),   &Patcher::m_bgcolor>   ("bgcolor")
                .template member<decltype(Patcher::m_gridsize),  &Patcher::m_gridsize>  ("gridsize");
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
            
            //! Get an object with the id.
            /** The function retrieves an object with an id.
             @param id   The id of the object.
             */
            inline model::Object* getObjectWithId(const int64_t ID) const noexcept
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                
                auto predicate = [&ID](const model::Object& object)
                {
                    return (object.getId() == ID);
                };
                
                const auto it = find_if(m_objects.begin(), m_objects.end(), predicate);
                if(it != m_objects.cend())
                {
                    return it.operator->();
                }
                
                return nullptr;
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
            
            //! Append a dico.
            /** The function reads a dico and add the objects and links to the patcher.
             @param dico The dico.
             */
            void add(std::map<const std::string, Atom> const& dico);
            
            //! @brief Removes an object from the Patcher.
            //! @param obj The pointer to the object to remove.
            void remove(model::Object* obj);
            
        private:
            
            //! object are stored in a flip::Array to maintain a z-order.
            flip::Array<model::Object>  m_objects;
            flip::Collection<Link>      m_links;
            std::vector<uint64_t>       m_free_ids;
            mutable std::mutex          m_mutex;        // lock-free (grahams) ??
            
            Attribute::String           m_name;
            Attribute::RGBA             m_bgcolor;
            Attribute::Int              m_gridsize;
        };
    }
}


#endif // KIWI_MODEL_PATCHER_HPP_INCLUDED
