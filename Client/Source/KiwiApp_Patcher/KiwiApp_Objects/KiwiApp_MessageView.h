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
    //                                   MESSAGE VIEW                                   //
    // ================================================================================ //
    
    //! @brief The *message* object view.
    class MessageView
    : public EditableObjectView
    , public juce::TextEditor::Listener
    {
    public: // methods
        
        // @brief The declaration method.
        static void declare();
        
        //! @brief Creation method.
        static std::unique_ptr<ObjectView> create(model::Object & object_model);
        
        //! @brief Constructor.
        MessageView(model::Object & object_model);
        
        //! @brief Destructor.
        ~MessageView();
        
    private: // methods
        
        //! @brief Called when the message is clicked.
        void mouseDown(juce::MouseEvent const& e) override;
        
        //! @brief Called when the bang is unclicked.
        void mouseUp(juce::MouseEvent const& e) override;
        
        //! @brief Called when the text is being typed.
        //! @details Used to resize in order to keep text visible.
        void textEditorTextChanged(juce::TextEditor& editor) override;
        
        void paint(juce::Graphics& g) override;
        
        //! @brief Paints elements over the text editor.
        void paintOverChildren (juce::Graphics& g) override;
        
        //! @brief Called whenever one of the object's attribute has changed.
        void attributeChanged(std::string const& name, tool::Parameter const& param) override;
        
        //! @brief Called when the label text has changed.
        //! @details Overrides EditableObjectView::textChanged.
        void textChanged() override;
        
        //! @brief Constructs the label's text editor.
        //! @details Overrides EditableOjectView::createTextEditor.
        juce::TextEditor* createdTextEditor() override;
        
    private: // members
        
        flip::Signal<> &    m_output_message;
        bool                m_active;
        
    private: // deleted methods
        
        MessageView() = delete;
        MessageView(MessageView const& other) = delete;
        MessageView(MessageView && other) = delete;
        MessageView& operator=(MessageView const& other) = delete;
        MessageView& operator=(MessageView && other) = delete;
    };
}
