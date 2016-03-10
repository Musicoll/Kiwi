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
        
        //! The patcher manages objects, links and a set of attributes
        class Patcher : public flip::Object,
        public Attribute::Manager
        {
        private:
            
            //! object are stored in a flip::Array to maintain a z-order.
            flip::Array<model::Object>   m_objects;
            flip::Collection<Link>      m_links;
            std::vector<uint64_t>       m_free_ids;
            mutable std::mutex          m_mutex;        // lock-free (grahams) ??
            
            Attribute::Int              m_gridsize;
            Attribute::RGBA             m_bgcolor;
            
            void createObject(std::string const& name, std::string const& text);
            
        public:
            
            //! flip default constructor (does nothing)
            Patcher() = default;
            
            //! Destructor.
            ~Patcher();
            
            //! initialize patcher model
            void init();
            
            //! static flip declare method
            template<class TModel>
            static void declare()
            {
                TModel::template declare<Patcher>()
                .name("cicm.kiwi.Patcher")
                .template member<decltype(Patcher::m_objects),   &Patcher::m_objects>   ("objects")
                .template member<decltype(Patcher::m_links),     &Patcher::m_links>     ("links")
                .template member<decltype(Patcher::m_bgcolor),   &Patcher::m_bgcolor>   ("bgcolor")
                .template member<decltype(Patcher::m_gridsize),  &Patcher::m_gridsize>  ("gridsize");
            }
            
            //! Get the objects.
            /** The function retrieves the objects from the patcher.
             @return A vector with the objects.
             */
            flip::Array<Object>::difference_type getNumberOfObjects() const
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                return std::count_if(m_objects.begin(), m_objects.end(), [](Object const&){return true;});
            }
            
            //! Get the objects.
            /** The function retrieves the objects from the patcher.
             @return A vector with the objects.
             */
            flip::Array<model::Object>& getObjects() noexcept
            {
                std::lock_guard<std::mutex> guard(m_mutex);
                return m_objects;
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
            
            //! Append a dico.
            /** The function reads a dico and add the objects and links to the patcher.
             @param dico The dico.
             */
            void add(std::map<const std::string, Atom> const& dico);
            
            //! Free a object.
            /** The function removes a object from the patcher.
             @param object        The pointer to the object.
             */
            void remove(model::Object* object);
            
            void bgcolorAttrSetter(std::vector<Atom> const& atoms)
            {
                std::cout << "bgcolorAttrSetter \n";
            }
            
            //! Retrieve the "gridsize" attribute value of the patcher.
            /** The function retrieves the "gridsize" attribute value of the patcher.
             @return The "gridsize" attribute value of the patcher.
             */
            inline Atom::int_t getGridSize() const noexcept
            {
                //return getAttributeValue("gridsize").getInt();
                return 0;
            }
            
            //! Set the "gridsize" attribute value of the patcher.
            /** The function sets the "gridsize" attribute value of the patcher.
             @param value The "gridsize" attribute value of the patcher.
             */
            inline void setGridSize(Atom const& value) noexcept
            {
                //setAttributeValue<flip::Int>("gridsize", flip::Int(value.getInt()));
            }
        };
    }
}


#endif // KIWI_MODEL_PATCHER_HPP_INCLUDED
