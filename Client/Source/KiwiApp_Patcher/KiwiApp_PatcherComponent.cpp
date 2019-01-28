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

#include "KiwiApp_PatcherComponent.h"
#include "KiwiApp_PatcherView.h"

#include "../KiwiApp_Resources/KiwiApp_BinaryData.h"
#include "../KiwiApp.h"
#include "../KiwiApp_General/KiwiApp_CommandIDs.h"
#include "../KiwiApp_Components/KiwiApp_CustomToolbarButton.h"

namespace kiwi
{
    class PatcherToolbar::PatcherToolbarLookAndFeel
    : public LookAndFeel
    {
    public:
        
        PatcherToolbarLookAndFeel(PatcherToolbar& toolbar)
        : m_toolbar(toolbar)
        {}
        
        void paintToolbarBackground(juce::Graphics& g,
                                    int width, int height,
                                    juce::Toolbar&) override
        {
            m_toolbar.paintToolbarBackground(g, width, height);
        }
        
    private:
        PatcherToolbar& m_toolbar;
    };
    
    // ================================================================================ //
    //                            PATCHER COMPONENT TOOLBAR                             //
    // ================================================================================ //
    
    PatcherToolbar::PatcherToolbar(PatcherManager& patcher_manager)
    : m_patcher_manager(patcher_manager)
    , m_factory(m_patcher_manager)
    , m_toolbar_look_and_feel(std::make_unique<PatcherToolbarLookAndFeel>(*this))
    {
        m_toolbar.setLookAndFeel(m_toolbar_look_and_feel.get());
        
        m_toolbar.setVertical(false);
        m_toolbar.setStyle(juce::Toolbar::ToolbarItemStyle::iconsOnly);
        m_toolbar.setColour(juce::Toolbar::ColourIds::labelTextColourId, juce::Colours::black);
        m_toolbar.setColour(juce::Toolbar::ColourIds::buttonMouseOverBackgroundColourId, juce::Colours::whitesmoke.contrasting(0.1));
        m_toolbar.setColour(juce::Toolbar::ColourIds::buttonMouseDownBackgroundColourId, juce::Colours::whitesmoke.contrasting(0.2));
        
        m_toolbar.setWantsKeyboardFocus(false);
        setWantsKeyboardFocus(false);
        
        addAndMakeVisible(m_toolbar);
        m_toolbar.addDefaultItems(m_factory);
    }
    
    PatcherToolbar::~PatcherToolbar()
    {
        m_toolbar.setLookAndFeel(nullptr);
    }
    
    void PatcherToolbar::resized()
    {
        m_toolbar.setBounds(getLocalBounds());
    }
    
    void PatcherToolbar::paintToolbarBackground(juce::Graphics& g, int width, int height)
    {
        g.fillAll(findColour(juce::Toolbar::ColourIds::backgroundColourId));
    }
    
    int PatcherToolbar::getToolbarItemIndex(Factory::ItemIds item_id)
    {
        for(int index = 0; index < m_toolbar.getNumItems(); index++)
        {
            if(m_toolbar.getItemId(index) == item_id)
            {
                return index;
            }
        }
        
        return -1;
    }
    
    void PatcherToolbar::removeUsersIcon()
    {
        const int users_item_index = getToolbarItemIndex(Factory::ItemIds::users);
        if(users_item_index >= 0)
        {
            m_toolbar.removeToolbarItem(users_item_index);
            m_toolbar.repaint();
        }
    }
    
    void PatcherToolbar::setStackOverflowIconVisible(bool should_be_visible)
    {
        const int stack_overflow_item_index = getToolbarItemIndex(Factory::ItemIds::stack_overflow);
        const bool is_visible = (stack_overflow_item_index >= 0);
        
        if(should_be_visible && !is_visible)
        {
            const int index = getToolbarItemIndex(Factory::ItemIds::lock_unlock) + 1;
            m_toolbar.addItem(m_factory, Factory::ItemIds::stack_overflow, index);
        }
        else if(!should_be_visible && is_visible)
        {
            m_toolbar.removeToolbarItem(stack_overflow_item_index);
        }
    }
    
