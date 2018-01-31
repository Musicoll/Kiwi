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
    
    EditableObjectView::EditableObjectView(model::Object & object_model) :
    ObjectView(object_model),
    m_label(*this),
    m_editable(true),
    m_listeners()
    {
    }

    EditableObjectView::~EditableObjectView()
    {
    }

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
    
    void EditableObjectView::labelTextChanged (juce::Label* label)
    {
        textChanged();
        
        m_listeners.call(&EditableObjectView::Listener::textChanged, m_label.getText().toStdString());
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
    
    // ================================================================================ //
    //                                   LABEL                                          //
    // ================================================================================ //
    
    EditableObjectView::Label::Label(EditableObjectView & object_view):
    juce::Label(),
    m_object_view(object_view)
    {
        addListener(&m_object_view);
    }
    
    EditableObjectView::Label::~Label()
    {
        removeListener(&m_object_view);
    }
    
    juce::TextEditor * EditableObjectView::Label::createEditorComponent()
    {
        return m_object_view.createdTextEditor();
    }
    
}
