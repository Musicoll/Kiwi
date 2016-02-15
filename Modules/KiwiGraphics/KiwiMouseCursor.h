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

#ifndef KIWI_MOUSECURSOR_H_INCLUDED
#define KIWI_MOUSECURSOR_H_INCLUDED

#include <KiwiGraphics/KiwiFont.h>

namespace kiwi
{
    namespace graphics
    {
        // ================================================================================ //
        //                                   MOUSECURSOR                                    //
        // ================================================================================ //
        
        //! The mouse cursor represent a cursor type.
        /**
         The mouse cursor represent a cursor type.
         */
        class MouseCursor
        {
        public:
            
            //! Enum describing the type of predefined mouse cursor
            enum class Type : ulong
            {
                Default = 0,                ///< Default (parent view cursor)
                None,                       ///< None
                Arrow,                      ///< Arrow
                Wait,                       ///< Wait
                Ibeam,                      ///< I-Beam
                Crosshair,                  ///< Crosshair
                Copying,                    ///< Copying
                HandPointing,               ///< Pointing Hand
                HandDragging,               ///< Dragging Hand
                ResizingLeftRight,          ///< Left-Right
                ResizingUpDown,             ///< Up-Down
                ResizingFourWay,            ///< Four Way
                ResizingTop,                ///< Top Edge
                ResizingBottom,             ///< Bottom Edge
                ResizingLeft,               ///< Left Edge
                ResizingRight,              ///< Right Edge
                ResizingTopLeft,            ///< Top-Left Corner
                ResizingTopRight,           ///< Top-Right Corner
                ResizingBottomLeft,         ///< Bottom-Left Corner
                ResizingBottomRight         ///< Bottom-Right Corner
            };
            
            //! Mouse cursor constructor.
            /** Initializes a mouse cursor.
             @param type The type of cursor as defined by the Type \Enum.
             */
            constexpr MouseCursor(const Type type = Type::Default) noexcept : m_type(type) {}
            
            //! Destructor.
            inline ~MouseCursor() noexcept {}
            
            //! Retrieve the mouse cursor type.
            /** The function retrieves the mouse cursor type.
             @return The mouse cursor type.
             */
            inline Type type() const noexcept {return m_type;}
            
            //! Mouse cursor equal oeprator.
            /** Initializes the mouse cursor with another.
             @param other The other mouse cursor.
             */
            inline MouseCursor& operator=(MouseCursor const& other) noexcept
            {
                m_type = other.m_type;
                return *this;
            }
            
            //! Return true if this cursor is the same than an other.
            /** The function returns true if this cursor is the same than an other.
             @param other The other mouse cursor.
             @return true if they are equal, otherwise false.
             */
            inline bool operator==(MouseCursor const& other) const noexcept
            {
                return m_type == other.m_type;
            }
            
            //! Return true if this cursor is not the same than an other.
            /** The function returns true if this cursor is not the same than an other.
             @param other The other mouse cursor.
             @return true if they are not equal, otherwise false.
             */
            inline bool operator!=(MouseCursor const& other) const noexcept
            {
                return m_type != other.m_type;
            }
            
        private:
            Type m_type;
        };
    }
}

#endif // KIWI_MOUSECURSOR_H_INCLUDED
