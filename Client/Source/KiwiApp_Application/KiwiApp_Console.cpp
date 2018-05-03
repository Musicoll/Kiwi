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

#include "KiwiApp_Console.h"

#include "../KiwiApp.h"
#include "../KiwiApp_General/KiwiApp_StoredSettings.h"
#include "../KiwiApp_Ressources/KiwiApp_BinaryData.h"
#include "../KiwiApp_General/KiwiApp_CommandIDs.h"

namespace kiwi
{
    // ================================================================================ //
    //                                  CONSOLE COMPONENT                               //
    // ================================================================================ //
    
    ConsoleContent::ConsoleContent(sConsoleHistory history) :
    m_history(history),
    m_font(16.f)
    {
        assert(history);

        history->addListener(*this);
        
        setSize(300, 500);
        
        juce::TableHeaderComponent* header = new juce::TableHeaderComponent();
        
        header->addColumn(juce::String("Message"),
                          Column::Message, 100, 40, 10000,
                          juce::TableHeaderComponent::defaultFlags, -1);
        
        header->setStretchToFitActive(false);
        header->setColumnVisible(1, true);
        header->addListener(this);
        
        m_table.setWantsKeyboardFocus(true);
        m_table.setMultipleSelectionEnabled(true);
        m_table.setMouseMoveSelectsRows(false);
        m_table.setHeaderHeight(m_font.getHeight() + 15);
        m_table.setRowHeight(m_font.getHeight() + 5);
        m_table.setColour(juce::ListBox::ColourIds::backgroundColourId, juce::Colours::transparentWhite);
        m_table.getViewport()->setScrollBarsShown(true, false, true, false);
        m_table.setModel(this);
        m_table.setHeader(header);
        addAndMakeVisible(m_table);
    }
    
    ConsoleContent::~ConsoleContent()
    {
        sConsoleHistory history = getHistory();
        if(history)
        {
            history->removeListener(*this);
        }
    }
    
    sConsoleHistory ConsoleContent::getHistory()
    {
        return m_history.lock();
    }
    
    // ================================================================================ //
    //                                      COMMAND                                     //
    // ================================================================================ //
    
    void ConsoleContent::copy()
    {
        sConsoleHistory history = getHistory();
        if(history)
        {
            juce::String text;
            juce::SparseSet<int> selection = m_table.getSelectedRows();
            
            for(size_t i = 0; i < selection.size(); i++)
            {
                auto msg = history->get(selection[i]).first;
                if(msg && !msg->text.empty())
                {
                    text += msg->text + "\n";
                }
            }
            
            juce::SystemClipboard::copyTextToClipboard(text);
        }
    }
    
    void ConsoleContent::erase()
    {
        sConsoleHistory history = getHistory();
        if(history)
        {
            juce::SparseSet<int> selection = m_table.getSelectedRows();
            std::vector<size_t> select;
            
            for(size_t i = 0; i < selection.size(); i++)
            {
                select.push_back(selection[i]);
            }
            
            history->erase(select);
            m_table.setVerticalPosition(0);
        }
    }
    
    // ================================================================================ //
    //                                  HISTORY LISTENER                                //
    // ================================================================================ //
    
    void ConsoleContent::consoleHistoryChanged(ConsoleHistory const&)
    {
        tool::Scheduler<> &scheduler = KiwiApp::useScheduler();
        
        scheduler.defer([this]()
        {
            m_table.updateContent();
            m_table.repaint();
        });
    }
    
    // ================================================================================ //
    //                                      COMPONENT                                   //
    // ================================================================================ //
    
    void ConsoleContent::resized()
    {
        m_table.setBounds(getLocalBounds());
        updateRighmostColumnWidth(&m_table.getHeader());
    }
    
    void ConsoleContent::paint(juce::Graphics& g)
    {
        g.fillAll(juce::Colours::lightgrey);
        
        int width = m_table.getVisibleContentWidth();
        int rowheight = m_table.getRowHeight();
        g.setColour(juce::Colours::black.withAlpha(0.15f));
        for(int i = m_table.getHeaderHeight() - 1; i < getHeight(); i+= rowheight)
        {
             g.drawHorizontalLine(i, 0, width);
        }
    }
    
