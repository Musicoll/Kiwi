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

#include "KiwiApp_CustomToolbarButton.h"

namespace kiwi
{
    
    
    CustomToolbarButton::CustomToolbarButton(const int iid,
                                             const juce::String& buttonText,
                                             const juce::Colour bgcolor,
                                             juce::Drawable* const normalIm,
                                             juce::Drawable* const toggledOnIm)
    : ToolbarItemComponent(iid, buttonText, true),
    normalImage(normalIm),
    toggledOnImage(toggledOnIm),
    currentImage(nullptr),
    m_bgcolor(bgcolor)
    {
        jassert(normalImage != nullptr);
    }
    
    CustomToolbarButton::~CustomToolbarButton()
    {
    }
    
    //==============================================================================
    bool CustomToolbarButton::getToolbarItemSizes(int toolbarDepth, bool /*isToolbarVertical*/, int& preferredSize, int& minSize, int& maxSize)
    {
        preferredSize = minSize = maxSize = toolbarDepth;
        return true;
    }
    
    void CustomToolbarButton::paintButtonArea(juce::Graphics& g,
                                              int width, int height,
                                              bool mouse_over, bool mouse_down)
    {
        const auto bounds = g.getClipBounds().reduced(1.).toFloat();
        const bool over_or_down = (mouse_over || mouse_down);
        g.setColour(m_bgcolor.contrasting(over_or_down ? 0.f : 0.1f).withAlpha(isEnabled() ? 1.0f : 0.8f));
        g.fillEllipse(bounds.reduced(mouse_down ? 1. : 0.));
    }
    
    void CustomToolbarButton::contentAreaChanged(const juce::Rectangle<int>&)
    {
        buttonStateChanged();
    }
    
    void CustomToolbarButton::setCurrentImage(juce::Drawable* const newImage)
    {
        if(newImage != currentImage)
        {
            removeChildComponent(currentImage);
            currentImage = newImage;
            
            if(currentImage != nullptr)
            {
                enablementChanged();
                addAndMakeVisible(currentImage);
                updateDrawable();
            }
        }
    }
    
    void CustomToolbarButton::updateDrawable()
    {
        if(currentImage != nullptr)
        {
            currentImage->setInterceptsMouseClicks(false, false);
            currentImage->setTransformToFit(getContentArea().reduced(6).toFloat(),
                                            juce::RectanglePlacement::centred);
            
            currentImage->setAlpha(isEnabled() ? 1.0f : 0.5f);
        }
    }
    
    void CustomToolbarButton::resized()
    {
        ToolbarItemComponent::resized();
        updateDrawable();
    }
    
    void CustomToolbarButton::enablementChanged()
    {
        ToolbarItemComponent::enablementChanged();
        updateDrawable();
    }
    
    juce::Drawable* CustomToolbarButton::getImageToUse() const
    {
        if(getStyle() == juce::Toolbar::textOnly)
            return nullptr;
        
        if(getToggleState() && toggledOnImage != nullptr)
            return toggledOnImage;
        
        return normalImage;
    }
    
    void CustomToolbarButton::buttonStateChanged()
    {
        setCurrentImage(getImageToUse());
    }
}
