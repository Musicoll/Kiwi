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
    
    CommentView::CommentView(model::Object & object_model)
    : EditableObjectView(object_model)
    {
        setColour(ObjectView::ColourIds::Background, juce::Colours::transparentWhite);
        
        juce::Label& label = getLabel();
        
        const auto comment_text = object_model.getAttribute("text")[0].getString();
        label.setText(comment_text, juce::NotificationType::dontSendNotification);
        
        label.setColour(juce::Label::backgroundColourId,
                        findColour(ObjectView::ColourIds::Background));
        
        label.setColour(juce::Label::backgroundWhenEditingColourId,
                        findColour(ObjectView::ColourIds::Background));
        
        label.setColour(juce::Label::textColourId,
                        findColour(ObjectView::ColourIds::Text).brighter(0.4));
        
        label.setColour(juce::Label::textWhenEditingColourId,
                        findColour(ObjectView::ColourIds::Text));

        addAndMakeVisible(label);
    }

    CommentView::~CommentView()
    {}
    
    void CommentView::lockStatusChanged(bool is_locked)
    {
        m_patcher_view_locked = is_locked;
        repaint();
    }
    
    void CommentView::paintOverChildren (juce::Graphics& g)
    {
        if(!m_patcher_view_locked)
        {
            // dashed outline
            juce::Path path;
            path.addRectangle(getLocalBounds());
            const juce::PathStrokeType path_stroke(1.f);
            float const dashed_length[2] {2.f, 2.f};
            path_stroke.createDashedStroke(path, path, dashed_length, 2);
            
            g.setColour(findColour (ObjectView::ColourIds::Outline));
            g.strokePath(path, path_stroke);
        }
    }
    
    void CommentView::textEditorTextChanged(juce::TextEditor& editor)
    {
        setSize(std::max(getWidth(), getMinWidth()),
                std::max(editor.getTextHeight() + 2, getMinHeight()));
    }
    
    void CommentView::attributeChanged(std::string const& name, tool::Parameter const& param)
    {
        static const std::string text_param = "text";
        
        if (name == text_param)
        {
            const auto new_text = param[0].getString();
            getLabel().setText(new_text, juce::NotificationType::dontSendNotification);
            checkComponentBounds(this);
        }
    }
    
    void CommentView::textChanged()
    {
        const auto new_text = getLabel().getText().toStdString();
        setAttribute("text", tool::Parameter(tool::Parameter::Type::String, {new_text}));
    }
    
    juce::TextEditor* CommentView::createdTextEditor()
    {
        auto* editor = new juce::TextEditor();
        
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
        editor->setFont(getFont());
        editor->setJustification(getLabel().getJustificationType());
        
        editor->setScrollbarsShown(false);
        editor->setScrollToShowCursor(true);
        editor->setReturnKeyStartsNewLine(true);
        editor->setMultiLine(true, true);
        editor->setInterceptsMouseClicks(true, false);
        
        editor->addListener(this);
        return editor;
    }
}
