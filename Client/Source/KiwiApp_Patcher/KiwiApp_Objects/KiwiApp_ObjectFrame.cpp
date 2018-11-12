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

#define _USE_MATH_DEFINES
#include <cmath>

#include <KiwiApp.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectFrame.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ClassicView.h>

namespace kiwi
{
    // ================================================================================ //
    //                                     OBJECT FRAME                                 //
    // ================================================================================ //
    
    ObjectFrame::ObjectFrame(PatcherView& patcher_view,
                             std::unique_ptr<ObjectView> object_view)
    : m_object_view(std::move(object_view))
    , m_patcher_view(patcher_view)
    , m_io_width(6)
    , m_io_height(3)
    , m_outline(10, 4)
    {
        const auto& model_object = getModel();
        updateInlets(model_object);
        updateOutlets(model_object);
        
        const bool pv_locked = isLocked();
        setInterceptsMouseClicks(pv_locked, pv_locked);
        m_object_view->lockStatusChanged(pv_locked);
        
        updateBoundsFromModel(false);
        
        addChildComponent(m_outline);
        updateOutline();
        
        addAndMakeVisible(m_object_view.get());
    }
    
    ObjectFrame::~ObjectFrame()
    {}
    
    size_t ObjectFrame::getNumberOfInlets() const noexcept
    {
        return m_inlets.size();
    }
    
    size_t ObjectFrame::getNumberOfOutlets() const noexcept
    {
        return m_outlets.size();
    }
    
    void ObjectFrame::childBoundsChanged(juce::Component * child)
    {
        juce::Rectangle<int> object_bounds = getObjectBounds();
        
        setBounds(getObjectBounds().expanded(m_outline.getBorderThickness()));
    }
    
    void ObjectFrame::resized()
    {
        m_object_view->setBounds(getLocalBounds().reduced(m_outline.getBorderThickness()));
        
        m_outline.setBounds(getLocalBounds());
    }
    
