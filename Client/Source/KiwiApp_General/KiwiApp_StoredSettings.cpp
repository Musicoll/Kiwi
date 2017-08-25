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

#include "KiwiApp_StoredSettings.h"
#include "../KiwiApp.h"
#include "../KiwiApp_Ressources/KiwiApp_BinaryData.h"
#include "KiwiApp_IDs.h"

namespace kiwi
{
    // ================================================================================ //
    //                                  GLOBAL FUNCTIONS                                //
    // ================================================================================ //
    
    StoredSettings& getAppSettings()
    {
        return KiwiApp::useSettings();
    }
    
    juce::PropertiesFile& getGlobalProperties()
    {
        return getAppSettings().getGlobalProperties();
    }
    
    juce::PropertiesFile::Options getPropertyFileOptionsFor(juce::String const& filename, juce::String const& suffix)
    {
        assert(suffix.isNotEmpty() && "need an extension");
        
        juce::PropertiesFile::Options options;
        options.applicationName     = filename;
        options.filenameSuffix      = suffix;
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
    
    bool saveJsonToFile(juce::String const& filename, json const& j)
    {
        juce::File file = getPropertyFileOptionsFor(filename, "json").getDefaultFile();
        juce::TemporaryFile tempFile(file);
        
        {
            juce::FileOutputStream out(tempFile.getFile());
            
            if (!out.openedOk())
                return false;
            
            std::string json_str = j.dump(4);
            out.write(json_str.data(), json_str.size());
            
            out.flush(); // (called explicitly to force an fsync on posix)
            
            if (out.getStatus().failed())
                return false;
        }
        
        return tempFile.overwriteTargetFileWithTemporary();
    }
    
    json getJsonFromFile(juce::String const& filename)
    {
        juce::File file = getPropertyFileOptionsFor(filename, "json").getDefaultFile();
        std::string json_user_str = file.loadFileAsString().toStdString();
        return json::parse(json_user_str);
    }
    
    // ================================================================================ //
    //                                  NETWORK SETTINGS                                //
    // ================================================================================ //
    
    NetworkSettings::NetworkSettings() : m_settings(Ids::NETWORK_CONFIG)
    {
        resetToDefault();
        m_settings.addListener(this);
    }
    
    NetworkSettings::~NetworkSettings()
    {
        m_settings.removeListener(this);
    }
    
    void NetworkSettings::resetToDefault()
    {
        m_settings.removeAllChildren(nullptr);
        m_settings.removeAllProperties(nullptr);
        
        std::unique_ptr<juce::XmlElement> xml(nullptr);
        
        auto str = juce::String::createStringFromData(binary_data::settings::network_settings,
                                                      binary_data::settings::network_settings_size);
        
        xml.reset(juce::XmlDocument::parse(str));
        assert(xml != nullptr);
        
        // load default settings
        m_settings = juce::ValueTree::fromXml(*xml);
    }
    
    bool NetworkSettings::readFromXml(juce::XmlElement const& xml)
    {
        if(xml.hasTagName(m_settings.getType().toString()))
        {
            const auto new_settings(juce::ValueTree::fromXml(xml));
            
            for(int i = new_settings.getNumProperties(); --i >= 0;)
            {
                const auto key = new_settings.getPropertyName(i);
                if(m_settings.hasProperty(key))
                {
                    m_settings.setProperty(key, new_settings.getProperty(key), nullptr);
                }
            }
            
            return true;
        }
        
        return false;
    }
    
    juce::ValueTree& NetworkSettings::use()
    {
        return m_settings;
    }
    
    std::string NetworkSettings::getHost() const
    {
        return m_settings.getProperty(Ids::host).toString().toStdString();
    }
    
    juce::Value NetworkSettings::getHostValue()
    {
        return m_settings.getPropertyAsValue(Ids::host, nullptr);
    }
    
    uint16_t NetworkSettings::getApiPort() const
    {
        int port = m_settings.getProperty(Ids::api_port);
        return static_cast<uint16_t>(port);
    }
    
    juce::Value NetworkSettings::getApiPortValue()
    {
        return m_settings.getPropertyAsValue(Ids::api_port, nullptr);
    }
    
    uint16_t NetworkSettings::getSessionPort() const
    {
        int port = m_settings.getProperty(Ids::session_port);
        return static_cast<uint16_t>(port);
    }
    
    juce::Value NetworkSettings::getSessionPortValue()
    {
        return m_settings.getPropertyAsValue(Ids::session_port, nullptr);
    }
    
    uint16_t NetworkSettings::getRefreshInterval() const
    {
        int time = m_settings.getProperty(Ids::refresh_interval);
        return static_cast<uint16_t>(time);
    }
    
    juce::Value NetworkSettings::getRefreshIntervalValue()
    {
        return m_settings.getPropertyAsValue(Ids::refresh_interval, nullptr);
    }
    
    void NetworkSettings::setRememberUserFlag(bool remember_me)
    {
        m_settings.setProperty(Ids::remember_me, remember_me, nullptr);
    }
    
    bool NetworkSettings::getRememberUserFlag() const
    {
        return m_settings.getProperty(Ids::remember_me);
    }
    
    void NetworkSettings::addListener(Listener& listener)
    {
        m_listeners.add(listener);
    }
    
    void NetworkSettings::removeListener(Listener& listener)
    {
        m_listeners.remove(listener);
    }
    
    void NetworkSettings::valueTreePropertyChanged(juce::ValueTree&, juce::Identifier const& id)
    {
        m_listeners.call(&Listener::networkSettingsChanged, *this, id);
    }
    
    // ================================================================================ //
    //                                  STORED SETTINGS                                 //
    // ================================================================================ //
    
    StoredSettings::StoredSettings()
    {
        reload();
        m_network.use().addListener(this);
    }
    
    StoredSettings::~StoredSettings()
    {
        m_network.use().removeListener(this);
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
   
        // Try to reload User settings
        std::unique_ptr<juce::XmlElement> xml(getGlobalProperties().getXmlValue("Network Settings"));
        if(xml)
        {
            m_network.readFromXml(*xml);
        }
    }
    
    NetworkSettings& StoredSettings::network()
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
        saveValueTree(m_network.use(), "Network Settings");
    }
}
