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

#ifndef KIWI_APP_PATCHERVIEW_HELPER_HPP_INCLUDED
#define KIWI_APP_PATCHERVIEW_HELPER_HPP_INCLUDED

#include "KiwiApp_PatcherView.hpp"
#include "KiwiApp_ObjectView.hpp"
#include "KiwiApp_LinkView.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      HITTESTER                                   //
    // ================================================================================ //
	
    //! @brief The HitTester class...
    class HitTester
    {
    public:
        
        //! @brief The target type
        enum class Target : int
		{
			Nothing = 0,
			Patcher = 1,
			Box		= 2,
			Link	= 3
		};
		
        //! @brief The Zone
        enum class Zone : int
        {
            Outside     = 1<<0,
            Inside      = 1<<1,
            Inlet       = 1<<2,
            Outlet      = 1<<3,
            Border      = 1<<4
        };
		
        //! @brief The Border type
        enum Border
		{
			None	= 1<<0,
			Left	= 1<<1,
			Right	= 1<<2,
			Top		= 1<<3,
			Bottom	= 1<<4,
		};
        
        //! @brief Contructor.
        HitTester(PatcherView const& patcher);
        
        //! @brief Destructor.
        ~HitTester();
        
        //! @brief Reset the hit test.
        //! @brief @details Resets the hit test as it has never touched any box, link or patcher zone.
        void reset();
        
        //! @brief Run a hit test on object and links for a given point.
        //! @details The function try to find an object or a link under the point,
        //! otherwise it will consider that the patcher has been touched.
        //! @param point The point to test.
        void test(juce::Point<int> const& point) noexcept;
        
        //! @brief Test a point.
        //! @details Try to find an object box under the point.
        //! @param point The point to test.
        //! @return true if the point hit an object box.
        bool testObjects(juce::Point<int> const& point) noexcept;
        
        //! @brief Test a point.
        //! @details Try to find a link.
        //! @param point The point to test.
        //! @return true if the point hit a link.
        bool testLinks(juce::Point<int> const& point) noexcept;
        
        //! @brief Test a rectangle.
        //! @details Try to find the objects and links that overlaps the rectangle.
        //! @param rect The rectangle to test.
        //! @param objects The vector of objects that will be filled.
        //! @param links The vector of links that will be filled.
        void test(juce::Rectangle<int> const& rect,
                  std::vector<ObjectView*>& objects,
                  std::vector<LinkView*>& links);
        
        //! @brief Test a rectangle.
        //! @details Try to find the objects that overlaps the given rectangle.
        //! @param rect The rectangle.
        //! @param objects The vector of objects that will be filled.
        void testObjects(juce::Rectangle<int> const& rect, std::vector<ObjectView*>& objects);
        
        //! @brief Test a rectangle.
        //! @details Try to find the links that overlaps the given rectangle.
        //! @param rect The rectangle.
        //! @param links The vector of links that will be filled.
        void testLinks(juce::Rectangle<int> const& rect, std::vector<LinkView*>& links);
        
        //! @brief Get the last touched Target.
        inline Target getTarget() const noexcept            { return m_target; }
        
        //! @brief Returns true if nothing has been hit by the test, otherwise returns false.
        inline bool nothingTouched() const noexcept          { return m_target == Target::Nothing; }
        
        //! @brief Returns true if the test hit the patcher, otherwise return false.
        inline bool patcherTouched() const noexcept          { return m_target == Target::Patcher; }
        
        //! @brief Returns true if the test hit an object, otherwise return false.
        inline bool objectTouched() const noexcept           { return m_target == Target::Box; }
        
        //! @brief Returns true if the test hit a link, otherwise return false.
        inline bool linkTouched() const noexcept             { return m_target == Target::Link; }
        
        //! @brief Get the patcher.
        PatcherView const& getPatcher() const noexcept;
        
        //! @brief Get the object box that has been touched by the last hit-test.
        ObjectView* getObject() const noexcept;
        
        //! @brief Get the link that has been touched by the last hit-test.
        LinkView* getLink() const noexcept;
        
        //! @brief Get the Zone of the Target that result of the hit-test.
        //! @details Get the Zone of the Target that result of the hit-test.
        //! If a patcher has hit, it can be inside or outside.
        //! If an object box has hit, it can be one of all the zones.
        //! If a link has hit, it can only be the four first zones.
        //! It nothing hit, it will be outside.
        //! @return The Zone of the Target that result of the hit-test.
        Zone getZone() const noexcept;
		
		//! @brief Returns the type of border (if a border of an object box has hit).
        int getBorder() const noexcept;
		
        //! @brief Returns the index of the Zone of the object box.
        //! @details Get the index of the Zone of the object box.
        //! If the object Zone is an inlet or an outlet, it will be their index.
        //! If the object Zone is a border or a corner it will be their position.
        //! @return The index of the zone of the object box.
        size_t getIndex() const noexcept;
        
    private: // members
        
        PatcherView const& m_patcher;
        ObjectView*        m_object    = nullptr;
        LinkView*          m_link      = nullptr;
        Target          m_target	= Target::Nothing;
        Zone            m_zone		= Zone::Outside;
        int             m_border	= Border::None;
        size_t          m_index     = 0;
        
        friend class LinkView;
        friend class ObjectView;
    };
    
    // ================================================================================ //
    //                                  PATCHER VIEWPORT                                //
    // ================================================================================ //
    
    //! @brief The PatcherView Viewport
    class PatcherViewport : public juce::Viewport
    {
    public:
        
        //! @brief Constructor
        PatcherViewport(PatcherView& patcher);
        
        //! @brief Destructor
        ~PatcherViewport() = default;
        
        //! @brief Called by juce::Viewport when the visible area changed
        void visibleAreaChanged(juce::Rectangle<int> const& new_visible_area) override;
        
        //! @brief Overriden from juce::Viewport to update patcher area on viewport resize.
        void resized() override;
        
        //! @brief Make the object visible in the viewport area.
        void jumpViewToObject(ObjectView const&);
        
        //! @brief Attempts to brings the center of the given bounds to the center of the viewport view area
        void bringRectToCentre(juce::Rectangle<int> bounds);
        
        //! @brief Transforms a given point into a point relative to the patcher origin position.
        juce::Point<int> getRelativePosition(juce::Point<int> point) const noexcept;
        
        //! @brief Returns the current patcher area relative to the patcher origin position.
        juce::Rectangle<int> getRelativeViewArea() const noexcept;
        
        //! @brief Get the view position relative to the patcher origin position.
        juce::Point<int> getRelativeViewPosition() const noexcept;
        
        //! @brief Set the new view position relative to the patcher origin position.
        void setRelativeViewPosition(juce::Point<int> position);
        
        //! @brief Set the zoom factor.
        //! @details The non-zooming factor is 1., 2. means zoom at 200%, -0.5 zoom at 50% and so on.
        void setZoomFactor(double zoom_factor);
        
        //! @brief Returns the current zoom factor.
        double getZoomFactor() const noexcept;
        
        //! @brief Returns the position of the patcher origin relative to the component position.
        juce::Point<int> getOriginPosition() const noexcept;
        
        //! @brief Reset the objects area.
        void resetObjectsArea();
        
        //! @brief Returns the current objects area.
        juce::Rectangle<int> getObjectsArea() const noexcept;
        
        //! @brief Update patcher size.
        void updatePatcherArea(bool keep_same_view_position);
        
    private: // members
        
        //! @internal Hook method called by PatcherViewport just before resized.
        void viewportResized(juce::Rectangle<int> const& last_bounds,
                             juce::Rectangle<int> const& new_bounds);
        
    private: // members
        
        PatcherView&                m_patcher;
        juce::Viewport              m_viewport;
        Component                   m_magnifier;
        juce::Rectangle<int>        m_last_bounds;
        juce::Rectangle<int>        m_patching_area;
        double                      m_zoom_factor;
        bool                        m_can_hook_resized;
    };
    
    // ================================================================================ //
    //                                   IOLET HILIGHTER                                //
    // ================================================================================ //
    
    class IoletHighlighter : public juce::Component
    {
    public:
        
        //! @brief Constructor.
        IoletHighlighter();
        
        //! @brief Destructor.
        ~IoletHighlighter() = default;
        
        //! @brief The paint method.
        void paint(juce::Graphics& g) override;
        
        //! @brief Stop highlighting.
        void hide();
        
        //! @brief Highlight inlet
        void highlightInlet(ObjectView const& object, const size_t index);
        
        //! @brief Highlight outlet
        void highlightOutlet(ObjectView const& object, const size_t index);
        
    private: // members
        
        bool            m_is_inlet;
        std::string     m_text;
    };
    
    // ================================================================================ //
    //										JLASSO                                      //
    // ================================================================================ //
    
    class Lasso : public juce::Component
    {
    public:
        
        //! @brief Contructor.
        Lasso(PatcherView& patcher);
        
        //! @grief Destructor.
        ~Lasso();
        
        //! @brief The paint method.
        void paint(juce::Graphics& g) override;
        
        //! @brief Begins the selection of the links and the boxes.
        //! @param point The starting point.
        //! @param preserve_selection The lasso should preserve the current selection.
        void begin(juce::Point<int> const& point, const bool preserve_selection);
        
        //! @brief Perform the selection of the links and the boxes.
        //! @param point The draging point.
        //! @param objects The lasso should add objects to the selection.
        //! @param links The lasso should add links to the selection.
        //! @param preserve The lasso should preserve the last selection.
        void perform(juce::Point<int> const& point, bool objects, bool links, const bool preserve);
        
        //! @brief Ends the selection of the links and the boxes.
        void end();

        //! Retrieve Returns true if the Lasso is performing the selection.
        bool isPerforming() const noexcept;
        
    private: // members
        
        PatcherView&        m_patcher;
        
        std::set<flip::Ref> m_objects;
        std::set<flip::Ref> m_links;
        
        juce::Point<int>    m_start;
        bool				m_dragging;
    };
    
}

#endif // KIWI_APP_PATCHERVIEW_HELPER_HPP_INCLUDED
