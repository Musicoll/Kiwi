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

#include <flip/BackEndBinary.h>
#include <flip/contrib/DataProviderFile.h>
#include <flip/BackEndIR.h>

#include <KiwiModel/KiwiModel_Def.h>

#include <KiwiEngine/KiwiEngine_Console.h>

#include "KiwiApp_DocumentBrowserView.h"
#include "KiwiApp_Instance.h"

#include "../KiwiApp_Ressources/KiwiApp_BinaryData.h"
#include "../KiwiApp_Utils/KiwiApp_SuggestList.h"
#include "../KiwiApp.h"

#include <juce_gui_basics/juce_gui_basics.h>

namespace kiwi
{
    // ================================================================================ //
    //                                  DOCUMENT BROWSER                                //
    // ================================================================================ //
    
    DocumentBrowserView::DocumentBrowserView(DocumentBrowser& browser, bool enabled)
    : m_browser(browser)
    {
        setSize(1, 1);
        
        DocumentBrowser::Drive & drive = *m_browser.getDrive();
        
        auto drive_view = std::make_unique<DriveView>(drive);
        drive_view->setSize(getWidth(), drive_view->getHeight());
        addAndMakeVisible(drive_view.get());
        m_drives.emplace_back(std::move(drive_view));
        
        resized();
        
        setSize(300, 300);
        
        setEnabled(enabled);
    }
    
    DocumentBrowserView::~DocumentBrowserView()
    {
    }
    
    void DocumentBrowserView::resized()
    {
        const auto bounds = getLocalBounds();
        
        int last_bottom = 0;
        for(auto const& drive_view_uptr : m_drives)
        {
            drive_view_uptr->setBounds(bounds);
            last_bottom = drive_view_uptr->getBottom();
        }
    }
    
    void DocumentBrowserView::paint(juce::Graphics& g)
    {
        ;
    }
    
    void DocumentBrowserView::enablementChanged()
    {
        for(auto & drive : m_drives)
        {
            drive->setEnabled(isEnabled());
        }
    }
    
    // ================================================================================ //
    //                               DOCUMENT SESSION VIEW                              //
    // ================================================================================ //
    
    DocumentBrowserView::DriveView::RowElem::RowElem(DriveView& drive_view,
                                                     std::string const& name,
                                                     std::string const& tooltip)
    : m_drive_view(drive_view)
    , m_name(name)
    , m_open_btn("open", std::unique_ptr<juce::Drawable>(juce::Drawable::createFromImageData(binary_data::images::open_png, binary_data::images::open_png_size)), ImageButton::ButtonStyle::ImageFitted)
    , m_kiwi_filetype_img(juce::ImageCache::getFromMemory(binary_data::images::kiwi_filetype_png,
                                                        binary_data::images::kiwi_filetype_png_size))
    {
        setTooltip(tooltip);
        
        if(!m_drive_view.isShowingTrashedDocuments())
        {
            m_open_btn.setCommand(std::bind(&DriveView::openDocument, &m_drive_view, m_row));
            m_open_btn.setSize(40, 40);
            m_open_btn.setTooltip("open this patcher");
            addChildComponent(m_open_btn);
        }
        
        // label setup
        m_name_label.setText(m_name, juce::NotificationType::dontSendNotification);
        m_name_label.setSize(1, 1);
        m_name_label.setEditable(false, true, true);
        m_name_label.addListener(this);
        m_name_label.setColour(juce::Label::textColourId, juce::Colours::black);
        m_name_label.setInterceptsMouseClicks(false, false);
        addAndMakeVisible(m_name_label);
    }
    
    DocumentBrowserView::DriveView::RowElem::~RowElem()
    {
        removeMouseListener(this);
    }
    
    void DocumentBrowserView::DriveView::RowElem::update(std::string const& name,
                                                         std::string const& tooltip,
                                                         int row,
                                                         bool now_selected)
    {
        setTooltip(tooltip);
        
        m_row = row;
        m_open_btn.setCommand(std::bind(&DriveView::openDocument, &m_drive_view, m_row));
        
        m_name = name;
        m_name_label.setText(m_name, juce::NotificationType::dontSendNotification);
        
        m_selected = now_selected;
        m_open_btn.setVisible(m_selected && !m_drive_view.isShowingTrashedDocuments());
        repaint();
    }
    
