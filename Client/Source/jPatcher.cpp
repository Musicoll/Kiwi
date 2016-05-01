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

#include "jPatcher.hpp"
#include "jMouseEvent.hpp"

namespace kiwi
{
    jPatcher::jPatcher(model::Patcher& patcher_model) : m_model(patcher_model)
    {
        setSize(600, 400);
    }
    
    jPatcher::~jPatcher()
    {
        ;
    }
    
    void jPatcher::paint(juce::Graphics & g)
    {
        const bool locked = false;
        const juce::Colour bgcolor = juce::Colours::lightgrey;
        const int grid_size = 20;
        const juce::Rectangle<int> bounds(g.getClipBounds());
        
        g.setColour(bgcolor);
        g.fillAll();
        
        if (!locked)
        {
            g.setColour(bgcolor.contrasting(0.5));
            for(int x = bounds.getX() - (bounds.getX() % grid_size); x < bounds.getRight(); x += grid_size)
            {
                for(int y = bounds.getY() - (bounds.getY() % grid_size) ; y < bounds.getBottom(); y += grid_size)
                {
                    g.setPixel(x, y);
                }
            }
        }
    }
    
    void jPatcher::resized()
    {
        ;
    }
    
    void jPatcher::mouseDown(juce::MouseEvent const& e)
    {
        auto event = jMouseEvent(gui::MouseEvent::Type::Down, e);
        //receive(jMouseEvent(gui::MouseEvent::Type::Down, e));
    }
    
    void jPatcher::mouseDrag(juce::MouseEvent const& event)
    {
        ;
    }
    
    void jPatcher::mouseUp(juce::MouseEvent const& event)
    {
        ;
    }
    
    void jPatcher::mouseDoubleClick(juce::MouseEvent const& event)
    {
        ;
    }
}
