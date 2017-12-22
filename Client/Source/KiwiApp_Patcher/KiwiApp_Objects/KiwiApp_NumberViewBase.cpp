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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Number.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_NumberViewBase.h>
#include <KiwiApp_Patcher/KiwiApp_Factory.h>

namespace kiwi {
    
    // ================================================================================ //
    //                                   NUMBER VIEW BASE                               //
    // ================================================================================ //
    
    NumberViewBase::NumberViewBase(model::Object & object_model) :
    EditableObjectView(object_model),
    m_value(0),
    m_indent(9),
    m_icon_colour(findColour (ObjectView::ColourIds::Outline))
    {
        juce::Label & label = getLabel();
        
        label.setText("0.", juce::NotificationType::dontSendNotification);
        
        label.setColour(juce::Label::backgroundColourId, findColour(ObjectView::ColourIds::Background));
        label.setColour(juce::Label::backgroundWhenEditingColourId,
                        findColour(ObjectView::ColourIds::Background));
        label.setColour(juce::Label::textColourId, findColour(ObjectView::ColourIds::Text));
        label.setColour(juce::Label::textWhenEditingColourId, findColour(ObjectView::ColourIds::Text));
        label.setMinimumHorizontalScale(1);
        label.setInterceptsMouseClicks(false, false);
        
        addAndMakeVisible(label);
    }
    
    NumberViewBase::~NumberViewBase()
    {
    }
    
    void NumberViewBase::paint(juce::Graphics & g)
    {
        g.fillAll(findColour(ObjectView::ColourIds::Background));
    }
    
    void NumberViewBase::paintOverChildren (juce::Graphics& g)
    {
        g.setColour (findColour (ObjectView::ColourIds::Outline));
        
        drawOutline(g);
        
        juce::Path corner;
        
        g.setColour(m_icon_colour);
        
        juce::Rectangle<int> triangle_bounds = getLocalBounds().withWidth(m_indent - 4).withHeight(getHeight() - 8).translated(2, 4);
        
        corner.addTriangle(triangle_bounds.getTopLeft().toFloat(),
                           triangle_bounds.getTopRight().toFloat() + juce::Point<float>(0, (triangle_bounds.getHeight() / 2.)),
                           triangle_bounds.getBottomLeft().toFloat());
        
        g.fillPath(corner);
    }
    
    void NumberViewBase::resized()
    {
        juce::Rectangle<int> label_bounds = getLocalBounds();
        label_bounds.removeFromLeft(m_indent);
        getLabel().setBounds(label_bounds);
    }
    
    void NumberViewBase::setIconColour(juce::Colour colour)
    {
        m_icon_colour = colour;
    }
    
    double NumberViewBase::getDisplayNumber() const
    {
        return m_value;
    }
    
    void NumberViewBase::setDisplayNumber(double number)
    {
        m_value = number;
        
        juce::String display_value(std::to_string(m_value));
        
        display_value = display_value.trimCharactersAtEnd("0");
        
        getLabel().setText(display_value, juce::NotificationType::dontSendNotification);
    }
    
    void NumberViewBase::textChanged()
    {
        m_value = getLabel().getText().getFloatValue();
        
        displayNumberChanged(m_value);
    }
    
    juce::TextEditor* NumberViewBase::createdTextEditor()
    {
        juce::TextEditor * editor = new juce::TextEditor();
        
        editor->setBounds(getLocalBounds());
        editor->setBorder(juce::BorderSize<int>(0));
        
        
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
        
        editor->setScrollbarsShown(false);
        editor->setScrollToShowCursor(true);
        editor->setReturnKeyStartsNewLine(false);
        editor->setMultiLine(true, false);
        editor->setInterceptsMouseClicks(true, false);
        
        editor->setInputFilter(new juce::TextEditor::LengthAndCharacterRestriction(-1, "0123456789.-"), true);
        
        return editor;
    }
}

