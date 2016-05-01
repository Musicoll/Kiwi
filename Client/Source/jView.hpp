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

#ifndef KIWI_JUCE_VIEW_HPP_INCLUDED
#define KIWI_JUCE_VIEW_HPP_INCLUDED

#include <KiwiGui/KiwiView/KiwiView.hpp>

#include "../JuceLibraryCode/JuceHeader.h"

namespace kiwi
{
    // ================================================================================ //
    //                                      jVIEW                                       //
    // ================================================================================ //
    
    //! @brief The juce Patcher Component.
    class jView : public gui::View, public juce::Component
    {
    public:
        jView();
        ~jView();
        
        // gui::View
        void redraw() override;
        
        // juce::Component
        void paint(juce::Graphics& g) override;
        void resized() override;

        void mouseDown(juce::MouseEvent const& event) override;
        void mouseDrag(juce::MouseEvent const& event) override;
        void mouseUp(juce::MouseEvent const& event) override;
        void mouseDoubleClick(juce::MouseEvent const& event) override;
        
    private:
    };
}

#endif // KIWI_JUCE_VIEW_HPP_INCLUDED
