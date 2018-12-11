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

#include "KiwiApp_TooltipWindow.h"
#include "../KiwiApp_General/KiwiApp_LookAndFeel.h"

namespace kiwi
{
    // ================================================================================ //
    //                              CUSTOM TOOLTIP CLIENT                               //
    // ================================================================================ //
    
    bool CustomTooltipClient::drawTooltip(juce::Graphics& g,
                                          juce::String const& text,
                                          int width, int height)
    {
        return false;
    }
    
    // ================================================================================ //
    //                              CUSTOM TOOLTIP WINDOW                               //
    // ================================================================================ //
    
    TooltipWindow::TooltipWindow(Component* const parent_comp, const int delay_ms)
    : Component("tooltip")
    , m_milliseconds_before_tip_appears(delay_ms)
    , m_mouse_clicks(0), m_mouse_wheel_moves(0)
    , m_last_comp_change_time(0), m_last_hide_time(0)
    , m_reentrant(false)
    {
        if(juce::Desktop::getInstance().getMainMouseSource().canHover())
        {
            startTimer(123);
        }
        
        setAlwaysOnTop(true);
        setOpaque(false);
        
        if(parent_comp != nullptr)
        {
            parent_comp->addChildComponent(this);
        }
    }
    
    TooltipWindow::~TooltipWindow()
    {
        hideTip();
    }
    
    void TooltipWindow::setMillisecondsBeforeTipAppears(const int new_delay_ms) noexcept
    {
        m_milliseconds_before_tip_appears = new_delay_ms;
    }
    
    void TooltipWindow::paint(juce::Graphics& g)
    {
        if(!m_custom_client
           || (m_custom_client
               && !m_custom_client->drawTooltip(g, m_tip_showing, getWidth(), getHeight())))
        {
            getLookAndFeel().drawTooltip(g, m_tip_showing, getWidth(), getHeight());
        }
    }
    
    void TooltipWindow::mouseEnter(juce::MouseEvent const&)
    {
        hideTip();
    }
    
    void TooltipWindow::updatePosition(juce::String const& tip,
                                       juce::Point<int> pos,
                                       juce::Rectangle<int> parentArea)
    {
        const juce::TextLayout tl(LookAndFeel::layoutTooltipText(tip));
        
        const int w = (int)(tl.getWidth() + 14.0f);
        const int h = (int)(tl.getHeight() + 6.0f);
        
        if(m_custom_client != nullptr)
        {
            setBounds(m_custom_client->getTooltipBounds(tip, pos, parentArea, w, h));
        }
        else
        {
            setBounds(juce::Rectangle<int>(pos.x > parentArea.getCentreX() ? pos.x - (w + 12) : pos.x + 24,
                                           pos.y > parentArea.getCentreY() ? pos.y - (h + 6) : pos.y + 6,
                                           w, h).constrainedWithin(parentArea));
        }
        
        setVisible(true);
    }
    
    void TooltipWindow::displayTip(juce::Point<int> screenPos, const juce::String& tip)
    {
        jassert(tip.isNotEmpty());
        
        if(! m_reentrant)
        {
            juce::ScopedValueSetter<bool> setter(m_reentrant, true, false);
            
            if(m_tip_showing != tip)
            {
                m_tip_showing = tip;
                repaint();
            }
            
            if(auto* parent = getParentComponent())
            {
                updatePosition(tip, parent->getLocalPoint(nullptr, screenPos),
                               parent->getLocalBounds());
            }
            else
            {
                updatePosition(tip, screenPos, juce::Desktop::getInstance().getDisplays()
                               .findDisplayForPoint(screenPos).userArea);
                
                addToDesktop(  juce::ComponentPeer::windowHasDropShadow
                             | juce::ComponentPeer::windowIsTemporary
                             | juce::ComponentPeer::windowIgnoresKeyPresses
                             | juce::ComponentPeer::windowIgnoresMouseClicks);
            }
            
            toFront(false);
        }
    }
    
    void TooltipWindow::setCustomTooltipClient(CustomTooltipClient& client)
    {
        m_custom_client = &client;
    }
    
    void TooltipWindow::unsetCustomTooltipClient(CustomTooltipClient& client)
    {
        if(m_custom_client == &client)
        {
            m_custom_client = nullptr;
        }
    }
    
    void TooltipWindow::hideTip()
    {
        if(! m_reentrant)
        {
            m_tip_showing.clear();
            removeFromDesktop();
            setVisible(false);
            m_custom_client = nullptr;
        }
    }
    
    void TooltipWindow::timerCallback()
    {
        juce::Desktop& desktop = juce::Desktop::getInstance();
        const juce::MouseInputSource mouseSource(desktop.getMainMouseSource());
        const unsigned int now = juce::Time::getApproximateMillisecondCounter();
        
        const bool has_custom_tooltip = (m_custom_client != nullptr);
        
        Component* const new_comp = ! mouseSource.isTouch() ? mouseSource.getComponentUnderMouse() : nullptr;
        
        juce::String new_tip;
        
        if(juce::Process::isForegroundProcess())
        {
            if(has_custom_tooltip)
            {
                new_tip = m_custom_client->getTooltip();
            }
            else if(new_comp
                    && !juce::ModifierKeys::getCurrentModifiers().isAnyMouseButtonDown())
            {
                if(juce::TooltipClient* const ttc = dynamic_cast<juce::TooltipClient*>(new_comp))
                    if(! new_comp->isCurrentlyBlockedByAnotherModalComponent())
                        new_tip = ttc->getTooltip();
            }
        }
        
        const bool tip_changed = (new_tip != m_last_tip_under_mouse
                                  || m_custom_client != m_last_custom_client
                                  || new_comp != m_last_component_under_mouse);
        
        m_last_custom_client = m_custom_client;
        m_last_component_under_mouse = new_comp;
        m_last_tip_under_mouse = new_tip;
        
        const int click_count = desktop.getMouseButtonClickCounter();
        const int wheel_count = desktop.getMouseWheelMoveCounter();
        const bool mouse_was_clicked = (click_count > m_mouse_clicks || wheel_count > m_mouse_wheel_moves);
        m_mouse_clicks = click_count;
        m_mouse_wheel_moves = wheel_count;
        
        const juce::Point<float> mousePos(mouseSource.getScreenPosition());
        const bool mouse_moved_quickly = !has_custom_tooltip
        ? mousePos.getDistanceFrom(m_last_mouse_pos) > 12 : false;
        m_last_mouse_pos = mousePos;
        
        if(tip_changed || mouse_was_clicked || mouse_moved_quickly)
            m_last_comp_change_time = now;
        
        if(isVisible() || now < m_last_hide_time + 500)
        {
            // if a tip is currently visible(or has just disappeared), update to a new one
            // immediately if needed..
            if((!has_custom_tooltip && (new_comp == nullptr || mouse_was_clicked || new_tip.isEmpty()))
               || (has_custom_tooltip && (mouse_was_clicked || new_tip.isEmpty())))
            {
                if(isVisible())
                {
                    m_last_hide_time = now;
                    hideTip();
                }
            }
            else if(tip_changed)
            {
                displayTip(mousePos.roundToInt(), new_tip);
            }
        }
        else
        {
            // if there isn't currently a tip, but one is needed, only let it
            // appear after a timeout..
            if(new_tip.isNotEmpty()
               && new_tip != m_tip_showing
               && now > m_last_comp_change_time + (unsigned int) m_milliseconds_before_tip_appears)
            {
                displayTip(mousePos.roundToInt(), new_tip);
            }
        }
    }
}
