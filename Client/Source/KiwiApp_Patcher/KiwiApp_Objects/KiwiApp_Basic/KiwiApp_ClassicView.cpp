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

#include <KiwiModel/KiwiModel_Factory.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_Basic/KiwiApp_ClassicView.h>

#include <KiwiApp_Components/KiwiApp_SuggestEditor.h>

namespace kiwi
{
    // ================================================================================ //
    //                                   CLASSIC VIEW                                   //
    // ================================================================================ //
    
    ClassicView::ClassicView(model::Object & object_model) :
    ObjectView(object_model),
    m_label(*this, object_model.getText()),
    m_listeners()
    {
        juce::Colour bg_colour = object_model.getName() == "errorbox" ?
                                 findColour(ObjectView::ColourIds::Error).withAlpha(0.4f) :
                                 findColour(ObjectView::ColourIds::Background);
        
        
        m_label.setColour(juce::Label::backgroundColourId, bg_colour);
        m_label.setColour(juce::Label::backgroundWhenEditingColourId, bg_colour);
        m_label.setColour(juce::Label::textColourId, findColour(ObjectView::ColourIds::Text));
        m_label.setColour(juce::Label::textWhenEditingColourId, findColour(ObjectView::ColourIds::Text));
        
        addAndMakeVisible(m_label);
    }

    ClassicView::~ClassicView()
    {
    }
    
    void ClassicView::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void ClassicView::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    void ClassicView::edit()
    {
        m_label.showEditor();
    }
    
    void ClassicView::paintOverChildren (juce::Graphics& g)
    {
        g.setColour (findColour (ObjectView::ColourIds::Outline));
        
        drawOutline(g);
    }
    
    void ClassicView::resized()
    {
        m_label.setBounds(getLocalBounds());
    }
    
    bool ClassicView::isEditing() const
    {
        return m_label.isBeingEdited();
    }
    
    void ClassicView::textEditorTextChanged(juce::TextEditor& editor)
    {
        const int text_width = editor.getFont().getStringWidth(editor.getText());
        
        if(editor.getWidth() < text_width + 16)
        {
            setSize(text_width + 16, getHeight());
        }
    }
    
    void ClassicView::labelTextChanged (juce::Label* labelThatHasChanged)
    {
        m_listeners.call(&ClassicView::Listener::textChanged, m_label.getText().toStdString());
    }
    
    void ClassicView::editorHidden (juce::Label* label, juce::TextEditor& text_editor)
    {
        m_listeners.call(&ClassicView::Listener::editorHidden);
    }
    
    void ClassicView::editorShown (juce::Label* label, juce::TextEditor& text_editor)
    {
        m_listeners.call(&ClassicView::Listener::editorShown);
    }
    
    // ================================================================================ //
    //                                   LABEL                                          //
    // ================================================================================ //
    
    ClassicView::Label::Label(ClassicView & classic_view, std::string const& label):
    juce::Label("object name", label),
    m_classic_view(classic_view)
    {
        addListener(&classic_view);
    }
    
    ClassicView::Label::~Label()
    {
        removeListener(&m_classic_view);
    }
    
    juce::TextEditor* ClassicView::Label::createEditorComponent()
    {
        juce::TextEditor * editor = new SuggestEditor(model::Factory::getNames());
        
        editor->setBounds(getLocalBounds());
        editor->setBorder(juce::BorderSize<int>(0));

        
        editor->setColour(juce::TextEditor::ColourIds::textColourId,
                          findColour(juce::Label::textWhenEditingColourId));
        
        editor->setColour(juce::TextEditor::backgroundColourId,
                            findColour(juce::Label::backgroundWhenEditingColourId));
        
        editor->setColour(juce::TextEditor::highlightColourId,
                            findColour(ObjectView::ColourIds::Highlight, true).withAlpha(0.4f));
        
        
        editor->setColour(juce::TextEditor::outlineColourId,
                            juce::Colours::transparentWhite);
        
        editor->setColour(juce::TextEditor::focusedOutlineColourId,
                            juce::Colours::transparentWhite);
        
        editor->setScrollbarsShown(false);
        editor->setScrollToShowCursor(true);
        editor->setReturnKeyStartsNewLine(false);
        editor->setMultiLine(true, false);
        
        editor->addListener(&m_classic_view);
        
        return editor;
    }
}
