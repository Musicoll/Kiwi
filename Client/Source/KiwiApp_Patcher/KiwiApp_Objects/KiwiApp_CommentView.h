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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Message.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_EditableObjectView.h>

namespace kiwi {
    
    // ================================================================================ //
    //                                   COMMENT VIEW                                   //
    // ================================================================================ //
    
    //! @brief The *comment* object view.
    class CommentView
    : public EditableObjectView
    , public juce::TextEditor::Listener
    {
    public: // methods
        
        // @brief The declaration method.
        static void declare();
        
        //! @brief Creation method.
        static std::unique_ptr<ObjectView> create(model::Object & object_model);
        
        //! @brief Constructor.
        CommentView(model::Object & object_model);
        
        //! @brief Destructor.
        ~CommentView();
        
    private: // methods
        
        //! @brief Validate the new width and height for the box
        void validateSize(int& new_width, int& new_height) override;
        
        //! @brief Called every time a patcher is locked or unlocked.
        void lockStatusChanged(bool is_locked) override;
        
        //! @brief Called when the text is being typed.
        //! @details Used to resize in order to keep text visible.
        void textEditorTextChanged(juce::TextEditor& editor) override final;
        
        //! @brief Paints elements over the text editor.
        void paintOverChildren (juce::Graphics& g) override final;
        
        //! @brief Called whenever one of the object's attribute has changed.
        void attributeChanged(std::string const& name, tool::Parameter const& param) override final;
        
        //! @brief Called when the label text has changed.
        //! @details Overrides EditableObjectView::textChanged.
        void textChanged() override final;
        
        //! @brief Constructs the label's text editor.
        //! @details Overrides EditableOjectView::createTextEditor.
        juce::TextEditor* createdTextEditor() override final;
    
    private: // variables
        
        bool m_patcher_view_locked {false};
        
    private: // deleted methods
        
        CommentView() = delete;
        CommentView(CommentView const& other) = delete;
        CommentView(CommentView && other) = delete;
        CommentView& operator=(CommentView const& other) = delete;
        CommentView& operator=(CommentView && other) = delete;
    };
}
