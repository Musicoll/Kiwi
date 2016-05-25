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

#include <KiwiCore/KiwiAtom.hpp>
#include <mutex>
#include <algorithm>

// ---- Flip headers ---- //
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
#include "flip/Signal.h"

namespace kiwi
{
    class ObjectFactory;
    
    namespace model
    {
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        //! @brief The Object is an abstract base class for kiwi objects.
        //! @details objects can be instantiated in a Patcher.
        class Object : public flip::Object
        {
        public:
 
            //! @brief Constructor.
            Object();
            
            //! @brief Copy constructor (needed for flip::Array)
            Object(model::Object const&);
            
            //! @brief Destructor.
            virtual ~Object();
            
            //! @brief Returns the name of the Object.
            //! @return The name of the Object.
            inline std::string getName() const     { return m_name; }
            
            //! @brief Returns the name of the Object.
            //! @return The name of the Object.
            inline std::string getText() const     { return m_text; }
            
            //! @brief Returns the number of inlets.
            //! @return The number of inlets.
            inline size_t getNumberOfInlets() const noexcept
            {
                return static_cast<size_t>(m_inlets);
            }
            
            //! @brief Returns true if the inlets changed.
            bool inletsChanged() const noexcept
            {
                return m_inlets.changed();
            }
            
            //! @brief Returns the number of outlets.
            //! @return The number of outlets.
            inline size_t getNumberOfOutlets() const noexcept
            {
                return static_cast<size_t>(m_outlets);
            }
            
            //! @brief Returns true if the outlets changed.
            bool outletsChanged() const noexcept
            {
                return m_outlets.changed();
            }
            
            //! @brief Set the x/y position.
            void setPosition(double x, double y)
            {
                m_position_x = x;
                m_position_y = y;
            }
            
            //! @brief Returns true if the object's position changed.
            bool positionChanged() const noexcept
            {
                return (m_position_x.changed() || m_position_y.changed());
            }
            
            //! @brief Returns the x position.
            double getX() const noexcept
            {
                return !removed() ? m_position_x.value() : m_position_x.before();
            }
            
            //! @brief Returns the y position.
            double getY() const noexcept
            {
                return !removed() ? m_position_y.value() : m_position_y.before();
            }
            
            //! @brief Call signalTrigger() to hmmm.. trigger the signal.
            flip::Signal<> signalTrigger;
            
        protected:
            
            //! @brief Set the number of inlets.
            //! @param inlets The number of inlets.
            inline void setNumberOfInlets(size_t inlets)
            {
                m_inlets = static_cast<flip::Int::internal_type>(inlets);
            }
            
            //! @brief Set the number of inlets.
            //! @param inlets The number of inlets.
            inline void setNumberOfOutlets(size_t outlets)
            {
                m_outlets = static_cast<flip::Int::internal_type>(outlets);
            }
            
        public:
            
            //! @internal flip Default constructor
            Object(flip::Default&);
            
            //! @internal flip static declare method
            static void declare();
            
        private:
            
            //! @brief Signal types
            enum SignalType { Trigger };
            
            flip::String    m_name;
            flip::String    m_text;
            flip::Int       m_inlets;
            flip::Int       m_outlets;
            
            flip::Float     m_position_x;
            flip::Float     m_position_y;
            
            friend class kiwi::ObjectFactory;
        };
    }
}


#endif // KIWI_MODEL_OBJECT_HPP_INCLUDED
