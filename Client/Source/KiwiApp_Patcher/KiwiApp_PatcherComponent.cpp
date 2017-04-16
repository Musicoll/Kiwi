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

#include "KiwiApp_PatcherComponent.hpp"
#include "../KiwiApp_BinaryData.hpp"
#include "../KiwiApp.hpp"
#include "../KiwiApp_CommandIDs.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                            PATCHER COMPONENT TOOLBAR                             //
    // ================================================================================ //
    
    PatcherToolbarFactory::PatcherToolbarFactory()
    {
        
    }
    
    void PatcherToolbarFactory::getAllToolbarItemIds(juce::Array<int>& ids)
    {
        ids.add(lock_unlock);
        ids.add(zoom_in);
        ids.add(zoom_out);
        ids.add(separatorBarId);
        ids.add(spacerId);
        ids.add(flexibleSpacerId);
        ids.add(dsp_on_off);
    }
    
    void PatcherToolbarFactory::getDefaultItemSet(juce::Array<int>& ids)
    {
        ids.add(dsp_on_off);
        ids.add(separatorBarId);
        ids.add(lock_unlock);
        ids.add(spacerId);
        ids.add(flexibleSpacerId);
        ids.add(zoom_in);
        ids.add(zoom_out);
    }
    
    juce::ToolbarItemComponent* PatcherToolbarFactory::createItem(int itemId)
    {
        juce::ToolbarItemComponent* btn = nullptr;
        
        if(itemId == lock_unlock)
        {
            btn = new juce::ToolbarButton(itemId, "lock/unlock",
                                          juce::Drawable::createFromImageData(binary_data::images::locked_png, binary_data::images::locked_png_size),
                                          juce::Drawable::createFromImageData(binary_data::images::unlocked_png, binary_data::images::unlocked_png_size));
            
            btn->setCommandToTrigger(&KiwiApp::getCommandManager(), CommandIDs::editModeSwitch, true);
        }
        else if(itemId == zoom_in)
        {
            btn = new juce::ToolbarButton(itemId, "zoom in", juce::Drawable::createFromImageData(binary_data::images::zoom_in_png, binary_data::images::zoom_in_png_size), nullptr);
            
            btn->setCommandToTrigger(&KiwiApp::getCommandManager(), CommandIDs::zoomIn, true);
        }
        else if(itemId == zoom_out)
        {
            btn = new juce::ToolbarButton(itemId, "zoom out", juce::Drawable::createFromImageData(binary_data::images::zoom_out_png, binary_data::images::zoom_out_png_size), nullptr);
            
            btn->setCommandToTrigger(&KiwiApp::getCommandManager(), CommandIDs::zoomOut, true);
        }
        else if(itemId == dsp_on_off)
        {
            btn = new juce::ToolbarButton(itemId, "DSP on/off",
                                          juce::Drawable::createFromImageData(binary_data::images::dsp_off_png, binary_data::images::dsp_off_png_size),
                                          juce::Drawable::createFromImageData(binary_data::images::dsp_on_png, binary_data::images::dsp_on_png_size));
            
            btn->setCommandToTrigger(&KiwiApp::getCommandManager(), CommandIDs::switchDsp, true);
        }
        
        return btn;
    }
    
    // ================================================================================ //
    //                                PATCHER COMPONENT                                 //
    // ================================================================================ //
    
    PatcherComponent::PatcherComponent(PatcherView& patcherview) :
    m_patcherview(patcherview)
    {
        setSize(600, 400);
        
        addAndMakeVisible(&patcherview.getViewport(), true);
        patcherview.updateWindowTitle();
        patcherview.grabKeyboardFocus();
        
        KiwiApp::bindToCommandManager(this);
        KiwiApp::bindToKeyMapping(this);
        
        m_toolbar.setVertical(false);
        m_toolbar.setStyle(juce::Toolbar::ToolbarItemStyle::iconsOnly);
        m_toolbar.setColour(juce::Toolbar::ColourIds::backgroundColourId, juce::Colours::whitesmoke);
        m_toolbar.setColour(juce::Toolbar::ColourIds::labelTextColourId, juce::Colours::black);
        m_toolbar.setColour(juce::Toolbar::ColourIds::buttonMouseOverBackgroundColourId, juce::Colours::whitesmoke.contrasting(0.1));
        m_toolbar.setColour(juce::Toolbar::ColourIds::buttonMouseDownBackgroundColourId, juce::Colours::whitesmoke.contrasting(0.2));
        
        m_toolbar.setWantsKeyboardFocus(false);
        addAndMakeVisible(m_toolbar);
        
        // And use our item factory to add a set of default icons to it...
        m_toolbar.addDefaultItems(m_toolbar_factory);
    }
    
    PatcherView& PatcherComponent::usePatcherView()
    {
        return m_patcherview;
    }
    
    void PatcherComponent::resized()
    {
        const int toolbar_size = 40;
        m_toolbar.setBounds(getLocalBounds().withBottom(toolbar_size).reduced(0, 2));
        m_patcherview.getViewport().setBounds(getLocalBounds().withTop(toolbar_size));
    }
    
    void PatcherComponent::paint(juce::Graphics& g)
    {
        const int toolbar_size = 40;
        const auto header_bounds = getLocalBounds().withBottom(toolbar_size);
        
        g.setColour(juce::Colour(0xff444444));
        g.fillRect(header_bounds);
    }
    
    // ================================================================================ //
    //                              APPLICATION COMMAND TARGET                          //
    // ================================================================================ //
    
    juce::ApplicationCommandTarget* PatcherComponent::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void PatcherComponent::getAllCommands(juce::Array<juce::CommandID>& commands)
    {
        m_patcherview.getAllCommands(commands);
    }
    
    void PatcherComponent::getCommandInfo(const juce::CommandID commandID, juce::ApplicationCommandInfo& result)
    {
        m_patcherview.getCommandInfo(commandID, result);
    }
    
    bool PatcherComponent::perform(InvocationInfo const& info)
    {
        bool performed = m_patcherview.perform(info);
        return performed;
    }
}
