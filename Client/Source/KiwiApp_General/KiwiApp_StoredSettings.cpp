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
#include "../KiwiApp_Resources/KiwiApp_BinaryData.h"
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
        const juce::File file = getPropertyFileOptionsFor(filename, "json").getDefaultFile();
        
        if (file.exists())
        {
            std::string json_user_str = file.loadFileAsString().toStdString();
            json j;
            try
            {
                j = json::parse(json_user_str);
            }
            catch (json::parse_error& e)
            {
                std::cerr << "message: " << e.what() << '\n';
            }
            
            return j;
        }
        
        return {};
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
        m_settings.removeAllChildren(nullptr);
        
        std::unique_ptr<juce::XmlElement> xml(nullptr);
        
        auto str = juce::String::createStringFromData(binary_data::settings::network_settings,
                                                      binary_data::settings::network_settings_size);
        
        xml.reset(juce::XmlDocument::parse(str));
        assert(xml != nullptr);
        
        juce::ValueTree tree = juce::ValueTree::fromXml(*xml);
        
        if (tree.hasProperty(Ids::remember_me))
        {
            m_settings.setProperty(Ids::remember_me, tree.getProperty(Ids::remember_me), nullptr);
        }
        
        juce::ValueTree server_tree = tree.getChildWithName(Ids::server_address).createCopy();
        
        if (server_tree.isValid())
        {
            m_settings.addChild(server_tree, 0, nullptr);
        }
    }
    
    juce::ValueTree NetworkSettings::getServerAddress()
    {
        return m_settings.getChildWithName(Ids::server_address).createCopy();
    }
    
    void NetworkSettings::setServerAddress(std::string const& host, uint16_t api_port, uint16_t session_port)
    {
        juce::ValueTree tree(Ids::server_address);
        tree.setProperty(Ids::host, juce::String(host), nullptr);
        tree.setProperty(Ids::api_port, api_port, nullptr);
        tree.setProperty(Ids::session_port, session_port, nullptr);
        
        m_settings.removeChild(m_settings.getChildWithName(Ids::server_address), nullptr);
        m_settings.addChild(tree, 0, nullptr);
    }
    
    void NetworkSettings::readFromXml(juce::XmlElement const& xml)
    {
        if(xml.hasTagName(m_settings.getType().toString()))
        {
            const auto new_settings(juce::ValueTree::fromXml(xml));
            
            if (new_settings.hasProperty(Ids::remember_me))
            {
                m_settings.setProperty(Ids::remember_me,
                                       new_settings.getProperty(Ids::remember_me),
                                       nullptr);
            }
            
            juce::ValueTree server_tree = new_settings.getChildWithName(Ids::server_address).createCopy();
            
            if (server_tree.isValid())
            {
                m_settings.removeChild(m_settings.getChildWithName(Ids::server_address), nullptr);
                m_settings.addChild(server_tree, 0, nullptr);
            }
        }
    }
    
    juce::ValueTree& NetworkSettings::use()
    {
        return m_settings;
    }
    
    std::string NetworkSettings::getHost() const
    {
        return m_settings.getChildWithName(Ids::server_address).getProperty(Ids::host).toString().toStdString();
    }
    
    uint16_t NetworkSettings::getApiPort() const
    {
        int port = m_settings.getChildWithName(Ids::server_address).getProperty(Ids::api_port);
        return static_cast<uint16_t>(port);
    }
    
    uint16_t NetworkSettings::getSessionPort() const
    {
        int port = m_settings.getChildWithName(Ids::server_address).getProperty(Ids::session_port);
        return static_cast<uint16_t>(port);
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
    
    void NetworkSettings::valueTreeChildAdded(juce::ValueTree& parent, juce::ValueTree& child)
    {
        m_listeners.call(&Listener::networkSettingsChanged, *this, child.getType());
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
        if (xml)
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
