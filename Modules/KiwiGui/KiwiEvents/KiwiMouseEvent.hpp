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

#ifndef KIWI_GUI_MOUSE_EVENT_HPP_INCLUDED
#define KIWI_GUI_MOUSE_EVENT_HPP_INCLUDED

#include <KiwiGui/KiwiGraphics/KiwiPoint.hpp>

namespace kiwi
{
    namespace gui
    {
        // ================================================================================ //
        //                                      MOUSE EVENT                                 //
        // ================================================================================ //
        
        //! @brief The mouse event.
        class MouseEvent
        {
        public:
            
            //! @brief The mouse event Type.
            enum Type : unsigned int
            {
                Unknown     = 0,
                Enter       = 1,
                Leave       = 2,
                Move        = 3,
                Drag        = 4,
                Down        = 5,
                Up          = 6,
                DoubleClick = 7,
                Wheel       = 8
            };
            
            //! @brief The mouse event modifiers.
            enum Modifier : unsigned int
            {
                Nothing     = 0,
                Shift       = 1,
                Ctrl        = 2,
                Alt         = 4,
                Left        = 16,
                Right       = 32,
                Middle      = 64,
                
            #ifdef __APPLE__
                Cmd          = 8,
                Popup        = Right | Ctrl,
            #else
                Cmd          = Ctrl,
                Popup        = Right
            #endif
            };
            
            //! Constructor.
            /** The function initializes a mouse event.
             @param type    The type of mouse event.
             @param x       The abscissa of mouse event.
             @param y       The ordinate of mouse event.
             @param mod     The modifiers of the mouse event.
             */
            MouseEvent(const Type type, const int x, const int y, const unsigned int mod) noexcept;
            
            //! Constructor.
            /** The function initializes a mouse event.
             @param type    The type of mouse event.
             @param position The position of mouse event.
             @param mod     The modifiers of the mouse event.
             */
            MouseEvent(const Type type, Point const& position, const unsigned int mod) noexcept;
            
            //! Constructor.
            /** The function initializes a mouse event.
             @param type    The type of mouse event.
             @param x       The abscissa of mouse event.
             @param y       The ordinate of mouse event.
             @param mod     The modifiers of the mouse event.
             @param wheel_x The wheel abscissa of mouse event.
             @param wheel_y The wheel ordinate of mouse event.
             @param clicked If the mouse event was already clicked.
             @param down_x  The last down abscissa of mouse event.
             @param down_y  The last down ordinate of mouse event.
             @param nclicks the number of multiclicks.
             */
            MouseEvent(const Type type, const int x, const int y, const unsigned int mod, const double wheel_x, const double wheel_y, const bool clicked, const int down_x, const int down_y, const unsigned int nclicks) noexcept;
            
            //! Constructor.
            /** The function initializes a mouse event.
             @param type    The type of mouse event.
             @param position The position of mouse event.
             @param mod     The modifiers of the mouse event.
             @param wheel   The wheel offset of mouse event.
             @param clicked If the mouse event was already clicked.
             @param down_position The last down position of mouse event.
             @param nclicks the number of multiclicks.
             */
            MouseEvent(const Type type, Point const& position, const unsigned int mod, Point const& wheel, const bool clicked, Point const& down_position, const unsigned int nclicks) noexcept;
            
            //! Destructor.
            /** The function does nothing.
             */
            virtual ~MouseEvent() noexcept;
            
            //! Retrieve the type of the mouse event.
            /** The function retrieves the type of the mouse event.
             @return The type of mouse event.
             */
            inline Type getType() const noexcept
            {
                return m_type;
            }
            
            //! Retrieve if the type of the mouse event is unknown.
            /** The function retrieves if the type of the mouse event is unknown.
             @return true if the type of the mouse event is unknown, otherwise false.
             */
            inline bool isUnknown() const noexcept
            {
                return m_type == Unknown;
            }
            
            //! Retrieve if the type of the mouse event is enter.
            /** The function retrieves if the type of the mouse event is enter.
             @return true if the type of the mouse event is enter, otherwise false.
             */
            inline bool isEnter() const noexcept
            {
                return m_type == Enter;
            }
            
