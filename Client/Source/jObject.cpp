/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiModel/KiwiModelPatcherView.hpp>

#include "KiwiDocumentManager.hpp"
#include "jObject.hpp"
#include "KiwiApp.hpp"
#include "jPatcherHelper.hpp"
#include "jPatcher.hpp"

namespace kiwi
{
    jObject::jObject(jPatcher& patcher_view, model::Object& object_m) :
    m_patcher_view(patcher_view),
    m_model(&object_m),
    m_io_color(0.3, 0.3, 0.3),
    m_selection_width(4.f),
    m_is_selected(m_patcher_view.isSelected(*this)),
    m_is_editing(false),
    m_is_errorbox(false)
    {
        m_inlets = m_model->getNumberOfInlets();
        m_outlets = m_model->getNumberOfOutlets();
        
        m_is_errorbox = (dynamic_cast<model::ErrorBox*>(m_model) != nullptr);
        //m_is_errorbox = (m_model->getName() == "errorbox");
        
        lockStatusChanged(m_patcher_view.isLocked());
        updateBounds();
    }
    
    jObject::~jObject()
    {
        ;
    }
    
    void jObject::updateBounds()
    {
        if(m_model && !m_model->removed())
        {
            const juce::Point<int> origin = m_patcher_view.getOriginPosition();
            
            const juce::Rectangle<int> bounds(m_model->getX() + origin.getX(),
                                              m_model->getY() + origin.getY(),
                                              m_model->getWidth(),
                                              m_model->getHeight());
            
            const auto new_bounds = bounds.expanded(m_selection_width);
            
            m_local_box_bounds = bounds.withPosition(bounds.getX() - new_bounds.getX(),
                                                     bounds.getY() - new_bounds.getY());
            
            setBounds(new_bounds);
        }
    }
    
    void jObject::paint(juce::Graphics & g)
    {
        const auto bounds = getLocalBounds();
        const auto box_bounds = m_local_box_bounds;
        
        const juce::Colour selection_color = juce::Colour::fromFloatRGBA(0., 0.5, 1., 0.8);
        const juce::Colour other_view_selected_color = juce::Colour::fromFloatRGBA(0.8, 0.3, 0.3, 0.3);
        const juce::Colour distant_selected_color(0xAAFF9B71);
        
        const juce::Colour errorbox_overlay_color = juce::Colour::fromFloatRGBA(0.6, 0.1, 0.1, 0.4);
        
        const bool selected = m_is_selected;
        const bool other_selected = ! m_distant_selection.empty();
        const bool other_view_selected = (other_selected &&
                                          m_distant_selection.find(KiwiApp::userID())
                                          != m_distant_selection.end());
        
        const bool distant_selected = ((other_selected && (other_view_selected && m_distant_selection.size() > 1))
                                       || (other_selected && !other_view_selected));
        
        if(selected || other_view_selected || distant_selected)
        {
            const juce::Colour color = selected ? selection_color : other_view_selected ? other_view_selected_color : distant_selected_color;
        
            g.setColour(color.darker(0.4));
            g.drawRect(bounds.reduced(m_selection_width*0.5 + 1), 1);
            
            g.setColour(color);
            g.drawRoundedRectangle(bounds.reduced(m_selection_width*0.5).toFloat(),
                                   m_selection_width*0.5, m_selection_width*0.5);
            
            g.setColour(color.darker(0.4));
            g.drawRoundedRectangle(bounds.reduced(1).toFloat(), m_selection_width*0.5, 1);
        }

        g.setColour(juce::Colours::white);
        g.fillRect(box_bounds);
        
        if(!selected)
        {
            g.setColour(juce::Colours::black);
            g.drawRect(box_bounds);
        }
        
        if(!m_is_editing)
        {
            g.setColour(juce::Colours::black);

            std::string object_name = m_model->getName();
            if(object_name == "errorbox")
            {
                g.drawFittedText(m_model->getText().c_str()+9,
                                 box_bounds.reduced(5),
                                 juce::Justification::centredLeft, 1, 1.);
            }
            else if(object_name != "newbox")
            {
                g.drawFittedText(m_model->getText(),
                                 box_bounds.reduced(5),
                                 juce::Justification::centredLeft, 1, 1.);
            }
        }
        
        if(m_is_errorbox)
        {
            g.setColour(errorbox_overlay_color);
            g.fillRect(box_bounds);
        }
        
        if(!m_is_locked)
        {
            g.setColour(selected ? m_io_color.brighter(0.6) : m_io_color);
            drawInletsOutlets(g);
        }
    }
    
