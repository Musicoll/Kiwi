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

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_EditableObjectView.h>

namespace kiwi {
    
    // ================================================================================ //
    //                                 EDITABLE OBJECT VIEW                             //
    // ================================================================================ //
    
    EditableObjectView::EditableObjectView(model::Object & object_model)
    : ObjectView(object_model)
    , m_label(*this)
    {
        m_label.setFont(14);
        m_label.setMinimumHorizontalScale(0);
        m_label.setInterceptsMouseClicks(false, false);
        m_label.setBorderSize(juce::BorderSize<int>(getPadding()));
        m_label.setJustificationType(juce::Justification::topLeft);
    }

    EditableObjectView::~EditableObjectView()
    {}

    void EditableObjectView::setEditable(bool editable)
    {
        m_editable = editable;
    }
    
    void EditableObjectView::edit()
    {
        if (m_editable)
        {
            m_label.showEditor();
        }
    }
    
    void EditableObjectView::setText(juce::String const& new_text)
    {
        m_label.setText(new_text, juce::NotificationType::dontSendNotification);
    }
    
    juce::String EditableObjectView::getText() const
    {
        return m_label.getText();
    }
    
    void EditableObjectView::setFont(juce::Font font)
    {
        m_label.setFont(font);
    }
    
    juce::Font EditableObjectView::getFont() const
    {
        return m_label.getFont();
    }
    
    void EditableObjectView::setPadding(int padding)
    {
        m_padding = std::min<int>(0, padding);
    }
    
    int EditableObjectView::getPadding() const
    {
        return m_padding;
    }
    
    int EditableObjectView::getMinHeight() const
    {
        return getFont().getHeight() + m_padding * 2;
    }
    
    int EditableObjectView::getMinWidth() const
    {
        return getFont().getHeight() + m_padding * 2;
    }
    
    void EditableObjectView::resized()
    {
        m_label.setBounds(getLocalBounds());
    }
    
    juce::Rectangle<float> EditableObjectView::getTextBoundingBox(juce::String const& text,
                                                                  float max_width) const
    {
        juce::GlyphArrangement arr;
        const int padding = getPadding();
        const int side_padding = padding * 2;
        const float max_line_width = max_width - side_padding;
        
        arr.addJustifiedText (getFont(), text,
                              padding, padding,
                              max_line_width,
                              juce::Justification::topLeft);
        
        const auto bounds = arr.getBoundingBox(0, -1, true);
        return bounds.withHeight(bounds.getHeight() + side_padding);
    }
    
    void EditableObjectView::labelTextChanged (juce::Label* label)
    {
        textChanged();
        
        m_listeners.call(&EditableObjectView::Listener::textChanged,
                         m_label.getText().toStdString());
    }
    
    juce::Label & EditableObjectView::getLabel()
    {
        return m_label;
    }
    
    void EditableObjectView::editorHidden (juce::Label* label, juce::TextEditor& text_editor)
    {
        m_label.setInterceptsMouseClicks(false, false);
        
        m_listeners.call(&EditableObjectView::Listener::editorHidden);
    }
    
    void EditableObjectView::editorShown(juce::Label* label, juce::TextEditor& text_editor)
    {
        m_label.setInterceptsMouseClicks(true, true);
        
        m_listeners.call(&EditableObjectView::Listener::editorShown);
    }
    
    void EditableObjectView::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void EditableObjectView::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    void EditableObjectView::paint(juce::Graphics& g)
    {
        g.fillAll(findColour(EditableObjectView::ColourIds::Background));
    }
    
    void EditableObjectView::drawLabel(juce::Graphics& g)
    {
        if(!m_label.isBeingEdited())
        {
            g.setColour (m_label.findColour(juce::Label::ColourIds::textColourId));

            g.setFont(getFont());
            g.drawMultiLineText(m_label.getText(),
                                getPadding(),
                                getFont().getHeight(),
                                m_label.getWidth() - getPadding() * 2);
        }
    }
    
    // ================================================================================ //
    //                                   LABEL                                          //
    // ================================================================================ //
    
    EditableObjectView::Label::Label(EditableObjectView & object_view)
    : juce::Label()
    , m_object_view(object_view)
    {
        addListener(&m_object_view);
    }
    
    EditableObjectView::Label::~Label()
    {
        removeListener(&m_object_view);
    }
    
    juce::TextEditor* EditableObjectView::Label::createEditorComponent()
    {
        return m_object_view.createdTextEditor();
    }
    
    void EditableObjectView::Label::paint(juce::Graphics& g)
    {
        m_object_view.drawLabel(g);
    }
    
}
