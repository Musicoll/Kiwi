/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright(c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright(c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiApp_ImageButton.hpp"

namespace kiwi
{
    
    // ================================================================================ //
    //                                   IMAGE BUTTON                                   //
    // ================================================================================ //
    
    ImageButton::ImageButton(juce::String const& name,
                             std::unique_ptr<juce::Drawable> drawable,
                             ButtonStyle style)
    : juce::Button(name),
    m_style(style),
    m_current_image(nullptr),
    m_edge_indent(3)
    {
        setImages(drawable.get());
    }
    
    ImageButton::~ImageButton()
    {
    }
    
    //==============================================================================
    static std::unique_ptr<juce::Drawable> copyDrawableIfNotNull(juce::Drawable const* const d)
    {
        return d != nullptr ? std::unique_ptr<juce::Drawable>(d->createCopy()) : nullptr;
    }
    
    void ImageButton::setImages(juce::Drawable const* normal,
                                juce::Drawable const* over,
                                juce::Drawable const* down,
                                juce::Drawable const* disabled,
                                juce::Drawable const* normalOn,
                                juce::Drawable const* overOn,
                                juce::Drawable const* downOn,
                                juce::Drawable const* disabledOn)
    {
        jassert(normal != nullptr); // you really need to give it at least a normal image..
        
        m_normal_image      = copyDrawableIfNotNull(normal);
        m_over_image        = copyDrawableIfNotNull(over);
        m_down_image        = copyDrawableIfNotNull(down);
        m_disabled_image    = copyDrawableIfNotNull(disabled);
        m_normal_image_on   = copyDrawableIfNotNull(normalOn);
        m_over_image_on     = copyDrawableIfNotNull(overOn);
        m_down_image_on     = copyDrawableIfNotNull(downOn);
        m_disabled_image_on = copyDrawableIfNotNull(disabledOn);
        m_current_image     = nullptr;
        
        buttonStateChanged();
    }
    
    void ImageButton::setCommand(std::function<void(void)> fn)
    {
        m_command = fn;
    }
    
    void ImageButton::clicked(juce::ModifierKeys const& modifiers)
    {
        if(m_command) { m_command(); }
    }
    
    //==============================================================================
    ImageButton::ButtonStyle ImageButton::getStyle() const noexcept
    {
        return m_style;
    }
    
    void ImageButton::setButtonStyle(const ImageButton::ButtonStyle new_style)
    {
        if(m_style != new_style)
        {
            m_style = new_style;
            buttonStateChanged();
        }
    }
    
    void ImageButton::setEdgeIndent(const int num_pixels_indent)
    {
        m_edge_indent = num_pixels_indent;
        repaint();
        resized();
    }
    
    juce::Rectangle<float> ImageButton::getImageBounds() const
    {
        juce::Rectangle<int> r(getLocalBounds());
        
        if(m_style != ButtonStyle::ImageStretched)
        {
            int indentX = juce::jmin(m_edge_indent, proportionOfWidth(0.3f));
            int indentY = juce::jmin(m_edge_indent, proportionOfHeight(0.3f));
            
            if(m_style == ButtonStyle::ImageOnButtonBackground)
            {
                indentX = juce::jmax(getWidth()  / 4, indentX);
                indentY = juce::jmax(getHeight() / 4, indentY);
            }
            else if(m_style == ButtonStyle::ImageAboveTextLabel)
            {
                r = r.withTrimmedBottom(juce::jmin(16, proportionOfHeight(0.25f)));
            }
            
            r = r.reduced(indentX, indentY);
        }
        
        return r.toFloat();
    }
    
    void ImageButton::resized()
    {
        Button::resized();
    }
    
    void ImageButton::buttonStateChanged()
    {
        repaint();
    }
    
    void ImageButton::enablementChanged()
    {
        Button::enablementChanged();
        buttonStateChanged();
    }
    
    void ImageButton::colourChanged()
    {
        repaint();
    }
    
    void ImageButton::paintButton(juce::Graphics& g, const bool mouse_is_over, const bool mouse_is_down)
    {
        juce::LookAndFeel& lf = getLookAndFeel();
        
        float btn_opacity = isOver() ? 1.f : 0.9f;
        
        juce::Drawable* image = nullptr;
        
        if(isEnabled())
        {
            image = getCurrentImage();
        }
        else
        {
            image = getToggleState() ? m_disabled_image_on.get() : m_disabled_image.get();
            
            if(image == nullptr)
            {
                btn_opacity = 0.4f;
                image = getNormalImage();
            }
        }
        
        if(m_style == ButtonStyle::ImageRaw)
        {
            image->drawAt(g, 0.f, 0.f, btn_opacity);
        }
        else
        {
            const auto bounds = getImageBounds();
            image->drawWithin(g, isDown() ? bounds.reduced(1.) : getImageBounds(),
                              m_style == ButtonStyle::ImageStretched ? juce::RectanglePlacement::stretchToFit : juce::RectanglePlacement::centred, btn_opacity);
        }
        
        if(m_style == ButtonStyle::ImageOnButtonBackground)
            lf.drawButtonBackground(g, *this,
                                    findColour(getToggleState() ? juce::TextButton::buttonOnColourId
                                               : juce::TextButton::buttonColourId),
                                    mouse_is_over, mouse_is_down);
        else
        {
            const bool toggleState = getToggleState();
            
            g.fillAll(findColour(toggleState ? ImageButton::ColourIds::backgroundOnColourId
                                 : ImageButton::ColourIds::backgroundColourId));
            
            const int textH = (getStyle() == ButtonStyle::ImageAboveTextLabel)
            ? juce::jmin(16, proportionOfHeight(0.25f))
            : 0;
            
            if(textH > 0)
            {
                g.setFont((float) textH);
                
                g.setColour(findColour(toggleState ? ImageButton::ColourIds::textColourOnId
                                                : ImageButton::ColourIds::textColourId)
                             .withMultipliedAlpha(isEnabled() ? 1.0f : 0.4f));
                
                g.drawFittedText(getButtonText(),
                                 2, getHeight() - textH - 1,
                                 getWidth() - 4, textH,
                                 juce::Justification::centred, 1);
            }
        }
    }
    
    //==============================================================================
    juce::Drawable* ImageButton::getCurrentImage() const noexcept
    {
        return isDown() ? getDownImage() : isOver() ? getOverImage() : getNormalImage();
    }
    
    juce::Drawable* ImageButton::getNormalImage() const noexcept
    {
        return(getToggleState() && m_normal_image_on != nullptr) ? m_normal_image_on.get()
        : m_normal_image.get();
    }
    
    juce::Drawable* ImageButton::getOverImage() const noexcept
    {
        if(getToggleState())
        {
            if(m_over_image_on   != nullptr)   return m_over_image_on.get();
            if(m_normal_image_on != nullptr)   return m_normal_image_on.get();
        }
        
        return m_over_image != nullptr ? m_over_image.get() : m_normal_image.get();
    }
    
    juce::Drawable* ImageButton::getDownImage() const noexcept
    {
        if(juce::Drawable* const d = getToggleState() ? m_down_image_on.get() : m_down_image.get())
            return d;
        
        return getOverImage();
    }
}