    void jObject::drawInletsOutlets(juce::Graphics & g)
    {
        const juce::Rectangle<int> bounds = m_local_box_bounds;
        
        for(unsigned int i = 0; i < m_inlets; ++i)
        {
            g.fillRect(getInletLocalBounds(i, bounds));
        }
        
        for(unsigned int i = 0; i < m_outlets; ++i)
        {
            g.fillRect(getOutletLocalBounds(i, bounds));
        }
    }
    
    juce::Rectangle<int> jObject::getBoxBounds() const
    {
        return m_local_box_bounds.withPosition(getPosition() + m_local_box_bounds.getPosition());
    }
    
    bool jObject::hitTest(int x, int y)
    {
        bool allow_click;
        bool allow_click_onchild;
        getInterceptsMouseClicks(allow_click, allow_click_onchild);
        
        if (! allow_click)
            return false;
        
        return m_local_box_bounds.contains(x, y);
    }
    
    bool jObject::hitTest(juce::Point<int> const& pt, HitTester& hit) const
    {
        const auto box_bounds = m_local_box_bounds;
        
        // test body and iolets
        if(box_bounds.contains(pt))
        {
            if(!m_is_selected)
            {
                // test inlets
                if(m_inlets > 0 && pt.getY() > m_io_height)
                {
                    for(size_t i = 0; i < m_inlets; ++i)
                    {
                        if(getInletLocalBounds(i, box_bounds).contains(pt))
                        {
                            hit.m_zone    = HitTester::Zone::Inlet;
                            hit.m_index   = i;
                            return true;
                        }
                    }
                }
                
                // test outlets
                if(m_outlets > 0 && pt.getY() > box_bounds.getHeight() - m_io_height)
                {
                    for(size_t i = 0; i < m_outlets; ++i)
                    {
                        if(getOutletLocalBounds(i, box_bounds).contains(pt))
                        {
                            hit.m_zone    = HitTester::Zone::Outlet;
                            hit.m_index   = i;
                            return true;
                        }
                    }
                }
            }
            
            // hit the body of the box
            hit.m_zone    = HitTester::Zone::Inside;
            hit.m_index   = 0;
            return true;
        }
        else if(m_is_selected) // test borders
        {
            const auto resizer_bounds = box_bounds.expanded(m_selection_width, m_selection_width);
            
            if(pt.getY() >= resizer_bounds.getY() && pt.getY() <= box_bounds.getY())
            {
                hit.m_zone = HitTester::Zone::Border;
                hit.m_border |= HitTester::Border::Top;
            }
            
            if(pt.getX() >= box_bounds.getRight() && pt.getX() <= resizer_bounds.getRight())
            {
                hit.m_zone = HitTester::Zone::Border;
                hit.m_border |= HitTester::Border::Right;
            }
            
            if(pt.getY() >= box_bounds.getBottom() && pt.getY() <= resizer_bounds.getBottom())
            {
                hit.m_zone = HitTester::Zone::Border;
                hit.m_border |= HitTester::Border::Bottom;
            }
            
            if(pt.getX() <= box_bounds.getX() && pt.getX() >= resizer_bounds.getX())
            {
                hit.m_zone = HitTester::Zone::Border;
                hit.m_border |= HitTester::Border::Left;
            }
            
            // remove Border::None flag if we hit a border.
            if(hit.m_zone == HitTester::Zone::Border)
            {
                hit.m_border &= ~HitTester::Border::None;
                return true;
            }
        }
        
        hit.m_zone    = HitTester::Zone::Outside;
        hit.m_index   = 0;
        return false;
    }
    