    PatcherToolbar::Factory::Factory(PatcherManager& patcher_manager)
    : m_patcher_manager(patcher_manager)
    {}
    
    void PatcherToolbar::Factory::getAllToolbarItemIds(juce::Array<int>& ids)
    {
        ids.add(ItemIds::lock_unlock);
        ids.add(ItemIds::zoom_in);
        ids.add(ItemIds::zoom_out);
        ids.add(separatorBarId);
        ids.add(spacerId);
        ids.add(flexibleSpacerId);
        ids.add(ItemIds::dsp_on_off);
        ids.add(ItemIds::stack_overflow);
        
        if(m_patcher_manager.isConnected())
        {
            ids.add(ItemIds::users);
        }
    }
    
    void PatcherToolbar::Factory::getDefaultItemSet(juce::Array<int>& ids)
    {
        if(m_patcher_manager.isConnected())
        {
            ids.add(ItemIds::users);
            ids.add(separatorBarId);
        }
        
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
        else if(itemId == ItemIds::stack_overflow)
        {
            btn = new CustomToolbarButton(itemId, "clear stack-overflow message lock",
                                          juce::Colour(0xFFE52E2E),
                                          IMG(infinite_loop_png), IMG(infinite_loop_png));
            btn->onClick = [this](){
                m_patcher_manager.clearStackOverflow();
            };
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
    
    PatcherToolbar::UsersItemComponent::UsersItemComponent(const int toolbarItemId,
                                                           PatcherManager& patcher_manager)
    : ToolbarItemComponent (toolbarItemId, "Custom Toolbar Item", false)
    , m_patcher_manager(patcher_manager)
    , m_users()
    , m_user_nb(m_patcher_manager.getNumberOfUsers())
    , m_users_img(juce::ImageCache::getFromMemory(binary_data::images::users_png,
                                                  binary_data::images::users_png_size))
    , m_flash_alpha(0.f)
    {
        updateUsers();
        m_patcher_manager.addListener(*this);
    }
    
    PatcherToolbar::UsersItemComponent::~UsersItemComponent()
    {
        m_patcher_manager.removeListener(*this);
    }
    
    void PatcherToolbar::UsersItemComponent::updateUsers()
    {
        auto users_ids = m_patcher_manager.getConnectedUsers();
        m_user_nb = users_ids.size();
        
        startFlashing();
        
        Component::SafePointer<UsersItemComponent> form(this);
        
        auto success = [form](Api::Users users)
        {
            KiwiApp::useScheduler().schedule([form, users]()
            {
                if (form)
                {
                    form.getComponent()->m_users.clear();
                    
                    for(Api::User const& user : users)
                    {
                        form.getComponent()->m_users.push_back(user.getName());
                    }
                }
            });
        };
        
        auto fail = [form](Api::Error error)
        {
            KiwiApp::useScheduler().schedule([form, error]()
            {
                if (form)
                {
                    form.getComponent()->m_users.clear();
                }
            });
        };
        
        KiwiApp::useApi().getUsers(users_ids, success, fail);
    }
    
    void PatcherToolbar::UsersItemComponent::connectedUserChanged(PatcherManager& manager)
    {
        if(&manager == &m_patcher_manager)
        {
            updateUsers();
            
            startFlashing();
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
        
        const auto online_color = juce::Colour::fromRGB(72, 165, 93);
        const auto badge_color = (juce::Colours::grey.withAlpha(0.5f)
                                  .overlaidWith(online_color.withAlpha(m_flash_alpha)));
        
        g.setColour(badge_color);
        g.drawEllipse(label_bounds.expanded(2).toFloat(), 0.5f);
        g.fillEllipse(label_bounds.expanded(2).toFloat());
        
        g.setColour(juce::Colours::whitesmoke);
        g.drawText(std::to_string(m_user_nb), label_bounds, juce::Justification::centred);
        
        auto flag_rect = juce::Rectangle<float>(0, 0, 3, height).reduced(0, 2);
        
        g.setColour(online_color);
        g.fillRect(flag_rect);
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
    
    void PatcherToolbar::UsersItemComponent::startFlashing()
    {
        m_flash_alpha = 1.0f;
        startTimerHz (20);
    }
    
    void PatcherToolbar::UsersItemComponent::stopFlashing()
    {
        m_flash_alpha = 0.0f;
        stopTimer();
        repaint();
    }
    
    void PatcherToolbar::UsersItemComponent::mouseDown(juce::MouseEvent const& e)
    {
        if (m_users.size() > 0)
        {
            juce::PopupMenu m;
            
            for(std::string const& username : m_users)
            {
                m.addItem(1, username, false, false);
            }
            
            m.showAt(this);
        }
    }
    
    void PatcherToolbar::UsersItemComponent::timerCallback()
    {
        // Reduce the alpha level of the flash slightly so it fades out
        m_flash_alpha -= 0.075f;
        
        if(m_flash_alpha < 0.05f)
        {
            stopFlashing();
        }
        
        repaint();
    }
    
    // ================================================================================ //
    //                                PATCHER COMPONENT                                 //
    // ================================================================================ //
    
    PatcherComponent::PatcherComponent(PatcherView& patcherview)
    : m_patcher_manager(patcherview.usePatcherManager())
    , m_patcherview(patcherview)
    , m_toolbar(m_patcher_manager)
    {
        setSize(m_patcherview.getWidth(), m_patcherview.getHeight());
        
        addAndMakeVisible(&patcherview.useViewport(), true);
        
        KiwiApp::bindToCommandManager(this);
        KiwiApp::bindToKeyMapping(this);

        addAndMakeVisible(m_toolbar);
        
        m_patcher_manager.addListener(*this);
        
        m_toolbar.setStackOverflowIconVisible(m_patcher_manager.hasStackOverflow());
    }
    
    PatcherComponent::~PatcherComponent()
    {
        m_patcher_manager.removeListener(*this);
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
        auto bounds = getLocalBounds();
        const int toolbar_size = 36;
        m_toolbar.setBounds(bounds.removeFromTop(toolbar_size));
        m_patcherview.useViewport().setBounds(bounds);
    }
    
    void PatcherComponent::paint(juce::Graphics& g)
    {
        ;
    }
    
    void PatcherComponent::removeUsersIcon()
    {
        m_toolbar.removeUsersIcon();
    }
    
    void PatcherComponent::stackOverflowDetected(PatcherManager& manager, std::vector<flip::Ref> culprits)
    {
        m_toolbar.setStackOverflowIconVisible(true);
    }
    
    void PatcherComponent::stackOverflowCleared(PatcherManager& manager)
    {
        m_toolbar.setStackOverflowIconVisible(false);
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
    
    PatcherViewWindow::PatcherViewWindow(PatcherManager& manager,
                                         PatcherView& patcherview)
    : Window("Untitled", nullptr, true, true, "", !KiwiApp::isMacOSX())
    , juce::ComponentMovementWatcher(this)
    , m_patcher_component(patcherview)
    {
        setContentNonOwned(&m_patcher_component, true);
        setVisible(true);
        
        getLookAndFeel().setUsingNativeAlertWindows(false);
        
        const auto user_area = juce::Desktop::getInstance()
        .getDisplays().getMainDisplay().userArea;
        
        setResizeLimits(50, 50, user_area.getWidth(), user_area.getHeight());
        
        patcherview.windowInitialized();
    }
    
    void PatcherViewWindow::removeUsersIcon()
    {
        m_patcher_component.removeUsersIcon();
    }
    
    bool PatcherViewWindow::showOkCancelBox(juce::AlertWindow::AlertIconType icon_type,
                                            std::string const& title,
                                            std::string const& message,
                                            std::string const& button_1,
                                            std::string const& button_2)
    {
        return juce::AlertWindow::showOkCancelBox(icon_type, title, message, button_1, button_2, this);
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
    
    void PatcherViewWindow::componentMovedOrResized(bool was_moved, bool was_resized)
    {
        m_patcher_component.usePatcherView().saveState();
    }
}
