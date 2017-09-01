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

#include <KiwiApp.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectFrame.h>

namespace kiwi
{
    // ================================================================================ //
    //                                     OBJECT FRAME                                 //
    // ================================================================================ //
    
    ObjectFrame::ObjectFrame(PatcherView& patcher_view, std::unique_ptr<ObjectView> object_view) :
    m_object_view(std::move(object_view)),
    m_patcher_view(patcher_view),
    m_io_width(6),
    m_io_height(3),
    m_selection_width(4),
    m_inlets(getModel().getNumberOfInlets()),
    m_outlets(getModel().getNumberOfOutlets())
    {
        initColours();
        
        addAndMakeVisible(m_object_view.get());
        
        updateBounds(false);
        
        setInterceptsMouseClicks(isLocked(), isLocked());
    }
    
    ObjectFrame::~ObjectFrame()
    {
        ;
    }
    
    void ObjectFrame::childBoundsChanged(juce::Component * child)
    {
        juce::Rectangle<int> object_bounds = getObjectBounds();
        
        setBounds(getObjectBounds().expanded(m_selection_width));
    }
    
    void ObjectFrame::resized()
    {
        m_object_view->setBounds(getLocalBounds().reduced(m_selection_width));
    }
    
    void ObjectFrame::updateBounds(bool animate)
    {
        model::Object const& model = getModel();
        
        if(!model.removed())
        {
            const juce::Point<int> origin = m_patcher_view.getOriginPosition();
            
            const juce::Rectangle<int> object_bounds(model.getX() + origin.getX(),
                                                     model.getY() + origin.getY(),
                                                     model.getWidth(),
                                                     model.getHeight());
            
            const juce::Rectangle<int> frame_bounds = object_bounds.expanded(m_selection_width);
            
            
            if (animate)
            {
                juce::ComponentAnimator& animator = juce::Desktop::getInstance().getAnimator();
                animator.animateComponent(this, frame_bounds, 1., 150., false, 0.8, 1.);
            }
            else
            {
                setBounds(frame_bounds);
            }
        }
    }
    
    void ObjectFrame::paint(juce::Graphics & g)
    {
        const auto bounds = getLocalBounds();
        
        std::set<uint64_t> const& distant_selection = getDistantSelection();
        
        const bool selected = isSelected();
        const bool other_selected = ! distant_selection.empty();
        const bool other_view_selected = (other_selected &&
                                          distant_selection.find(KiwiApp::userID())
                                          != distant_selection.end());
        
        const bool distant_selected = ((other_selected && (other_view_selected && distant_selection.size() > 1))
                                       || (other_selected && !other_view_selected));
        
        if(selected || other_view_selected || distant_selected)
        {
            juce::Colour selection_color;
            
            if (selected)
            {
                selection_color = findColour(ObjectFrame::ColourIds::Selection);
            }
            else if(other_view_selected)
            {
                selection_color = findColour(ObjectFrame::ColourIds::SelectionOtherView);
            }
            else
            {
                selection_color = findColour(ObjectFrame::ColourIds::SelectionDistant);
            }
        
            g.setColour(selection_color.darker(0.4));
            g.drawRect(bounds.reduced(m_selection_width*0.5 + 1), 1);
            
            g.setColour(selection_color);
            g.drawRoundedRectangle(bounds.reduced(m_selection_width*0.5).toFloat(),
                                   m_selection_width*0.5, m_selection_width*0.5);
            
            g.setColour(selection_color.darker(0.4));
            g.drawRoundedRectangle(bounds.reduced(1).toFloat(), m_selection_width*0.5, 1);
        }
    }
    
    void ObjectFrame::paintOverChildren (juce::Graphics& g)
    {
        if(!isLocked())
        {
            g.setColour(findColour(ObjectFrame::ColourIds::Pin));
            drawInletsOutlets(g);
        }
    }
    
    void ObjectFrame::drawInletsOutlets(juce::Graphics & g)
    {
        const juce::Rectangle<int> bounds = m_object_view->getBounds();
        
        for(unsigned int i = 0; i < m_inlets; ++i)
        {
            g.fillRect(getInletLocalBounds(i));
        }
        
        for(unsigned int i = 0; i < m_outlets; ++i)
        {
            g.fillRect(getOutletLocalBounds(i));
        }
    }
    
    juce::Rectangle<int> ObjectFrame::getObjectBounds() const
    {
        return m_object_view->getBounds().withPosition(getPosition() + m_object_view->getBounds().getPosition());
    }
    
    void ObjectFrame::mouseDown(juce::MouseEvent const& e)
    {
        m_object_view->mouseDown(e);
    }
    
    void ObjectFrame::mouseUp(juce::MouseEvent const& e)
    {
        m_object_view->mouseUp(e);
    }
    
    bool ObjectFrame::hitTest(int x, int y)
    {
        bool allow_click;
        bool allow_click_onchild;
        getInterceptsMouseClicks(allow_click, allow_click_onchild);
        
        if (! allow_click)
            return false;
        
        return m_object_view->getBounds().contains(x, y);
    }
    
