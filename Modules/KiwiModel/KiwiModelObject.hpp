/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_MODEL_OBJECT_HPP_INCLUDED
#define KIWI_MODEL_OBJECT_HPP_INCLUDED

#include <KiwiModel/KiwiModelAtom.hpp>
#include <mutex>
#include <algorithm>

// ---- Flip headers ---- //
#include "flip/Bool.h"
#include "flip/Int.h"
#include "flip/Float.h"
#include "flip/String.h"
#include "flip/Array.h"
#include "flip/Collection.h"
#include "flip/Object.h"
#include "flip/ObjectRef.h"
#include "flip/Signal.h"

namespace kiwi
{
    namespace model
    {
        class Factory;
        
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        //! @brief The Object is a base class for kiwi objects.
        //! @details objects can be instantiated in a Patcher.
        class Object : public flip::Object
        {
        public: // methods
 
            //! @brief Constructor.
            Object();
            
            //! @brief Copy constructor (needed for flip::Array)
            Object(model::Object const&);
            
            //! @brief Destructor.
            virtual ~Object();
            
            //! @brief Returns the name of the Object.
            std::string getName() const;
            
            //! @brief Returns the text of the Object.
            std::string getText() const;
            
            //! @brief Returns the number of inlets.
            size_t getNumberOfInlets() const noexcept;
            
            //! @brief Returns true if the inlets changed.
            bool inletsChanged() const noexcept;
            
            //! @brief Returns the number of outlets.
            size_t getNumberOfOutlets() const noexcept;
            
            //! @brief Returns true if the outlets changed.
            bool outletsChanged() const noexcept;
            
            //! @brief Sets the x/y graphical position of the object.
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
            
            //! @brief Sets the width of the object.
            void setWidth(double new_width);
            
            //! @brief Sets the height of the object.
            void setHeight(double new_height);
            
            //! @brief Returns the object's width.
            double getWidth() const noexcept;
            
            //! @brief Returns the object's height.
            double getHeight() const noexcept;
            
            //! @brief Call signalTrigger() to hmmm.. trigger the signal.
            flip::Signal<> signalTrigger;
            
        protected: // methods
            
            //! @brief Sets the number of inlets.
            void setNumberOfInlets(size_t inlets);
            
            //! @brief Sets the number of inlets.
            void setNumberOfOutlets(size_t outlets);
            
        public: // internal methods
            
            //! @internal flip Default constructor
            Object(flip::Default&);
            
            //! @internal flip static declare method
            static void declare();
            
        private: // members
            
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
            
            friend class Factory;
        };
    }
}


#endif // KIWI_MODEL_OBJECT_HPP_INCLUDED