            //! Retrieve if the type of the mouse event is leave.
            /** The function retrieves if the type of the mouse event is leave.
             @return true if the type of the mouse event is leave, otherwise false.
             */
            inline bool isLeave() const noexcept
            {
                return m_type == Leave;
            }
            
            //! Retrieve if the type of the mouse event is move.
            /** The function retrieves if the type of the mouse event is move.
             @return true if the type of the mouse event is move, otherwise false.
             */
            inline bool isMove() const noexcept
            {
                return m_type == Move;
            }
            
            //! Retrieve if the type of the mouse event is drag.
            /** The function retrieves if the type of the mouse event is drag.
             @return true if the type of the mouse event is drag, otherwise false.
             */
            inline bool isDrag() const noexcept
            {
                return m_type == Drag;
            }
            
            //! Retrieve if the type of the mouse event is down.
            /** The function retrieves if the type of the mouse event is down.
             @return true if the type of the mouse event is down, otherwise false.
             */
            inline bool isDown() const noexcept
            {
                return m_type == Down;
            }
            
            //! Retrieve if the type of the mouse event is up.
            /** The function retrieves if the type of the mouse event is up.
             @return true if the type of the mouse event is up, otherwise false.
             */
            inline bool isUp() const noexcept
            {
                return m_type == Up;
            }
            
            //! Retrieve if the type of the mouse event is double click.
            /** The function retrieves if the type of the mouse event is double click.
             @return true if the type of the mouse event is double click, otherwise false.
             */
            inline bool isDoubleClick() const noexcept
            {
                return m_type == DoubleClick;
            }
            
            //! Retrieve if the type of the mouse event is wheel.
            /** The function retrieves if the type of the mouse event is wheel.
             @return true if the type of the mouse event is wheel, otherwise false.
             */
            inline bool isWheel() const noexcept
            {
                return m_type == Wheel;
            }
            
            //! Retrieve if the abscissa of the mouse event.
            /** The function retrieves the abscissa of the mouse event.
             @return The abscissa of the mouse event.
             */
            inline double getX() const noexcept
            {
                return m_position.x();
            }
            
            //! Retrieve if the ordinate of the mouse event.
            /** The function retrieves the ordinate of the mouse event.
             @return The ordinate of the mouse event.
             */
            inline double getY() const noexcept
            {
                return m_position.y();
            }
            
            //! Retrieve if the position of the mouse event.
            /** The function retrieves the position of the mouse event.
             @return The position of the mouse event.
             */
            inline Point getPosition() const noexcept
            {
                return m_position;
            }
            
            //! Retrieve if the lastest abscissa of the mouse event.
            /** The function retrieves the lastest down abscissa of the mouse event.
             @return The lastest down abscissa of the mouse event.
             */
            inline double getDownX() const noexcept
            {
                return m_position_down.x();
            }
            
            //! Retrieve if the lastest ordinate of the mouse event.
            /** The function retrieves the lastest down ordinate of the mouse event.
             @return The lastest down ordinate of the mouse event.
             */
            inline double getDownY() const noexcept
            {
                return m_position_down.y();
            }
            
            //! Retrieve if the lastest position of the mouse event.
            /** The function retrieves the lastest down position of the mouse event.
             @return The lastest down position of the mouse event.
             */
            inline Point getDownPosition() const noexcept
            {
                return m_position_down;
            }
            
            //! Retrieve if the abscissa wheel offset of the mouse event.
            /** The function retrieves the abscissa wheel offset of the mouse event.
             @return The abscissa wheel offset of the mouse event.
             */
            inline double getWheelOffsetX() const noexcept
            {
                return m_wheel_offset.x();
            }
            
            //! Retrieve if the ordinate wheel offset of the mouse event.
            /** The function retrieves the ordinate wheel offset of the mouse event.
             @return The ordinate wheel offset of the mouse event.
             */
            inline double getWheelOffsetY() const noexcept
            {
                return m_wheel_offset.y();
            }
            
            //! Retrieve if the wheel offset of the mouse event.
            /** The function retrieves the wheel offset of the mouse event.
             @return The wheel offset of the mouse event.
             */
            inline Point getWheelOffset() const noexcept
            {
                return m_wheel_offset;
            }
            
            //! Retrieve the modifiers pressed during the mouse event.
            /** The function retrieves the modifiers pressed during the mouse event.
             @return The modifiers pressed during the mouse event.
             */
            inline unsigned int getModifiers() const noexcept
            {
                return m_modifiers;
            }
            
