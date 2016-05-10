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

#ifndef KIWI_JOBJECT_HPP_INCLUDED
#define KIWI_JOBJECT_HPP_INCLUDED

#include <KiwiModel/KiwiObject.hpp>

#include "../JuceLibraryCode/JuceHeader.h"

namespace kiwi
{
    //! @brief The juce object Component.
    class jObject : public juce::Component
    {
    public:
        
        jObject();
        ~jObject();
        
        void objectChanged(model::Object& object);
        
        // juce::Component
        void paint(juce::Graphics& g) override;
        void mouseDown(juce::MouseEvent const& event) override;
        void mouseDrag(juce::MouseEvent const& event) override;
        
        //! @brief Returns the inlet position relative to the parent jPatcher component for a given index.
        juce::Point<int> getInletPatcherPosition(const size_t index) const;
        
        //! @brief Returns the outlet position relative to the parent jPatcher component for a given index.
        juce::Point<int> getOutletPatcherPosition(const size_t index) const;
        
    private:
        
        void drawInletsOutlets(juce::Graphics & g);
        
        //! @brief Returns the inlet local bounds for a given index.
        juce::Rectangle<int> getInletLocalBounds(const size_t index,
                                                 juce::Rectangle<int> const& object_bounds) const;
        
        //! @brief Returns the outlet local bounds for a given index.
        juce::Rectangle<int> getOutletLocalBounds(const size_t index,
                                                  juce::Rectangle<int> const& object_bounds) const;
        
        model::Object*      m_model = nullptr;
        const unsigned int  m_io_width = 5;
        const unsigned int  m_io_height = 3;
        size_t              m_inlets = 0;
        size_t              m_outlets = 0;
        const juce::Colour  m_io_color;
    };
}

#endif // KIWI_JOBJECT_HPP_INCLUDED