    void ConsoleContent::scrollToTop()
    {
        m_table.scrollToEnsureRowIsOnscreen(0);
    }
    
    void ConsoleContent::scrollToBottom()
    {
        m_table.scrollToEnsureRowIsOnscreen(m_table.getNumRows());
    }
    
    void ConsoleContent::clearAll()
    {
        m_table.selectRangeOfRows(0, m_table.getNumRows());
        erase();
    }
    
    // ================================================================================ //
    //                              TABLE LIST BOX MODEL                                //
    // ================================================================================ //
    
    void ConsoleContent::selectedRowsChanged(int row)
    {
        KiwiApp::commandStatusChanged();
    }
    
    void ConsoleContent::deleteKeyPressed(int lastRowSelected)
    {
        erase();
    }
    
    void ConsoleContent::backgroundClicked(const juce::MouseEvent& mouse)
    {
        m_table.deselectAllRows();
    }
    
    int ConsoleContent::getNumRows()
    {
        sConsoleHistory history = getHistory();
        return history ? history->size() : 0;
    }
    
    int ConsoleContent::getNumSelectedRows() const
    {
        return m_table.getNumSelectedRows();
    }
    
    void ConsoleContent::paintRowBackground(juce::Graphics& g,
                                            int rowNumber, int width, int height,
                                            bool selected)
    {
        sConsoleHistory history = getHistory();
        if(!history) return; //abort
        
        const juce::Colour bgcolor(juce::Colours::lightgrey);
        
        auto msg = history->get(rowNumber).first;
        if(msg)
        {
            if(selected)
            {
                g.fillAll(juce::Colours::lightsteelblue);
            }
            else if(msg->type == engine::Console::Message::Type::Error)
            {
                g.fillAll(juce::Colours::lightpink);
            }
            else if(msg->type == engine::Console::Message::Type::Warning)
            {
                g.fillAll(juce::Colours::lightgoldenrodyellow);
            }
            else
            {
                g.fillAll(bgcolor);
            }
        }
        else
        {
            g.fillAll(bgcolor);
        }
    
        g.setColour(juce::Colours::black.withAlpha(0.15f));
        g.drawHorizontalLine(height - 1, 0, width);
    }
    
    void ConsoleContent::paintOverChildren(juce::Graphics &g)
    {
        int numColumns = m_table.getHeader().getNumColumns(true);
        float left = 0, width = 0;
        
        for(int i = 0; i < numColumns - 1; i++)
        {
            width = m_table.getHeader().getColumnWidth(m_table.getHeader().getColumnIdOfIndex(i, true));
            
            if(m_table.getVisibleContentWidth() >= width + left)
            {
                g.setColour(juce::Colours::black.withAlpha(0.15f));
                g.drawVerticalLine(left + width - 0.5, m_table.getHeaderHeight(), getHeight());
            }
            
            left += width;
        }
    }
    
    void ConsoleContent::paintCell(juce::Graphics& g,
                             int rowNumber, int columnId, int width, int height,
                             bool rowIsSelected)
    {
        sConsoleHistory history = getHistory();
        if(!history) return; //abort
        
        auto pair = history->get(rowNumber);
        auto msg = pair.first;
        if(msg)
        {
            g.setColour(juce::Colours::black.brighter(0.4));
            g.setFont(m_font);
            
            switch (columnId)
            {
                case Column::Message:
                {
                    auto repeat_times = pair.second;
                    if(repeat_times == 0)
                    {
                        g.drawText(msg->text, 2, 0, width - 4, height,
                                   juce::Justification::centredLeft, true);
                    }
                    else
                    {
                        g.drawText("[" + std::to_string(repeat_times) + "x] " + msg->text,
                                   2, 0, width - 4, height,
                                   juce::Justification::centredLeft, true);
                    }
                    break;
                }
                    
                default: break;
            }
        }
    }
    
