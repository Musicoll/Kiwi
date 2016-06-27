/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_STORED_SETTINGS_HPP_INCLUDED
#define KIWI_STORED_SETTINGS_HPP_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

namespace kiwi
{
    // ================================================================================ //
    //                                  STORED SETTINGS                                 //
    // ================================================================================ //
    
    //! @brief Settings storage class utility.
    class StoredSettings : public juce::ValueTree::Listener
    {
    public:
        
        //! @brief Constructor.
        StoredSettings();
        
        //! @brief Destructor.
        ~StoredSettings();
        
        //! @brief Returns the global properties file.
        PropertiesFile& getGlobalProperties();
        
        //! @brief Flush settings.
        void flush();
        
        //! @brief Reload settings.
        void reload();
        
    private:
        std::vector<std::unique_ptr<PropertiesFile>> m_property_files;
        ValueTree                                    m_defaults;
        
        void changed();
        void updateGlobalPreferences();

        void valueTreePropertyChanged(ValueTree&, const Identifier&) override  { changed(); }
        void valueTreeChildAdded(ValueTree&, ValueTree&) override              { changed(); }
        void valueTreeChildRemoved(ValueTree&, ValueTree&, int) override       { changed(); }
        void valueTreeChildOrderChanged(ValueTree&, int, int) override         { changed(); }
        void valueTreeParentChanged(ValueTree&) override                       { changed(); }
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StoredSettings)
    };
    
    StoredSettings& getAppSettings();
    juce::PropertiesFile& getGlobalProperties();
};

#endif   // JUCER_STOREDSETTINGS_H_INCLUDED
