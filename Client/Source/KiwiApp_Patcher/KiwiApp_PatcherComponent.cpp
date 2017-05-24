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

#include "KiwiApp_PatcherComponent.h"
#include "KiwiApp_PatcherView.h"

#include "../KiwiApp_Ressources/KiwiApp_BinaryData.h"
#include "../KiwiApp.h"
#include "../KiwiApp_General/KiwiApp_CommandIDs.h"
#include "../KiwiApp_Components/KiwiApp_CustomToolbarButton.h"

namespace kiwi
{
    // ================================================================================ //
    //                            PATCHER COMPONENT TOOLBAR                             //
    // ================================================================================ //
    
    PatcherToolbar::PatcherToolbar(PatcherManager& patcher_manager) :
    m_patcher_manager(patcher_manager),
    m_factory(m_patcher_manager)
    {
        m_toolbar.setVertical(false);
        m_toolbar.setStyle(juce::Toolbar::ToolbarItemStyle::iconsOnly);
        auto& lf = KiwiApp::useLookAndFeel();
        m_toolbar.setColour(juce::Toolbar::ColourIds::backgroundColourId, lf.getCurrentColourScheme()
                            .getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
        m_toolbar.setColour(juce::Toolbar::ColourIds::labelTextColourId, juce::Colours::black);
        m_toolbar.setColour(juce::Toolbar::ColourIds::buttonMouseOverBackgroundColourId, juce::Colours::whitesmoke.contrasting(0.1));
        m_toolbar.setColour(juce::Toolbar::ColourIds::buttonMouseDownBackgroundColourId, juce::Colours::whitesmoke.contrasting(0.2));
        
        m_toolbar.setWantsKeyboardFocus(false);
        setWantsKeyboardFocus(false);
        
        addAndMakeVisible(m_toolbar);
        m_toolbar.addDefaultItems(m_factory);
    }
    
    void PatcherToolbar::resized()
    {
        m_toolbar.setBounds(getLocalBounds());
    }
    
    void PatcherToolbar::paint(juce::Graphics& g)
    {
        g.fillAll(juce::Colour(0xff444444));
    }
    
    PatcherToolbar::Factory::Factory(PatcherManager& patcher_manager) : m_patcher_manager(patcher_manager)
    {
        
    }
    
    void PatcherToolbar::Factory::getAllToolbarItemIds(juce::Array<int>& ids)
    {
        ids.add(ItemIds::lock_unlock);
        ids.add(ItemIds::zoom_in);
        ids.add(ItemIds::zoom_out);
        ids.add(separatorBarId);
        ids.add(spacerId);
        ids.add(flexibleSpacerId);
        ids.add(ItemIds::dsp_on_off);
        ids.add(ItemIds::users);
    }
    
    void PatcherToolbar::Factory::getDefaultItemSet(juce::Array<int>& ids)
    {
        ids.add(ItemIds::users);
        ids.add(separatorBarId);
        ids.add(ItemIds::dsp_on_off);
        ids.add(separatorBarId);
        ids.add(ItemIds::lock_unlock);
        ids.add(spacerId);
        ids.add(flexibleSpacerId);
        ids.add(ItemIds::zoom_in);
        ids.add(ItemIds::zoom_out);
    }
    
#define IMG_DATA(NAME) binary_data::images::NAME
#define IMG_SIZE(NAME) binary_data::images::NAME ## _size
#define IMG(name) juce::Drawable::createFromImageData(IMG_DATA(name), IMG_SIZE(name))
    
    juce::ToolbarItemComponent* PatcherToolbar::Factory::createItem(int itemId)
    {
        juce::ToolbarItemComponent* btn = nullptr;
        
        if(itemId == ItemIds::lock_unlock)
        {
            btn = new CustomToolbarButton(itemId, "lock/unlock", juce::Colours::whitesmoke,
                                          IMG(locked_png), IMG(unlocked_png));
            btn->setCommandToTrigger(&KiwiApp::getCommandManager(), CommandIDs::editModeSwitch, true);
        }
        else if(itemId == ItemIds::zoom_in)
        {
            btn = new CustomToolbarButton(itemId, "zoom in", juce::Colours::whitesmoke,
                                          IMG(zoom_in_png), nullptr);
            btn->setCommandToTrigger(&KiwiApp::getCommandManager(), CommandIDs::zoomIn, true);
        }
        else if(itemId == ItemIds::zoom_out)
        {
            btn = new CustomToolbarButton(itemId, "zoom out", juce::Colours::whitesmoke,
                                          IMG(zoom_out_png), nullptr);
            btn->setCommandToTrigger(&KiwiApp::getCommandManager(), CommandIDs::zoomOut, true);
        }
        else if(itemId == ItemIds::dsp_on_off)
        {
            btn = new CustomToolbarButton(itemId, "DSP on/off", juce::Colours::whitesmoke,
                                          IMG(dsp_off_png), IMG(dsp_on_png));
            btn->setCommandToTrigger(&KiwiApp::getCommandManager(), CommandIDs::switchDsp, true);
        }
        else if(itemId == ItemIds::users)
        {
            btn = new UsersItemComponent(itemId, m_patcher_manager);
        }
        
        return btn;
    }
    
#undef IMG_DATA
#undef IMG_SIZE
#undef IMG
    
    // ================================================================================ //
    //                             TOOLBAR USER COMPONENT                               //
    // ================================================================================ //
    
    PatcherToolbar::UsersItemComponent::UsersItemComponent(const int toolbarItemId, PatcherManager& patcher_manager)
    : ToolbarItemComponent (toolbarItemId, "Custom Toolbar Item", false),
    m_patcher_manager(patcher_manager),
    m_users(m_patcher_manager.getNumberOfUsers()),
    m_users_img(juce::ImageCache::getFromMemory(binary_data::images::users_png,
                                                binary_data::images::users_png_size))
    {
        m_patcher_manager.addListener(*this);
    }
    
    PatcherToolbar::UsersItemComponent::~UsersItemComponent()
    {
        m_patcher_manager.removeListener(*this);
    }
    
    void PatcherToolbar::UsersItemComponent::connectedUserChanged(PatcherManager& manager)
    {
        if(&manager == &m_patcher_manager)
        {
            m_users = m_patcher_manager.getNumberOfUsers();
            repaint();
        }
    }
    
    void PatcherToolbar::UsersItemComponent::paintButtonArea(juce::Graphics& g, int width, int height,
                                                             bool isMouseOver, bool isMouseDown)
    {
        int padding = 2;
        int count_size = 14;
        juce::Rectangle<int> image_bounds(height - padding * 2, padding, height - padding * 2, height - padding * 2);

        g.drawImage(m_users_img, image_bounds.toFloat(), juce::RectanglePlacement::centred);
        
        int center_y = height / 2;
        juce::Rectangle<int> label_bounds(center_y - count_size / 2,
                                          center_y - count_size / 2,
                                          count_size, count_size);
        
        g.setColour(juce::Colours::grey);
        g.drawEllipse(label_bounds.expanded(2).toFloat(), 1.5f);
        
        g.drawText(std::to_string(m_users), label_bounds, juce::Justification::centred);
    }
    
    bool PatcherToolbar::UsersItemComponent::getToolbarItemSizes(int toolbarDepth, bool isVertical,
                             int& preferredSize, int& minSize, int& maxSize)
    {
        if (isVertical)
            return false;
        
        maxSize = minSize = preferredSize = 50;
        return true;
    }
    
    void PatcherToolbar::UsersItemComponent::contentAreaChanged(const juce::Rectangle<int>& newArea)
    {
        repaint();
    }
    
    void PatcherToolbar::UsersItemComponent::mouseEnter(juce::MouseEvent const& e)
    {
        ;
    }
    
    void PatcherToolbar::UsersItemComponent::mouseExit(juce::MouseEvent const& e)
    {
        ;
    }
    
    // ================================================================================ //
    //                                PATCHER COMPONENT                                 //
    // ================================================================================ //
    
    PatcherComponent::PatcherComponent(PatcherView& patcherview) :
    m_patcher_manager(patcherview.usePatcherManager()),
    m_patcherview(patcherview),
    m_toolbar(m_patcher_manager)
    {
        setSize(600, 400);
        
        addAndMakeVisible(&patcherview.getViewport(), true);
        patcherview.updateWindowTitle();
        
        KiwiApp::bindToCommandManager(this);
        KiwiApp::bindToKeyMapping(this);

        addAndMakeVisible(m_toolbar);
    }
    
    PatcherComponent::~PatcherComponent()
    {
        ;
    }
    
    PatcherView& PatcherComponent::usePatcherView()
    {
        return m_patcherview;
    }
    
    //! @brief Returns the patcher manager.
    PatcherManager& PatcherComponent::usePatcherManager()
    {
        return m_patcher_manager;
    }
    
    void PatcherComponent::resized()
    {
        const int toolbar_size = 36;
        m_toolbar.setBounds(getLocalBounds().withBottom(toolbar_size));
        m_patcherview.getViewport().setBounds(getLocalBounds().withTop(toolbar_size));
    }
    
    void PatcherComponent::paint(juce::Graphics& g)
    {
        ;
    }
    
    // ================================================================================ //
    //                              APPLICATION COMMAND TARGET                          //
    // ================================================================================ //
    
    juce::ApplicationCommandTarget* PatcherComponent::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void PatcherComponent::getAllCommands(juce::Array<juce::CommandID>& commands)
    {
        m_patcherview.getAllCommands(commands);
    }
    
    void PatcherComponent::getCommandInfo(const juce::CommandID commandID, juce::ApplicationCommandInfo& result)
    {
        m_patcherview.getCommandInfo(commandID, result);
    }
    
    bool PatcherComponent::perform(InvocationInfo const& info)
    {
        bool performed = m_patcherview.perform(info);
        return performed;
    }
    
    // ================================================================================ //
    //                                PATCHER VIEW WINDOW                               //
    // ================================================================================ //
    
    PatcherViewWindow::PatcherViewWindow(PatcherManager& manager, PatcherView& patcherview) :
    Window("Untitled", nullptr, true, true, juce::String::empty, !KiwiApp::isMacOSX()),
    m_patcher_component(patcherview)
    {
        // Todo: Add size infos to the Patcher Model
        setSize(600, 500);
        centreWithSize(getWidth(), getHeight());
        
        setContentNonOwned(&m_patcher_component, true);
        setVisible(true);
    }
    
    PatcherManager& PatcherViewWindow::getPatcherManager()
    {
        return m_patcher_component.usePatcherManager();
    }
    
    PatcherView& PatcherViewWindow::getPatcherView()
    {
        return m_patcher_component.usePatcherView();
    }
    
    void PatcherViewWindow::closeButtonPressed()
    {
        KiwiApp::use().useInstance().removePatcherWindow(*this);
    }
}