    void ObjectFrame::updateBoundsFromModel(bool animate)
    {
        model::Object const& model = getModel();
        
        if(!model.removed())
        {
            const juce::Point<int> origin = m_patcher_view.getOriginPosition();
            
            juce::Rectangle<int> object_bounds(model.getX() + origin.getX(),
                                               model.getY() + origin.getY(),
                                               model.getWidth(), model.getHeight());
            
            m_object_view->checkBounds(object_bounds,
                                       m_object_view->getBounds(), {},
                                       false, false, false, false);
            
            const juce::Rectangle<int> frame_bounds = object_bounds.expanded(m_outline.getBorderThickness());
            
            
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
    
    void ObjectFrame::paintOverChildren (juce::Graphics& g)
    {
        const bool locked = isLocked();
        if(!locked)
        {
            drawInletsOutlets(g);
        }
        
        const auto distant_selection = getDistantSelection();
        const bool other_selected = ! distant_selection.empty();
        const bool other_view_selected = (other_selected &&
                                          distant_selection.find(KiwiApp::userID())
                                          != distant_selection.end());
        
        const bool other_user_selected = ((other_selected && (other_view_selected
                                                              && distant_selection.size() > 1))
                                          || (other_selected && !other_view_selected));
        
        if(other_user_selected || other_view_selected)
        {
            const bool selected = isSelected();
            
            g.setColour(findColour(other_user_selected
                                   ? PatcherView::ColourIds::SelectionOtherUser
                                   : PatcherView::ColourIds::SelectionOtherView)
                        .withAlpha(0.5f));
            
            if(!locked)
            {
                g.fillRoundedRectangle(getLocalBounds().toFloat()
                                       .reduced(selected ? 5.f : 1.f),
                                       selected ? 0.f : 5.f);
            }
            else
            {
                const float amount = 3.f;
                g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(amount), amount, amount);
            }
        }
    }
    
    void ObjectFrame::drawInletsOutlets(juce::Graphics & g)
    {
        const juce::Rectangle<int> bounds = m_object_view->getBounds();
        
        const auto pin_control_color = findColour(ObjectView::ColourIds::PinControl);
        const auto pin_signal_color = findColour(ObjectView::ColourIds::PinSignal);
        
        for(unsigned int i = 0; i < getNumberOfInlets(); ++i)
        {
            g.setColour(m_inlets[i].is_signal ? pin_signal_color : pin_control_color);
            g.fillRect(getInletLocalBounds(i));
        }
        
        for(unsigned int i = 0; i < getNumberOfOutlets(); ++i)
        {
            g.setColour(m_outlets[i].is_signal ? pin_signal_color : pin_control_color);
            g.fillRect(getOutletLocalBounds(i));
        }
    }
    
    juce::Rectangle<int> ObjectFrame::getObjectBounds() const
    {
        const auto object_view_bounds = m_object_view->getBounds();
        return object_view_bounds.withPosition(getPosition() + object_view_bounds.getPosition());
    }
    
    juce::ComponentBoundsConstrainer* ObjectFrame::getBoundsConstrainer() const
    {
        return m_object_view.get();
    }
    
    int ObjectFrame::getResizingFlags() const
    {
        return m_object_view->getResizingFlags();
    }
    
    void ObjectFrame::mouseDown(juce::MouseEvent const& e)
    {
        m_object_view->mouseDown(e.getEventRelativeTo(m_object_view.get()));
    }
    
    void ObjectFrame::mouseUp(juce::MouseEvent const& e)
    {
        m_object_view->mouseUp(e.getEventRelativeTo(m_object_view.get()));
    }
    
    void ObjectFrame::mouseDrag(juce::MouseEvent const& e)
    {
        m_object_view->mouseDrag(e.getEventRelativeTo(m_object_view.get()));
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
            const auto ninlets = getNumberOfInlets();
            const auto noutlets = getNumberOfOutlets();
            
            // test inlets
            if(ninlets > 0 && pt.getY() > getPinHeight())
            {
                for(size_t i = 0; i < ninlets; ++i)
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
            if(noutlets > 0 && pt.getY() > box_bounds.getHeight() - getPinHeight())
            {
                for(size_t i = 0; i < noutlets; ++i)
                {
                    if(getOutletLocalBounds(i).contains(pt))
                    {
                        hit.m_zone    = HitTester::Zone::Outlet;
                        hit.m_index   = i;
                        return true;
                    }
                }
            }
            
            // hit the body of the box
            hit.m_zone    = HitTester::Zone::Inside;
            hit.m_index   = 0;
            return true;
        }
        else if(m_outline.isVisible()
                && m_outline.hitTest(pt, hit)) // test borders
        {
            return true;
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
    
    bool ObjectFrame::isSelected() const
    {
        return getPatcherView().isSelected(*this);
    }
    
    std::set<uint64_t> ObjectFrame::getDistantSelection() const
    {
        return getPatcherView().getDistantSelection(*this);
    }
    
    bool ObjectFrame::isLocked() const
    {
        return getPatcherView().isLocked();
    }
    
    void ObjectFrame::updateInlets(model::Object const& object)
    {
        m_inlets.clear();
        for(auto& inlet : object.getInlets())
        {
            m_inlets.emplace_back(inlet.hasType(kiwi::model::PinType::IType::Signal));
        }
    }
    
    void ObjectFrame::updateOutlets(model::Object const& object)
    {
        m_outlets.clear();
        for(auto& outlet : object.getOutlets())
        {
            m_outlets.emplace_back(outlet.getType() == kiwi::model::PinType::IType::Signal);
        }
    }
    
    void ObjectFrame::objectChanged(model::Patcher::View& view, model::Object& object)
    {
        if(object.inletsChanged())
        {
            updateInlets(object);
            repaint();
        }
        
        if(object.outletsChanged())
        {
            updateOutlets(object);
            repaint();
        }
        
        if(object.boundsChanged())
        {
            const auto ctrl = object.document().controller();
            const bool animate = (ctrl == flip::Controller::UNDO
                                  || ctrl == flip::Controller::EXTERNAL);
            
            updateBoundsFromModel(animate);
            
            repaint();
        }
    }
    
    void ObjectFrame::attributeChanged(std::string const& name, tool::Parameter const& parameter)
    {
        m_object_view->modelAttributeChanged(name, parameter);
    }
    
    void ObjectFrame::textChanged(std::string const& new_text)
    {
        if (auto* editable_object_view = dynamic_cast<EditableObjectView*>(m_object_view.get()))
        {
            editable_object_view->removeListener(*this);
            
            if (dynamic_cast<ClassicView*>(m_object_view.get()))
            {
                getPatcherView().objectTextChanged(*this, new_text);
            }
        }
    }
    
    void ObjectFrame::editorHidden()
    {
        setInterceptsMouseClicks(isLocked(), isLocked());
        
        getPatcherView().objectEditorHidden(*this);
    }
    
    void ObjectFrame::editorShown()
    {
        setInterceptsMouseClicks(true, true);
        
        getPatcherView().objectEditorShown(*this);
    }
    
    void ObjectFrame::editObject()
    {
        if (auto* editable_object_view = dynamic_cast<EditableObjectView*>(m_object_view.get()))
        {
            editable_object_view->addListener(*this);
            editable_object_view->edit();
        }
    }
    
    void ObjectFrame::updateOutline()
    {
        m_outline.setVisible(isSelected());
    }

    void ObjectFrame::selectionChanged()
    {
        updateOutline();
        repaint();
    }
    
    void ObjectFrame::lockStatusChanged()
    {
        const bool locked = isLocked();
        setInterceptsMouseClicks(locked, locked);
        m_object_view->lockStatusChanged(locked);
        repaint();
    }
    
    void ObjectFrame::patcherViewOriginPositionChanged()
    {
        updateBoundsFromModel(false);
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
        juce::Rectangle<int> rect;
        
        const auto ninlets = getNumberOfInlets();

        if(ninlets > 0 && index < ninlets)
        {
            juce::Rectangle<int> inlet_bounds = m_object_view->getBounds();
            
            auto pin_width = getPinWidth();
            
            const auto border_width = (m_outline.getResizeLength()
                                       - m_outline.getBorderThickness());
            
            const auto space_to_remove = juce::jlimit<int>(0, border_width, inlet_bounds.getWidth() - (pin_width * ninlets) - border_width);
            
            if(space_to_remove > 0)
            {
                inlet_bounds.removeFromLeft(space_to_remove);
                inlet_bounds.removeFromRight(space_to_remove);
            }
            
            if(ninlets == 1 && index == 0)
            {
                rect.setBounds(inlet_bounds.getX(),
                               inlet_bounds.getY(),
                               pin_width, getPinHeight());
            }
            else if(ninlets > 1)
            {
                const double ratio = (inlet_bounds.getWidth() - pin_width) / (double)(ninlets - 1);
                rect.setBounds(inlet_bounds.getX() + ratio * index,
                               inlet_bounds.getY(),
                               pin_width, getPinHeight());
            }
        }
        
        return rect;
    }
    
    juce::Rectangle<int> ObjectFrame::getOutletLocalBounds(const size_t index) const
    {
        juce::Rectangle<int> rect;
        const auto noutlets = getNumberOfOutlets();
        
        if(noutlets > 0 && index < noutlets)
        {
            juce::Rectangle<int> outlet_bounds = m_object_view->getBounds();
            
            auto pin_width = getPinWidth();
            
            const auto border_width = (m_outline.getResizeLength()
                                       - m_outline.getBorderThickness());
            
            const auto space_to_remove = juce::jlimit<int>(0, border_width, outlet_bounds.getWidth() - (pin_width * noutlets) - border_width);
            
            if(space_to_remove > 0)
            {
                outlet_bounds.removeFromLeft(space_to_remove);
                outlet_bounds.removeFromRight(space_to_remove);
            }
            
            if(noutlets == 1 && index == 0)
            {
                rect.setBounds(outlet_bounds.getX(),
                               outlet_bounds.getY() + outlet_bounds.getHeight() - getPinHeight(),
                               pin_width, getPinHeight());
            }
            
            if(noutlets > 1)
            {
                const double ratio = (outlet_bounds.getWidth() - getPinWidth()) / (double)(noutlets - 1);
                rect.setBounds(outlet_bounds.getX() + ratio * index,
                               outlet_bounds.getY() + outlet_bounds.getHeight() - getPinHeight(),
                               pin_width, getPinHeight());
            }
        }
        
        return rect;
    }
    
    PatcherView& ObjectFrame::getPatcherView() const
    {
        return m_patcher_view;
    }
    
    // ================================================================================ //
    //                                     OUTLINE                                      //
    // ================================================================================ //
    
    ObjectFrame::Outline::Border operator|(ObjectFrame::Outline::Border const& l_border,
                                           ObjectFrame::Outline::Border const& r_border)
    {
        return static_cast<ObjectFrame::Outline::Border>(static_cast<int>(l_border) | static_cast<int>(r_border));
    }
    
    ObjectFrame::Outline::Outline(int resize_length, int resize_thickness)
    : m_resize_length(resize_length)
    , m_resize_thickness(resize_thickness)
    {}
    
    ObjectFrame::Outline::~Outline()
    {}
    
    int ObjectFrame::Outline::getBorderThickness() const
    {
        return m_resize_thickness;
    }
    
    int ObjectFrame::Outline::getResizeLength() const
    {
        return m_resize_length;
    }
    
    void ObjectFrame::Outline::updateCorners()
    {
        juce::Rectangle<float> bounds = getLocalBounds().toFloat();
        
        std::array<juce::Rectangle<float>, 3> corner;
        
        corner[0].setBounds(bounds.getX(),
                            bounds.getY(),
                            m_resize_thickness,
                            m_resize_length);
        
        corner[1].setBounds(bounds.getX(),
                            bounds.getY(),
                            m_resize_length,
                            m_resize_thickness);
        
        corner[2]  = corner[0];
        corner[2].reduceIfPartlyContainedIn(corner[1]);
        
        m_corners[Border::Top | Border::Left] = corner;
        
        juce::AffineTransform transform = juce::AffineTransform::rotation(M_PI / 2, bounds.getX(), bounds.getY());
        transform = transform.translated(bounds.getTopRight());
        
        m_corners[Border::Top | Border::Right][0] = corner[0].transformedBy(transform);
        m_corners[Border::Top | Border::Right][1] = corner[1].transformedBy(transform);
        m_corners[Border::Top | Border::Right][2] = corner[2].transformedBy(transform);
        
        transform = juce::AffineTransform::rotation(M_PI);
        transform = transform.translated(bounds.getBottomRight());
        
        m_corners[Border::Bottom | Border::Right][0] = corner[0].transformedBy(transform);
        m_corners[Border::Bottom | Border::Right][1] = corner[1].transformedBy(transform);
        m_corners[Border::Bottom | Border::Right][2] = corner[2].transformedBy(transform);
        
        transform = juce::AffineTransform::rotation((3 * M_PI) / 2);
        transform = transform.translated(bounds.getBottomLeft());
        
        m_corners[Border::Bottom | Border::Left][0] = corner[0].transformedBy(transform);
        m_corners[Border::Bottom | Border::Left][1] = corner[1].transformedBy(transform);
        m_corners[Border::Bottom | Border::Left][2] = corner[2].transformedBy(transform);
    }
    
    void ObjectFrame::Outline::resized()
    {
        updateCorners();
    }
    
    bool ObjectFrame::Outline::hitTest(juce::Point<int> const& _pt, HitTester& hit_tester) const
    {
        const auto pt = _pt.toFloat();
        bool success = false;
        
        if (m_corners.at(Border::Left | Border::Top)[1].contains(pt)
            || m_corners.at(Border::Top | Border::Right)[0].contains(pt))
        {
            hit_tester.m_zone = HitTester::Zone::Border;
            hit_tester.m_border |= HitTester::Border::Top;
            success = true;
        }
        
        if(m_corners.at(Border::Top | Border::Right)[1].contains(pt)
                || m_corners.at(Border::Right | Border::Bottom)[0].contains(pt))
        {
            hit_tester.m_zone = HitTester::Zone::Border;
            hit_tester.m_border |= HitTester::Border::Right;
            success = true;
        }
        
        if(m_corners.at(Border::Right | Border::Bottom)[1].contains(pt)
                || m_corners.at(Border::Bottom | Border::Left)[0].contains(pt))
        {
            hit_tester.m_zone = HitTester::Zone::Border;
            hit_tester.m_border |= HitTester::Border::Bottom;
            success = true;
        }
        
        if(m_corners.at(Border::Bottom | Border::Left)[1].contains(pt)
                || m_corners.at(Border::Left | Border::Top)[0].contains(pt))
        {
            hit_tester.m_zone = HitTester::Zone::Border;
            hit_tester.m_border |= HitTester::Border::Left;
            success = true;
        }
        
        if(hit_tester.m_zone == HitTester::Zone::Border)
        {
            hit_tester.m_border &= ~HitTester::Border::None;
        }
        
        
        return success;
    }
    
    void ObjectFrame::Outline::drawCorner(juce::Graphics & g, Border border)
    {
        g.fillRect(m_corners[border][2]);
        g.fillRect(m_corners[border][1]);
    }
    
    void ObjectFrame::Outline::paint(juce::Graphics & g)
    {
        g.setColour(findColour(PatcherView::ColourIds::Selection));
        
        const auto outline_bounds = getLocalBounds().toFloat();
        const auto object_bounds = outline_bounds.reduced(m_resize_thickness);
        
        const float thickness = m_resize_thickness;
        g.drawRect(outline_bounds.reduced(thickness*0.75), thickness*0.25);
        
        /* // The following code do the same
        juce::RectangleList<float> rectangles (outline_bounds);
        rectangles.subtract(object_bounds);
        rectangles.subtract(object_bounds.reduced(m_resize_thickness, -m_resize_thickness));
        rectangles.subtract(object_bounds.reduced(-m_resize_thickness, m_resize_thickness));
        rectangles.consolidate();
        const juce::PathStrokeType stroke (1.f);
        g.strokePath(path, stroke);
        */
        
        drawCorner(g, Border::Left | Border::Top);
        drawCorner(g, Border::Top | Border::Right);
        drawCorner(g, Border::Right | Border::Bottom);
        drawCorner(g, Border::Bottom | Border::Left);
    }
}
