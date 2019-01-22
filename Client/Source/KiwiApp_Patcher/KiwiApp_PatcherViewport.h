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
        void jumpViewToObject(ObjectFrame const&);
        
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
        Component                   m_magnifier;
        juce::Rectangle<int>        m_last_bounds {};
        juce::Rectangle<int>        m_patching_area {};
        double                      m_zoom_factor = 1.;
        bool                        m_can_hook_resized = false;
    };
}
