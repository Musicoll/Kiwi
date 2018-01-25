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

#include <KiwiEngine/KiwiEngine_Console.h>

#include "KiwiApp_DocumentBrowserView.h"
#include "KiwiApp_Instance.h"

#include "../KiwiApp_Ressources/KiwiApp_BinaryData.h"
#include "../KiwiApp.h"

namespace kiwi
{
    // ================================================================================ //
    //                                  DOCUMENT BROWSER                                //
    // ================================================================================ //
    
    DocumentBrowserView::DocumentBrowserView(DocumentBrowser& browser) :
    m_browser(browser)
    {
        setSize(1, 1);
        m_browser.addListener(*this);
        for(auto* drive : m_browser.getDrives())
        {
            driveAdded(*drive);
        }
        
        setSize(200, 300);
    }
    
    DocumentBrowserView::~DocumentBrowserView()
    {
        m_browser.removeListener(*this);
    }
    
    void DocumentBrowserView::driveAdded(DocumentBrowser::Drive& drive)
    {
        auto drive_view = std::make_unique<DriveView>(drive);
        drive_view->setSize(getWidth(), drive_view->getHeight());
        addAndMakeVisible(drive_view.get());
        m_drives.emplace_back(std::move(drive_view));
        resized();
    }
    
    void DocumentBrowserView::driveRemoved(DocumentBrowser::Drive const& drive)
    {
        const auto drive_view_it = std::find_if(m_drives.begin(), m_drives.end(), [&drive](std::unique_ptr<DriveView> const& drive_view){
            return (*drive_view.get() == drive);
        });
        
        if(drive_view_it != m_drives.end())
        {
            removeChildComponent((*drive_view_it).get());
            m_drives.erase(drive_view_it);
        }
    }
    
