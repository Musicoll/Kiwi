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

#ifndef KIWI_JLINK_HPP_INCLUDED
#define KIWI_JLINK_HPP_INCLUDED

#include <KiwiModel/KiwiLink.hpp>

#include "jObject.hpp"

namespace kiwi
{
    class jPatcher;
    class jLink;
    
    // ================================================================================ //
    //                                      JLINK                                       //
    // ================================================================================ //
    
    //! @brief The juce link Component.
    class jLink : public juce::Component, public ComponentListener
    {
    public:
        
        //! @brief Constructor
        jLink(jPatcher const& jpatcher, model::Link& link_m);
        
        //! @brief Destructor
        ~jLink();
        
        //! @brief Get the Link model
        model::Link& getModel() const {return *m_model;};
        
        //! Returns true if the link is selected.
        bool isSelected() const noexcept;
        
        void linkChanged(model::Link& link);
        void objectChanged(model::Object& object);
        void localSelectionChanged(bool selected_for_view);
        void distantSelectionChanged(std::set<uint64_t> distant_user_id_selection);
                
        // juce::Component
        void paint(juce::Graphics& g) override;
        
        //! @brief internal kiwi jPatcher Hit-Testing.
        bool hitTest(juce::Point<int> const& pt, HitTester& result) const;
        
        //! @brief internal kiwi jPatcher HitTesting (overlaps a rectangle).
        bool hitTest(juce::Rectangle<float> const& rect);
        
        //! @brief Called when the component's position or size changes.
        //! @param component    the component that was moved or resized
        //! @param wasMoved     true if the component's top-left corner has just moved
        //! @param wasResized   true if the component's width or height has just changed
        void componentMovedOrResized(Component& component, bool was_moved, bool was_resized) override;
        
    private:
        
        void updateBounds();
        
    private: // members
        
        jPatcher const&     m_jpatcher;
        model::Link*        m_model;
        
        juce::Point<int>    m_last_inlet_pos,
                            m_last_outlet_pos;
        
        juce::Path          m_path;
        
        bool                m_is_selected = 0;
        std::set<uint64_t>  m_distant_selection;
    };
    
    // ================================================================================ //
    //                                   JLINK CREATOR                                  //
    // ================================================================================ //
    
    //! @brief The jLink creator helper.
    class jLinkCreator : public juce::Component
    {
    public:
        
        //! @brief Constructor
        jLinkCreator(jObject& binded_object,
                     const size_t index,
                     bool is_sender,
                     juce::Point<int> dragged_pos);
        
        //! @brief Destructor
        ~jLinkCreator() = default;
        
        //! @brief Get the binded object
        jObject& getBindedObject() const {return m_binded_object;};
        
        //! @brief Get the portlet index
        size_t getBindedIndex() const {return m_index;};
        
        //! @brief Returns true if the link is binded to an outlet.
        size_t isBindedToSender() const {return m_is_sender;};
        
        //! @brief Set end position of the link
        void setEndPosition(juce::Point<int> const& pos);
        
        //! @brief Get The end position of the link
        juce::Point<int> getEndPosition() const noexcept;
        
        // juce::Component
        void paint(juce::Graphics& g) override;
        
    private:
        
        void updateBounds();
        
    private: // members
        
        jObject&            m_binded_object;
        const size_t        m_index;
        const bool          m_is_sender;
        juce::Point<int>    m_outlet_pos;
        juce::Point<int>    m_inlet_pos;
        juce::Path          m_path;
    };
}

#endif // KIWI_JLINK_HPP_INCLUDED
