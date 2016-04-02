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
                const std::string         name;
                const std::string         text;
                const std::vector<Atom>   args;
                
                initInfos(std::string const& object_name, std::string const& object_text) :
                name(object_name),
                text(object_text),
                args(AtomHelper::parse(text))
                {
                    ;
                }
            };

            //! @brief The type of input/output this IO accepts.
            enum class IoType : uint8_t
            {
                Message = 0,
                Signal  = 1,
                Both    = 2
            };
            
            //! @brief Represents either an Inlet or an Outlet of a given type.
            class Iolet : public flip::Object
            {
            public:
                Iolet(flip::Default&) {}
                Iolet(IoType type, bool is_inlet) : m_io_type(type), m_is_inlet(is_inlet) {}
                ~Iolet() = default;
                
                IoType getType() const noexcept {return m_io_type;}
                
            private:
                friend class model::Object;
                flip::Enum<IoType>  m_io_type;
                flip::Bool          m_is_inlet;
            };
            
            //! @internal flip Default constructor
            Object(flip::Default&) {}
            
            //! @brief Constructor.
            Object(initInfos const& infos);
            
            //! @brief Destructor.
            virtual ~Object() noexcept {}
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                if(! TModel::template has<model::Object::IoType>())
                {
                    TModel::template declare<model::Object::IoType>()
                    .name("IoType")
                    .template enumerator<IoType::Message>("Message")
                    .template enumerator<IoType::Signal>("Signal");
                }
                
                if(! TModel::template has<model::Object::Iolet>())
                {
                    TModel::template declare<model::Object::Iolet>()
                    .name("Iolet")
                    .template member<flip::Enum<IoType>, &Iolet::m_io_type>("type")
                    .template member<flip::Bool, &Iolet::m_is_inlet>("is_inlet");
                }
                
                if(TModel::template has<model::Object>()) return;
                
                TModel::template declare<model::Object>()
                .name("cicm.kiwi.Object")
                .template member<flip::String, &Object::m_name>("name")
                .template member<flip::String, &Object::m_text>("text")
                .template member<flip::Array<Iolet>, &Object::m_inlets>("inlets")
                .template member<flip::Array<Iolet>, &Object::m_outlets>("outlets");
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
                return static_cast<uint32_t>(m_inlets.count_if([](Iolet const&){return true;}));
            }
            
            //! @brief Adds an inlet to the Object.
            //! @param type The type of input this inlet accepts.
            void addInlet(IoType type)
            {
                m_inlets.emplace(m_inlets.end(), type, true);
            }
            
            //! @brief Retrieves the inlets of the Object.
            flip::Array<Iolet> const& getInlets() const noexcept { return m_inlets; }
            
            /*
            //! @brief Removes the rightmost inlet of the Object.
            void removeInlet()
            {
                if(m_inlets.cbegin() != m_inlets.cend())
                {
                    m_inlets.erase(m_inlets.rbegin());
                }
            }
            */
            
            //! @brief Adds an outlet to the Object.
            //! @param type The type of output this outlet accepts.
            void addOutlet(IoType type)
            {
                m_outlets.emplace(m_outlets.end(), type, false);
            }
            
            //! @brief Retrieves the inlets of the Object.
            flip::Array<Iolet> const& getOutlets() const noexcept { return m_outlets; }
            
            /*
            //! @brief Removes the rightmost outlet of the Object.
            void removeOutlet()
            {
                if(m_outlets.cbegin() != m_outlets.cend())
                {
                    m_outlets.erase(m_outlets.crbegin());
                }
            }
            */
            
            //! @brief Returns the number of outlets.
            //! @return The number of outlets.
            inline uint32_t getNumberOfOutlets() const noexcept
            {
                return static_cast<uint32_t>(m_outlets.count_if([](Iolet const&){return true;}));
            }
            
        private:
            flip::String        m_name;
            flip::String        m_text;
            flip::Array<Iolet>  m_inlets;
            flip::Array<Iolet>  m_outlets;
        };
    }
}


#endif // KIWI_MODEL_OBJECT_HPP_INCLUDED