    void ConsoleContent::sortOrderChanged(int newSortColumnId, bool isForwards)
    {
        sConsoleHistory history = getHistory();
        if(!history) return; //abort
        
        history->sort(static_cast<ConsoleHistory::Sort>(newSortColumnId));
        m_table.updateContent();
    }
    
    void ConsoleContent::cellDoubleClicked(int rowNumber, int columnId, const juce::MouseEvent& mouse)
    {
        // TODO : hilight (if possible) object corresponding to the dblclicked row
    }
    
    juce::Component* ConsoleContent::refreshComponentForCell(int, int, bool,
                                                             Component* existingComponentToUpdate)
    {
        // Just return 0, as we'll be painting these columns directly.
        jassert(existingComponentToUpdate == 0);
        return nullptr;
    }
    
    // This is overloaded from TableListBoxModel, and should choose the best width for the specified
    // column.
    int ConsoleContent::getColumnAutoSizeWidth(int columnId)
    {
        int widest = 32;
        
        sConsoleHistory history = getHistory();
        if(history)
        {
            // find the widest bit of text in this column..
            for(int i = getNumRows(); --i >= 0;)
            {
                if(auto msg = history->get(i).first)
                {
                    widest = std::max(widest, m_font.getStringWidth(msg->text));
                }
            }
        }
        
        return widest + 8;
    }
    
    void ConsoleContent::tableColumnsResized(juce::TableHeaderComponent* tableHeader)
    {
        if(tableHeader == &m_table.getHeader())
        {
            updateRighmostColumnWidth(&m_table.getHeader());
        }
    }
    
    void ConsoleContent::updateRighmostColumnWidth(juce::TableHeaderComponent* header)
    {
        int rightmostColumnId   = 0;
        int rightmostColumnX    = 0;
        
        for(int i = 0; i <= header->getNumColumns(false) - 1; i++)
        {
            if(header->isColumnVisible(i) && rightmostColumnId <= header->getIndexOfColumnId(i, true))
            {
                rightmostColumnId = i;
            }
        }
        
        rightmostColumnId   = header->getColumnIdOfIndex(header->getNumColumns(true)-1, true);
        rightmostColumnX    = header->getTotalWidth() - header->getColumnWidth(rightmostColumnId);
        
        if(rightmostColumnX <= getWidth())
        {
            m_table.getHeader().setColumnWidth(rightmostColumnId, getWidth() - rightmostColumnX);
        }
    }
    
    // ================================================================================ //
    //                                   CONSOLE TOOLBAR                                //
    // ================================================================================ //
    
    ConsoleToolbarFactory::ConsoleToolbarFactory()
    {
        
    }
    
    void ConsoleToolbarFactory::getAllToolbarItemIds(juce::Array<int>& ids)
    {
        ids.add(clear);
        ids.add(scroll_to_top);
        ids.add(scroll_to_bottom);
        ids.add(separatorBarId);
        ids.add(spacerId);
        ids.add(flexibleSpacerId);
    }
    
    void ConsoleToolbarFactory::getDefaultItemSet(juce::Array<int>& ids)
    {
        ids.add(clear);
        ids.add(flexibleSpacerId);
        ids.add(scroll_to_top);
        ids.add(scroll_to_bottom);
    }

    juce::ToolbarItemComponent* ConsoleToolbarFactory::createItem(int itemId)
    {
        juce::ToolbarItemComponent* btn = nullptr;
        
        if(itemId == clear)
        {
            btn = new juce::ToolbarButton(itemId, "clear", juce::Drawable::createFromImageData(binary_data::images::trash_png, binary_data::images::trash_png_size), nullptr);
            
            btn->setCommandToTrigger(&KiwiApp::getCommandManager(), CommandIDs::clearAll, true);
        }
        else if(itemId == scroll_to_top)
        {
            btn = new juce::ToolbarButton(itemId, "top", juce::Drawable::createFromImageData(binary_data::images::arrow_up_png, binary_data::images::arrow_up_png_size), nullptr);
            
            btn->setCommandToTrigger(&KiwiApp::getCommandManager(), CommandIDs::scrollToTop, true);
        }
        else if(itemId == scroll_to_bottom)
        {
            btn = new juce::ToolbarButton(itemId, "bottom", juce::Drawable::createFromImageData(binary_data::images::arrow_down_png, binary_data::images::arrow_down_png_size), nullptr);
            
            btn->setCommandToTrigger(&KiwiApp::getCommandManager(), CommandIDs::scrollToBottom, true);
        }
        
        return btn;
    }
    
