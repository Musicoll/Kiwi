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

#ifndef KIWI_COMMAND_IDS_HPP_INCLUDED
#define KIWI_COMMAND_IDS_HPP_INCLUDED

enum CommandIDs
{
    newPatcher                  = 0x200010,        ///< Create a new blank patcher window.
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
    
    duplicate                   = 0xf1000a,        ///< Duplicate selected objects and paste them on patcher.
    pasteReplace                = 0xf1000b,        ///< Replace an objects by the one in the clipboard.
    
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
    
    showObjectInspector         = 0xf20500,        ///< Shows the selected objects properties inspector.
    openObjectHelp              = 0xf20510         ///< Open selected object help patcher.
};

namespace CommandCategories
{
    static const char* const general       = "General";
    static const char* const editing       = "Editing";
    static const char* const view          = "View";
    static const char* const windows       = "Windows";
}

#endif // KIWI_COMMAND_IDS_HPP_INCLUDED