    void DocumentBrowserView::DriveView::RowElem::paint(juce::Graphics& g)
    {
        const bool is_trash_row = m_drive_view.isShowingTrashedDocuments();
        const auto bounds = getLocalBounds();
        const juce::Colour bg_color(is_trash_row ? 0xDDDDDDDD : 0xDDFFFFFF);
        const juce::Colour selected_color_color(juce::Colours::lightblue);
        
        g.fillAll(((!is_trash_row && m_selected)
                   ? selected_color_color
                   : (m_mouseover ? bg_color.darker(0.1f) : bg_color)));
        
        if(!is_trash_row)
        {
            // document status notifier (connected / disconnected / not-connected)
            g.setColour(juce::Colours::grey);
            g.fillRect(0, 0, 5, getHeight());
        }
        
        g.setColour(bg_color.darker(0.5f));
        g.drawHorizontalLine(getBottom() - 1, 0., getWidth());
        
        g.drawImage(m_kiwi_filetype_img,
                    juce::Rectangle<float>(is_trash_row ? 5 : 10, 5, 30, 30),
                    juce::RectanglePlacement::stretchToFit, false);
    }
    
    void DocumentBrowserView::DriveView::RowElem::resized()
    {
        const auto bounds = getLocalBounds();
        
        if(!m_drive_view.isShowingTrashedDocuments())
        {
            m_open_btn.setBounds(bounds.reduced(5).withLeft(bounds.getWidth() - 40));
        }
        
        m_name_label.setBounds(bounds.reduced(5).withRight(m_open_btn.getX() - 5).withLeft(40));
    }
    
    void DocumentBrowserView::DriveView::RowElem::mouseEnter(juce::MouseEvent const& e)
    {
        m_mouseover = true;
        repaint();
    }
    
    void DocumentBrowserView::DriveView::RowElem::mouseExit(juce::MouseEvent const& e)
    {
        m_mouseover = false;
        repaint();
    }
    
    void DocumentBrowserView::DriveView::RowElem::showPopup()
    {
        juce::PopupMenu m;
        
        enum Commands : int
        {
            None = 0,
            Rename,
            MoveToTrash,
            RestoreTrashed,
            Upload,
            Duplicate,
            Download,
            SortByName,
            SortByCreationTime,
            SortByCreator,
            SortByLastOpenedTime
        };
        
        if (!m_drive_view.isShowingTrashedDocuments())
        {
            m.addItem(Commands::Rename, "Rename");
            m.addItem(Commands::MoveToTrash, "Move to trash");
            m.addItem(Commands::Upload, "Upload");
            m.addItem(Commands::Duplicate, "Duplicate");
        }
        else
        {
            m.addItem(Commands::RestoreTrashed, "Restore");
        }
        
        m.addItem(Commands::Download, "Download");
        
        DriveView::SortBy current_sort = m_drive_view.getSortType();
        
        juce::PopupMenu sort_menu;
        sort_menu.addItem(Commands::SortByName, "Name",
                          current_sort != DriveView::SortBy::name,
                          current_sort == DriveView::SortBy::name);
        
        sort_menu.addItem(Commands::SortByCreationTime, "Creation date",
                          current_sort != DriveView::SortBy::creationTime,
                          current_sort == DriveView::SortBy::creationTime);
        
        sort_menu.addItem(Commands::SortByCreator, "Creator",
                          current_sort != DriveView::SortBy::author,
                          current_sort == DriveView::SortBy::author);
        
        sort_menu.addItem(Commands::SortByLastOpenedTime, "Last opened",
                          current_sort != DriveView::SortBy::openedTime,
                          current_sort == DriveView::SortBy::openedTime);
        
        m.addSubMenu("Sort by", sort_menu);
        
        int result = m.show();
        
        switch(result)
        {
            case Commands::Rename: // rename
            {
                m_name_label.showEditor();
                break;
            }
            case Commands::MoveToTrash: // delete
            {
                m_drive_view.deleteDocumentForRow(m_row);
                break;
            }
            case Commands::RestoreTrashed: // restore
            {
                m_drive_view.restoreDocumentForRow(m_row);
                break;
            }
            case Commands::Download: // download document
            {
                m_drive_view.downloadDocumentForRow(m_row);
                break;
            }
            case Commands::Upload: // upload document
            {
                m_drive_view.uploadDocument();
                break;
            }
            case Commands::Duplicate: // duplicate document
            {
                m_drive_view.duplicateDocumentForRow(m_row);
                break;
            }
            case Commands::SortByName: // sort by name
            {
                m_drive_view.setSortType(DriveView::SortBy::name);
                break;
            }
            case Commands::SortByCreationTime: // sort by creation date
            {
                m_drive_view.setSortType(DriveView::SortBy::creationTime);
                break;
            }
            case Commands::SortByCreator: // sort by author
            {
                m_drive_view.setSortType(DriveView::SortBy::author);
                break;
            }
            case Commands::SortByLastOpenedTime:
            {
                m_drive_view.setSortType(DriveView::SortBy::openedTime);
                break;
            }
        }
    }
    
