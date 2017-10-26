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

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectView.h>

namespace kiwi
{
    // ================================================================================ //
    //                                   CLASSIC VIEW                                   //
    // ================================================================================ //
    
    //! @brief The view of any textual kiwi object.
    class ClassicView : public ObjectView,
                        public juce::TextEditor::Listener,
                        public juce::Label::Listener
    {
    public: // classes
        
        class Label : public juce::Label
        {
        public: // methods
            
            //! @brief Constructor.
            Label(ClassicView & classic_view, std::string const& label);
            
            //! @brief Destructor.
            ~Label();
            
            //! @brief Called to create the text editor once edit is called.
            juce::TextEditor* createEditorComponent() override final;
            
        private: // members
            
            ClassicView & m_classic_view;
        };
        
        struct Listener
        {
            //! @brief Destructor.
            virtual ~Listener() = default;
            
            //! @brief Called when the text has been edited and return key was pressed.
            virtual void textChanged(std::string const& new_text) =  0;
            
            //! @brief Called when the classic view ends its edition.
            virtual void editorHidden() =  0;
            
            //! @brief Called when the classic view enters its edition mode.
            virtual void editorShown() =  0;
        };
        
    public: // methods
        
        //! @brief Constructor.
        ClassicView(model::Object & object_model);
        
        //! @brief Destructor.
        ~ClassicView();
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief Remove a listener.
        void removeListener(Listener& listener);
        
        //! @brief Lets the user edit the text of the box.
        void edit();
        
        //! @brief Returns true if the classic view is currently in edition mode.
        bool isEditing() const;
        
    private: // methods
        
        //! @brief Called when the object is resized.
        void resized() override final;
        
        //! @brief Called when the text is being typed.
        //! @details Used to resize in order to keep text visible.
        void textEditorTextChanged(juce::TextEditor& editor) override final;
        
        //! @brief Paints elements over the text editor.
        void paintOverChildren (juce::Graphics& g) override final;
        
        //! @brief Called when the label text has changed.
        void labelTextChanged (juce::Label* label) override final;
        
        //! @brief Called when edition ends.
        void editorHidden (juce::Label* label, juce::TextEditor& text_editor) override final;
        
        //! @brief Called when edition begins.
        void editorShown(juce::Label* label, juce::TextEditor& text_editor) override final;
        
    private: // members
        
        Label                               m_label;
        tool::Listeners<Listener>         m_listeners;
        
    private: // deleted methods
        
        ClassicView() = delete;
        ClassicView(ClassicView const& other) = delete;
        ClassicView(ClassicView && other) = delete;
        ClassicView& operator=(ClassicView const& other) = delete;
        ClassicView& operator=(ClassicView && other) = delete;
    };
}
