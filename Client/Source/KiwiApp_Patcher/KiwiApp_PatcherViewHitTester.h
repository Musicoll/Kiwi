/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

namespace kiwi
{
    class PatcherView;
    class ObjectFrame;
    class LinkView;
    
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
                  std::vector<ObjectFrame*>& objects,
                  std::vector<LinkView*>& links);
        
        //! @brief Test a rectangle.
        //! @details Try to find the objects that overlaps the given rectangle.
        //! @param rect The rectangle.
        //! @param objects The vector of objects that will be filled.
        void testObjects(juce::Rectangle<int> const& rect, std::vector<ObjectFrame*>& objects);
        
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
        ObjectFrame* getObject() const noexcept;
        
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
        
        PatcherView const&  m_patcher;
        ObjectFrame*        m_object    = nullptr;
        LinkView*           m_link      = nullptr;
        Target              m_target	= Target::Nothing;
        Zone                m_zone		= Zone::Outside;
        int                 m_border	= Border::None;
        size_t              m_index     = 0;
        
        friend class LinkView;
        friend class ObjectFrame;
    };
}
