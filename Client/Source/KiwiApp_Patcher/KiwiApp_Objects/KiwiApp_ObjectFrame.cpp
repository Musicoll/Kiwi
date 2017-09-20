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
    m_inlets(getModel().getNumberOfInlets()),
    m_outlets(getModel().getNumberOfOutlets()),
    m_outline(10, 4, 1)
    {
        initColours();
        
        addChildComponent(m_outline);
        
        addAndMakeVisible(m_object_view.get());
        
        updateBounds(false);
        
        updateOutline();
        
        setInterceptsMouseClicks(isLocked(), isLocked());
    }
    
    ObjectFrame::~ObjectFrame()
    {
        ;
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
            
            // hit the body of the box
            hit.m_zone    = HitTester::Zone::Inside;
            hit.m_index   = 0;
            return true;
        }
        else if(isSelected()) // test borders
        {
            if (m_outline.hitTest(pt, hit))
            {
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
    
    void ObjectFrame::textChanged(std::string const& new_text)
    {
        dynamic_cast<ClassicView*>(m_object_view.get())->removeListener(*this);
        
        getPatcherView().objectTextChanged(*this, new_text);
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
        ClassicView * object_view = dynamic_cast<ClassicView*>(m_object_view.get());
        
        if (object_view != nullptr)
        {
            object_view->addListener(*this);
            object_view->edit();
        }
    }
    
    void ObjectFrame::updateOutline()
    {
        std::set<uint64_t> const& distant_selection = getDistantSelection();
        
        const bool selected = isSelected();
        const bool other_selected = ! distant_selection.empty();
        const bool other_view_selected = (other_selected &&
                                          distant_selection.find(KiwiApp::userID())
                                          != distant_selection.end());
        
        const bool distant_selected = ((other_selected && (other_view_selected && distant_selection.size() > 1))
                                       || (other_selected && !other_view_selected));
        
        if (selected)
        {
            m_outline.setVisible(true);
            
            m_outline.setResizeColour(findColour(ObjectFrame::ColourIds::Selection));
            
            if (distant_selected)
            {
                m_outline.setInnerColour(findColour(ObjectFrame::ColourIds::SelectionDistant));
            }
            else if (other_view_selected)
            {
                m_outline.setInnerColour(findColour(ObjectFrame::ColourIds::SelectionOtherView));
            }
            else
            {
                m_outline.setInnerColour(findColour(ObjectFrame::ColourIds::Selection));
            }
        }
        else if(distant_selected)
        {
            m_outline.setVisible(true);
            m_outline.setResizeColour(findColour(ObjectFrame::ColourIds::SelectionDistant));
            m_outline.setInnerColour(findColour(ObjectFrame::ColourIds::SelectionDistant));
        }
        else if (other_view_selected)
        {
            m_outline.setVisible(true);
            m_outline.setResizeColour(findColour(ObjectFrame::ColourIds::SelectionOtherView));
            m_outline.setInnerColour(findColour(ObjectFrame::ColourIds::SelectionOtherView));
        }
        else
        {
            m_outline.setVisible(false);
        }
    }

    void ObjectFrame::selectionChanged()
    {
        updateOutline();
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
        juce::Rectangle<int> inlet_bounds = m_object_view->getBounds();
        
        inlet_bounds.removeFromLeft(m_outline.getResizeLength() - m_outline.getBorderThickness());
        inlet_bounds.removeFromRight(m_outline.getResizeLength() - m_outline.getBorderThickness());
        
        juce::Rectangle<int> rect;
        
        if(m_inlets > 0 && index < m_inlets)
        {
            if(m_inlets == 1 && index == 0)
            {
                rect.setBounds(inlet_bounds.getX(), inlet_bounds.getY(), getPinWidth(), getPinHeight());
            }
            
            if(m_inlets > 1)
            {
                const double ratio = (inlet_bounds.getWidth() - getPinWidth()) / (double)(m_inlets - 1);
                rect.setBounds(inlet_bounds.getX() + ratio * index, inlet_bounds.getY(),
                               getPinWidth(), getPinHeight());
            }
        }
        
        return rect;
    }
    
    juce::Rectangle<int> ObjectFrame::getOutletLocalBounds(const size_t index) const
    {
        juce::Rectangle<int> outlet_bounds = m_object_view->getBounds();
        
        outlet_bounds.removeFromLeft(m_outline.getResizeLength() - m_outline.getBorderThickness());
        outlet_bounds.removeFromRight(m_outline.getResizeLength() - m_outline.getBorderThickness());
        
        juce::Rectangle<int> rect;
        
        if(m_outlets > 0 && index < m_outlets)
        {
            if(m_outlets == 1 && index == 0)
            {
                rect.setBounds(outlet_bounds.getX(),
                               outlet_bounds.getY() + outlet_bounds.getHeight() - getPinHeight(),
                               getPinWidth(), getPinHeight());
            }
            
            if(m_outlets > 1)
            {
                const double ratio = (outlet_bounds.getWidth() - getPinWidth()) / (double)(m_outlets - 1);
                rect.setBounds(outlet_bounds.getX() + ratio * index,
                               outlet_bounds.getY() + outlet_bounds.getHeight() - getPinHeight(),
                               getPinWidth(), getPinHeight());
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
    
    ObjectFrame::Outline::Outline(int resize_length,
                                  int resize_thickness,
                                  int inner_thickness):
    juce::Component(),
    m_resize_length(resize_length),
    m_resize_thickness(resize_thickness),
    m_inner_thickness(inner_thickness),
    m_corners(),
    m_borders(),
    m_resize_colour(),
    m_inner_colour()
    {
    }
    
    ObjectFrame::Outline::~Outline()
    {
    }
    
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
        juce::Rectangle<int> bounds = getLocalBounds();
        
        std::array<juce::Rectangle<int>, 3> corner;
        
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
    
    void ObjectFrame::Outline::updateBorders()
    {
        juce::Rectangle<int> bounds = getLocalBounds();
        
        m_borders[Border::Top].setBounds(m_resize_length,
                                         m_resize_thickness - m_inner_thickness,
                                         bounds.getWidth() - 2 * m_resize_length,
                                         m_inner_thickness);
        
        m_borders[Border::Right].setBounds(bounds.getRight() - m_resize_thickness,
                                           m_resize_length,
                                           m_inner_thickness,
                                           bounds.getHeight() - 2 * m_resize_length);
        
        m_borders[Border::Bottom].setBounds(m_resize_length,
                                            getBottom() - m_resize_thickness,
                                            bounds.getWidth() - 2 * m_resize_length,
                                            m_inner_thickness);
        
        m_borders[Border::Left].setBounds(m_resize_thickness - m_inner_thickness,
                                            m_resize_length,
                                            m_inner_thickness,
                                            bounds.getHeight() - 2 * m_resize_length);
    }
    
    void ObjectFrame::Outline::resized()
    {
        updateBorders();
        updateCorners();
    }
    
    bool ObjectFrame::Outline::hitTest(juce::Point<int> const& pt, HitTester& hit_tester) const
    {
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
        g.setColour(m_resize_colour);
        
        g.fillRect(m_corners[border][2]);
        g.fillRect(m_corners[border][1]);
    }
    
    void ObjectFrame::Outline::drawBorder(juce::Graphics & g, Border border)
    {
        g.setColour(m_inner_colour);
        
        g.fillRect(m_borders[border]);
    }
    
    void ObjectFrame::Outline::paint(juce::Graphics & g)
    {
        drawBorder(g, Border::Left);
        drawBorder(g, Border::Top);
        drawBorder(g, Border::Right);
        drawBorder(g, Border::Bottom);

        drawCorner(g, Border::Left | Border::Top);
        drawCorner(g, Border::Top | Border::Right);
        drawCorner(g, Border::Right | Border::Bottom);
        drawCorner(g, Border::Bottom | Border::Left);
    }
    
    void ObjectFrame::Outline::setResizeColour(juce::Colour colour)
    {
        m_resize_colour = colour;
    }
    
    void ObjectFrame::Outline::setInnerColour(juce::Colour colour)
    {
        m_inner_colour = colour;
    }
}
