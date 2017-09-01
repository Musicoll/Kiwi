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

namespace kiwi
{
    // ================================================================================ //
    //                                   CLASSIC VIEW                                   //
    // ================================================================================ //
    
    ClassicView::ClassicView(model::Object & object_model) :
    ObjectView(object_model),
    m_text(object_model.getText()),
    m_editor(nullptr),
    m_indent(4),
    m_listeners(),
    m_deleted(false)
    {
        if(object_model.getName() == "errorbox")
        {
            setColour(ObjectView::ColourIds::Background, findColour(ObjectView::ColourIds::Error).withAlpha(0.4f));
        }
    }

    ClassicView::~ClassicView()
    {
        removeTextEditor();
        m_deleted = true;
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
        if(hasKeyboardFocus(true)) return; // abort
        
        m_editor.reset(new SuggestEditor(model::Factory::getNames()));
        
        m_editor->setBounds(getLocalBounds());
        m_editor->setIndents(m_indent, m_indent);
        m_editor->setBorder(juce::BorderSize<int>(0));
        
        m_editor->setColour(juce::TextEditor::ColourIds::textColourId,
                            findColour(ObjectView::ColourIds::Text));
        
        m_editor->setColour(juce::TextEditor::backgroundColourId,
                            findColour(ObjectView::ColourIds::Background));
        
        m_editor->setColour(juce::TextEditor::highlightColourId,
                            findColour(ObjectView::ColourIds::Highlight).withAlpha(0.4f));
        
        m_editor->setColour(juce::TextEditor::highlightColourId,
                            findColour(ObjectView::ColourIds::Highlight).withAlpha(0.4f));
        
        m_editor->setColour(juce::TextEditor::outlineColourId,
                            juce::Colours::transparentWhite);
        
        m_editor->setColour(juce::TextEditor::focusedOutlineColourId,
                            juce::Colours::transparentWhite);
        
        m_editor->setScrollbarsShown(false);
        m_editor->setScrollToShowCursor(true);
        m_editor->setReturnKeyStartsNewLine(false);
        m_editor->setMultiLine(true, false);
        
        m_editor->setText(m_text, juce::dontSendNotification);
        m_editor->setHighlightedRegion({0, static_cast<int>(m_text.length())});
        m_editor->setCaretVisible(m_text.empty());
        
        m_editor->addListener(this);
        addAndMakeVisible(*m_editor);
        
        m_editor->grabKeyboardFocus();
    }
    
    void ClassicView::removeTextEditor()
    {
        if(m_editor != nullptr)
        {
            m_editor->removeListener(this);
            removeChildComponent(m_editor.get());
            m_editor.reset();
        }
    }
    
    void ClassicView::paintOverChildren (juce::Graphics& g)
    {
        g.setColour (findColour (ObjectView::ColourIds::Outline));
        
        drawOutline(g);
    }
    
    juce::Rectangle<int> ClassicView::getTextArea() const
    {
        return getLocalBounds().reduced(m_indent);
    }
    
    void ClassicView::paint(juce::Graphics& g)
    {
        g.fillAll (findColour (ObjectView::ColourIds::Background));
        
        if (!isEditing())
        {
            g.setColour (findColour (ObjectView::ColourIds::Text));
            
            g.drawText(m_text, getTextArea(), juce::Justification::centredLeft);
        }
    }
    
    void ClassicView::resized()
    {
        if (m_editor)
        {
            m_editor->setBounds(getLocalBounds());
        }
    }
    
    bool ClassicView::isEditing() const
    {
        return m_editor != nullptr;
    }
    
    void ClassicView::textEditorTextChanged(juce::TextEditor& editor)
    {
        const int text_width = editor.getFont().getStringWidth(editor.getText());
        
        if(editor.getWidth() < text_width + 16)
        {
            setSize(text_width + 16, getHeight());
        }
    }
    
    void ClassicView::textEdited(juce::TextEditor& editor)
    {
        assert(&editor == m_editor.get());
        
        m_listeners.call(&Listener::textEdited, m_text);
        
        if (!m_deleted)
        {
            removeTextEditor();
        }
    }
    
    void ClassicView::textEditorReturnKeyPressed(juce::TextEditor& editor)
    {
        m_text = editor.getText().toStdString();
        
        textEdited(editor);
    }
    
    void ClassicView::textEditorFocusLost(juce::TextEditor& editor)
    {
        m_text = editor.getText().toStdString();
        
        textEdited(editor);
    }
}
