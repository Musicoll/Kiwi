/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiApp_BeaconDispatcher.hpp"
#include "KiwiApp_StoredSettings.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                  BEACON DISPATCHER                               //
    // ================================================================================ //
    
    BeaconDispatcher::BeaconDispatcher(engine::Instance& instance) :
    m_instance(instance),
    m_beacon_name_editor("beacon_name_editor"),
    m_message_editor("message_editor"),
    m_beacon_name_label("beacon_name_label", "Name: "),
    m_message_label("message_label", "Message: "),
    m_send_button("Send")
    {
        setInterceptsMouseClicks(false, true);
        
        // beacon name field
        m_beacon_name_editor.setReturnKeyStartsNewLine(false);
        m_beacon_name_editor.setMultiLine(false, false);
        
        // message field
        m_message_editor.setReturnKeyStartsNewLine(false);
        m_message_editor.setMultiLine(false, false);
        
        //button
        m_send_button.addListener(this);
        
        addAndMakeVisible(m_beacon_name_label);
        addAndMakeVisible(m_beacon_name_editor);
        addAndMakeVisible(m_message_editor);
        addAndMakeVisible(m_message_label);
        addAndMakeVisible(m_send_button);
        
        setSize(280, 120);
    }
    
    void BeaconDispatcher::resized()
    {
        auto bounds = getLocalBounds();
        
        int padding = 4;
        int row_height = 40;
        int label_height = 20;
        
        auto row_bounds = bounds.removeFromTop(row_height);
        m_beacon_name_label.setBounds(row_bounds.removeFromTop(label_height));
        m_beacon_name_editor.setBounds(row_bounds.reduced(padding, 0));
        
        row_bounds = bounds.removeFromTop(row_height);
        m_message_label.setBounds(row_bounds.removeFromTop(label_height));
        m_message_editor.setBounds(row_bounds.reduced(padding, 0));
        
        row_bounds = bounds.removeFromTop(30);
        m_send_button.setSize(60, 20);
        m_send_button.setTopRightPosition(getWidth() - padding, row_bounds.getY() + 10);
    }
    
    void BeaconDispatcher::buttonClicked(juce::Button* btn)
    {
        if(btn == &m_send_button)
        {
            std::string name = m_beacon_name_editor.getText().toStdString();
            std::string args = m_message_editor.getText().toStdString();
            send(name, AtomHelper::parse(args));
        }
    }
    
    void BeaconDispatcher::send(std::string const& name, std::vector<Atom> const& args) const
    {
        if(!name.empty() && !args.empty())
        {
            engine::Beacon& beacon = m_instance.getBeacon(name);
            beacon.dispatch(args);
        }
    }
}