    void DocumentBrowserView::DriveView::RowElem::mouseDown(juce::MouseEvent const& e)
    {
        m_drive_view.selectRow(m_row);
        
        if (e.mods.isPopupMenu())
        {
            showPopup();
        }
    }
    
    void DocumentBrowserView::DriveView::RowElem::mouseUp(juce::MouseEvent const& e)
    {
    }
    
    void DocumentBrowserView::DriveView::RowElem::mouseDoubleClick(juce::MouseEvent const& event)
    {
        m_drive_view.listBoxItemDoubleClicked(m_row, event);
    }
    
    void DocumentBrowserView::DriveView::RowElem::labelTextChanged(juce::Label* label)
    {
        if(label == &m_name_label)
        {
            m_drive_view.renameDocumentForRow(m_row, m_name_label.getText().toStdString());
            
            m_name_label.setText(m_name, juce::NotificationType::dontSendNotification);
        }
    }
    
    void DocumentBrowserView::DriveView::RowElem::showEditor()
    {
        m_name_label.showEditor();
    }
    
    // ================================================================================ //
    //                             BROWSER DRIVE VIEW HEADER                            //
    // ================================================================================ //
    
    DocumentBrowserView::DriveView::Header::Header(DocumentBrowserView::DriveView& drive_view)
    : m_drive_view(drive_view)
    , m_refresh_btn("refresh",
                    std::unique_ptr<juce::Drawable>
                    (juce::Drawable::createFromImageData(binary_data::images::refresh_png,
                                                         binary_data::images::refresh_png_size)))
    , m_create_document_btn("create",
                            std::unique_ptr<juce::Drawable>
                            (juce::Drawable::createFromImageData(binary_data::images::plus_png,
                                                                 binary_data::images::plus_png_size)))
    , m_trash_btn("trash",
                  std::unique_ptr<juce::Drawable>
                  (juce::Drawable::createFromImageData(binary_data::images::trash_png,
                                                       binary_data::images::trash_png_size)))
    , m_search_btn("search",
                   std::unique_ptr<juce::Drawable>
                   (juce::Drawable::createFromImageData(binary_data::images::search_png,
                                                        binary_data::images::search_png_size)))
    , m_folder_bounds()
    , m_label("drive_name", m_drive_view.getDriveName())
    , m_folder_img(juce::ImageCache::getFromMemory(binary_data::images::folder_png,
                                                   binary_data::images::folder_png_size))
    , m_disable_folder_img(m_folder_img)
    {
        m_folder_img.duplicateIfShared();
        m_disable_folder_img.multiplyAllAlphas(0.5);
        
        m_label.setFont(m_label.getFont().withHeight(20));
        m_label.setEditable(false);
        m_label.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
        addAndMakeVisible(m_label);
        
        m_create_document_btn.setCommand([this]()
        {
            if (!m_drive_view.isShowingTrashedDocuments())
                m_drive_view.createDocument();
        });
        
        m_create_document_btn.setSize(40, 40);
        m_create_document_btn.setTooltip("Create a new hosted patcher");
        addAndMakeVisible(m_create_document_btn);
        
        m_refresh_btn.setCommand([this](){
            m_drive_view.refresh();
        });
        
        m_refresh_btn.setSize(40, 40);
        m_refresh_btn.setTooltip("Refresh list");
        addAndMakeVisible(m_refresh_btn);
        
        m_trash_btn.setCommand([this]() {
            const bool was_in_trash_mode = m_drive_view.isShowingTrashedDocuments();
            const bool is_in_trash_mode = !was_in_trash_mode;
            m_drive_view.setTrashMode(is_in_trash_mode);
            m_trash_btn.setTooltip(juce::String(is_in_trash_mode ? "Hide" : "Show")
                                   + " trashed documents");
            
            m_create_document_btn.setEnabled(!is_in_trash_mode);
            m_trash_btn.setToggleState(is_in_trash_mode,
                                       juce::NotificationType::dontSendNotification);
        });
        
        m_trash_btn.setToggleState(m_drive_view.isShowingTrashedDocuments(),
                                   juce::NotificationType::dontSendNotification);
        
        m_trash_btn.setSize(40, 40);
        m_trash_btn.setTooltip("Display trashed documents");
        addAndMakeVisible(m_trash_btn);
        
        // searchbar
        m_searchbar.setMultiLine(false);
        m_searchbar.setFont(18);
        m_searchbar.setSize(100, m_searchbar.getFont().getHeight() + 10);
        m_searchbar.setTextToShowWhenEmpty("search...",
                                           m_searchbar.findColour(juce::TextEditor::ColourIds::textColourId)
                                           .contrasting(0.5));
        m_searchbar.addListener(this);
        
        // search_btn
        m_search_btn.setCommand([this]() {
            showSearchBar(m_search_btn.getToggleState());
        });
        
        m_search_btn.setClickingTogglesState(true);
        
        m_search_btn.setSize(40, 40);
        m_search_btn.setTooltip("Search documents");
        addAndMakeVisible(m_search_btn);
    }
    
