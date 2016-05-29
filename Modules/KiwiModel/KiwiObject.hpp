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
            std::string getName() const;
            
            //! @brief Returns the name of the Object.
            //! @return The name of the Object.
            std::string getText() const;
            
            //! @brief Returns the number of inlets.
            //! @return The number of inlets.
            size_t getNumberOfInlets() const noexcept;
            
            //! @brief Returns true if the inlets changed.
            bool inletsChanged() const noexcept;
            
            //! @brief Returns the number of outlets.
            //! @return The number of outlets.
            size_t getNumberOfOutlets() const noexcept;
            
            //! @brief Returns true if the outlets changed.
            bool outletsChanged() const noexcept;
            
            //! @brief Set the x/y position.
            void setPosition(double x, double y);
            
            //! @brief Returns true if the object's position changed.
            bool positionChanged() const noexcept;
            
            //! @brief Returns true if the object's size changed.
            bool sizeChanged() const noexcept;
            
            //! @brief Returns true if the position or the size of the object changed.
            bool boundsChanged() const noexcept;
            
            //! @brief Returns the x position.
            double getX() const noexcept;
            
            //! @brief Returns the y position.
            double getY() const noexcept;
            
            //! @brief Returns the object's width.
            double getWidth() const noexcept;
            
            //! @brief Returns the object's height.
            double getHeight() const noexcept;
            
            //! @brief Call signalTrigger() to hmmm.. trigger the signal.
            flip::Signal<> signalTrigger;
            
        protected:
            
            //! @brief Set the number of inlets.
            //! @param inlets The number of inlets.
            void setNumberOfInlets(size_t inlets);
            
            //! @brief Set the number of inlets.
            //! @param inlets The number of inlets.
            void setNumberOfOutlets(size_t outlets);
            
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
            flip::Float     m_width;
            flip::Float     m_height;
            
            friend class kiwi::ObjectFactory;
        };
    }
}


#endif // KIWI_MODEL_OBJECT_HPP_INCLUDED
