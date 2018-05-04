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

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_CommentView.h>
#include <KiwiApp_Patcher/KiwiApp_Factory.h>

namespace kiwi {
    
    // ================================================================================ //
    //                                   COMMENT VIEW                                   //
    // ================================================================================ //
    
    void CommentView::declare()
    {
        Factory::add<CommentView>("comment", &CommentView::create);
    }
    
    std::unique_ptr<ObjectView> CommentView::create(model::Object & object_model)
    {
        return std::make_unique<CommentView>(object_model);
    }
    
    CommentView::CommentView(model::Object & object_model) :
    EditableObjectView(object_model)
    {
        setColour(ObjectView::ColourIds::Background, juce::Colours::transparentWhite);
        
        juce::Label & label = getLabel();
        
        label.setJustificationType(juce::Justification::topLeft);
        
        label.setText(object_model.getAttribute("text")[0].getString(),
                      juce::NotificationType::dontSendNotification);
        
        label.setColour(juce::Label::backgroundColourId, findColour(ObjectView::ColourIds::Background));
        label.setColour(juce::Label::backgroundWhenEditingColourId,
                          findColour(ObjectView::ColourIds::Background));
        label.setColour(juce::Label::textColourId, findColour(ObjectView::ColourIds::Text));
        label.setColour(juce::Label::textWhenEditingColourId, findColour(ObjectView::ColourIds::Text));

        addAndMakeVisible(label);
    }

    CommentView::~CommentView()
    {
    }
    
    void CommentView::paintOverChildren (juce::Graphics& g)
    {
        g.setColour (findColour (ObjectView::ColourIds::Outline));
        
        drawOutline(g);
    }
    
    void CommentView::resized()
    {
        getLabel().setBounds(getLocalBounds());
    }
    
    void CommentView::textEditorTextChanged(juce::TextEditor& editor)
    {
        setSize(std::max(editor.getTextWidth() + 16, 40), editor.getTextHeight());
    }
    
    void CommentView::attributeChanged(std::string const& name, tool::Parameter const& param)
    {
        if (name == "text")
        {
            getLabel().setText(param[0].getString(), juce::NotificationType::dontSendNotification);
        }
    }
    
    void CommentView::textChanged()
    {
        model::Object & model = getModel();
        
        model.setWidth(getWidth());
        
        model.setHeight(getHeight());
        
        setAttribute("text", tool::Parameter(tool::Parameter::Type::String,
                                             {getLabel().getText().toStdString()}));
    }
    
    juce::TextEditor* CommentView::createdTextEditor()
    {
        juce::TextEditor * editor = new juce::TextEditor();
        
        editor->setBounds(getLocalBounds());
        editor->setBorder(juce::BorderSize<int>(0));
        editor->setFont(getLabel().getFont());
        
        
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
        editor->setReturnKeyStartsNewLine(true);
        editor->setMultiLine(true, true);
        editor->setInterceptsMouseClicks(true, false);
        
        editor->addListener(this);
        
        return editor;
    }
}
