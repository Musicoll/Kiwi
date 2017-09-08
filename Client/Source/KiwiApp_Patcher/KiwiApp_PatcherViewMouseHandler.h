/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
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

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

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
            None =              0,
            CopyOnDrag =        1,
            Object =            2,
            CreateLink =        3,
            Lasso =             4,
            MoveObject =        5,
            PopupMenu =         6,
            ObjectEdition =     7,
            SwitchSelection =   8,
            Selection =         9,
            SwitchLock =        10
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
        
    private: // members
        
        PatcherView &                               m_patcher_view;
        Action                                      m_current_action;
        juce::Point<int>                            m_last_drag;
        
    private: // deleted methods
        
        MouseHandler() = delete;
        MouseHandler(MouseHandler const& other) = delete;
        MouseHandler(MouseHandler && other) = delete;
        MouseHandler& operator=(MouseHandler const& other) = delete;
        MouseHandler& operator=(MouseHandler && other) = delete;
    };
}
