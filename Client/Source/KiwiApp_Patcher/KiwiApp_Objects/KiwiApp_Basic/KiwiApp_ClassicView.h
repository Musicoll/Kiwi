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

#include <KiwiEngine/KiwiEngine_Listeners.h>

#include <KiwiApp_Patcher/KiwiApp_ObjectView.h>
#include <KiwiApp_Components/KiwiApp_SuggestEditor.h>

namespace kiwi
{
    // ================================================================================ //
    //                                   CLASSIC VIEW                                   //
    // ================================================================================ //
    
    //! @brief The view of any textual kiwi object.
    class ClassicView : public ObjectView, public juce::TextEditor::Listener
    {
    public: // classes
        
        struct Listener
        {
            //! @brief Destructor.
            virtual ~Listener() = default;
            
            //! @brief Called when the text has been edited and return key was pressed.
            virtual void textEdited(std::string const& new_text) =  0;
        };
        
    public: // methods
        
        //! @brief Constructor.
        ClassicView(model::Object& object_model);
        
        //! @brief Destructor.
        ~ClassicView();
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief Remove a listener.
        void removeListener(Listener& listener);
        
        //! @brief The component's graphical rendering method.
        void paint(juce::Graphics& g) override final;
        
        //! @brief Called when the object is resized.
        void resized() override final;
        
        //! @brief Lets the user edit the text of the box.
        void edit();
        
    private: // methods
        
        //! @brief Called when the text is being typed.
        //! @details Used to resize in order to keep text visible.
        void textEditorTextChanged(juce::TextEditor& editor) override final;
        
        //! @brief Called when escape key is pressed.
        //! @details Exits edit mode without keeping typed text.
        void textEditorEscapeKeyPressed(juce::TextEditor& editor) override final;
        
        //! @brief Called when return key is pressed.
        //! @brief Exits edit mode keeping edited text as is.
        void textEditorReturnKeyPressed(juce::TextEditor& editor) override final;
        
        //! @brief Called when the editor has lost focus.
        //! @brief Exits edit mode keeping edited text as is.
        void textEditorFocusLost(juce::TextEditor& editor) override final;
        
        //! @brief Returns the text area.
        juce::Rectangle<int> getTextArea() const;
        
        //! @brief Paints elements over the text editor.
        void paintOverChildren (juce::Graphics& g) override final;
        
        //! @brief Called when the classic's view text has been edited.
        void textEdited(juce::TextEditor& editor);
        
        ///! @brief Calls this to end edition and supress the text editor.
        void removeTextEditor();
        
        //! @brief Returns true if the classic view is currently in edition mode.
        bool isEditing() const;
        
    private: // members
        
        std::string                         m_text;
        std::unique_ptr<juce::TextEditor>   m_editor;
        size_t                              m_indent;
        juce::BorderSize<int>               m_border;
        engine::Listeners<Listener>         m_listeners;
    };
}
