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

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ClassicView.h>

#include <KiwiApp_Components/KiwiApp_SuggestEditor.h>

namespace kiwi
{
    // ================================================================================ //
    //                                   CLASSIC VIEW                                   //
    // ================================================================================ //
    
    ClassicView::ClassicView(model::Object & object_model) :
    EditableObjectView(object_model)
    {
        juce::Label & label = getLabel();
        
        const auto object_text = object_model.getText();
        label.setText(object_text, juce::NotificationType::dontSendNotification);
        
        juce::Colour bg_colour = (object_model.getName() == "errorbox" ?
                                  findColour(ObjectView::ColourIds::Error) :
                                  findColour(ObjectView::ColourIds::Background));
        
        setColour(ObjectView::ColourIds::Background, bg_colour);
        setColour(ObjectView::ColourIds::Outline, juce::Colours::transparentBlack);
        
        label.setColour(juce::Label::backgroundColourId, bg_colour);
        label.setColour(juce::Label::backgroundWhenEditingColourId, bg_colour);
        label.setColour(juce::Label::textColourId, findColour(ObjectView::ColourIds::Text));
        label.setColour(juce::Label::textWhenEditingColourId, findColour(ObjectView::ColourIds::Text));
        
        addAndMakeVisible(label);
    }

    ClassicView::~ClassicView()
    {}
    
    void ClassicView::validateSize(int& new_width, int& new_height)
    {
        new_width = std::max(new_width, getMinWidth());
        const auto text_bounds = getTextBoundingBox(getLabel().getText(), new_width);
        new_height = std::max<int>(text_bounds.getHeight(), getMinHeight());
    }
    
    void ClassicView::paintOverChildren (juce::Graphics& g)
    {
        g.setColour(findColour(EditableObjectView::ColourIds::Background).contrasting(0.4));
        g.drawLine(0, 0, getWidth(), 0, 6);
        g.drawLine(0, getHeight(), getWidth(), getHeight(), 6);
    }
    
    void ClassicView::textChanged()
    {}
    
    void ClassicView::textEditorTextChanged(juce::TextEditor& editor)
    {
        const auto text = editor.getText();
        
        auto single_line_text_width = editor.getFont().getStringWidthFloat(text) + getPadding() * 2 + 10;
        auto prev_width = getWidth();
        auto text_bounds = getTextBoundingBox(text, single_line_text_width);
        
        setSize(std::max<int>(prev_width, single_line_text_width),
                std::max(std::min<int>(text_bounds.getHeight(), getHeight()), getMinHeight()));
    }
    
    juce::TextEditor* ClassicView::createdTextEditor()
    {
        juce::TextEditor * editor = new SuggestEditor(model::Factory::getNames());
        
        editor->setColour(juce::TextEditor::ColourIds::textColourId,
                          getLabel().findColour(juce::Label::textWhenEditingColourId));
        
        editor->setColour(juce::TextEditor::backgroundColourId,
                          getLabel().findColour(juce::Label::backgroundWhenEditingColourId));
        
        editor->setColour(juce::TextEditor::highlightColourId,
                          findColour(ObjectView::ColourIds::Highlight, true).withAlpha(0.4f));
        
        editor->setColour(juce::TextEditor::outlineColourId,
                          juce::Colours::transparentWhite);
        
        editor->setColour(juce::TextEditor::focusedOutlineColourId,
                          juce::Colours::transparentWhite);
        
        editor->setBounds(getLocalBounds());
        editor->setIndents(getPadding(), getPadding());
        editor->setBorder(juce::BorderSize<int>(0));
        editor->setFont(getLabel().getFont());
        editor->setJustification(getLabel().getJustificationType());
        
        editor->setScrollbarsShown(false);
        editor->setScrollToShowCursor(true);
        editor->setReturnKeyStartsNewLine(false);
        editor->setMultiLine(true, true);
        editor->setInterceptsMouseClicks(true, false);
        
        editor->addListener(this);
        
        return editor;
    }
}
