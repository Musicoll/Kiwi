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

#include <KiwiEngine/KiwiEngine_Console.hpp>

#include "KiwiApp_DocumentBrowserView.hpp"
#include "KiwiApp_Instance.hpp"

#include "KiwiApp_BinaryData.hpp"
#include "KiwiApp.hpp"

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
    
    DocumentBrowserView::DriveView::RowElem::RowElem(DriveView& drive_view, std::string const& name) :
    m_drive_view(drive_view),
    m_name(name),
    m_open_btn("open", std::unique_ptr<juce::Drawable>(juce::Drawable::createFromImageData(binary_data::images::open_png, binary_data::images::open_png_size)), ImageButton::ButtonStyle::ImageFitted),
    m_kiwi_filetype_img(juce::ImageCache::getFromMemory(binary_data::images::kiwi_filetype_png,
                                                        binary_data::images::kiwi_filetype_png_size)),
    m_row(-1)
    {
        addMouseListener(this, true);
        
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
    
    void DocumentBrowserView::DriveView::RowElem::update(std::string const& name, int row, bool now_selected)
    {
        if(row != m_row)
        {
            m_row = row;
            m_open_btn.setCommand(std::bind(&DriveView::openDocument, &m_drive_view, m_row));
        }
        
        if(name != m_name)
        {
            m_name = name;
            m_name_label.setText(m_name, juce::NotificationType::dontSendNotification);
        }
        
        if(m_selected != now_selected)
        {
            m_selected = now_selected;
            m_open_btn.setVisible(m_selected);
            repaint();
        }
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
    
    void DocumentBrowserView::DriveView::RowElem::mouseDown(juce::MouseEvent const& e)
    {
        m_select_row_on_mouse_up = false;
        
        if(isEnabled())
        {
            if(!m_selected)
            {
                m_drive_view.selectRowsBasedOnModifierKeys(m_row, e.mods, false);
            }
            else
            {
                m_select_row_on_mouse_up = true;
            }
        }
    }
    
    void DocumentBrowserView::DriveView::RowElem::mouseUp(juce::MouseEvent const& e)
    {
        if(m_select_row_on_mouse_up && e.mouseWasClicked() && isEnabled())
        {
            m_drive_view.selectRowsBasedOnModifierKeys(m_row, e.mods, true);
        }
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
        }
    }
    
    void DocumentBrowserView::DriveView::RowElem::showEditor()
    {
        m_name_label.showEditor();
    }
    
    // ================================================================================ //
    //                             BROWSER DRIVE VIEW HEADER                            //
    // ================================================================================ //
    
    DocumentBrowserView::DriveView::Header::Header(juce::ListBox& listbox, DocumentBrowser::Drive& drive) :
    m_listbox(listbox),
    m_drive(drive),
    m_refresh_btn("refresh", std::unique_ptr<juce::Drawable>(juce::Drawable::createFromImageData(binary_data::images::refresh_png, binary_data::images::refresh_png_size))),
    m_create_document_btn("create", std::unique_ptr<juce::Drawable>(juce::Drawable::createFromImageData(binary_data::images::plus_png, binary_data::images::plus_png_size))),
    m_folder_img(juce::ImageCache::getFromMemory(binary_data::images::folder_png,
                                                 binary_data::images::folder_png_size))
    {
        m_create_document_btn.setCommand(std::bind(&DocumentBrowser::Drive::createNewDocument, &m_drive));
        m_create_document_btn.setSize(40, 40);
        m_create_document_btn.setTooltip("Create a new patcher on this drive");
        m_create_document_btn.setColour(ImageButton::ColourIds::textColourId, juce::Colours::whitesmoke);
        addAndMakeVisible(m_create_document_btn);
        
        m_refresh_btn.setCommand(std::bind(&DocumentBrowser::Drive::refresh, &m_drive));
        m_refresh_btn.setSize(40, 40);
        m_refresh_btn.setTooltip("Refresh Document list");
        m_refresh_btn.setColour(ImageButton::ColourIds::textColourId, juce::Colours::whitesmoke);
        addAndMakeVisible(m_refresh_btn);
    }
    
    void DocumentBrowserView::DriveView::Header::resized()
    {
         m_refresh_btn.setTopRightPosition(getWidth(), 5);
         m_create_document_btn.setTopRightPosition(m_refresh_btn.getX(), 5);
    }
    
    void DocumentBrowserView::DriveView::Header::paint(juce::Graphics& g)
    {
        auto& lf = KiwiApp::useLookAndFeel();
        const juce::Colour color = lf.getCurrentColourScheme().getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::windowBackground);
        
        g.fillAll(color);
        
        const auto bounds = getLocalBounds();
        
        const auto folder_img_bounds = bounds.withRight(getHeight()).reduced(10);
        
        g.drawImage(m_folder_img, folder_img_bounds.toFloat(),
                    juce::RectanglePlacement::yMid, false);
        
        g.setColour(juce::Colours::whitesmoke);
        g.drawFittedText(m_drive.getName(),
                         bounds
                         .withLeft(folder_img_bounds.getRight() + 10)
                         .withRight(m_create_document_btn.getX() - 5),
                         juce::Justification::centredLeft, 1);
        
        g.setColour(color.darker(0.3));
        g.drawHorizontalLine(bounds.getBottom() - 1, 0, getWidth());
    }
    
    void DocumentBrowserView::DriveView::Header::mouseDown(juce::MouseEvent const& e)
    {
        m_listbox.getModel()->backgroundClicked(e);
    }
    
    // ================================================================================ //
    //                                 BROWSER DRIVE VIEW                               //
    // ================================================================================ //
    
    DocumentBrowserView::DriveView::DriveView(DocumentBrowser::Drive& drive) :
    juce::ListBox("document list", this),
    m_drive(drive)
    {
        m_drive.addListener(*this);
        
        setMultipleSelectionEnabled(false);
        setRowSelectedOnMouseDown(true);
        setRowHeight(40);
        
        auto* header = new Header(*this, m_drive);
        header->setSize(getWidth(), 50);
        setHeaderComponent(header);
        
        getViewport()->setScrollBarThickness(10);
        juce::ListBox::setColour(juce::ListBox::backgroundColourId, juce::Colour(0xFFD4D4D4));
        
        updateContent();
    }
    
    DocumentBrowserView::DriveView::~DriveView()
    {
        m_drive.removeListener(*this);
    }
    
    void DocumentBrowserView::DriveView::driveChanged()
    {
        updateContent();
    }
    
    int DocumentBrowserView::DriveView::getNumRows()
    {
        return m_drive.getDocuments().size();
    }
    
    void DocumentBrowserView::DriveView::paintListBoxItem(int rowNumber, juce::Graphics& g,
                                                          int width, int height, bool selected)
    {
        ;
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
                c = new RowElem(*this, documents[row]->getName());
            }
            
            static_cast<RowElem*>(c)->update(documents[row]->getName(), row, selected);
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