    void DocumentBrowserView::DriveView::Header::enablementChanged()
    {
        const bool enabled = isEnabled();
        m_create_document_btn.setEnabled(enabled);
        m_refresh_btn.setEnabled(enabled);
        m_trash_btn.setEnabled(enabled);
    }
    
    void DocumentBrowserView::DriveView::Header::resized()
    {
        auto bounds = getLocalBounds();
        
        if(m_search_btn.getToggleState())
        {
            m_searchbar.setBounds(bounds.removeFromBottom(m_searchbar.getFont().getHeight() + 10));
        }
        
        m_folder_bounds = bounds.removeFromLeft(50).reduced(10);
        
        m_trash_btn.setTopRightPosition(getWidth(), 5);
        m_refresh_btn.setTopRightPosition(m_trash_btn.getX(), 5);
        m_search_btn.setTopRightPosition(m_refresh_btn.getX(), 5);
        m_create_document_btn.setTopRightPosition(m_search_btn.getX(), 5);
        
        int remaining_width = std::max(0, m_create_document_btn.getX() - m_folder_bounds.getRight());
        
        m_label.setBounds(bounds.withX(m_folder_bounds.getRight() + 5).withWidth(remaining_width));
    }
    
    void DocumentBrowserView::DriveView::Header::paint(juce::Graphics& g)
    {
        auto& lf = KiwiApp::useLookAndFeel();
        const juce::Colour color = lf.getCurrentColourScheme().getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::windowBackground);
        
        g.fillAll(color);
        
        if (isEnabled())
        {
            g.drawImage(m_folder_img, m_folder_bounds.toFloat(),
                        juce::RectanglePlacement::yMid, false);
        }
        else
        {
            g.drawImage(m_disable_folder_img, m_folder_bounds.toFloat(),
                        juce::RectanglePlacement::yMid, false);
        }
        
        const auto bounds = getLocalBounds();
        
