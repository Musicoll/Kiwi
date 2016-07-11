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

#include "BeaconDispatcher.hpp"
#include "StoredSettings.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                  BEACON DISPATCHER                               //
    // ================================================================================ //
    
    BeaconDispatcher::BeaconDispatcher(engine::Instance& instance) : m_instance(instance)
    {
        setInterceptsMouseClicks(false, true);
        
        // beacon name field
        m_beacon_name_editor.reset(new juce::TextEditor("beacon_name_editor"));
        m_beacon_name_editor->setReturnKeyStartsNewLine(false);
        m_beacon_name_editor->setMultiLine(false, false);
        m_beacon_name_editor->setColour(juce::TextEditor::highlightColourId,
                                        juce::Colour::fromFloatRGBA(0., 0.5, 1., 0.4));
        
        m_beacon_name_editor->setColour(juce::TextEditor::focusedOutlineColourId,
                                        juce::Colour::fromFloatRGBA(0.4, 0.4, 0.4, 0.6));
        
        m_beacon_name_editor->setColour(juce::TextEditor::outlineColourId,
                                        juce::Colour::fromFloatRGBA(0.6, 0.6, 0.6, 0.6));
        
        m_beacon_name_editor->setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentWhite);
        
        // message field
        m_message_editor.reset(new juce::TextEditor("message_editor"));
        m_message_editor->setReturnKeyStartsNewLine(false);
        m_message_editor->setMultiLine(false, false);
        m_message_editor->setColour(juce::TextEditor::highlightColourId,
                                        juce::Colour::fromFloatRGBA(0., 0.5, 1., 0.4));
        
        m_message_editor->setColour(juce::TextEditor::focusedOutlineColourId,
                                    juce::Colour::fromFloatRGBA(0.4, 0.4, 0.4, 0.6));
        
        m_message_editor->setColour(juce::TextEditor::outlineColourId,
                                    juce::Colour::fromFloatRGBA(0.6, 0.6, 0.6, 0.6));
        
        m_message_editor->setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentWhite);
        
        //button
        m_send_button.reset(new juce::TextButton("Send"));
        m_send_button->addListener(this);
        
        addAndMakeVisible(m_beacon_name_editor.get());
        addAndMakeVisible(m_message_editor.get());
        addAndMakeVisible(m_send_button.get());
    }
    
    void BeaconDispatcher::resized()
    {
        m_beacon_name_editor->setBounds(10, 10, getWidth() - 20, 20);
        m_message_editor->setBounds(10, 40, getWidth() - 20, 20);
        
        m_send_button->setSize(60, 20);
        m_send_button->setTopRightPosition(getWidth() - 10, 70);
    }
    
    void BeaconDispatcher::buttonClicked(juce::Button* btn)
    {
        if(btn == m_send_button.get())
        {
            std::string name = m_beacon_name_editor->getText().toStdString();
            std::string args = m_message_editor->getText().toStdString();
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
    
    // ================================================================================ //
    //                               BEACON DISPATCHER WINDOW                           //
    // ================================================================================ //
    
    BeaconDispatcherWindow::BeaconDispatcherWindow(engine::Instance& instance) :
    jWindow("Beacon dispatcher", juce::Colours::white, minimiseButton | closeButton, true)
    {
        setContentOwned(new BeaconDispatcher(instance), false);
        setResizable(false, false);
        setResizeLimits(50, 50, 32000, 32000);
        
        setTopLeftPosition(10, 10);
        setSize(300, 100);
        setVisible(true);
    }
    
    BeaconDispatcherWindow::~BeaconDispatcherWindow()
    {
        ;
    }
    
    void BeaconDispatcherWindow::closeButtonPressed()
    {
        setVisible(false);
    }
}