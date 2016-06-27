/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright(c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2(or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#include "StoredSettings.hpp"
#include "Application.hpp"
//#include "../Application/jucer_GlobalPreferences.h"

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
    
    static PropertiesFile::Options getPropertyFileOptionsFor(const String& filename)
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
    
    static std::unique_ptr<juce::PropertiesFile> createPropsFile(const String& filename)
    {
        return std::make_unique<juce::PropertiesFile>(getPropertyFileOptionsFor(filename));
    }
    
    // ================================================================================ //
    //                                  STORED SETTINGS                                 //
    // ================================================================================ //
    
    StoredSettings::StoredSettings() : m_defaults("KIWI_APP_DEFAULT_SETTINGS")
    {
        reload();
        m_defaults.addListener(this);
    }
    
    StoredSettings::~StoredSettings()
    {
        m_defaults.removeListener(this);
        flush();
    }
    
    PropertiesFile& StoredSettings::getGlobalProperties()
    {
        return *m_property_files[0];
    }
    
    void StoredSettings::updateGlobalPreferences()
    {
        ;
    }
    
    void StoredSettings::flush()
    {
        updateGlobalPreferences();
        
        for(int i = m_property_files.size(); --i >= 0;)
        {
            m_property_files[i]->saveIfNeeded();
        }
    }
    
    void StoredSettings::reload()
    {
        m_property_files.clear();
        m_property_files.emplace_back(createPropsFile("kiwi_settings"));
        
        std::unique_ptr<juce::XmlElement> defaults(m_property_files[0]->getXmlValue("KIWI_APP_DEFAULT_SETTINGS"));
        
        if(defaults != nullptr)
        {
            m_defaults = juce::ValueTree::fromXml(*defaults);
        }
    }
    
    void StoredSettings::changed()
    {
        ScopedPointer<XmlElement> data(m_defaults.createXml());
        m_property_files[0]->setValue("KIWI_APP_DEFAULT_SETTINGS", data);
    }
}
