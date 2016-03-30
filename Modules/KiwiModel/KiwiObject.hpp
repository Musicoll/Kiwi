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

#ifndef KIWI_MODEL_OBJECT_HPP_INCLUDED
#define KIWI_MODEL_OBJECT_HPP_INCLUDED

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
#include "flip/Variant.h"
#include "flip/Optional.h"
#include "flip/Object.h"
#include "flip/ObjectRef.h"

namespace kiwi
{
    namespace model
    {
        class Patcher;
        
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        //! @brief The Object is an abstract base class for kiwi objects.
        //! @details objects can be instantiated in a Patcher.
        class Object : public flip::Object
        {
        public:
            
            struct initInfos
            {
                std::string         name;
                std::string         text;
                std::vector<Atom>   args;
                
                initInfos(std::string const& object_name, std::string const& object_text) :
                name(object_name),
                text(object_text)
                {
                    ;
                }
            };
            
            //! @internal flip Default constructor
            Object(flip::Default&) {}
            
            //! @brief Constructor.
            Object(std::string const& name, std::string const& text);
            
            //! @brief Destructor.
            virtual ~Object() noexcept {}
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                if(TModel::template has<model::Object>()) return;
                
                TModel::template declare<model::Object>()
                .name("cicm.kiwi.Object")
                .template member<flip::String, &Object::m_name>("name")
                .template member<flip::String, &Object::m_text>("text")
                .template member<flip::Int,    &Object::m_number_of_inlets>("ninlets")
                .template member<flip::Int,    &Object::m_number_of_outlets>("noutlets");
            }
            
            //! @brief Returns the name of the Object.
            //! @return The name of the Object.
            inline std::string getName() const     { return m_name; }
            
            //! @brief Returns the text of the Object.
            //! @return The text of the Object.
            inline std::string getText() const     { return m_text; }
            
            //! @brief Returns the number of inlets.
            //! @return The number of inlets.
            inline uint32_t getNumberOfInlets() const noexcept
            {
                return static_cast<uint32_t>(m_number_of_inlets);
            }
            
            //! @brief Sets the number of inlets.
            //! @param value The new number of inlets.
            void setNumberOfInlets(uint32_t value) noexcept
            {
                m_number_of_inlets = static_cast<flip::Int::internal_type>(value);
            }
            
            //! @brief Returns the number of outlets.
            //! @return The number of outlets.
            inline uint32_t getNumberOfOutlets() const noexcept
            {
                return static_cast<uint32_t>(m_number_of_outlets);
            }
            
            //! @brief Sets the number of outlets.
            //! @param value The new number of outlets.
            void setNumberOfOutlets(uint32_t value) noexcept
            {
                m_number_of_outlets = static_cast<flip::Int::internal_type>(value);
            }
            
        private:
            flip::String    m_name;
            flip::String    m_text;
            flip::Int       m_number_of_inlets;
            flip::Int       m_number_of_outlets;
            
            //Object(Object const&) = delete;
        };
    }
}


#endif // KIWI_MODEL_OBJECT_HPP_INCLUDED
