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

#include "KiwiApp_SettingsPanel.h"
#include "../KiwiApp.h"

namespace kiwi
{
    // ================================================================================ //
    //                                  SETTINGS PANEL                                  //
    // ================================================================================ //
    
    SettingsPanel::SettingsPanel() : juce::PropertyPanel("Application settings")
    {
        auto& net_config = getAppSettings().network();
        
        juce::Array<juce::PropertyComponent*> props {
            
            new juce::TextPropertyComponent(net_config.getHostValue(), "Host", 20, false),
            new juce::TextPropertyComponent(net_config.getApiPortValue(), "API Port", 5, false),
            new juce::TextPropertyComponent(net_config.getSessionPortValue(), "Session Port", 5, false),
            new juce::TextPropertyComponent(net_config.getRefreshIntervalValue(), "Refresh interval time", 5, false)
            
        };
        
        addSection("Network config", props, true, 0);
        
        setSize(300, 400);
    }
    
    SettingsPanel::~SettingsPanel()
    {
        clear();
    }
}
