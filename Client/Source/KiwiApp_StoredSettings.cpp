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
    
    juce::PropertiesFile& StoredSettings::getGlobalProperties()
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
    
    juce::ValueTree& StoredSettings::useDefault()
    {
        return m_defaults;
    }
    
    void StoredSettings::changed()
    {
        juce::ScopedPointer<juce::XmlElement> data(m_defaults.createXml());
        m_property_files[0]->setValue("KIWI_APP_DEFAULT_SETTINGS", data);
    }
}