        g.setColour(color.darker(0.3));
        g.drawHorizontalLine(bounds.getBottom() - 1, 0, getWidth());
    }
    
    void DocumentBrowserView::DriveView::Header::mouseDown(juce::MouseEvent const& e)
    {
        m_drive_view.getModel()->backgroundClicked(e);
    }
    
    void DocumentBrowserView::DriveView::Header::setText(std::string const& text)
    {
        m_label.setText(text, juce::NotificationType::dontSendNotification);
        repaint();
    }
    
    void DocumentBrowserView::DriveView::Header::showSearchBar(bool show)
    {
        if(show)
        {
            addAndMakeVisible(m_searchbar);
            m_searchbar.grabKeyboardFocus();
            setSize(getWidth(), m_toolbar_thickness + m_searchbar.getHeight());
            m_drive_view.resized();
        }
        else
        {
            m_drive_view.setFilter("");
            
            m_searchbar.clear();
            removeChildComponent(&m_searchbar);
            setSize(getWidth(), m_toolbar_thickness);
            m_drive_view.resized();
        }
    }
    
    void DocumentBrowserView::DriveView::Header::textEditorTextChanged(juce::TextEditor& editor)
    {
        if(&editor == &m_searchbar)
        {
            const auto newtext = editor.getText().toStdString();
            m_drive_view.setFilter(newtext);
        }
    }
    
    void DocumentBrowserView::DriveView::Header::textEditorEscapeKeyPressed (juce::TextEditor& editor)
    {
        if(&editor == &m_searchbar)
        {
            m_search_btn.setToggleState(false, juce::NotificationType::sendNotificationSync);
        }
    }
    
    // ================================================================================ //
    //                                 BROWSER DRIVE VIEW                               //
    // ================================================================================ //
    
    DocumentBrowserView::DriveView::DriveView(DocumentBrowser::Drive& drive)
    : juce::ListBox("document list", this)
    , m_drive(drive)
    , m_trash_mode(false)
    , m_enabled(true)
    , m_sorter()
    {
        m_drive.addListener(*this);
        
        setMultipleSelectionEnabled(false);
        setRowSelectedOnMouseDown(false);
        setRowHeight(40);
        
        auto* header = new Header(*this);
        header->setSize(getWidth(), 50);
        setHeaderComponent(header);
        
        getViewport()->setScrollBarThickness(10);
        juce::ListBox::setColour(juce::ListBox::backgroundColourId, juce::Colour(0xFFD4D4D4));
        
        restoreState();
        update();
    }
    
    DocumentBrowserView::DriveView::~DriveView()
    {
        saveState();
        m_drive.removeListener(*this);
    }
    
    void DocumentBrowserView::DriveView::saveState()
    {
        const juce::String sort_str = [](SortBy sort) {
            switch (sort) {
                case SortBy::name: return "name";
                case SortBy::author: return "author";
                case SortBy::openedTime: return "openedTime";
                case SortBy::creationTime:
                default: return "creationTime";
            }
        }(getSortType());
        
        getGlobalProperties().setValue("DocumentBrowser_sort_type", sort_str);
    }
    
    void DocumentBrowserView::DriveView::restoreState()
    {
        const SortBy sort = [](juce::String sort_str) {
            
            SortBy sort = SortBy::creationTime;
            
            if(sort_str == "name")
            {
                sort = SortBy::name;
            }
            else if(sort_str == "author")
            {
                sort = SortBy::author;
            }
            else if(sort_str == "openedTime")
            {
                sort = SortBy::openedTime;
            }
            else if(sort_str == "creationTime")
            {
                sort = SortBy::creationTime;
            }
            
            return sort;
            
        }(getGlobalProperties().getValue("DocumentBrowser_sort_type"));
        
        setSortType(sort);
    }
    
    bool DocumentBrowserView::DriveView::Comp::compare(DocumentBrowser::Drive::DocumentSession const& lhs,
                                                       DocumentBrowser::Drive::DocumentSession const& rhs) const
    {
        bool type_ordered = false;
        
        switch(m_type)
        {
            case SortBy::name:
            {
                type_ordered = juce::String(lhs.getName()).compareNatural(rhs.getName()) < 0;
                break;
            }
            case SortBy::author:
            {
                type_ordered = juce::String(lhs.getAuthor()).compareNatural(rhs.getAuthor()) < 0;
                break;
            }
            case SortBy::creationTime:
            {
                type_ordered = lhs.getCreationTime() > rhs.getCreationTime();
                break;
            }
            case SortBy::openedTime:
            {
                type_ordered = lhs.getOpenedTime() > rhs.getOpenedTime();
                break;
            }
                
            default: break;
        }
        
        bool trash_order = (m_trashed_first && lhs.isTrashed() > rhs.isTrashed())
                            || (!m_trashed_first && lhs.isTrashed() < rhs.isTrashed());
        
        return trash_order
               || (lhs.isTrashed() == rhs.isTrashed() && type_ordered);
    }
    
    void DocumentBrowserView::DriveView::enablementChanged()
    {
        getHeaderComponent()->setEnabled(isEnabled());
        update();
    }
    
    void DocumentBrowserView::DriveView::driveChanged()
    {
        update();
    }
    
    std::vector<DocumentBrowser::Drive::DocumentSession*> DocumentBrowserView::DriveView::getDisplayedDocuments()
    {
        auto& all_documents = m_drive.getDocuments();
        std::vector<DocumentBrowser::Drive::DocumentSession*> docs;
        docs.reserve(all_documents.size());
        
        for(auto& doc_uptr : all_documents)
        {
            auto* doc = doc_uptr.get();
            
            if(m_trash_mode ? doc->isTrashed() : !doc->isTrashed())
            {
                docs.emplace_back(doc);
            }
        }
        
        std::string const& filter = m_sorter.m_filter;
        
        if(!filter.empty())
        {
            struct ScoredEntry
            {
                ScoredEntry(DocumentBrowser::Drive::DocumentSession* _document, int _score) : document(_document), score(_score) {}
                bool operator<(ScoredEntry const& entry) const { return (score >= entry.score); }
                
                DocumentBrowser::Drive::DocumentSession* document = nullptr;
                int score = 0;
            };
            
            std::set<ScoredEntry> scored_entries;
            
            for(auto* doc : docs)
            {
                const auto r = SuggestList::computeScore(filter.c_str(), doc->getName().c_str());
                if(r.first)
                {
                    scored_entries.insert({doc, r.second});
                }
            }
            
            docs.clear();
            
            for(auto const& scored_entry : scored_entries)
            {
                docs.emplace_back(scored_entry.document);
            }
        }
        
        return docs;
    }
    
    DocumentBrowser::Drive::DocumentSession* DocumentBrowserView::DriveView::getDocumentForRow(int row)
    {
        auto documents = getDisplayedDocuments();
        if(row < documents.size())
        {
            return documents.at(row);
        }
        return nullptr;
    }
    
    int DocumentBrowserView::DriveView::getNumRows()
    {
        int num_rows = 0;
        
        if (isEnabled())
        {
            return getDisplayedDocuments().size();
        }
        
        return num_rows;
    }
    
    void DocumentBrowserView::DriveView::paintListBoxItem(int rowNumber, juce::Graphics& g,
                                                          int width, int height, bool selected)
    {
        // using custom components instead.
    }
    
    std::string const& DocumentBrowserView::DriveView::getDriveName() const
    {
        return m_drive.getName();
    }
    
    void DocumentBrowserView::DriveView::refresh()
    {
        m_drive.refresh();
    }
    
    void DocumentBrowserView::DriveView::createDocument()
    {
        juce::AlertWindow alert("Create a new hosted Patcher", "name:",
                                juce::AlertWindow::AlertIconType::NoIcon);
        
        alert.addButton("Cancel", 0);
        alert.addButton("Ok", 1);
        alert.addTextEditor("title", "Untitled");

        if(alert.runModalLoop())
        {
            auto& text_editor = *alert.getTextEditor("title");
            auto text = text_editor.getText();
            m_drive.createNewDocument(text.toStdString());
        }
    }
    
    void DocumentBrowserView::DriveView::setTrashMode(bool trash_mode)
    {
        m_trash_mode = trash_mode;
        
        m_sorter.m_trashed_first = m_trash_mode;
        
        m_drive.setSort([sorter = m_sorter](DocumentBrowser::Drive::DocumentSession const& l_hs,
                                            DocumentBrowser::Drive::DocumentSession const& r_hs)
        {
            return sorter.compare(l_hs, r_hs);
        });
    }
    
    DocumentBrowserView::DriveView::SortBy DocumentBrowserView::DriveView::getSortType() const
    {
        return m_sorter.m_type;
    }
    
    void DocumentBrowserView::DriveView::setSortType(SortBy sort_type)
    {
        m_sorter.m_type = sort_type;
        
        m_drive.setSort([sorter = m_sorter](DocumentBrowser::Drive::DocumentSession const& lhs,
                                            DocumentBrowser::Drive::DocumentSession const& rhs)
        {
            return sorter.compare(lhs, rhs);
            
        });
    }
    
    void DocumentBrowserView::DriveView::setFilter(std::string const& text)
    {
        m_sorter.m_filter = text;
        update();
    }
    
    bool DocumentBrowserView::DriveView::isShowingTrashedDocuments() const
    {
        return m_trash_mode;
    }
    
    void DocumentBrowserView::DriveView::update()
    {
        dynamic_cast<Header*>(getHeaderComponent())->setText(getDriveName());
        updateContent();
        repaint();
    }
    
    std::string DocumentBrowserView::DriveView::createDocumentToolTip(DocumentBrowser::Drive::DocumentSession const& doc)
    {
        std::string tooltip = "name: " + doc.getName() + "\n"
        + "created by: " + doc.getAuthor() + " (" + doc.getCreationDate() + ")\n"
        + "last opened by: " + doc.getOpenedUser() + " (" + doc.getOpenedDate() + ")";
        
        if (doc.isTrashed())
        {
            tooltip += "\ntrashed at: " + doc.getTrashedDate();
        }
        
        return tooltip;
    }
    
    juce::Component* DocumentBrowserView::DriveView::refreshComponentForRow(int row, bool selected,
                                                                            juce::Component* c)
    {
        auto documents = getDisplayedDocuments();
        
        if(row >= documents.size())
        {
            if(c != nullptr)
            {
                delete c;
                c = nullptr;
            }
        }
        else
        {
            if(c == nullptr)
            {
                c = new RowElem(*this, documents[row]->getName(), createDocumentToolTip(*documents[row]));
            }
            
            static_cast<RowElem*>(c)->update(documents[row]->getName(),
                                             createDocumentToolTip(*documents[row]),
                                             row,
                                             selected);
        }

        return c;
    }
    
    void DocumentBrowserView::DriveView::backgroundClicked(juce::MouseEvent const&)
    {
        deselectAllRows();
    }
    
    void DocumentBrowserView::DriveView::returnKeyPressed(int last_row_selected)
    {
        if(getNumSelectedRows() > 0)
        {
            auto* c = getComponentForRowNumber(last_row_selected);
            if(c)
            {
                static_cast<RowElem*>(c)->showEditor();
            }
        }
    }
    
    void DocumentBrowserView::DriveView::openDocument(int row)
    {
        if(auto* doc = getDocumentForRow(row))
        {
            doc->open();
        }
    }
    
    void DocumentBrowserView::DriveView::restoreDocumentForRow(int row)
    {
        if(auto* doc = getDocumentForRow(row))
        {
            doc->untrash();
        }
    }
    
    void DocumentBrowserView::DriveView::deleteDocumentForRow(int row)
    {
        if(auto* doc = getDocumentForRow(row))
        {
            doc->trash();
        }
    }
    
    void DocumentBrowserView::DriveView::renameDocumentForRow(int row, std::string const& new_name)
    {
        if(auto* doc = getDocumentForRow(row))
        {
            doc->rename(new_name);
        }
    }
    
    void DocumentBrowserView::DriveView::duplicateDocumentForRow(int row)
    {
        if(auto* doc = getDocumentForRow(row))
        {
            doc->duplicate();
        }
    }
    
    void DocumentBrowserView::DriveView::uploadDocument()
    {
        auto directory = juce::File::getSpecialLocation(juce::File::userHomeDirectory);
        
        juce::FileChooser file_chooser("Upload file", directory, "*.kiwi");
        
        if(file_chooser.browseForFileToOpen())
        {
            juce::File result = file_chooser.getResult();
            
            // Test that document is a valid kiwi document.
            flip::DataProviderFile provider(result.getFullPathName().toStdString().c_str());
            flip::BackEndIR back_end;
            
            back_end.register_backend<flip::BackEndBinary>();
            
            std::string current_version(KIWI_MODEL_VERSION_STRING);
            
            if (back_end.read(provider) && current_version.compare(back_end.version) == 0)
            {
                juce::MemoryBlock buffer;
                result.loadFileAsData(buffer);
                
                std::string data((char *) buffer.getData(), buffer.getSize() / sizeof(char));
                
                m_drive.uploadDocument(result.getFileNameWithoutExtension().toStdString(),
                                       data);
            }
            else
            {
                KiwiApp::error("Not a valid Kiwi file");
            }
        }
    }
    
    void DocumentBrowserView::DriveView::downloadDocumentForRow(int row)
    {
        auto* document = getDocumentForRow(row);
        
        if(document == nullptr)
            return; // abort
        
        auto directory = juce::File::getSpecialLocation(juce::File::userHomeDirectory);
        
        juce::File suggest_file =
        directory.getChildFile(juce::String(document->getName())).withFileExtension(".kiwi;");
        
        juce::FileChooser saveFileChooser("Download file", suggest_file, "*.kiwi;");
        
        if (saveFileChooser.browseForFileToSave(true))
        {
            juce::File result = saveFileChooser.getResult();
            
            document->download([result](std::string const& content)
            {   
                if(result.create().wasOk())
                {
                    result.replaceWithData(content.data(), content.size() * sizeof(char));
                }
            });
        }
    }
    
    void DocumentBrowserView::DriveView::listBoxItemDoubleClicked(int row, juce::MouseEvent const& e)
    {
        openDocument(row);
    }
}
