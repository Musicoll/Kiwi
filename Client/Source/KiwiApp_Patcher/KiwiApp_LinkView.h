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

#pragma once

#include <KiwiModel/KiwiModel_Link.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectFrame.h>

namespace kiwi
{
    class PatcherView;
    class LinkView;
    
    // ================================================================================ //
    //                                   LINK VIEW BASE                                 //
    // ================================================================================ //
    
    //! @brief The LinkView base class.
    class LinkViewBase : public juce::Component
    {
    public:
        LinkViewBase() = default;
        virtual ~LinkViewBase() = default;
        
    protected:
        void updateBounds();
        
    protected: // members
        juce::Point<int>    m_last_outlet_pos;
        juce::Point<int>    m_last_inlet_pos;
        juce::Path          m_path;
    };
    
    // ================================================================================ //
    //                                     LINK VIEW                                    //
    // ================================================================================ //
    
    //! @brief The juce link Component.
    class LinkView : public LinkViewBase, public juce::ComponentListener
    {
    public:
        
        //! @brief Constructor
        LinkView(PatcherView& patcherview, model::Link& link_m);
        
        //! @brief Destructor
        ~LinkView();
        
        //! @brief Get the Link model
        model::Link& getModel() const {return *m_model;};
        
        //! Returns true if the link is selected.
        bool isSelected() const noexcept;
        
        void linkChanged(model::Link& link);
        void objectChanged(model::Object& object);
        void localSelectionChanged(bool selected_for_view);
        void distantSelectionChanged(std::set<uint64_t> distant_user_id_selection);
        
        // ! @brief Returns true if this link reprensent a signal connection.
        bool isSignal() const;
                
        // juce::Component
        void paint(juce::Graphics& g) override;
        
        //! @brief internal kiwi PatcherView Hit-Testing.
        bool hitTest(juce::Point<int> const& pt, HitTester& result) const;
        
        //! @brief internal kiwi PatcherView HitTesting (overlaps a rectangle).
        bool hitTest(juce::Rectangle<float> const& rect);
        
        //! @brief Called when the component's position or size changes.
        //! @param component    the component that was moved or resized
        //! @param wasMoved     true if the component's top-left corner has just moved
        //! @param wasResized   true if the component's width or height has just changed
        void componentMovedOrResized(Component& component, bool was_moved, bool was_resized) override;
        
    private: // members
        
        PatcherView&        m_patcherview;
        model::Link*        m_model;
        bool                m_is_selected = 0;
        std::set<uint64_t>  m_distant_selection;
    };
    
    // ================================================================================ //
    //                                 LINK VIEW CREATOR                                //
    // ================================================================================ //
    
    //! @brief The LinkView creator helper.
    class LinkViewCreator : public LinkViewBase
    {
    public:
        
        //! @brief Constructor
        LinkViewCreator(ObjectFrame& binded_object,
                     const size_t index,
                     bool is_sender,
                     juce::Point<int> dragged_pos);
        
        //! @brief Destructor
        ~LinkViewCreator() = default;
        
        //! @brief Get the binded object
        ObjectFrame& getBindedObject() const {return m_binded_object;};
        
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
        
    private: // members
        
        ObjectFrame&        m_binded_object;
        const size_t        m_index;
        const bool          m_is_sender;
    };
}