    bool jObject::hitTest(juce::Rectangle<int> const& rect)
    {
        const juce::Rectangle<int> bounds = getBoxBounds();
        return rect.intersects(bounds) || rect.contains(bounds) || bounds.contains(rect);
    }
    
    bool jObject::isSelected()
    {
        return m_is_selected;
    }
    
    void jObject::objectChanged(model::Patcher::View& view, model::Object& object)
    {
        bool need_redraw = false;
        
        if(object.inletsChanged())
        {
            m_inlets = object.getNumberOfInlets();
            need_redraw = true;
        }
        
        if(object.outletsChanged())
        {
            m_outlets = object.getNumberOfOutlets();
            need_redraw = true;
        }
        
        if(object.boundsChanged())
        {
            updateBounds();
            need_redraw = false;
        }
        
        if(view.removed())
        {
            m_model = nullptr;
            need_redraw = false;
        }
        
        if(need_redraw)
        {
            repaint();
        }
    }

    void jObject::localSelectionChanged(bool selected)
    {
        if(m_is_selected != selected)
        {
            m_is_selected = selected;
            repaint();
        }
    }
    
    void jObject::distantSelectionChanged(std::set<uint64_t> distant_user_id_selection)
    {
        m_distant_selection = distant_user_id_selection;
        repaint();
    }
    
    juce::Point<int> jObject::getInletPatcherPosition(const size_t index) const
    {
        return getPosition() + getInletLocalBounds(index, m_local_box_bounds).getCentre();
    }
    
    juce::Point<int> jObject::getOutletPatcherPosition(const size_t index) const
    {
        return getPosition() + getOutletLocalBounds(index, m_local_box_bounds).getCentre();
    }
    
    juce::Rectangle<int> jObject::getInletLocalBounds(const size_t index,
                                                      juce::Rectangle<int> const& object_bounds) const
    {
        juce::Rectangle<int> rect;
        
        if(m_inlets > 0 && index < m_inlets)
        {
            if(m_inlets == 1 && index == 0)
            {
                rect.setBounds(object_bounds.getX(), object_bounds.getY(), m_io_width, m_io_height);
            }
            
            if(m_inlets > 1)
            {
                const double ratio = (object_bounds.getWidth() - m_io_width) / (double)(m_inlets - 1);
                rect.setBounds(object_bounds.getX() + ratio * index, object_bounds.getY(),
                               m_io_width, m_io_height);
            }
        }
        
        return rect;
    }
    
    juce::Rectangle<int> jObject::getOutletLocalBounds(const size_t index,
                                                       juce::Rectangle<int> const& object_bounds) const
    {
        juce::Rectangle<int> rect;
        
        if(m_outlets > 0 && index < m_outlets)
        {
            if(m_outlets == 1 && index == 0)
            {
                rect.setBounds(object_bounds.getX(),
                               object_bounds.getY() + object_bounds.getHeight() - m_io_height,
                               m_io_width, m_io_height);
            }
            
            if(m_outlets > 1)
            {
                const double ratio = (object_bounds.getWidth() - m_io_width) / (double)(m_outlets - 1);
                rect.setBounds(object_bounds.getX() + ratio * index,
                               object_bounds.getY() + object_bounds.getHeight() - m_io_height,
                               m_io_width, m_io_height);
            }
        }
        
        return rect;
    }
    
    void jObject::lockStatusChanged(bool locked)
    {
        m_is_locked = locked;
        
        setInterceptsMouseClicks(locked, locked);
        //setWantsKeyboardFocus(locked);
        //setMouseClickGrabsKeyboardFocus(locked);
    }
    
    void jObject::patcherViewOriginPositionChanged()
    {
        updateBounds();
    }
    
    void jObject::mouseDown(juce::MouseEvent const& event)
    {
        ;
    }
    
