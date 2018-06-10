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

#include <functional>

#include <juce_gui_basics/juce_gui_basics.h>

#include <KiwiModel/KiwiModel_Object.h>

#include <KiwiTool/KiwiTool_Listeners.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_EditableObjectView.h>

namespace kiwi
{
    // ================================================================================ //
    //                                   CLASSIC VIEW                                   //
    // ================================================================================ //
    
    //! @brief The view of any textual kiwi object.
    class ClassicView : public EditableObjectView,
                        public juce::TextEditor::Listener
    {
    public: // methods
        
        //! @brief Constructor.
        ClassicView(model::Object & object_model);
        
        //! @brief Destructor.
        ~ClassicView();
        
    private: // methods
        
        //! @brief Validate the new width and height for the box
        void validateSize(int& new_width, int& new_height) override;
        
        //! @brief Constructs the label's text editor.
        //! @brief Overrides EditableObjectView::createTextEditor.
        juce::TextEditor* createdTextEditor() override;
        
        //! @brief Called when label text changed.
        //! @details Overrides EditableObjectView::textChanged.
        void textChanged() override;
        
        //! @brief Called when the text is being typed.
        //! @details Used to resize in order to keep text visible.
        void textEditorTextChanged(juce::TextEditor& editor) override;
        
        //! @brief Paints elements over the text editor.
        void paintOverChildren (juce::Graphics& g) override;
        
    private: // deleted methods
        
        ClassicView() = delete;
        ClassicView(ClassicView const& other) = delete;
        ClassicView(ClassicView && other) = delete;
        ClassicView& operator=(ClassicView const& other) = delete;
        ClassicView& operator=(ClassicView && other) = delete;
    };
}
