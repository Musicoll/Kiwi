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
    
    MessageView::MessageView(model::Object & object_model) :
    EditableObjectView(object_model),
    m_output_message(object_model.getSignal<>(model::Message::Signal::outputMessage)),
    m_active(false)
    {
        juce::Label & label = getLabel();
        
        label.setText(object_model.getAttribute("text")[0].getString(),
                      juce::NotificationType::dontSendNotification);
        
        label.setColour(juce::Label::backgroundColourId, findColour(ObjectView::ColourIds::Background));
        label.setColour(juce::Label::backgroundWhenEditingColourId,
                          findColour(ObjectView::ColourIds::Background));
        label.setColour(juce::Label::textColourId, findColour(ObjectView::ColourIds::Text));
        label.setColour(juce::Label::textWhenEditingColourId, findColour(ObjectView::ColourIds::Text));
        
        label.setInterceptsMouseClicks(false, false);

        addAndMakeVisible(label);
    }

    MessageView::~MessageView()
    {
    }
    
    void MessageView::mouseDown(juce::MouseEvent const& e)
    {
        m_active = true;
        
        repaint();
        
        m_output_message();
    }
    
    void MessageView::mouseUp(juce::MouseEvent const& e)
    {
        m_active = false;
        
        repaint();
    }
    
    void MessageView::paintOverChildren (juce::Graphics& g)
    {
        g.setColour (findColour (ObjectView::ColourIds::Outline));
        
        drawOutline(g);
        
        juce::Path corner;
        
        juce::Rectangle<int> bounds = getLocalBounds();
        
        if (m_active)
        {
            g.setColour(findColour(ObjectView::ColourIds::Active));
        }
        
        corner.addTriangle(bounds.getTopRight().toFloat() - juce::Point<float>(10, 0),
                           bounds.getTopRight().toFloat(),
                           bounds.getTopRight().toFloat() + juce::Point<float>(0, 10));
        
        g.fillPath(corner);
    }
    
    void MessageView::resized()
    {
        getLabel().setBounds(getLocalBounds());
    }
    
    void MessageView::textEditorTextChanged(juce::TextEditor& editor)
    {
        const int text_width = editor.getFont().getStringWidth(editor.getText());
        
        if(editor.getWidth() < text_width + 16)
        {
            setSize(text_width + 16, getHeight());
        }
    }
    
    void MessageView::attributeChanged(std::string const& name, tool::Parameter const& param)
    {
        if (name == "text")
        {
            getLabel().setText(param[0].getString(), juce::NotificationType::dontSendNotification);
        }
    }
    
    void MessageView::textChanged()
    {
        juce::Label& label = getLabel();
        
        // Parse text and convert it back to string to display the formated version.
        const auto atoms = tool::AtomHelper::parse(label.getText().toStdString(),
                                                   tool::AtomHelper::ParsingFlags::Comma
                                                   | tool::AtomHelper::ParsingFlags::Dollar);
        
        auto formatted_text = tool::AtomHelper::toString(atoms);
        const int text_width = label.getFont().getStringWidth(formatted_text);
        model::Object & model = getModel();
        model.setWidth(text_width + 16);
        
        // set the attribute and label text with formated text
        getLabel().setText(formatted_text, juce::NotificationType::dontSendNotification);
        
        setAttribute("text", tool::Parameter(tool::Parameter::Type::String,
                                             {std::move(formatted_text)}));
    }
    
    juce::TextEditor* MessageView::createdTextEditor()
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
        
        editor->addListener(this);
        
        return editor;
    }
}