    void jObject::mouseDrag(juce::MouseEvent const& event)
    {
        ;
    }
    
    // ================================================================================ //
    //                                   JOBJECT BOX                                    //
    // ================================================================================ //
    
    jClassicBox::jClassicBox(jPatcher& patcher_view, model::Object& object_m) : jObject(patcher_view, object_m)
    {
        setWantsKeyboardFocus(true);
        setMouseClickGrabsKeyboardFocus(true);
    }

    jClassicBox::~jClassicBox()
    {
        removeTextEditor();
    }
    
    void jClassicBox::grabKeyboardFocus()
    {
        setInterceptsMouseClicks(true, true);
        
        m_editor.reset(new juce::TextEditor());
        m_editor->setBounds(m_local_box_bounds.expanded(m_selection_width*0.5));
        
        std::string text = m_model->getText();
        if(text.compare(0, 8, "errorbox") == 0)
        {
            text.erase(text.begin(), text.begin()+9);
        }
        else if(text == "newbox")
        {
            text.clear();
        }
        
        m_editor->setColour(juce::TextEditor::highlightColourId, juce::Colour::fromFloatRGBA(0., 0.5, 1., 0.4));
        m_editor->setColour(juce::TextEditor::focusedOutlineColourId, juce::Colour::fromFloatRGBA(0.4, 0.4, 0.4, 0.6));
        m_editor->setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentWhite);

        m_editor->setScrollbarsShown(false);
        m_editor->setScrollToShowCursor(true);
        m_editor->setReturnKeyStartsNewLine(false);
        m_editor->setMultiLine(true, false);
        
        m_editor->setText(text);
        m_editor->setCaretPosition(text.length());
        
        m_editor->addListener(this);
        addAndMakeVisible(m_editor.get());
        
        m_editor->setSelectAllWhenFocused(true);
        m_editor->grabKeyboardFocus();
        
        m_is_editing = true;
    }
    
    void jClassicBox::removeTextEditor()
    {
        if(m_editor)
        {
            m_editor->removeListener(this);
            removeChildComponent(m_editor.get());
            m_editor.reset();
            
            m_patcher_view.grabKeyboardFocus();
            m_is_editing = false;
        }
    }
    
    void jClassicBox::focusGained(FocusChangeType cause)
    {
        //KiwiApp::post("focusGained");
    }
    
    void jClassicBox::focusLost(FocusChangeType cause)
    {
        //KiwiApp::post("focusLost");
    }
    
    void jClassicBox::resized()
    {
        if(m_editor)
        {
            //auto ed_borders = m_editor->getBorder();
            m_editor->setBounds(m_local_box_bounds.expanded(m_selection_width*0.5));
        }
    }
    
    void jClassicBox::textEditorTextChanged(juce::TextEditor& e)
    {
        const juce::String new_text = e.getText();
        const juce::Font font = e.getFont();
        const int text_width = font.getStringWidth(new_text);
        
        //auto ed_borders = e.getBorder();
        
        const int ed_width = e.getWidth();
        
        // box grows only up
        if(ed_width < text_width + 16)
        {
            const int new_width = text_width + 24;
            m_local_box_bounds.setWidth(new_width-8);
            setSize(new_width, getHeight());
        }
    }
    
    void jClassicBox::textEditorReturnKeyPressed(juce::TextEditor& e)
    {
        //KiwiApp::post("textEditorReturnKeyPressed");
        
        m_patcher_view.grabKeyboardFocus();
    }
    
    void jClassicBox::textEditorEscapeKeyPressed(juce::TextEditor& e)
    {
        //KiwiApp::post("textEditorEscapeKeyPressed");
        
        removeTextEditor();
    }
    
    void jClassicBox::textEditorFocusLost(juce::TextEditor& e)
    {
        const bool locked = m_is_locked;
        setInterceptsMouseClicks(locked, locked);
        
        std::string new_text = e.getText().toStdString();
        
        removeTextEditor();
        
        m_patcher_view.boxHasBeenEdited(*this, new_text);
    }
}
