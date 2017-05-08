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

#pragma once

#include <juce_data_structures/juce_data_structures.h>
#include <KiwiEngine/KiwiEngine_Listeners.hpp>

namespace kiwi
{
    // ================================================================================ //
    //                                  NETWORK SETTINGS                                //
    // ================================================================================ //
 
    class StoredSettings;
    
    class NetworkSettings : juce::ValueTree::Listener
    {
    public: // methods
        
        //! @brief Constructor
        NetworkSettings();
        
        //! @brief Destructor
        ~NetworkSettings();
        
        //! @brief Reset to default settings values
        void resetToDefault();
        
        //! @brief Restore settings with an xml.
        bool readFromXml(juce::XmlElement const& xml);
        
        //! @brief Returns the Host as a string
        std::string getHost() const;
        
        //! @brief Returns the Host as a juce::Value
        juce::Value getHostValue();
        
        //! @brief Returns the api port as an integer
        uint16_t getApiPort() const;
        
        //! @brief Returns the api port as a juce::Value
        juce::Value getApiPortValue();
        
        //! @brief Returns the session port as an integer
        uint16_t getSessionPort() const;
        
        //! @brief Returns the session port as a juce::Value
        juce::Value getSessionPortValue();
        
        //! @brief Returns the session port as an integer
        uint16_t getRefreshInterval() const;
        
        //! @brief Returns the session port as a juce::Value
        juce::Value getRefreshIntervalValue();
        
        //! @brief NetworkSettings Listener
        struct Listener
        {
            //! @brief Destructor.
            virtual ~Listener() = default;
            
            //! @brief Called when a document session has been added.
            virtual void networkSettingsChanged(NetworkSettings const&, juce::Identifier const&) = 0;
        };
                                        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief remove a listener.
        void removeListener(Listener& listener);
        
    private: // methods
        
        //! @brief Returns the underlying value tree
        juce::ValueTree& use();
        
        void valueTreePropertyChanged(juce::ValueTree&, juce::Identifier const&) override;
        void valueTreeChildAdded(juce::ValueTree&, juce::ValueTree&) override               {}
        void valueTreeChildRemoved(juce::ValueTree&, juce::ValueTree&, int) override        {}
        void valueTreeChildOrderChanged(juce::ValueTree&, int, int) override                {}
        void valueTreeParentChanged(juce::ValueTree&) override                              {}
        
    private: // variables
        
        juce::ValueTree m_settings;
        engine::Listeners<Listener> m_listeners;
        
        friend StoredSettings;
    };
    
    // ================================================================================ //
    //                                  STORED SETTINGS                                 //
    // ================================================================================ //
    
    //! @brief Settings storage class utility.
    class StoredSettings : public juce::ValueTree::Listener
    {
    public: // methods
        
        //! @brief Constructor.
        StoredSettings();
        
        //! @brief Destructor.
        ~StoredSettings();
        
        //! @brief Returns the global properties file.
        juce::PropertiesFile& getGlobalProperties();
        
        //! @brief Flush settings.
        void flush();
        
        //! @brief Reload settings.
        void reload();
        
        //! @brief Returns the NetworkSettings
        NetworkSettings& network();
        
    private: // methods
        
        void saveValueTree(juce::ValueTree const& vt, std::string const& key_name);
        void changed();

        void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) override   { changed(); }
        void valueTreeChildAdded(juce::ValueTree&, juce::ValueTree&) override               { changed(); }
        void valueTreeChildRemoved(juce::ValueTree&, juce::ValueTree&, int) override        { changed(); }
        void valueTreeChildOrderChanged(juce::ValueTree&, int, int) override                { changed(); }
        void valueTreeParentChanged(juce::ValueTree&) override                              { changed(); }
        
    private: // members
        
        std::vector<std::unique_ptr<juce::PropertiesFile>> m_property_files;
        NetworkSettings                                    m_network;
        
    private: // deleted methods
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StoredSettings)
    };
    
    StoredSettings& getAppSettings();
    juce::PropertiesFile& getGlobalProperties();
};
