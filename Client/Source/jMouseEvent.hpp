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

#ifndef KIWI_JUCE_MOUSE_EVENT_HPP_INCLUDED
#define KIWI_JUCE_MOUSE_EVENT_HPP_INCLUDED

#include <KiwiGui/KiwiEvents/KiwiMouseEvent.hpp>

#include "../JuceLibraryCode/JuceHeader.h"

namespace kiwi
{
    class jMouseEvent : public gui::MouseEvent
    {
    public:
        
        jMouseEvent(gui::MouseEvent::Type const& type, juce::MouseEvent const& event) noexcept;
        
        jMouseEvent(juce::MouseEvent const& event, juce::MouseWheelDetails const& wheel) noexcept;
        
        ~jMouseEvent() noexcept;
        
        void setMouseUnlimited(bool isLimited, bool visibleUntilLimits = false) const override;
        
    private:
        
        const MouseInputSource m_source;
    };
}

#endif // KIWI_JUCE_MOUSE_EVENT_HPP_INCLUDED
