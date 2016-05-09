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

#include "jWindow.hpp"

namespace kiwi
{
    jWindow::jWindow(std::string const& name, juce::Colour color, int requiredButtons, bool addToDesktop) :
    DocumentWindow(name, color, requiredButtons, addToDesktop)
    {
        setUsingNativeTitleBar(true);
        setSize(600, 500);
        centreWithSize(getWidth(), getHeight());
        setResizable(true, true);
        setVisible(true);
    }
    
    jWindow::~jWindow()
    {
        ;
    }
    
    void jWindow::closeButtonPressed()
    {
        juce::JUCEApplication::quit();
    }
}