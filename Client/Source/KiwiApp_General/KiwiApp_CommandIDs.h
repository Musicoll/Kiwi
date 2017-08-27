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

namespace kiwi
{
    // ================================================================================ //
    //                                     COMMAND IDs                                  //
    // ================================================================================ //
    
    enum CommandIDs
    {
        newPatcher                  = 0x200010,        ///< Create a new blank patcher window.
        newPatcherView              = 0x200020,        ///< Create a new patcher view.
        openFile                    = 0x200030,        ///< Open a file in a new window.
        closePatcher                = 0x200051,        ///< Close the current patcher.
        save                        = 0x200060,        ///< Save the current patcher document.
        saveAs                      = 0x200061,        ///< Save the current patcher document as.
        
        minimizeWindow              = 0x201010,        ///< Reduce the current window.
        maximizeWindow              = 0x201020,        ///< Maximise the  current window.
        closeWindow                 = 0x201031,        ///< Close the current window.
        
        showConsoleWindow           = 0x202000,        ///< Make visible the "console" window.
        showAudioStatusWindow       = 0x202010,        ///< Make visible the "audio status" window.
        showAboutAppWindow          = 0x202020,        ///< Make visible the "about app" window.
        showAppSettingsWindow       = 0x202030,        ///< Make visible the "application settings" window.
        showDocumentBrowserWindow   = 0x202040,        ///< Make visible the "document browser" window.
        showBeaconDispatcherWindow  = 0x202050,        ///< Make visible the "beacon dispatcher" window.
        addBeaconDispatcher         = 0x202051,        ///< Add a new "beacon dispatcher" element.
        removeBeaconDispatcher      = 0x202052,        ///< Remove a "beacon dispatcher" element.
        
        undo                        = 0xf1000a,        ///< Undo last action.
        redo                        = 0xf1000b,        ///< Redo last action.
        duplicate                   = 0xf1000c,        ///< Duplicate selected objects and paste them on patcher.
        pasteReplace                = 0xf1000d,        ///< Replace an objects by the one in the clipboard.
        
        toFront                     = 0xf2000a,        ///< Move selected object ahead of all other objects.
        toBack                      = 0xf2000b,        ///< Move selected object behind all other objects.
        
        zoomIn                      = 0xf20013,        ///< Magnify the patcher view of almost 10%.
        zoomOut                     = 0xf20014,        ///< Reduce the patcher view of almost 10%.
        zoomNormal                  = 0xf20015,        ///< Restore the patcher view zoom to 100%.
        
        editModeSwitch              = 0xf20100,        ///< Toggle Lock/Unlock patcher view.
        
        gridModeSwitch              = 0xf20200,        ///< Toggle grid patcher mode.
        enableSnapToGrid            = 0xf20201,        ///< Toggle snap to grid patcher mode.
        
        newBox                      = 0xf30300,        ///< Add a new "box" to the patcher.
        newMessage                  = 0xf30301,        ///< Add a new "message" object box to the patcher.
        newFlonum                   = 0xf30302,        ///< Add a new "flonum" object box to the patcher.
        newNumber                   = 0xf30303,        ///< Add a new "number" object box to the patcher.
        newComment                  = 0xf30304,        ///< Add a new "comment" object box to the patcher.
        newBang                     = 0xf30305,        ///< Add a new "button" object box to the patcher.
        newToggle                   = 0xf30306,        ///< Add a new "toggle" object box to the patcher.
        
        showPatcherInspector        = 0xf20400,        ///< Shows the patcher properties inspector.
        
        showObjectInspector         = 0xf20410,        ///< Shows the selected objects properties inspector.
        openObjectHelp              = 0xf20411,        ///< Open selected object help patcher.
        
        switchDsp                   = 0xf20420,        ///< Toggle DSP state
        startDsp                    = 0xf20421,        ///< Starts the dsp
        stopDsp                     = 0xf20422,        ///< Stops the dsp
        
        scrollToTop                 = 0xf30001,        ///< Scroll to the top
        scrollToBottom              = 0xf30002,        ///< Scroll to the bottom
        
        clearAll                    = 0xf40001,        ///< Clear all content
        
        login                       = 0xf50000,        ///< Log-in the user.
        signup                      = 0xf50010,        ///< Register the user.
        logout                      = 0xf50020,        ///< Log-out the user.
        remember_me                 = 0xf50030,        ///< Toggle the "remember me" option to save user profile.
    };
    
    // ================================================================================ //
    //                                 COMMAND CATEGORIES                               //
    // ================================================================================ //
    
    namespace CommandCategories
    {
        static const char* const general       = "General";
        static const char* const editing       = "Editing";
        static const char* const view          = "View";
        static const char* const windows       = "Windows";
    }
}
