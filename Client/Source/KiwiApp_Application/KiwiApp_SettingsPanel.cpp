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

#include <KiwiApp_Application/KiwiApp_SettingsPanel.h>
#include <KiwiApp_General/KiwiApp_IDs.h>
#include <KiwiApp.h>
#include <KiwiApp_General/KiwiApp_StoredSettings.h>

namespace kiwi
{
    // ================================================================================ //
    //                                  SETTINGS PANEL                                  //
    // ================================================================================ //
    
    SettingsPanel::SettingsPanel():
    m_settings(getAppSettings().network().getServerAddress()),
    m_pannel("Application settings"),
    m_apply_button("apply"),
    m_reset_button("reset")
    {
        
        juce::Array<juce::PropertyComponent*> props {
            
            new juce::TextPropertyComponent(m_settings
                                            .getPropertyAsValue(Ids::host, nullptr),"Host", 20,false),
            new juce::TextPropertyComponent(m_settings
                                            .getPropertyAsValue(Ids::api_port, nullptr), "API Port", 5, false),
            new juce::TextPropertyComponent(m_settings
                                            .getPropertyAsValue(Ids::session_port, nullptr), "Session Port", 5, false)
            
        };
        
        m_pannel.addSection("Network config", props, true, 0);
        
        m_apply_button.setButtonText("Apply");
        m_reset_button.setButtonText("Reset");
        
        m_apply_button.addListener(this);
        m_reset_button.addListener(this);
        
        setSize(400, m_pannel.getTotalContentHeight() + 50);
        
        addAndMakeVisible(m_pannel);
        addAndMakeVisible(m_apply_button);
        addAndMakeVisible(m_reset_button);
    }
    
    SettingsPanel::~SettingsPanel()
    {
        m_pannel.clear();
    }
    
    void SettingsPanel::resized()
    {
        juce::Rectangle<int> bounds = getLocalBounds();
        
        m_pannel.setBounds(bounds.withHeight(m_pannel.getTotalContentHeight()));
        
        m_apply_button.setBounds(bounds
                                 .withPosition(m_pannel.getX() + 10, m_pannel.getBottom() + 10)
                                 .withHeight(30));
        
        m_apply_button.changeWidthToFitText();
        
        m_reset_button.setBounds(m_apply_button.getBounds().withLeft(m_apply_button.getRight() + 10));
        
        m_reset_button.changeWidthToFitText();
    }
    
    void SettingsPanel::buttonClicked(juce::Button * button)
    {
        if (button->getName() == "apply")
        {
            getAppSettings().network().setServerAddress(m_settings[Ids::host].toString().toStdString(),
                                                        m_settings[Ids::api_port].operator int(),
                                                        m_settings[Ids::session_port].operator int());
        }
        else if(button->getName() == "reset")
        {
            NetworkSettings & network = getAppSettings().network();
            
            network.resetToDefault();
            
            m_settings.copyPropertiesFrom(network.getServerAddress(), nullptr);
        }
    }
}
