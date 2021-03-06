/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#pragma once

#include <map>

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

#include <flip/Ref.h>

#include <KiwiModel/KiwiModel_Object.h>



namespace kiwi
{
    class PatcherView;
    class ObjectFrame;
    class HitTester;
    
    // ================================================================================ //
    //                                      MOUSEHANDLER                                //
    // ================================================================================ //
    
    //! @brief The mouse handler is used to make the patcher view react to the mouse interactions.
    class MouseHandler
    {
    public: // enums
        
        enum class Action
        {
            None = 0,
            CopyOnDrag,
            ForwardToObject,
            CreateLink,
            Lasso,
            MoveObjects,
            ResizeObjects,
            EditObject,
            SwitchSelection,
            Selection,
            SwitchLock,
            PopupMenu,
            OpenObjectHelp,
        };
        
        enum Direction : int
        {
            None    = 0,
            Up      = 1 << 0,
            Down    = 1 << 1,
            Left    = 1 << 2,
            Right   = 1 << 3
        };
        
    public: // methods
        
        //! @brief Returns the current action.
        Action getCurrentAction();
        
        //! @brief Constructor.
        MouseHandler(PatcherView & patcher_view);
        
        //! @brief Destructor.
        ~MouseHandler();
        
        //! @brief Handles patcher view's mouse down event.
        void handleMouseDown(juce::MouseEvent const& e);
        
        //! @brief Handles patcher view's mouse drag event.
        void handleMouseDrag(juce::MouseEvent const& e);
        
        //! @brief Handles patcher view's mouse up event.
        void handleMouseUp(juce::MouseEvent const& e);
        
        //! @brief Handles patcher view's mouse double click events.
        void handleMouseDoubleClick(juce::MouseEvent const& e);
        
        //! @brief Handles patcher view's mouse move events.
        void handleMouseMove(juce::MouseEvent const& e);
        
    private: // methods
        
        //! @brief Starts an interaction with the patcher view.
        void startAction(Action action, juce::MouseEvent const& e);
        
        //! @brief Continues an initiated interaction with the patcher view.
        void continueAction(juce::MouseEvent const& e);
        
        //! @brief Ends an ongoing interaction with patch.
        void endAction(juce::MouseEvent const& e);
        
        //! @brief Returns the resize direction according to the hit_tester.
        int getResizeDirection(HitTester const& hit_tester) const;
        
        //! @brief Returns the right resize mouse cursor.
        juce::MouseCursor::StandardCursorType getMouseCursorForBorder(HitTester const& hit_tester) const;
        
        //! @brief Resize the model object's bounds
        void resizeModelObjectBounds(model::Object& model,
                                     ObjectFrame& box,
                                     juce::Rectangle<int> prev_bounds,
                                     juce::Point<int> delta, bool fixed_ratio);
        
    private: // members
        
        PatcherView&                                m_patcher_view;
        Action                                      m_current_action = Action::None;
        juce::Point<int>                            m_last_drag {};
        std::map<flip::Ref, juce::Rectangle<int>>   m_mousedown_bounds {};
        int                                         m_direction = 0;
        
    private: // deleted methods
        
        MouseHandler() = delete;
        MouseHandler(MouseHandler const& other) = delete;
        MouseHandler(MouseHandler && other) = delete;
        MouseHandler& operator=(MouseHandler const& other) = delete;
        MouseHandler& operator=(MouseHandler && other) = delete;
    };
}
