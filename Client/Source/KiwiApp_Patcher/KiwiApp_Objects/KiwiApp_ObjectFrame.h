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

#include <memory>
#include <string>

#include <juce_gui_basics/juce_gui_basics.h>

#include <KiwiModel/KiwiModel_Patcher.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_EditableObjectView.h>
#include <KiwiApp_Patcher/KiwiApp_PatcherView.h>

namespace kiwi
{
    // ================================================================================ //
    //                                     OBJECT FRAME                                 //
    // ================================================================================ //
    
    //! @brief A juce component holding the object's graphical representation.
    //! @details ObjectFrame is implemented as a wrapper around an object view that displays
    //! selections and outlet and handle certain interactions.
    class ObjectFrame
    : public juce::Component
    , public EditableObjectView::Listener
    {
    public: // methods
        
        //! @brief Constructor.
        ObjectFrame(PatcherView& patcher_view, std::unique_ptr<ObjectView> object_view);
        
        //! @brief Destructor.
        ~ObjectFrame();
        
        //! @brief Called whenever an object's model has changed.
        void objectChanged(model::Patcher::View& view, model::Object& object);
        
        //! @brief Updates the inner object's view attributes.
        void attributeChanged(std::string const& name, tool::Parameter const& parameter);
        
        //! @brief Called whenever the client wants to edit an object.
        //! @details Will only edit the object if its a textual object.
        void editObject();
        
        //! @brief Called by the patcher every time the selection status of this object has changed.
        //! @details Function called when local selection or distant selection has changed.
        void selectionChanged();
        
        //! @brief Called every time a patcher is locked or unlocked.
        void lockStatusChanged();
        
        //! @brief Called when the patcher's origin changes.
        void patcherViewOriginPositionChanged();
        
        //! @brief Returns The object's bounds relative to the patcher position.
        juce::Rectangle<int> getObjectBounds() const;
        
        //! @brief Returns the inlet position relative to the parent PatcherView component for a given index.
        juce::Point<int> getInletPatcherPosition(const size_t index) const;
        
        //! @brief Returns the outlet position relative to the parent PatcherView component for a given index.
        juce::Point<int> getOutletPatcherPosition(const size_t index) const;
        
        //! @brief Returns the object's model.
        model::Object& getModel() const;
        
        //! @brief Overloaded from juce::Component to exclude border size.
        bool hitTest(int x, int y) override;
        
        //! @brief Internal kiwi PatcherView HitTesting.
        bool hitTest(juce::Point<int> const& pt, HitTester& result) const;
        
        //! @brief Internal kiwi PatcherView HitTesting (overlaps a rectangle).
        bool hitTest(juce::Rectangle<int> const& rect) const;
        
        //! @brief Returns true if the object is selected.
        bool isSelected() const;
        
        //! @brief Called when object's frame is clicked.
        void mouseDown(juce::MouseEvent const& e) override final;
        
        //! @brief Called when object's frame is clicked.
        void mouseUp(juce::MouseEvent const& e) override final;
        
        //! @brief Called when object's frame is clicked.
        void mouseDrag(juce::MouseEvent const& e) override final;
        
        //! @brief Get the ComponentBoundsConstrainer.
        juce::ComponentBoundsConstrainer* getBoundsConstrainer() const;
        
        //! @brief Get the resizing Flag as a set of border.
        //! @see HitTester::Border
        int getResizingFlags() const;
        
    public: // classes
        
        struct Outline : public juce::Component
        {
        public: // classes
            
            enum Border : int
            {
                Top =       1 << 0,
                Bottom =    1 << 1,
                Left =      1 << 2,
                Right =     1 << 3
            };
            
        public: // methods
            
            //! @brief Constructor.
            Outline(int resize_length, int resize_thickness);
            
            using juce::Component::hitTest; // silent virtual function overload hiding warning
            
            //! @brief Tests if the point reaches an interactive resiable corner.
            bool hitTest(juce::Point<int> const& pt, HitTester& hit_tester) const;
            
            //! @brief Returns the corner border width.
            int getBorderThickness() const;
            
            //! @brief Returns the corner border length.
            int getResizeLength() const;
            
            ~Outline();
            
        private: // methods
            
            //! @brief Draws a corner.
            void drawCorner(juce::Graphics & g, Border border);
            
            //! @brief Graphical rendering method.
            void paint(juce::Graphics & g) override final;
            
            //! @brief Call once size changed. Recompute borders and corners position.
            void resized() override final;
            
            //! @brief Update the corners position
            void updateCorners();
            
        private: // members
            
            int                                                     m_resize_length;
            int                                                     m_resize_thickness;
            std::map<Border, std::array<juce::Rectangle<float>, 3>> m_corners;
        };
        
    private: // methods
        
        //! @brief Paints outlets, inlets over child component.
        void paintOverChildren (juce::Graphics& g) override final;
        
        //! @brief Called whenever the object's size changes.
        void resized() override final;
        
        //! @brief Called once a ClassicView's text has changed.
        void textChanged(std::string const& new_text) override final;
        
        //! @brief Called when the classic view ends its edition.
        void editorHidden() override final;
        
        //! @brief Called when the classic view enters its edition mode.
        void editorShown() override final;
        
        //! @brief Called to update the bounds of the object.
        void updateBoundsFromModel(bool animate);
        
        //! @brief Updates the outline according to the selection status.
        //! @details Makes it visible or not and updates its colour.
        void updateOutline();
        
        //! @brief Called when the object's view size has changed.
        //! @details The obect frame adapts to fit the underlying object's size.
        void childBoundsChanged(juce::Component * child) override final;
        
        //! @brief Returns the patcher view component.
        PatcherView& getPatcherView() const;
        
        //! @brief Returns a list of Users that selected an object.
        std::set<uint64_t> getDistantSelection() const;
        
        //! @brief Returns the number of inlets
        size_t getNumberOfInlets() const noexcept;
        
        //! @brief Returns the number of outlets
        size_t getNumberOfOutlets() const noexcept;
        
        //! @brief Update the inlets
        void updateInlets(model::Object const& object);
        
        //! @brief Update the outlets
        void updateOutlets(model::Object const& object);
        
        //! @brief Draws the inlets/outlets of the object.
        void drawInletsOutlets(juce::Graphics & g);
        
        //! @brief Returns the inlet local bounds for a given index.
        juce::Rectangle<int> getInletLocalBounds(const size_t index) const;
        
        //! @brief Returns the outlet local bounds for a given index.
        juce::Rectangle<int> getOutletLocalBounds(const size_t index) const;
        
        //! @brief Returns the width of any outlet/inlet represented in the object's frame.
        size_t getPinWidth() const;
        
        //! @brief Returns the height of any outlet/inlet represented in the object's frame.
        size_t getPinHeight() const;
        
        //! @brief Returns true if the object is locked.
        bool isLocked() const;
        
    private: // members
        
        struct Pin
        {
            Pin(bool pin_is_signal)
            : is_signal(pin_is_signal)
            {}
            
            bool is_signal {false};
        };
        
        std::unique_ptr<ObjectView> m_object_view;
        PatcherView&                m_patcher_view;
        const size_t                m_io_width;
        const size_t                m_io_height;
        std::vector<Pin>            m_inlets;
        std::vector<Pin>            m_outlets;
        Outline                     m_outline;
        
    private: // deleted methods
        
        ObjectFrame() = delete;
        ObjectFrame(ObjectFrame const& other) = delete;
        ObjectFrame(ObjectFrame && other) = delete;
        ObjectFrame& operator=(ObjectFrame const& other) = delete;
        ObjectFrame& operator=(ObjectFrame && other) = delete;
    };
}

