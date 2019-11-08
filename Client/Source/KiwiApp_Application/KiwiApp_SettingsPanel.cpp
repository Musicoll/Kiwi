/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
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
    m_settings("panel_settings"),
    m_pannel("Application settings"),
    m_apply_button("Apply"),
    m_reset_button("Reset")
    {
        
        loadAppSettings();
        
        juce::Array<juce::PropertyComponent*> props {
            
            new juce::TextPropertyComponent(m_settings
                                            .getPropertyAsValue("host", nullptr),"Host", 20,false),
            new juce::TextPropertyComponent(m_settings
                                            .getPropertyAsValue("api_port", nullptr), "API Port", 5, false),
            new juce::TextPropertyComponent(m_settings
                                            .getPropertyAsValue("session_port", nullptr), "Session Port", 5, false),
            new juce::BooleanPropertyComponent(m_settings
                                               .getPropertyAsValue("verify_certificate", nullptr), "Verify Certificate", "")
            
        };
        
        m_pannel.addSection("Network config", props, true, 0);
        
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
    
    void SettingsPanel::loadAppSettings()
    {
        m_settings.setProperty("host", juce::String(getAppSettings().network().getHost()), nullptr);
        m_settings.setProperty("api_port", getAppSettings().network().getApiPort(), nullptr);
        m_settings.setProperty("session_port", getAppSettings().network().getSessionPort(), nullptr);
        m_settings.setProperty("verify_certificate", getAppSettings().network().getVerifyCertificate(), nullptr);
    }
    
    void SettingsPanel::storeAppSettings()
    {
        getAppSettings().network().setServerAddress(m_settings["host"].toString().toStdString(),
                                                    m_settings["api_port"].operator int(),
                                                    m_settings["session_port"].operator int());
        getAppSettings().network().setVerifyCertificate(m_settings["verify_certificate"]);
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
    
    void SettingsPanel::buttonClicked(juce::Button* button)
    {
        if (button == &m_apply_button)
        {
            storeAppSettings();
        }
        else if(button == &m_reset_button)
        {
            NetworkSettings & network = getAppSettings().network();
            
            network.resetToDefault();
            
            loadAppSettings();
        }
    }
}