            //! Retrieve if no modifiers has been pressed during the mouse event.
            /** The function retrieves if no modifiers has been pressed during the mouse event.
             @return True if no modifiers has been pressed during the mouse event, otherwise false.
             */
            inline bool hasNoModifier() const noexcept
            {
                return m_modifiers == Nothing;
            }
            
            //! Retrieve if the shift modifier has been pressed during the mouse event.
            /** The function retrieves if the shift modifier has been pressed during the mouse event.
             @return True if the shift modifier has been pressed during the mouse event, otherwise false.
             */
            inline bool hasShift() const noexcept
            {
                return m_modifiers & Shift;
            }
            
            //! Retrieve if the control modifier has been pressed during the mouse event.
            /** The function retrieves if the control modifier has been pressed during the mouse event.
             @return True if the control modifier has been pressed during the mouse event, otherwise false.
             */
            inline bool hasCtrl() const noexcept
            {
                return m_modifiers & Ctrl;
            }
            
            //! Retrieve if the alt modifier has been pressed during the mouse event.
            /** The function retrieves if the alt modifier has been pressed during the mouse event.
             @return True if the alt modifier has been pressed during the mouse event, otherwise false.
             */
            inline bool hasAlt() const noexcept
            {
                return m_modifiers & Alt;
            }
            
            //! Retrieve if the left button has been pressed during the mouse event.
            /** The function retrieves if the left button has been pressed during the mouse event.
             @return True if the left button has been pressed during the mouse event, otherwise false.
             */
            inline bool hasLeft() const noexcept
            {
                return m_modifiers & Left;
            }
            
            //! Retrieve if the right button has been pressed during the mouse event.
            /** The function retrieves if the right button has been pressed during the mouse event.
             @return True if the right button has been pressed during the mouse event, otherwise false.
             */
            inline bool hasRight() const noexcept
            {
                return m_modifiers & Right;
            }
            
            //! Retrieve if the middle button has been pressed during the mouse event.
            /** The function retrieves if the middle button has been pressed during the mouse event.
             @return True if the middle button has been pressed during the mouse event, otherwise false.
             */
            inline bool hasMiddle() const noexcept
            {
                return m_modifiers & Middle;
            }
            
            //! Retrieve if the command modifier has been pressed during the mouse event.
            /** The function retrieves if the command modifier has been pressed during the mouse event.
             @return True if the command modifier has been pressed during the mouse event, otherwise false.
             */
            inline bool hasCmd() const noexcept
            {
                return m_modifiers & Cmd;
            }
            
            //! Retrieve if the popup modifier has been pressed during the mouse event.
            /** The function retrieves if the popup modifier has been pressed during the mouse event.
             @return True if the popup modifier has been pressed during the mouse event, otherwise false.
             */
            inline bool hasPopup() const noexcept
            {
                return m_modifiers & Popup;
            }
            
            //! Retrieve if the popup modifier has been pressed during the mouse event.
            /** The function retrieves if the popup modifier has been pressed during the mouse event.
             @return True if the popup modifier has been pressed during the mouse event, otherwise false.
             */
            inline bool wasClicked() const noexcept
            {
                return m_was_clicked;
            }
            
            //! Retrieve the number of multiclicks.
            /** The function retrieves the number of multiclicks.
             @return The number of multiclicks.
             */
            inline unsigned int getNumberOfClicks() const noexcept
            {
                return m_nclicks;
            }
            
            //! Set that the mouse can be move or dragged without limits.
            /** The function sets that the mouse can be move or dragged without limits.
             @param isLimited true if the mouse can be dragged without limits, otherwise false.
             @param visibleUntilLimits true if the mouse should be visible until the limit of the screen are reached.
             */
            virtual void setMouseUnlimited(bool isLimited, bool visibleUntilLimits = false) const
            {
                ;
            }
            
        private:
            
            const Type          m_type;
            const Point         m_position;
            const Point         m_position_down;
            const Point         m_wheel_offset;
            const unsigned int  m_modifiers;
            const bool          m_was_clicked;
            const unsigned int  m_nclicks;
        };
    }
}

#endif // KIWI_GUI_MOUSE_EVENT_HPP_INCLUDED
