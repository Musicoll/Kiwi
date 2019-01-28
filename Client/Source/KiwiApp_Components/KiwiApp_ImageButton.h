/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
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

#include <juce_gui_extra/juce_gui_extra.h>

namespace kiwi
{
    // ================================================================================ //
    //                                   IMAGE BUTTON                                   //
    // ================================================================================ //
    
    //! @brief A button that displays a Drawable.
    class ImageButton : public juce::Button
    {
    public: // methods
        
        using ButtonStyle = juce::DrawableButton::ButtonStyle;
        using ColourIds = juce::DrawableButton::ColourIds;

        //! @brief Constructor.
        ImageButton(juce::String const& button_name,
                    std::unique_ptr<juce::Drawable> drawable,
                    ButtonStyle style = ButtonStyle::ImageAboveTextLabel);
        
        //! @brief Destructor.
        ~ImageButton();
        
        //! @brief Sets up the images to draw for the various button states.
        //! @see juce::DrawableButton::setImages()
        void setImages(juce::Drawable const* normal_image,
                       juce::Drawable const* over_image         = nullptr,
                       juce::Drawable const* down_image         = nullptr,
                       juce::Drawable const* disabled_image     = nullptr,
                       juce::Drawable const* normal_image_on    = nullptr,
                       juce::Drawable const* over_image_on      = nullptr,
                       juce::Drawable const* down_image_on      = nullptr,
                       juce::Drawable const* disabled_image_on  = nullptr);
        
        //! @brief Set the command to execute when the button has been clicked.
        void setCommand(std::function<void(void)> fn);
        
        //! @brief Changes the button's style.
        //! @see ButtonStyle
        void setButtonStyle(ButtonStyle new_style);
        
        //! @brief Returns the current style.
        ButtonStyle getStyle() const noexcept;
        
        //! @brief Gives the button an optional amount of space around the edge of the drawable.
        //! @details By default there's a gap of about 3 pixels.
        void setEdgeIndent(int num_pixels_indent);
        
        //! @brief Returns the image that the button is currently displaying.
        juce::Drawable* getCurrentImage() const noexcept;
        
        //! @brief Returns the image that the button will use for its normal state.
        juce::Drawable* getNormalImage() const noexcept;
        
        //! @brief Returns the image that the button will use when the mouse is over it.
        juce::Drawable* getOverImage() const noexcept;
        
        //! @brief Returns the image that the button will use when the mouse is held down on it.
        juce::Drawable* getDownImage() const noexcept;
        
        //! @brief Can be overridden to specify a custom position for the image within the button.
        virtual juce::Rectangle<float> getImageBounds() const;
        
    protected: // methods
        
        //! @internal
        void paintButton(juce::Graphics&, bool isMouseOverButton, bool isButtonDown) override;
        
        //! @internal
        void buttonStateChanged() override;
        
        //! @internal
        void resized() override;
        
        //! @internal
        void enablementChanged() override;
        
        //! @internal
        void colourChanged() override;
        
        //! @brief This method is called when the button has been clicked.
        void clicked(juce::ModifierKeys const& modifiers) override;
        
    private: // members
        
        ButtonStyle m_style;
        std::unique_ptr<juce::Drawable> m_normal_image, m_over_image, m_down_image, m_disabled_image,
        m_normal_image_on, m_over_image_on, m_down_image_on, m_disabled_image_on;
        juce::Drawable* m_current_image;
        
        int m_edge_indent;
        
        std::function<void(void)> m_command = nullptr;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImageButton)
    };
}
