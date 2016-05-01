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

#ifndef KIWI_APPLICATION_HPP_INCLUDED
#define KIWI_APPLICATION_HPP_INCLUDED

#include "jInstance.hpp"

namespace kiwi
{
    //==============================================================================
    class KiwiApplication  : public JUCEApplication
    {
    public:
        KiwiApplication() = default;
        ~KiwiApplication() = default;
        
        void initialise (String const& commandLine) override;
        
        void shutdown() override;
        
        void systemRequestedQuit() override;
        
        const String getApplicationName() override       { return ProjectInfo::projectName; }
        const String getApplicationVersion() override    { return ProjectInfo::versionString; }
        bool moreThanOneInstanceAllowed() override       { return true; }
        
    private:
        std::unique_ptr<jInstance> m_instance;
    };
    
    //==============================================================================
    // This macro generates the main() routine that launches the app.
    START_JUCE_APPLICATION (KiwiApplication)
}

#endif // KIWI_APPLICATION_HPP_INCLUDED
