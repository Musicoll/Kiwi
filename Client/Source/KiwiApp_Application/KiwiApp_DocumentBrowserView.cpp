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
#include "../KiwiApp.h"

namespace kiwi
{
    // ================================================================================ //
    //                                  DOCUMENT BROWSER                                //
    // ================================================================================ //
    
    DocumentBrowserView::DocumentBrowserView(DocumentBrowser& browser, bool enabled) :
    m_browser(browser)
    {
        setSize(1, 1);
        
        DocumentBrowser::Drive & drive = *m_browser.getDrive();
        
        auto drive_view = std::make_unique<DriveView>(drive);
        drive_view->setSize(getWidth(), drive_view->getHeight());
        addAndMakeVisible(drive_view.get());
        m_drives.emplace_back(std::move(drive_view));
        
        resized();
        
        setSize(200, 300);
        
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
        
        if (!m_drive_view.getTrashMode())
        {
            m.addItem(1, "Rename");
            m.addItem(2, "Delete");
            m.addItem(10, "Upload");
            m.addItem(11, "Duplicate");
        }
        else
        {
            m.addItem(3, "Restore");
        }
        
        m.addItem(9, "Download");
        
        DriveView::DataType current_sort = m_drive_view.getSortType();
        
        juce::PopupMenu sort_menu;
        sort_menu.addItem(4, "Name",
                          current_sort != DriveView::DataType::name,
                          current_sort == DriveView::DataType::name);
        
        sort_menu.addItem(5, "Created at",
                          current_sort != DriveView::DataType::creationDate,
                          current_sort == DriveView::DataType::creationDate);
        
        sort_menu.addItem(6, "Created by",
                          current_sort != DriveView::DataType::author,
                          current_sort == DriveView::DataType::author);
        
        sort_menu.addItem(7, "Last opened at",
                          current_sort != DriveView::DataType::openedDate,
                          current_sort == DriveView::DataType::openedDate);
        
        sort_menu.addItem(8, "Last opened by",
                          current_sort != DriveView::DataType::openedUser,
                          current_sort == DriveView::DataType::openedUser);
        
        m.addSubMenu("Sort by", sort_menu);
        
        int result = m.show();
        
        switch(result)
        {
            case 1: // rename
            {
                m_name_label.showEditor();
                break;
            }
            case 2: // delete
            {
                m_drive_view.deleteDocumentForRow(m_row);
                break;
            }
            case 3: // restore
            {
                m_drive_view.restoreDocumentForRow(m_row);
                break;
            }
            case 4: // sort by name
            {
                m_drive_view.setSortType(DriveView::DataType::name);
                break;
            }
            case 5: // sort by creation date
            {
                m_drive_view.setSortType(DriveView::DataType::creationDate);
                break;
            }
            case 6: // sort by author
            {
                m_drive_view.setSortType(DriveView::DataType::author);
                break;
            }
            case 7:
            {
                m_drive_view.setSortType(DriveView::DataType::openedDate);
                break;
            }
            case 8:
            {
                m_drive_view.setSortType(DriveView::DataType::openedUser);
                break;
            }
            case 9: // download document
            {
                m_drive_view.downloadDocumentForRow(m_row);
                break;
            }
            case 10: // upload document
            {
                m_drive_view.uploadDocument();
                break;
            }
            case 11: // duplicate document
            {
                m_drive_view.duplicateDocumentForRow(m_row);
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
    m_folder_bounds(),
    m_label("drive_name", m_drive_view.getDriveName()),
    m_folder_img(juce::ImageCache::getFromMemory(binary_data::images::folder_png,
                                                 binary_data::images::folder_png_size)),
    m_disable_folder_img(m_folder_img)
    {
        m_folder_img.duplicateIfShared();
        m_disable_folder_img.multiplyAllAlphas(0.5);
        
        m_label.setFont(m_label.getFont().withHeight(20));
        m_label.setEditable(false);
        m_label.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
        addAndMakeVisible(m_label);
        
        m_create_document_btn.setCommand([this]()
        {
            if (!m_drive_view.getTrashMode())
                m_drive_view.createDocument();
        });
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
            bool new_trash_mode = !m_drive_view.getTrashMode();
            m_drive_view.setTrashMode(new_trash_mode);
            m_trash_btn.setAlpha(new_trash_mode ? 1. : 0.5);
            m_create_document_btn.setAlpha(new_trash_mode ? 0.5 : 1.);
        });
        m_trash_btn.setAlpha(m_drive_view.getTrashMode() ? 1. : 0.5);
        m_trash_btn.setSize(40, 40);
        m_trash_btn.setTooltip("Display trashed documents");
        addAndMakeVisible(m_trash_btn);
        
    }
    
    void DocumentBrowserView::DriveView::Header::enablementChanged()
    {
        if (isEnabled())
        {
            m_refresh_btn.setCommand([this](){m_drive_view.refresh();});
            m_refresh_btn.setAlpha(1);
            
            m_create_document_btn.setCommand([this]()
                                             {
                                                 if (!m_drive_view.getTrashMode())
                                                     m_drive_view.createDocument();
                                             });
            m_create_document_btn.setAlpha(1);
        }
        else
        {
            m_refresh_btn.setCommand([](){});
            m_refresh_btn.setAlpha(0.5);
            
            m_create_document_btn.setCommand([](){});
            m_create_document_btn.setAlpha(0.5);
        }
    }
    
    void DocumentBrowserView::DriveView::Header::resized()
    {
        const auto bounds = getLocalBounds();
        
        m_folder_bounds = bounds.withRight(getHeight()).reduced(10);
        
        m_trash_btn.setTopRightPosition(getWidth(), 5);
        m_refresh_btn.setTopRightPosition(m_trash_btn.getX(), 5);
        m_create_document_btn.setTopRightPosition(m_refresh_btn.getX(), 5);
        
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
    
    // ================================================================================ //
    //                                 BROWSER DRIVE VIEW                               //
    // ================================================================================ //
    
    DocumentBrowserView::DriveView::DriveView(DocumentBrowser::Drive& drive) :
    juce::ListBox("document list", this),
    m_drive(drive),
    m_trash_mode(false),
    m_enabled(true),
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
            case DataType::name:
            {
                type_ordered = l_hs.getName() < r_hs.getName();
                break;
            }
            case DataType::author:
            {
                type_ordered = l_hs.getAuthor() < r_hs.getAuthor();
                break;
            }
            case DataType::creationDate:
            {
                type_ordered = l_hs.getCreationDate() > r_hs.getCreationDate();
                break;
            }
            case DataType::openedDate:
            {
                type_ordered = l_hs.getOpenedDate() > r_hs.getOpenedDate();
                break;
            }
            case DataType::openedUser:
            {
                type_ordered = l_hs.getOpenedUser() < r_hs.getOpenedUser();
                break;
            }
        }
        
        bool trash_order = (m_trashed_first && l_hs.isTrashed() > r_hs.isTrashed())
                            || (!m_trashed_first && l_hs.isTrashed() < r_hs.isTrashed());
        
        return trash_order
               || (l_hs.isTrashed() == r_hs.isTrashed() && type_ordered);
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
    
    int DocumentBrowserView::DriveView::getNumRows()
    {
        int num_rows = 0;
        
        if (isEnabled())
        {
            auto const& documents = m_drive.getDocuments();
            
            num_rows = std::count_if(documents.begin(),
                                     documents.end(),
                                     [trash_mode = m_trash_mode]
                                     (std::unique_ptr<DocumentBrowser::Drive::DocumentSession> const& doc)
                                     {
                                         return trash_mode ? doc->isTrashed() : !doc->isTrashed();
                                     });
        }
        
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
    
    DocumentBrowserView::DriveView::DataType DocumentBrowserView::DriveView::getSortType() const
    {
        return m_sorter.m_type;
    }
    
    void DocumentBrowserView::DriveView::setSortType(DataType sort_type)
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
        dynamic_cast<Header*>(getHeaderComponent())->setText(getDriveName());
        updateContent();
        repaint();
    }
    
    std::string DocumentBrowserView::DriveView::createDocumentToolTip(DocumentBrowser::Drive::DocumentSession const& doc)
    {
        std::string tooltip = "name: " + doc.getName() + "\n"
        + "created by : " + doc.getAuthor() + "\n"
        + "created at : " + doc.getCreationDate() + "\n"
        + "last opened at : " + doc.getOpenedDate() + "\n"
        + "last opened by : " + doc.getOpenedUser();
        
        if (doc.isTrashed())
        {
            tooltip += "\ntrashed at : " + doc.getTrashedDate();
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
    
    void DocumentBrowserView::DriveView::duplicateDocumentForRow(int row)
    {
        auto& documents = m_drive.getDocuments();
        
        if(row < documents.size())
        {
            documents[row]->duplicate();
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
        auto& document = m_drive.getDocuments()[row];
        
        auto directory = juce::File::getSpecialLocation(juce::File::userHomeDirectory);
        
        juce::File suggest_file =
        directory.getChildFile(juce::String(document->getName())).withFileExtension("kiwi");
        
        juce::FileChooser saveFileChooser("Download file", suggest_file, "*.kiwi");
        
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
