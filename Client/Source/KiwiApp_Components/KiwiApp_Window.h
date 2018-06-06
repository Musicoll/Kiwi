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

#include <juce_gui_basics/juce_gui_basics.h>

namespace kiwi
{
    // ================================================================================ //
    //                                      WINDOW                                      //
    // ================================================================================ //

    //! @brief Common interface for all windows held by the application.
    class Window : public juce::DocumentWindow, public juce::ApplicationCommandTarget
    {
    public: // methods
        
        //! @brief Constructor.
        //! @details Setup the window and restore its state from stored settings it they exist.
        //! @param content A component that will be owned and displayed by the window
        //! @param resizable Pass true to make a resizable window.
        //! @param is_main_window see isMainWindow()
        //! @param settings_name The name of the window settings
        //! (pass an empty string if you dont want to save the window state)
        //! @param add_menubar Pass true to add the kiwi application menubar to the window
        Window(std::string const& name,
               std::unique_ptr<juce::Component> content,
               bool resizable = false,
               bool is_main_window = true,
               juce::String settings_name = "",
               bool add_menubar = false);
        
        //! @brief Window destructor. Called whenever buttonPressed is called.
        //! @details Will store position into settings if needed.
        virtual ~Window();
        
        //! @brief Return true if window shall be a main window of kiwi.
        //! @details On windows platform, the application quit when all main windows are closed.
        bool isMainWindow() const;
        
        //! @brief Restore the window state
        //! @details If the settings name you passed in the ctor is not empty,
        //! the function will try to read and apply these settings.
        //! @see saveWindowState
        void restoreWindowState();
        
        //! @brief Save the window state
        //! @details If the settings name you passed in the ctor is not empty,
        //! the function will save window state.
        //! @see restoreWindowState
        void saveWindowState();
        
        //! @brief Close the window.
        void close();
        
        // ================================================================================ //
        //                              APPLICATION COMMAND TARGET                          //
        // ================================================================================ //
        
        ApplicationCommandTarget* getNextCommandTarget() override;
        void getAllCommands(juce::Array<juce::CommandID>& commands) override;
        void getCommandInfo(const juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
        bool perform(InvocationInfo const& info) override;
        
    protected: // methods
        
        //! @brief Called when close button is pressed. Request instance to close the window.
        void closeButtonPressed() override;
        
    private: // variables
        
        juce::String    m_settings_name     {};
        bool            m_is_main_window    {false};
    };
    
    //! @brief Singleton application window's ids
    //! @see Instance::showWindowWithId
    enum class WindowId : std::size_t
    {
        Console = 0,
        FormComponent,
        AboutKiwi,
        DocumentBrowser,
        ApplicationSettings,
        AudioSettings,
        BeaconDispatcher,
        count               // Number of WindowIds
    };
}
