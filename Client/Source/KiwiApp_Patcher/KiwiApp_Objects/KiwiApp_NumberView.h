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

#include <juce_gui_basics/juce_gui_basics.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_NumberViewBase.h>

namespace kiwi {
    
    // ================================================================================ //
    //                                   NUMBER VIEW                                    //
    // ================================================================================ //
    
    //! @brief The view of any textual kiwi object.
    class NumberView : public NumberViewBase
    {
    private: // members
        
        struct MouseInfo
        {
            double  m_mouse_down_value  = 0;
            int     m_mouse_down_y      = 0;
            bool    m_is_alt_down       = false;
            double  m_alt_down_value    = 0;
            int     m_alt_down_y        = 0;
        };
        
    public: // methods
        
        // @brief The declaration method.
        static void declare();
        
        //! @brief Creation method.
        static std::unique_ptr<ObjectView> create(model::Object & object_model);
        
        //! @brief Constructor.
        NumberView(model::Object & object_model);
        
        //! @brief Destructor.
        ~NumberView();
        
    private: // methods
        
        //! @brief Stores the initial value before draging.
        void mouseDown(juce::MouseEvent const& e) override final;
        
        //! @brief Increment value depending on mouse position.
        void mouseDrag(juce::MouseEvent const& e) override final;
        
        //! @brief Resets mouse information.
        void mouseUp(juce::MouseEvent const& e) override final;
        
        //! @brief Edits the label.
        void mouseDoubleClick(juce::MouseEvent const& e) override final;
        
        //! @brief Called whenever one of the object's attribute has changed.
        void parameterChanged(std::string const& name, tool::Parameter const& param) override final;
        
        //! @brief Sets the parameter value.
        void displayNumberChanged(double number) override final;
        
    private: // members
        
        flip::Signal<> &    m_output_message;
        int                 m_sensitivity;
        MouseInfo           m_mouse_info;
        
    private: // deleted methods
        
        NumberView() = delete;
        NumberView(NumberView const& other) = delete;
        NumberView(NumberView && other) = delete;
        NumberView& operator=(NumberView const& other) = delete;
        NumberView& operator=(NumberView && other) = delete;
    };
}

