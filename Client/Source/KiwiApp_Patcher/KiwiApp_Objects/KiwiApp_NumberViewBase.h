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

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_EditableObjectView.h>

namespace kiwi {
    
    // ================================================================================ //
    //                                   NUMBER VIEW BASE                               //
    // ================================================================================ //
    
    //! @brief The view of any textual kiwi object.
    class NumberViewBase : public EditableObjectView
    {
    public: // methods
        
        //! @brief Constructor.
        NumberViewBase(model::Object & object_model);
        
        //! @brief Destructor.
        ~NumberViewBase();
        
    protected: // methods.
        
        //! @brief Sets the displayed text and call repaint.
        void setDisplayNumber(double number);
        
        //! @brief Returns the displayed text as a number.
        double getDisplayNumber() const;
        
        //! @brief Sets the triangle icon colour.
        //! @details By default colour is outline colour.
        void setIconColour(juce::Colour colour);
        
    private: // methods
        
        //! @brief Overrides this method to react to a change in display number.
        virtual void displayNumberChanged(double number) = 0;
        
    private: // methods
        
        //! @brief Called when the object is resized.
        void resized() override final;
        
        //! @brief Draw the background of the number object.
        void paint(juce::Graphics& g) override final;
        
        //! @brief Paints elements over the text editor.
        void paintOverChildren (juce::Graphics& g) override final;
        
        //! @brief Called when the label text has changed.
        //! @details Overrides EditableObjectView::textChanged.
        void textChanged() override final;
        
        //! @brief Constructs the label's text editor.
        //! @details Overrides EditableOjectView::createTextEditor.
        juce::TextEditor* createdTextEditor() override final;
        
    private: // members
        
        double              m_value;
        int                 m_indent;
        juce::Colour        m_icon_colour;
        
    private: // deleted methods
        
        NumberViewBase() = delete;
        NumberViewBase(NumberViewBase const& other) = delete;
        NumberViewBase(NumberViewBase && other) = delete;
        NumberViewBase& operator=(NumberViewBase const& other) = delete;
        NumberViewBase& operator=(NumberViewBase && other) = delete;
    };
}

