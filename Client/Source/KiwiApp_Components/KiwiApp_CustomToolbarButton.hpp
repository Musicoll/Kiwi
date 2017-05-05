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

#ifndef KIWI_APP_CUSTOM_TOOLBAR_BUTTON_HPP_INCLUDED
#define KIWI_APP_CUSTOM_TOOLBAR_BUTTON_HPP_INCLUDED

#include <juce_gui_extra/juce_gui_extra.h>

namespace kiwi
{
    //! @brief A type of button designed to go on a toolbar.
    //! @details This class is a modified version of the juce::ToolbarButton class.
    //! @see juce::ToolbarButton, ToolbarItemFactory
    class CustomToolbarButton : public juce::ToolbarItemComponent
    {
    public: // methods
        
        //! @brief Constructor.
        //! @param itemId the ID for this toolbar item type. This is passed through to the
        //! ToolbarItemComponent constructor
        //! @param labelText    the text to display on the button(if the toolbar is using a style
        //! that shows text labels). This is passed through to the
        //! ToolbarItemComponent constructor
        //! @param normalImage  a drawable object that the button should use as its icon. The object
        //! that is passed-in here will be kept by this object and will be
        //! deleted when no longer needed or when this button is deleted.
        //! @param toggledOnImage  a drawable object that the button can use as its icon if the button
        //! is in a toggled-on state(see the Button::getToggleState() method). If
        //! nullptr is passed-in here, then the normal image will be used instead,
        //! regardless of the toggle state. The object that is passed-in here will be
        //! owned by this object and will be deleted when no longer needed or when
        //! this button is deleted.
        CustomToolbarButton(int itemId,
                            const juce::String& labelText,
                            const juce::Colour bgcolor,
                            juce::Drawable* normalImage,
                            juce::Drawable* toggledOnImage);
        
        //! @brief Destructor.
        ~CustomToolbarButton();
        
        //! @internal
        bool getToolbarItemSizes(int toolbarDepth, bool isToolbarVertical, int& preferredSize,
                                 int& minSize, int& maxSize) override;
        //! @internal
        void paintButtonArea(juce::Graphics&, int width, int height, bool isMouseOver, bool isMouseDown) override;
        //! @internal
        void contentAreaChanged(const juce::Rectangle<int>&) override;
        //! @internal
        void buttonStateChanged() override;
        //! @internal
        void resized() override;
        //! @internal
        void enablementChanged() override;
        
    private: // variables
        
        juce::ScopedPointer<juce::Drawable> normalImage, toggledOnImage;
        juce::Drawable* currentImage;
        const juce::Colour m_bgcolor;
        
        void updateDrawable();
        juce::Drawable* getImageToUse() const;
        void setCurrentImage(juce::Drawable*);
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomToolbarButton)
    };
}

#endif // KIWI_APP_CUSTOM_TOOLBAR_BUTTON_HPP_INCLUDED
