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

#ifndef KIWI_MODEL_ID_HPP_INCLUDED
#define KIWI_MODEL_ID_HPP_INCLUDED

#include "../KiwiCore/KiwiAtom.hpp"
#include <mutex>
#include <algorithm>

// ---- Flip headers ---- //
#include "flip/DataModel.h"
#include "flip/Bool.h"
#include "flip/Int.h"
#include "flip/Float.h"
#include "flip/Blob.h"
#include "flip/String.h"
#include "flip/Enum.h"
#include "flip/Array.h"
#include "flip/Collection.h"
#include "flip/Object.h"
#include "flip/ObjectRef.h"

namespace kiwi
{
    namespace model
    {
        class Object;
        class Link;
    }
    
    //! @brief kiwi wrapper of the flip::Ref class
    template <class TObjectType = flip::Object>
    struct Id
    {
        //! Construct a null ID
        Id() = default;
        
        Id(std::nullptr_t) : m_ref() {};
        
        Id(TObjectType const* object) : m_ref(object ? object->template ref() : flip::Ref::null) {};
        
        //! @brief Move constructor
        Id(Id&& id) : m_ref(std::move(id.m_ref)) {}
        
        //! @brief Destructor
        ~Id() = default;
        
        Id& operator=(Id const& rhs) { m_ref = rhs.m_ref; return *this; }
        
        //! @brief Returns a flip::Ref from the ID
        operator flip::Ref() const { return m_ref; }
        
        bool operator == (Id const& id) const { return (m_ref == id.m_ref); }
        bool operator == (flip::Ref const& ref) const { return (m_ref == ref); }
        
        bool operator != (Id const& id) const { return (m_ref != id.m_ref); }
        bool operator != (flip::Ref const& ref) const { return (m_ref != ref); }
        
        bool isNull() const { return m_ref == flip::Ref::null; }
        
    private:
        flip::Ref m_ref;
    };

    template <class TObjectType>
    bool operator == (flip::Ref const& ref, Id<TObjectType> const& id) { return id == ref; }
    
    template <class TObjectType>
    bool operator != (flip::Ref const& ref, Id<TObjectType> const& id) { return id != ref; }
    
    using ObjectId = Id<model::Object>;
    using LinkId = Id<model::Link>;
}


#endif // KIWI_MODEL_ID_HPP_INCLUDED
