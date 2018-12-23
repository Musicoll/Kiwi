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

#pragma once

#include "../KiwiApp_Components/KiwiApp_Window.h"
#include "KiwiApp_PatcherManager.h"

namespace kiwi
{
    // ================================================================================ //
    //                            PATCHER COMPONENT TOOLBAR                             //
    // ================================================================================ //
    
    class PatcherToolbar
    : public juce::Component
    {
    public: // methods
        
        //! @brief Constructor.
        PatcherToolbar(PatcherManager& patcher_manager);
        
        //! @brief Destructor.
        ~PatcherToolbar();
        
        //! @brief juce::Component::resized
        void resized() override;
        
        //! @brief Removes users icon.
        void removeUsersIcon();
        
        //! @brief Show or hide stack-overflow icon.
        void setStackOverflowIconVisible(bool visible);
        
    private: // classes
        
        //! called by custom toolbar lookandfeel.
        void paintToolbarBackground(juce::Graphics& g, int width, int height);
        
        //! @internal Toolbar item factory
        struct Factory : public juce::ToolbarItemFactory
        {
            Factory(PatcherManager& patcher_manager);
            
            enum ItemIds
            {
                lock_unlock         = 1,
                zoom_in             = 2,
                zoom_out            = 3,
                dsp_on_off          = 4,
                users               = 5,
                stack_overflow      = 6,
            };
            
            void getAllToolbarItemIds(juce::Array<int>& ids) override;
            
            void getDefaultItemSet(juce::Array<int>& ids) override;
            
            juce::ToolbarItemComponent* createItem(int itemId) override;
            
            PatcherManager& m_patcher_manager;
        };
        
        class UsersItemComponent;
        
        //! @brief Returns the index of a specific item id in the toolbar.
        //! @details returns -1 if the item is not found.
        int getToolbarItemIndex(Factory::ItemIds item_id);
        
    private: // variables
        
        PatcherManager& m_patcher_manager;
        
        juce::Toolbar   m_toolbar;
        Factory         m_factory;
        std::unique_ptr<juce::LookAndFeel> m_toolbar_look_and_feel = nullptr;
        
        class PatcherToolbarLookAndFeel;
    };
    
    // ================================================================================ //
    //                             TOOLBAR USER COMPONENT                               //
    // ================================================================================ //
    
    //! @brief A toolbar component that displays informations about the users of the patch
    class PatcherToolbar::UsersItemComponent
    : public juce::ToolbarItemComponent
    , public PatcherManager::Listener
    , private juce::Timer
    {
    public: // methods
        
        //! @brief Constructor.
        UsersItemComponent(const int toolbarItemId, PatcherManager& patcher_manager);
        
        //! @brief Destructor.
        ~UsersItemComponent();
        
        //! @brief Called when one or more users are connecting or disconnecting to the Patcher Document.
        void connectedUserChanged(PatcherManager& manager) override;
        
        //! @brief Call to update the number of users displayed and user names.
        void updateUsers();
        
        //! @brief Provides prefered size for this item.
        bool getToolbarItemSizes(int toolbarDepth, bool isVertical,
                                 int& preferredSize, int& minSize, int& maxSize) override;
        
        //! @brief Paint the button area.
        void paintButtonArea(juce::Graphics&, int width, int height,
                             bool isMouseOver, bool isMouseDown) override;
        
        //! @brief Called when content area changed.
        void contentAreaChanged(const juce::Rectangle<int>& newArea) override;
        
        //! @brief Starts this component flashing.
        void startFlashing();
        
        //! @brief Stops this component flashing.
        void stopFlashing();
        
        //! @brief Displays the list of usernames.
        void mouseDown(juce::MouseEvent const& e) override final;
        
    private: // methods
        
        //! @internal juce::Timer callback.
        void timerCallback() override;
        
    private: // variables
        
        PatcherManager&             m_patcher_manager;
        std::vector<std::string>    m_users;
        size_t                      m_user_nb;
        const juce::Image           m_users_img;
        float                       m_flash_alpha = 0.f;
    };
    
    // ================================================================================ //
    //                                PATCHER COMPONENT                                 //
    // ================================================================================ //
    
    //! @brief The PatcherComponent holds a patcher view and a patcher toolbar.
    class PatcherComponent
    : public juce::Component
    , private PatcherManager::Listener
    , public juce::ApplicationCommandTarget
    {
    public: // methods
        
        //! @brief Constructor
        PatcherComponent(PatcherView& patcherview);
        
        //! @brief Destructor
        ~PatcherComponent();
        
        //! @brief juce::Component::resized
        void resized() override;
        
        //! @brief Returns the patcher view.
        PatcherView& usePatcherView();
        
        //! @brief Returns the patcher manager.
        PatcherManager& usePatcherManager();
        
        //! @brief juce::Component::paint
        void paint(juce::Graphics& g) override;
        
        //! @brief Removes the users icon from toolbar;
        void removeUsersIcon();
        
        // -------------------------------------------------------------------------------- //
        //                              APPLICATION COMMAND TARGET                          //
        // -------------------------------------------------------------------------------- //
        
        juce::ApplicationCommandTarget* getNextCommandTarget() override;
        void getAllCommands(juce::Array<juce::CommandID>& commands) override;
        void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
        bool perform(const InvocationInfo& info) override;
        
    private: // methods
        
        //! @brief Called when a stack-overflow is detected;
        void stackOverflowDetected(PatcherManager& manager, std::vector<flip::Ref> culprits) override;
        
        //! @brief Called when a stack-overflow is cleared.
        void stackOverflowCleared(PatcherManager& manager) override;

    private: // members
        
        PatcherManager& m_patcher_manager;
        PatcherView&    m_patcherview;
        PatcherToolbar  m_toolbar;
    };
    
    // ================================================================================ //
    //                                PATCHER VIEW WINDOW                               //
    // ================================================================================ //
    
    class PatcherViewWindow
    : public Window
    , public juce::ComponentMovementWatcher
    {
    public: // methods
        
        PatcherViewWindow(PatcherManager& manager, PatcherView& patcherview);
        
        //! @brief Shows an modal window asking for user input.
        bool showOkCancelBox(juce::AlertWindow::AlertIconType icon_type,
                             std::string const& title,
                             std::string const& message,
                             std::string const& button_1,
                             std::string const& button_2);
        
        void closeButtonPressed() override;
        
        //! @brief returns the patcher manager.
        PatcherManager& getPatcherManager();
        
        //! @brief returns the PatcherView.
        PatcherView& getPatcherView();
        
        //! @brief Removes the connected users icon. Called once patcher disconnected.
        void removeUsersIcon();
        
    private: // methods
        
        void componentMovedOrResized(bool was_moved, bool was_resized) override;
        void componentPeerChanged() override {};
        void componentVisibilityChanged() override {}
        
    private: // variables
        
        PatcherComponent m_patcher_component;
    };
}
