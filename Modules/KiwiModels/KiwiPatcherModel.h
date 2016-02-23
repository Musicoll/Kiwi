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

#ifndef __DEF_KIWI_MODELS_PATCHER__
#define __DEF_KIWI_MODELS_PATCHER__

#include "KiwiLinkModel.h"

namespace kiwi
{
    // ================================================================================ //
    //                                      PATCHER                                     //
    // ================================================================================ //
    
    //! The patcher manages objects, links and a set of attributes
    class PatcherModel : public AttributeBase::Manager
    {
    private:
        flip::Array<ObjectModel>    m_objects;      // Array for z-order
        flip::Array<LinkModel>      m_links;        // => Collection ??
        std::vector<ulong>               m_free_ids;
        mutable std::mutex               m_mutex;        // lock-free (grahams) ??
        
        
        Attribute<flip::Int>        m_gridsize;
        Attribute<FlipRGBA>         m_bgcolor;
        Attribute<flip::Bool>       m_attr_bool;
        Attribute<FlipTag>          m_attr_tag;
        
        void createObject(Dico& dico);
        
    public:
        
        //! @internal temporary dummy atom tester
        FlipAtom m_atom;
        
        //! flip default constructor (does nothing)
        PatcherModel() {};
        
        //! Destructor.
        ~PatcherModel();
        
        //! initialize patcher model
        void init(Dico& dico);
        
        //! static flip declare method
        static void declare();

        //! Get the objects.
        /** The function retrieves the objects from the patcher.
         @return A vector with the objects.
         */
        inline ulong getNumberOfObjects() const
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            return m_objects.count_if([](const ObjectModel& obj) {return true;});
        }
        
        //! Get the objects.
        /** The function retrieves the objects from the patcher.
         @return A vector with the objects.
         */
        inline flip::Array<ObjectModel>& getObjects() noexcept
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            return m_objects;
        }
        
        //! Get an object with the id.
        /** The function retrieves an object with an id.
         @param id   The id of the object.
         */
        inline ObjectModel* getObjectWithId(const ulong ID) const noexcept
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            
            auto predicate = [&ID](const ObjectModel& object)
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
        void add(Dico const& dico);
        
        //! Free a object.
        /** The function removes a object from the patcher.
         @param object        The pointer to the object.
         */
        void remove(ObjectModel* object);
        
        //! Retrieve the "gridsize" attribute value of the patcher.
        /** The function retrieves the "gridsize" attribute value of the patcher.
         @return The "gridsize" attribute value of the patcher.
         */
        inline Atom getGridSize() const noexcept
        {
            return getAttributeValue(Tags::gridsize);
        }
        
        //! Set the "gridsize" attribute value of the patcher.
        /** The function sets the "gridsize" attribute value of the patcher.
         @param value The "gridsize" attribute value of the patcher.
         */
        inline void setGridSize(Atom const& value) noexcept
        {
            setAttributeValue(Tags::gridsize, value);
        }
    };
}


#endif


