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

#include <juce_gui_basics/juce_gui_basics.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectFrame.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectView.h>

namespace kiwi
{
    // ================================================================================ //
    //                                   OBJECT VIEW                                    //
    // ================================================================================ //
    
    ObjectView::ObjectView(model::Object & object_model):
    m_model(object_model)
    {
        setColour(ObjectView::ColourIds::Error, juce::Colour::fromFloatRGBA(0.6, 0.1, 0.1, 0.));
        setColour(ObjectView::ColourIds::Background, juce::Colours::white);
        setColour(ObjectView::ColourIds::Text, juce::Colours::black);
        setColour(ObjectView::ColourIds::Outline, juce::Colours::black);
        setColour(ObjectView::ColourIds::Highlight, juce::Colour::fromFloatRGBA(0., 0.5, 1., 0.));
        setColour(ObjectView::ColourIds::Active, juce::Colour::fromFloatRGBA(0.2, 0.8, 0.2, 1.));
    }
    
    ObjectView::~ObjectView()
    {
    }
    
    model::Object& ObjectView::getModel() const
    {
        return m_model;
    }
}