    void DocumentBrowserView::driveChanged(DocumentBrowser::Drive const& drive)
    {
        resized();
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
    
    // ================================================================================ //
    //                               DOCUMENT SESSION VIEW                              //
    // ================================================================================ //
    
    DocumentBrowserView::DriveView::RowElem::RowElem(DriveView& drive_view,
                                                     std::string const& name,
                                                     std::string const& tooltip) :
    m_drive_view(drive_view),
    m_name(name),
    m_open_btn("open", std::unique_ptr<juce::Drawable>(juce::Drawable::createFromImageData(binary_data::images::open_png, binary_data::images::open_png_size)), ImageButton::ButtonStyle::ImageFitted),
    m_kiwi_filetype_img(juce::ImageCache::getFromMemory(binary_data::images::kiwi_filetype_png,
                                                        binary_data::images::kiwi_filetype_png_size)),
    m_row(-1)
    {
        setTooltip(tooltip);
        
        m_open_btn.setCommand(std::bind(&DriveView::openDocument, &m_drive_view, m_row));
        m_open_btn.setSize(40, 40);
        m_open_btn.setTooltip("open this patcher");
        addChildComponent(m_open_btn);
        
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
        m_open_btn.setVisible(m_selected);
        repaint();
    }
    
    void DocumentBrowserView::DriveView::RowElem::paint(juce::Graphics& g)
    {
        const auto bounds = getLocalBounds();
        const juce::Colour bg_color(0xDDFFFFFF);
        const juce::Colour selected_color_color(juce::Colours::lightblue);
        
        g.setColour(m_selected ? selected_color_color : m_mouseover ? bg_color.darker(0.1f) : bg_color);
        g.fillAll();
        
        // document status notifier (connected / disconnected / not-connected)
        g.setColour(juce::Colours::grey);
        g.fillRect(0, 0, 5, getHeight());
        
        g.setColour(bg_color.darker(0.5f));
        g.drawHorizontalLine(getBottom() - 1, 0., getWidth());
        
        g.drawImage(m_kiwi_filetype_img,
                    juce::Rectangle<float>(10, 5, 30, 30),
                    juce::RectanglePlacement::stretchToFit, false);
    }
    
    void DocumentBrowserView::DriveView::RowElem::resized()
    {
        const auto bounds = getLocalBounds();
        m_open_btn.setBounds(bounds.reduced(5).withLeft(bounds.getWidth() - 40));
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
        
        m.addItem(1, "Rename");
        m.addItem(2, m_drive_view.getTrashMode() ? "Restore" : "Delete");
        
        DriveView::SortType current_sort = m_drive_view.getSortType();
        
        juce::PopupMenu sort_menu;
        sort_menu.addItem(3, "Name",
                          current_sort != DriveView::SortType::name,
                          current_sort == DriveView::SortType::name);
        
        sort_menu.addItem(4, "Created at",
                          current_sort != DriveView::SortType::creationDate,
                          current_sort == DriveView::SortType::creationDate);
        
        sort_menu.addItem(5, "Created by",
                          current_sort != DriveView::SortType::author,
                          current_sort == DriveView::SortType::author);
        
        m.addSubMenu("Sort by", sort_menu);
        
        int result = m.show();
        
        switch(result)
        {
            case 1: // rename
            {
                m_name_label.showEditor();
                break;
            }
            case 2: // delete/Restore
            {
                if (!m_drive_view.getTrashMode())
                {
                    m_drive_view.deleteDocumentForRow(m_row);
                }
                else
                {
                    m_drive_view.restoreDocumentForRow(m_row);
                }
                break;
            }
            case 3: // sort by name
            {
                m_drive_view.setSortType(DriveView::SortType::name);
                break;
            }
            case 4: // sort by creation date
            {
                m_drive_view.setSortType(DriveView::SortType::creationDate);
                break;
            }
            case 5:
            {
                m_drive_view.setSortType(DriveView::SortType::author);
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
    
    DocumentBrowserView::DriveView::Header::Header(DocumentBrowserView::DriveView& drive_view):
    m_drive_view(drive_view),
    m_refresh_btn("refresh",
                  std::unique_ptr<juce::Drawable>
                  (juce::Drawable::createFromImageData(binary_data::images::refresh_png,
                                                       binary_data::images::refresh_png_size))),
    m_create_document_btn("create",
                          std::unique_ptr<juce::Drawable>
                          (juce::Drawable::createFromImageData(binary_data::images::plus_png,
                                                               binary_data::images::plus_png_size))),
    m_trash_btn("trash",
                std::unique_ptr<juce::Drawable>
                (juce::Drawable::createFromImageData(binary_data::images::trash_png,
                                                     binary_data::images::trash_png_size))),
    m_search_bar(),
    m_folder_bounds(),
    m_label("drive_name", m_drive_view.getDriveName()),
    m_folder_img(juce::ImageCache::getFromMemory(binary_data::images::folder_png,
                                                 binary_data::images::folder_png_size))
    {
        m_label.setFont(m_label.getFont().withHeight(20));
        m_label.setEditable(false);
        m_label.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
        addAndMakeVisible(m_label);
        
        m_search_bar.setPopupMenuEnabled(false);
        addAndMakeVisible(m_search_bar);
        
        m_create_document_btn.setCommand([this](){m_drive_view.createDocument();});
        m_create_document_btn.setSize(40, 40);
        m_create_document_btn.setTooltip("Create a new patcher on this drive");
        m_create_document_btn.setColour(ImageButton::ColourIds::textColourId, juce::Colours::whitesmoke);
        addAndMakeVisible(m_create_document_btn);
        
        m_refresh_btn.setCommand([this](){m_drive_view.refresh();});
        m_refresh_btn.setSize(40, 40);
        m_refresh_btn.setTooltip("Refresh Document list");
        m_refresh_btn.setColour(ImageButton::ColourIds::textColourId, juce::Colours::whitesmoke);
        addAndMakeVisible(m_refresh_btn);
        
        m_trash_btn.setCommand([this]()
        {
            m_trash_btn.setAlpha(!m_drive_view.getTrashMode() ? 1. : 0.5);
            m_drive_view.setTrashMode(!m_drive_view.getTrashMode());
            
        });
        m_trash_btn.setAlpha(m_drive_view.getTrashMode() ? 1. : 0.5);
        m_trash_btn.setSize(40, 40);
        m_trash_btn.setTooltip("Display trashed documents");
        addAndMakeVisible(m_trash_btn);
        
    }
    
    void DocumentBrowserView::DriveView::Header::resized()
    {
        const auto bounds = getLocalBounds();
        
        m_folder_bounds = bounds.withRight(getHeight()).reduced(10);
        
        m_trash_btn.setTopRightPosition(getWidth(), 5);
        m_refresh_btn.setTopRightPosition(m_trash_btn.getX(), 5);
        m_create_document_btn.setTopRightPosition(m_refresh_btn.getX(), 5);
        
        int text_width = m_label.getFont().getStringWidth(m_label.getText());
        
        int remaining_width = std::max(0, m_create_document_btn.getX() - m_folder_bounds.getRight());
        
        m_label.setBounds(bounds.withX(m_folder_bounds.getRight() + 5).withWidth(std::min(text_width, remaining_width / 2)));
        
        m_search_bar.setBounds(bounds.withX(m_label.getBounds().getRight())
                               .withRight(m_create_document_btn.getX())
                               .reduced(0, 13));
    }
    
    void DocumentBrowserView::DriveView::Header::paint(juce::Graphics& g)
    {
        auto& lf = KiwiApp::useLookAndFeel();
        const juce::Colour color = lf.getCurrentColourScheme().getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::windowBackground);
        
        g.fillAll(color);
        
        g.drawImage(m_folder_img, m_folder_bounds.toFloat(),
                    juce::RectanglePlacement::yMid, false);
        
        const auto bounds = getLocalBounds();
        
        g.setColour(color.darker(0.3));
        g.drawHorizontalLine(bounds.getBottom() - 1, 0, getWidth());
    }
    
    void DocumentBrowserView::DriveView::Header::mouseDown(juce::MouseEvent const& e)
    {
        m_drive_view.getModel()->backgroundClicked(e);
    }
    
    void DocumentBrowserView::DriveView::Header::textEditorReturnKeyPressed(juce::TextEditor & text_editor)
    {
    }
    
    // ================================================================================ //
    //                                 BROWSER DRIVE VIEW                               //
    // ================================================================================ //
    
    DocumentBrowserView::DriveView::DriveView(DocumentBrowser::Drive& drive) :
    juce::ListBox("document list", this),
    m_drive(drive),
    m_trash_mode(false),
    m_sorter()
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
        
        m_drive.setSort([sorter = m_sorter](DocumentBrowser::Drive::DocumentSession const& l_hs,
                                            DocumentBrowser::Drive::DocumentSession const& r_hs)
        {
            return sorter.compare(l_hs, r_hs);
        });
        
        update();
    }
    
    DocumentBrowserView::DriveView::~DriveView()
    {
        m_drive.removeListener(*this);
    }
    
    bool DocumentBrowserView::DriveView::Comp::compare(DocumentBrowser::Drive::DocumentSession const& l_hs,
                                                       DocumentBrowser::Drive::DocumentSession const& r_hs) const
    {
        bool type_ordered = false;
        
        switch(m_type)
        {
            case SortType::name:
            {
                type_ordered = l_hs.getName() < r_hs.getName();
                break;
            }
            case SortType::author:
            {
                type_ordered = l_hs.getAuthor() < r_hs.getAuthor();
                break;
            }
            case SortType::creationDate:
            {
                type_ordered = l_hs.getCreationDate() < r_hs.getCreationDate();
                break;
            }
        }
        
        bool trash_order = (m_trashed_first && l_hs.isTrashed() > r_hs.isTrashed())
                            || (!m_trashed_first && l_hs.isTrashed() < r_hs.isTrashed());
        
        return trash_order
               || (l_hs.isTrashed() == r_hs.isTrashed() && type_ordered);
    }
    
    void DocumentBrowserView::DriveView::driveChanged()
    {
        update();
    }
    
    int DocumentBrowserView::DriveView::getNumRows()
    {
        int num_rows = 0;
        
        auto const& documents = m_drive.getDocuments();
        
        num_rows = std::count_if(documents.begin(),
                                documents.end(),
                                [trash_mode = m_trash_mode]
                                (std::unique_ptr<DocumentBrowser::Drive::DocumentSession> const& doc)
        {
            return trash_mode ? doc->isTrashed() : !doc->isTrashed();
        });
        
        return num_rows;
    }
    
    void DocumentBrowserView::DriveView::paintListBoxItem(int rowNumber, juce::Graphics& g,
                                                          int width, int height, bool selected)
    {
        ;
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
        m_drive.createNewDocument();
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
    
    DocumentBrowserView::DriveView::SortType DocumentBrowserView::DriveView::getSortType() const
    {
        return m_sorter.m_type;
    }
    
    void DocumentBrowserView::DriveView::setSortType(SortType sort_type)
    {
        m_sorter.m_type = sort_type;
        
        m_drive.setSort([sorter = m_sorter](DocumentBrowser::Drive::DocumentSession const& l_hs,
                                            DocumentBrowser::Drive::DocumentSession const& r_hs)
        {
            return sorter.compare(l_hs, r_hs);
            
        });
    }
    
    bool DocumentBrowserView::DriveView::getTrashMode() const
    {
        return m_trash_mode;
    }
    
    void DocumentBrowserView::DriveView::update()
    {
        updateContent();
        repaint();
    }
    
    std::string DocumentBrowserView::DriveView::createDocumentToolTip(DocumentBrowser::Drive::DocumentSession const& doc)
    {
        std::string tooltip = "name: " + doc.getName() + "\n"
        + "created by: " + doc.getAuthor() + "\n"
        + "created at: " + doc.getCreationDate() + "\n";
        
        if (doc.isTrashed())
        {
            tooltip += "trashed at: " + doc.getTrashedDate();
        }
        
        return tooltip;
    }
    
    juce::Component* DocumentBrowserView::DriveView::refreshComponentForRow(int row, bool selected,
                                                                            juce::Component* c)
    {
        auto const& documents = m_drive.getDocuments();
        
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
        auto& documents = m_drive.getDocuments();
        if(row < documents.size())
        {
            documents[row]->open();
        }
    }
    
    void DocumentBrowserView::DriveView::restoreDocumentForRow(int row)
    {
        auto & documents = m_drive.getDocuments();
        
        if (row < documents.size())
        {
            documents[row]->untrash();
        }
    }
    
    void DocumentBrowserView::DriveView::deleteDocumentForRow(int row)
    {
        auto & documents = m_drive.getDocuments();
        
        if (row < documents.size())
        {
            documents[row]->trash();
        }
    }
    
    void DocumentBrowserView::DriveView::renameDocumentForRow(int row, std::string const& new_name)
    {
        auto& documents = m_drive.getDocuments();
        if(row < documents.size())
        {
            documents[row]->rename(new_name);
        }
    }
    
    void DocumentBrowserView::DriveView::listBoxItemDoubleClicked(int row, juce::MouseEvent const& e)
    {
        openDocument(row);
    }
    
    bool DocumentBrowserView::DriveView::operator==(DocumentBrowser::Drive const& other_drive) const
    {
        return (m_drive == other_drive);
    }
}
