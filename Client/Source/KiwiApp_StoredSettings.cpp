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

#include "KiwiApp_StoredSettings.hpp"
#include "KiwiApp.hpp"

namespace kiwi
{
    StoredSettings& getAppSettings()
    {
        return KiwiApp::useSettings();
    }
    
    juce::PropertiesFile& getGlobalProperties()
    {
        return getAppSettings().getGlobalProperties();
    }
    
    static juce::PropertiesFile::Options getPropertyFileOptionsFor(const juce::String& filename)
    {
        juce::PropertiesFile::Options options;
        options.applicationName     = filename;
        options.filenameSuffix      = "settings";
        options.osxLibrarySubFolder = "Application Support";
        
        #if JUCE_LINUX
        options.folderName          = "~/.config/Kiwi";
        #else
        options.folderName          = "Kiwi";
        #endif
        
        return options;
    }
    
    static std::unique_ptr<juce::PropertiesFile> createPropsFile(const juce::String& filename)
    {
        return std::make_unique<juce::PropertiesFile>(getPropertyFileOptionsFor(filename));
    }
    
    // ================================================================================ //
    //                                  STORED SETTINGS                                 //
    // ================================================================================ //
    
    StoredSettings::StoredSettings() :
    m_globals("GLOBAL_SETTINGS"),
    m_network("NETWORK_CONFIG")
    {
        reload();
        m_globals.addListener(this);
        m_network.addListener(this);
    }
    
    StoredSettings::~StoredSettings()
    {
        m_globals.removeListener(this);
        m_network.removeListener(this);
        flush();
    }
    
    juce::PropertiesFile& StoredSettings::getGlobalProperties()
    {
        return *m_property_files[0];
    }
    
    void StoredSettings::flush()
    {
        for(int i = m_property_files.size(); --i >= 0;)
        {
            m_property_files[i]->saveIfNeeded();
        }
    }
    
    void StoredSettings::reload()
    {
        m_property_files.clear();
        m_property_files.emplace_back(createPropsFile("kiwi_settings"));
        
        {
            // reload Global Settings
            std::unique_ptr<juce::XmlElement> globals(getGlobalProperties()
                                                      .getXmlValue("Global Settings"));
            
            if(globals != nullptr)
            {
                m_globals = juce::ValueTree::fromXml(*globals);
            }
        }
   
        {
            // reload Network settings
            std::unique_ptr<juce::XmlElement> networks(getGlobalProperties()
                                                       .getXmlValue("Network Settings"));
            
            if(networks != nullptr)
            {
                m_network = juce::ValueTree::fromXml(*networks);
            }
        }
    }
    
    juce::ValueTree& StoredSettings::globals()
    {
        return m_globals;
    }
    
    juce::ValueTree& StoredSettings::network()
    {
        return m_network;
    }
    
    void StoredSettings::saveValueTree(juce::ValueTree const& vt, std::string const& key_name)
    {
        std::unique_ptr<juce::XmlElement> xml_value(vt.createXml());
        getGlobalProperties().setValue(key_name, xml_value.get());
    }
    
    void StoredSettings::changed()
    {
        saveValueTree(m_globals, "Global Settings");
        saveValueTree(m_network, "Network Settings");
    }
}
