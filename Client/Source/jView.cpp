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

#include "jView.hpp"
#include "jMouseEvent.hpp"

namespace kiwi
{
    jView::jView()
    {
        setSize(600, 400);
    }
    
    jView::~jView()
    {
        ;
    }
    
    // ================================================================================ //
    //                                      gui::View                                   //
    // ================================================================================ //
    
    void jView::redraw()
    {
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
            repaint();
        }
    }
    
    // ================================================================================ //
    //                                   juce::Component                                //
    // ================================================================================ //
    
    void jView::paint(juce::Graphics & g)
    {
        ;
    }
    
    void jView::resized()
    {
        ;
    }
    
    void jView::mouseDown(juce::MouseEvent const& event)
    {
        gui::View::receive(jMouseEvent(gui::MouseEvent::Type::Down, event));
    }
    
    void jView::mouseDrag(juce::MouseEvent const& event)
    {
        gui::View::receive(jMouseEvent(gui::MouseEvent::Type::Drag, event));
    }
    
    void jView::mouseUp(juce::MouseEvent const& event)
    {
        gui::View::receive(jMouseEvent(gui::MouseEvent::Type::Up, event));
    }
    
    void jView::mouseDoubleClick(juce::MouseEvent const& event)
    {
        gui::View::receive(jMouseEvent(gui::MouseEvent::Type::DoubleClick, event));
    }
}