    bool ObjectFrame::hitTest(juce::Point<int> const& pt, HitTester& hit) const
    {
        const auto box_bounds = m_object_view->getBounds();
        
        // test body and iolets
        if(box_bounds.contains(pt))
        {
            if(!isSelected())
            {
                // test inlets
                if(m_inlets > 0 && pt.getY() > m_io_height)
                {
                    for(size_t i = 0; i < m_inlets; ++i)
                    {
                        if(getInletLocalBounds(i).contains(pt))
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
                        if(getOutletLocalBounds(i).contains(pt))
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
        else if(isSelected()) // test borders
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
    
    bool ObjectFrame::hitTest(juce::Rectangle<int> const& rect) const
    {
        const juce::Rectangle<int> bounds = getObjectBounds();
        return rect.intersects(bounds) || rect.contains(bounds) || bounds.contains(rect);
    }
    
    void ObjectFrame::initColours()
    {
        setColour(ObjectFrame::ColourIds::Selection, juce::Colour::fromFloatRGBA(0., 0.5, 1., 0.8));
        setColour(ObjectFrame::ColourIds::SelectionOtherView, juce::Colour(0xAA9BFF71));
        setColour(ObjectFrame::ColourIds::SelectionDistant, juce::Colour(0xAA9BFF71));
        setColour(ObjectFrame::ColourIds::Pin, juce::Colour(0.3, 0.3, 0.3));
    }
    
    bool ObjectFrame::isSelected() const
    {
        return getPatcherView().isSelected(*this);
    }
    
    bool ObjectFrame::isEditing() const
    {
        bool is_editing = false;
        
        ClassicView * classic_view = dynamic_cast<ClassicView*>(m_object_view.get());
        
        if (classic_view != nullptr)
        {
            is_editing = classic_view->isEditing();
        }
        
        return is_editing;
    }
    
    std::set<uint64_t> ObjectFrame::getDistantSelection() const
    {
        return getPatcherView().getDistantSelection(*this);
    }
    
    bool ObjectFrame::isLocked() const
    {
        return getPatcherView().isLocked();
    }
    
    void ObjectFrame::objectChanged(model::Patcher::View& view, model::Object& object)
    {
        if(object.inletsChanged())
        {
            m_inlets = object.getNumberOfInlets();
            repaint();
        }
        
        if(object.outletsChanged())
        {
            m_outlets = object.getNumberOfOutlets();
            repaint();
        }
        
        if(object.boundsChanged())
        {
            const auto ctrl = object.document().controller();
            const bool animate = (ctrl == flip::Controller::UNDO
                                  || ctrl == flip::Controller::EXTERNAL);
            
            updateBounds(animate);
            
            repaint();
        }
    }
    
    void ObjectFrame::textEdited(std::string const& new_text)
    {
        dynamic_cast<ClassicView*>(m_object_view.get())->removeListener(*this);
        
        setInterceptsMouseClicks(isLocked(), isLocked());
        
        getPatcherView().objectEdited(*this, new_text);
    }
    
    void ObjectFrame::editObject()
    {
        ClassicView * object_view = dynamic_cast<ClassicView*>(m_object_view.get());
        
        if (object_view != nullptr)
        {
            object_view->addListener(*this);
            object_view->edit();
        }
    }

    void ObjectFrame::localSelectionChanged()
    {
        repaint();
    }
    
    void ObjectFrame::distantSelectionChanged()
    {
        repaint();
    }
    
    void ObjectFrame::lockStatusChanged()
    {
        repaint();
        setInterceptsMouseClicks(isLocked(), isLocked());
    }
    
    void ObjectFrame::patcherViewOriginPositionChanged()
    {
        updateBounds(false);
    }
    
    juce::Point<int> ObjectFrame::getInletPatcherPosition(const size_t index) const
    {
        return getPosition() + getInletLocalBounds(index).getCentre();
    }
    
    juce::Point<int> ObjectFrame::getOutletPatcherPosition(const size_t index) const
    {
        return getPosition() + getOutletLocalBounds(index).getCentre();
    }
    
    model::Object& ObjectFrame::getModel() const
    {
        return m_object_view->getModel();
    }
    
    size_t ObjectFrame::getPinWidth() const
    {
        return m_io_width;
    }
    
    size_t ObjectFrame::getPinHeight() const
    {
        return m_io_height;
    }
    
    juce::Rectangle<int> ObjectFrame::getInletLocalBounds(const size_t index) const
    {
        juce::Rectangle<int> object_bounds = m_object_view->getBounds();
        
        juce::Rectangle<int> rect;
        
        if(m_inlets > 0 && index < m_inlets)
        {
            if(m_inlets == 1 && index == 0)
            {
                rect.setBounds(object_bounds.getX(), object_bounds.getY(), getPinWidth(), getPinHeight());
            }
            
            if(m_inlets > 1)
            {
                const double ratio = (object_bounds.getWidth() - getPinWidth()) / (double)(m_inlets - 1);
                rect.setBounds(object_bounds.getX() + ratio * index, object_bounds.getY(),
                               getPinWidth(), getPinHeight());
            }
        }
        
        return rect;
    }
    
    juce::Rectangle<int> ObjectFrame::getOutletLocalBounds(const size_t index) const
    {
        juce::Rectangle<int> object_bounds = m_object_view->getBounds();
        
        juce::Rectangle<int> rect;
        
        if(m_outlets > 0 && index < m_outlets)
        {
            if(m_outlets == 1 && index == 0)
            {
                rect.setBounds(object_bounds.getX(),
                               object_bounds.getY() + object_bounds.getHeight() - getPinHeight(),
                               getPinWidth(), getPinHeight());
            }
            
            if(m_outlets > 1)
            {
                const double ratio = (object_bounds.getWidth() - getPinWidth()) / (double)(m_outlets - 1);
                rect.setBounds(object_bounds.getX() + ratio * index,
                               object_bounds.getY() + object_bounds.getHeight() - getPinHeight(),
                               getPinWidth(), getPinHeight());
            }
        }
        
        return rect;
    }
    
    PatcherView& ObjectFrame::getPatcherView() const
    {
        return m_patcher_view;
    }
}
