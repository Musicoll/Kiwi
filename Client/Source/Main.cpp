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

#include <KiwiEngine/KiwiEngine.hpp>

#include "../JuceLibraryCode/JuceHeader.h"

namespace kiwi
{
    //==============================================================================
    class KiwiApplication  : public JUCEApplication
    {
    public:
        //==============================================================================
        KiwiApplication() {}
        
        const String getApplicationName() override       { return ProjectInfo::projectName; }
        const String getApplicationVersion() override    { return ProjectInfo::versionString; }
        bool moreThanOneInstanceAllowed() override       { return true; }
        
        //==============================================================================
        void initialise (String const& commandLine) override
        {
            //m_instance = Instance::create(123456789ULL, "Main");
        }
        
        void shutdown() override
        {
        }
        
        //==============================================================================
        void systemRequestedQuit() override
        {
            //m_instance.reset();
            
            // This is called when the app is being asked to quit: you can ignore this
            // request and let the app carry on running, or call quit() to allow the app to close.
            quit();
        }
        
        void anotherInstanceStarted (const String& commandLine) override
        {
            // When another instance of the app is launched while this one is running,
            // this method is invoked, and the commandLine parameter tells you what
            // the other instance's command-line arguments were.
        }
        
    private:
        //std::unique_ptr<Instance> m_instance;
    };
    
    //==============================================================================
    // This macro generates the main() routine that launches the app.
    START_JUCE_APPLICATION (KiwiApplication)
}