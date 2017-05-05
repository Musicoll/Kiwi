/*
 ==============================================================================
 
 This file is part of the JUCE library.
 Copyright(c) 2017 - ROLI Ltd.
 
 JUCE is an open source library subject to commercial or open-source
 licensing.
 
 By using JUCE, you agree to the terms of both the JUCE 5 End-User License
 Agreement and JUCE 5 Privacy Policy(both updated and effective as of the
 27th April 2017).
 
 End User License Agreement: www.juce.com/juce-5-licence
 Privacy Policy: www.juce.com/juce-5-privacy-policy
 
 Or: You may also use this code under the terms of the GPL v3(see
 www.gnu.org/licenses).
 
 JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
 EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
 DISCLAIMED.
 
 ==============================================================================
 */

#include "KiwiApp_CustomToolbarButton.hpp"

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
