/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiModel/KiwiModel_DocumentManager.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_MessageView.h>
#include <KiwiApp_Patcher/KiwiApp_Factory.h>

namespace kiwi {
    
    // ================================================================================ //
    //                                   MESSAGE VIEW                                   //
    // ================================================================================ //
    
    void MessageView::declare()
    {
        Factory::add<MessageView>("message", &MessageView::create);
    }
    
    std::unique_ptr<ObjectView> MessageView::create(model::Object & object_model)
    {
        return std::make_unique<MessageView>(object_model);
    }
    
    MessageView::MessageView(model::Object & object_model)
    : EditableObjectView(object_model)
    , m_output_message(object_model.getSignal<>(model::Message::Signal::outputMessage))
    , m_active(false)
    {
        setColour(ObjectView::ColourIds::Background, juce::Colours::whitesmoke);
        setColour(ObjectView::ColourIds::Outline, juce::Colours::whitesmoke.withAlpha(0.f));
        
        juce::Label & label = getLabel();
        const auto object_text = object_model.getAttribute("text")[0].getString();
        label.setText(object_text, juce::NotificationType::dontSendNotification);
        
        label.setColour(juce::Label::backgroundColourId,
                        findColour(ObjectView::ColourIds::Background));
        
        label.setColour(juce::Label::backgroundWhenEditingColourId,
                        findColour(ObjectView::ColourIds::Background));
        
        label.setColour(juce::Label::textColourId,
                        findColour(ObjectView::ColourIds::Text));
        
        label.setColour(juce::Label::textWhenEditingColourId,
                        findColour(ObjectView::ColourIds::Text));

        addAndMakeVisible(label);
    }

    MessageView::~MessageView()
    {}
    
    void MessageView::mouseDown(juce::MouseEvent const& e)
    {
        m_active = true;
        m_output_message();
        repaint();
    }
    
    void MessageView::mouseUp(juce::MouseEvent const& e)
    {
        m_active = false;
        repaint();
    }
    
    void MessageView::paint(juce::Graphics& g)
    {
        const float roundness = 3.f;
        const float bordersize = 1.f;
        const auto bounds = getLocalBounds().toFloat().reduced(bordersize*0.5f);
        const auto bgcolor = findColour(ObjectView::ColourIds::Background);
        const auto active_color = findColour(ObjectView::ColourIds::Active);
        const bool clicked = m_active;
        
        g.setColour(bgcolor.contrasting(clicked ? 0.1 : 0.));
        g.fillRoundedRectangle(bounds, roundness);
        
        g.setColour(clicked ? active_color : bgcolor.contrasting(0.25));
        g.drawRoundedRectangle(bounds, roundness, bordersize);
    }
    
    void MessageView::paintOverChildren (juce::Graphics& g)
    {
        if(getLabel().isBeingEdited())
            return; // abort
        
        g.setColour(m_active
                    ? findColour(ObjectView::ColourIds::Active)
                    : findColour(ObjectView::ColourIds::Background).contrasting(0.25));
        
        const auto bounds = getLocalBounds();
        const auto topright = bounds.getTopRight().toFloat();
        
        juce::Path corner;
        corner.addTriangle(topright.translated(-10.f, 0.f),
                           topright,
                           topright.translated(0, 10));
        
        g.fillPath(corner);
    }
    
    void MessageView::textEditorTextChanged(juce::TextEditor& editor)
    {
        const auto text = editor.getText();
        auto single_line_text_width = editor.getFont().getStringWidthFloat(text) + 16 + 2;
        auto width = std::max<float>(getWidth() + 16, single_line_text_width);
        
        auto prev_width = getWidth();
        auto text_bounds = getTextBoundingBox(text, width);
        
        setSize(std::max<int>(prev_width, single_line_text_width),
                std::max(std::min<int>(text_bounds.getHeight(), getHeight()), getMinHeight()));
    }
    
    void MessageView::attributeChanged(std::string const& name, tool::Parameter const& param)
    {
        static const std::string name_text = "text";
        if (name == name_text)
        {
            setText(param[0].getString());
            checkComponentBounds(this);
        }
    }
    
    void MessageView::textChanged()
    {
        // Parse text and convert it back to string to display the formated version.
        const auto atoms = tool::AtomHelper::parse(getText().toStdString(),
                                                   tool::AtomHelper::ParsingFlags::Comma
                                                   | tool::AtomHelper::ParsingFlags::Dollar);
        
        auto formatted_text = tool::AtomHelper::toString(atoms);
        const int text_width = getFont().getStringWidth(formatted_text);
        model::Object & model = getModel();
        
        model.setWidth(std::max(26, text_width) + 10);
        model.setHeight(getMinHeight());
        
        // set the attribute and label text with formated text
        getLabel().setText(formatted_text, juce::NotificationType::dontSendNotification);
        
        setAttribute("text", tool::Parameter(tool::Parameter::Type::String,
                                             {std::move(formatted_text)}));
    }
    
    juce::TextEditor* MessageView::createdTextEditor()
    {
        auto* editor = new juce::TextEditor();
        
        editor->setColour(juce::TextEditor::ColourIds::textColourId,
                          getLabel().findColour(juce::Label::textWhenEditingColourId));
        
        editor->setColour(juce::TextEditor::backgroundColourId,
                          juce::Colours::transparentWhite);
        
        editor->setColour(juce::TextEditor::highlightColourId,
                          findColour(ObjectView::ColourIds::Highlight, true).withAlpha(0.4f));
        
        editor->setColour(juce::TextEditor::outlineColourId,
                          juce::Colours::transparentWhite);
        
        editor->setColour(juce::TextEditor::focusedOutlineColourId,
                          juce::Colours::transparentWhite);
        
        editor->setBounds(getLocalBounds());
        editor->setIndents(getPadding(), getPadding());
        editor->setBorder(juce::BorderSize<int>(0));
        editor->setFont(getFont());
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
