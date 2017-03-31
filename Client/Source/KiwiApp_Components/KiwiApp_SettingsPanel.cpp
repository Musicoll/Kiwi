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

#include "KiwiApp_SettingsPanel.hpp"
#include "../KiwiApp.hpp"

namespace kiwi
{
    // ================================================================================ //
    //                                  SETTINGS PANEL                                  //
    // ================================================================================ //
    
    SettingsPanel::SettingsPanel() : juce::PropertyPanel("Application settings")
    {
        auto& net_config = getAppSettings().network();
        
        juce::Array<juce::PropertyComponent*> props {
            
            new juce::TextPropertyComponent(net_config.getPropertyAsValue("host", nullptr),
                                            "Host", 20, false),
            
            new juce::TextPropertyComponent(net_config.getPropertyAsValue("api_port", nullptr),
                                            "API Port", 5, false),
            
            new juce::TextPropertyComponent(net_config.getPropertyAsValue("session_port", nullptr),
                                            "Session Port", 5, false),
            
            new juce::TextPropertyComponent(net_config.getPropertyAsValue("refresh_interval", nullptr),
                                            "Refresh interval time", 5, false)
            
        };
        
        addSection("Network config", props, true, 0);
    }
    
    SettingsPanel::~SettingsPanel()
    {
        clear();
    }
    
    
    // ================================================================================ //
    //                               SETTINGS PANEL WINDOW                              //
    // ================================================================================ //
    
    SettingsPanelWindow::SettingsPanelWindow() :
    AppWindow("Application settings")
    {
        setContentOwned(new SettingsPanel(), false);
        initBounds({0, 0, 300, 400});
        
        setResizeLimits(50, 50, 32000, 32000);
        setResizable(true, true);
        //setVisible(true);
    }
    
    bool SettingsPanelWindow::isMainWindow() const
    {
        return true;
    }
}
