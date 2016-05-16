/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_JPATCHER_HELPER_HPP_INCLUDED
#define KIWI_JPATCHER_HELPER_HPP_INCLUDED

#include "jPatcher.hpp"
#include "jObject.hpp"
#include "jLink.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                      HITTESTER                                   //
    // ================================================================================ //
	
    //! @brief The HitTester class...
    class HitTester
    {
        friend class jLink;
        friend class jObject;
    public:
        enum class Target : int
		{
			Nothing = 0,
			Patcher = 1,
			Box		= 2,
			Link	= 3
		};
		
        enum class Zone : int
        {
            Outside     = 1<<0,
            Inside      = 1<<1,
            Inlet       = 1<<2,
            Outlet      = 1<<3,
            Border      = 1<<4
        };
		
        enum Border
		{
			None	= 1<<0,
			Left	= 1<<1,
			Right	= 1<<2,
			Top		= 1<<3,
			Bottom	= 1<<4,
		};
        
        //! @brief Contructor.
        HitTester(jPatcher const& patcher);
        
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
                  std::vector<jObject*>& objects,
                  std::vector<jLink*>& links);
        
        //! @brief Test a rectangle.
        //! @details Try to find the objects that overlaps the given rectangle.
        //! @param rect The rectangle.
        //! @param objects The vector of objects that will be filled.
        void testObjects(juce::Rectangle<int> const& rect, std::vector<jObject*>& objects);
        
        //! @brief Test a rectangle.
        //! @details Try to find the links that overlaps the given rectangle.
        //! @param rect The rectangle.
        //! @param links The vector of links that will be filled.
        void testLinks(juce::Rectangle<int> const& rect, std::vector<jLink*>& links);
        
        //! @brief Get the last touched Target.
        inline Target getTarget() const noexcept            { return m_target; }
        
        //! @brief Returns true if nothing has been hit by the test, otherwise returns false.
        inline bool targetNothing() const noexcept          { return m_target == Target::Nothing; }
        
        //! @brief Returns true if the test hit the patcher, otherwise return false.
        inline bool targetPatcher() const noexcept          { return m_target == Target::Patcher; }
        
        //! @brief Returns true if the test hit an object, otherwise return false.
        inline bool targetObject() const noexcept           { return m_target == Target::Box; }
        
        //! @brief Returns true if the test hit a link, otherwise return false.
        inline bool targetLink() const noexcept             { return m_target == Target::Link; }
        
        //! @brief Get the patcher.
        jPatcher const& getPatcher() const noexcept;
        
        //! @brief Get the object box that has been touched by the last hit-test.
        jObject* getObject() const noexcept;
        
        //! @brief Get the link that has been touched by the last hit-test.
        jLink* getLink() const noexcept;
        
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
        
        jPatcher const& m_patcher;
        jObject*        m_object;
        jLink*          m_link;
        Target          m_target	= Target::Nothing;
        Zone            m_zone		= Zone::Outside;
        int             m_border	= Border::None;
        size_t          m_index = 0;
    };
    
    
    //! @brief The jPatcher Viewport
    class jPatcherViewport : public juce::Viewport
    {
    public:
        jPatcherViewport(jPatcher& patcher);
        
    private:
        jPatcher& m_patcher;
    };
    
}

#endif // KIWI_JPATCHER_HELPER_HPP_INCLUDED