    // ================================================================================ //
    //                                  CONSOLE COMPONENT                               //
    // ================================================================================ //
    
    Console::Console(sConsoleHistory history) :
    m_console(history)
    {
        addAndMakeVisible(m_console);
        
        KiwiApp::bindToCommandManager(this);
        KiwiApp::bindToKeyMapping(this);
        
        m_toolbar.setVertical(false);
        m_toolbar.setStyle(juce::Toolbar::ToolbarItemStyle::iconsOnly);
        m_toolbar.setColour(juce::Toolbar::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
        addAndMakeVisible(m_toolbar);
        
        // And use our item factory to add a set of default icons to it...
        m_toolbar.addDefaultItems(m_toolbar_factory);
    }
    
    void Console::resized()
    {
        const int toolbar_size = 40;
        const int toolbar_limit = getHeight() - toolbar_size;
        m_console.setBounds(getLocalBounds().withBottom(toolbar_limit));
        m_toolbar.setBounds(getLocalBounds().withTop(toolbar_limit).reduced(7));
    }
    
    void Console::paint(juce::Graphics& g)
    {
        const int toolbar_size = 40;
        const auto header_bounds = getLocalBounds().withTop(getHeight() - toolbar_size);
        
        auto& lf = KiwiApp::useLookAndFeel();
        g.setColour(lf.getCurrentColourScheme()
                    .getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
        g.fillRect(header_bounds);
    }
    
    // ================================================================================ //
    //                              APPLICATION COMMAND TARGET                          //
    // ================================================================================ //
    
    juce::ApplicationCommandTarget* Console::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void Console::getAllCommands(juce::Array<juce::CommandID>& commands)
    {
        commands.add(juce::StandardApplicationCommandIDs::copy);
        commands.add(CommandIDs::clearAll);
        commands.add(CommandIDs::scrollToTop);
        commands.add(CommandIDs::scrollToBottom);
    }
    
    void Console::getCommandInfo(const juce::CommandID commandID, juce::ApplicationCommandInfo& result)
    {
        switch (commandID)
        {
            case juce::StandardApplicationCommandIDs::copy:
            {
                result.setInfo(TRANS("Copy"), TRANS("Copy"), CommandCategories::editing, 0);
                result.addDefaultKeypress('c', juce::ModifierKeys::commandModifier);
                result.setActive(m_console.getNumSelectedRows() > 0);
                break;
            }
            case CommandIDs::clearAll:
            {
                result.setInfo(TRANS("Clear console history"), TRANS("Clear console history"), CommandCategories::editing, 0);
                //result.setActive(m_console.getNumRows() > 0);
                break;
            }
            case CommandIDs::scrollToTop:
                result.setInfo(TRANS("Scroll to the top"), TRANS("Scroll to the top"), CommandCategories::windows, 0);
                break;
                
            case CommandIDs::scrollToBottom:
                result.setInfo(TRANS("Scroll to the bottom"), TRANS("Scroll to the bottom"), CommandCategories::windows, 0);
                break;
                
            default: break;
        }
    }
    
    bool Console::perform(InvocationInfo const& info)
    {
        switch (info.commandID)
        {
            case juce::StandardApplicationCommandIDs::copy: m_console.copy(); break;
            case CommandIDs::clearAll:                      m_console.clearAll(); break;
            case CommandIDs::scrollToTop:                   m_console.scrollToTop(); break;
            case CommandIDs::scrollToBottom:                m_console.scrollToBottom(); break;
            default: return false;
        }
        return true;
    }
}
