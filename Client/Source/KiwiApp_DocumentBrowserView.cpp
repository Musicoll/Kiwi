/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
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
    
    DocumentBrowserView::DriveView::DocumentSessionView::DocumentSessionView(juce::ListBox& listbox,
                                                                             DocumentBrowser::Drive::DocumentSession& document) :
    m_listbox(listbox),
    m_document(&document),
    m_open_btn(std::make_unique<ImageButton>("open", std::unique_ptr<juce::Drawable>(juce::Drawable::createFromImageData(binary_data::images::open_png, binary_data::images::open_png_size)),
                                             ImageButton::ButtonStyle::ImageFitted)),
    m_kiwi_filetype_img(juce::ImageCache::getFromMemory(binary_data::images::kiwi_filetype_png,
                                                        binary_data::images::kiwi_filetype_png_size))
    {
        addMouseListener(this, true);
        
        m_open_btn->setCommand(std::bind(&DocumentBrowser::Drive::DocumentSession::open, m_document));
        m_open_btn->setSize(40, 40);
        m_open_btn->setTooltip("open this patcher");
        addChildComponent(*m_open_btn);
        
        // label setup
        m_name_label.setText(getName(), juce::NotificationType::dontSendNotification);
        m_name_label.setSize(1, 1);
        m_name_label.setEditable(false, true, true);
        m_name_label.addListener(this);
        m_name_label.setInterceptsMouseClicks(false, false);
        addAndMakeVisible(m_name_label);
    }
    
    DocumentBrowserView::DriveView::DocumentSessionView::~DocumentSessionView()
    {
        removeMouseListener(this);
    }
    
    uint16_t DocumentBrowserView::DriveView::DocumentSessionView::getSessionPort() const
    {
        return m_document ? m_document->useDrive().getSessionPort() : 0;
    }
    
    std::string DocumentBrowserView::DriveView::DocumentSessionView::getHost() const
    {
        return m_document ? m_document->useDrive().getHost() : "";
    }
    
    std::string DocumentBrowserView::DriveView::DocumentSessionView::getName() const
    {
        return m_document ? m_document->getName() : "";
    }
    
    uint64_t DocumentBrowserView::DriveView::DocumentSessionView::getSessionId() const
    {
        return m_document ? m_document->getSessionId() : 0;
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::documentSessionChanged()
    {
        if(m_document)
        {
            m_name_label.setText(m_document->getName(), juce::NotificationType::dontSendNotification);
        }
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::update(DocumentBrowser::Drive::DocumentSession* doc, int row, bool now_selected)
    {
        m_document = doc;
        m_row = row;
        
        if(m_document != nullptr)
        {
            m_open_btn->setCommand(std::bind(&DocumentBrowser::Drive::DocumentSession::open, m_document));
            m_name_label.setText(getName(), juce::NotificationType::dontSendNotification);
        }
        
        if(m_selected != now_selected)
        {
            m_selected = now_selected;
            repaint();
        }
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::paint(juce::Graphics& g)
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
    
    void DocumentBrowserView::DriveView::DocumentSessionView::resized()
    {
        const auto bounds = getLocalBounds();
        m_open_btn->setBounds(bounds.reduced(5).withLeft(bounds.getWidth() - 40));
        m_name_label.setBounds(bounds.reduced(5).withRight(m_open_btn->getX() - 5).withLeft(40));
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::mouseEnter(juce::MouseEvent const& e)
    {
        m_mouseover = true;
        m_open_btn->setVisible(true);
        repaint();
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::mouseExit(juce::MouseEvent const& e)
    {
        m_mouseover = false;
        m_open_btn->setVisible(false);
        repaint();
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::mouseDown(juce::MouseEvent const& e)
    {
        m_select_row_on_mouse_up = false;
        
        if(isEnabled())
        {
            if(!m_selected)
            {
                m_listbox.selectRowsBasedOnModifierKeys(m_row, e.mods, false);
            }
            else
            {
                m_select_row_on_mouse_up = true;
            }
        }
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::mouseUp(juce::MouseEvent const& e)
    {
        if(m_select_row_on_mouse_up && e.mouseWasClicked() && isEnabled())
        {
            m_listbox.selectRowsBasedOnModifierKeys(m_row, e.mods, true);
        }
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::mouseDoubleClick(juce::MouseEvent const& event)
    {
        if(m_document)
        {
            m_document->open();
        }
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::labelTextChanged(juce::Label* label)
    {
        if(m_document && label == &m_name_label)
        {
            m_document->rename(m_name_label.getText().toStdString());
        }
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::showEditor()
    {
        m_name_label.showEditor();
    }
    
    bool DocumentBrowserView::DriveView::DocumentSessionView::operator==(DocumentBrowser::Drive::DocumentSession const& other_document) const
    {
        return *m_document == other_document;
    }
    
    // ================================================================================ //
    //                            DOCUMENT BROWSER PANEL HEADER                         //
    // ================================================================================ //
    
    DocumentBrowserView::DriveView::Header::Header(juce::ListBox& listbox, DocumentBrowser::Drive& drive) :
    m_listbox(listbox),
    m_drive(drive),
    m_refresh_btn(std::make_unique<ImageButton>("refresh", std::unique_ptr<juce::Drawable>(juce::Drawable::createFromImageData(binary_data::images::refresh_png, binary_data::images::refresh_png_size)))),
    m_create_document_btn(std::make_unique<ImageButton>("create", std::unique_ptr<juce::Drawable>(juce::Drawable::createFromImageData(binary_data::images::plus_png, binary_data::images::plus_png_size)))),
    m_folder_img(juce::ImageCache::getFromMemory(binary_data::images::folder_png,
                                                 binary_data::images::folder_png_size))
    {
        m_create_document_btn->setCommand(std::bind(&DocumentBrowser::Drive::createNewDocument, &m_drive));
        m_create_document_btn->setSize(40, 40);
        m_create_document_btn->setTooltip("Create a new patcher on this drive");
        m_create_document_btn->setColour(ImageButton::ColourIds::textColourId, juce::Colours::whitesmoke);
        addAndMakeVisible(*m_create_document_btn);
        
        m_refresh_btn->setCommand(std::bind(&DocumentBrowser::Drive::refresh, &m_drive));
        m_refresh_btn->setSize(40, 40);
        m_refresh_btn->setTooltip("Refresh Document list");
        m_refresh_btn->setColour(ImageButton::ColourIds::textColourId, juce::Colours::whitesmoke);
        addAndMakeVisible(*m_refresh_btn);
    }
    
    void DocumentBrowserView::DriveView::Header::resized()
    {
         m_refresh_btn->setTopRightPosition(getWidth(), 5);
         m_create_document_btn->setTopRightPosition(m_refresh_btn->getX(), 5);
    }
    
    void DocumentBrowserView::DriveView::Header::paint(juce::Graphics& g)
    {
        const juce::Colour color(0xFF4E4E4E);
        g.fillAll(color);
        
        const auto bounds = getLocalBounds();
        
        const auto folder_img_bounds = bounds.withRight(getHeight()).reduced(10);
        
        g.drawImage(m_folder_img, folder_img_bounds.toFloat(),
                    juce::RectanglePlacement::yMid, false);
        
        g.setColour(juce::Colours::whitesmoke);
        g.drawFittedText(m_drive.getName(),
                         bounds
                         .withLeft(folder_img_bounds.getRight() + 10)
                         .withRight(m_create_document_btn->getX() - 5),
                         juce::Justification::centredLeft, 1);
        
        g.setColour(color.darker(0.3));
        g.drawHorizontalLine(bounds.getBottom() - 1, 0, getWidth());
    }
    
    void DocumentBrowserView::DriveView::Header::mouseDown(juce::MouseEvent const& e)
    {
        m_listbox.getModel()->backgroundClicked(e);
    }
    
    // ================================================================================ //
    //                                     DRIVE VIEW                                   //
    // ================================================================================ //
    
    DocumentBrowserView::DriveView::DriveView(DocumentBrowser::Drive& drive) :
    m_drive(drive),
    m_document_list("document list", this)
    {
        m_drive.addListener(*this);
        
        m_document_list.setMultipleSelectionEnabled(false);
        m_document_list.setRowSelectedOnMouseDown(true);
        m_document_list.setRowHeight(40);
        
        auto* header = new Header(m_document_list, m_drive);
        header->setSize(getWidth(), 50);
        m_document_list.setHeaderComponent(header);
        
        addAndMakeVisible(m_document_list);
        
        m_document_list.getViewport()->setScrollBarThickness(10);
        m_document_list.setColour(juce::ListBox::backgroundColourId, juce::Colour(0xFFD4D4D4));
        
        updateLayout();
    }
    
    DocumentBrowserView::DriveView::~DriveView()
    {
        m_drive.removeListener(*this);
    }
    
    void DocumentBrowserView::DriveView::resized()
    {
        m_document_list.setBounds(getLocalBounds());
    }
    
    void DocumentBrowserView::DriveView::updateLayout()
    {
        m_document_list.updateContent();
    }
    
    void DocumentBrowserView::DriveView::paint(juce::Graphics& g)
    {
        ;
    }
    
    void DocumentBrowserView::DriveView::driveChanged()
    {
        std::cout << "driveChanged" << '\n';
        updateLayout();
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
                                                                            juce::Component* component_to_update)
    {
        std::cout << "refreshComponentForRow " << row << '\n';
        auto const& documents = m_drive.getDocuments();
        
        if(row >= documents.size())
        {
            component_to_update = nullptr;
        }
        else if(component_to_update == nullptr)
        {
            component_to_update = new DocumentSessionView(m_document_list, *documents[row]);
            std::cout << "component created for row: " << row << std::endl;
        }
        
        if(component_to_update != nullptr)
        {
            static_cast<DocumentSessionView*>(component_to_update)->update(documents[row].get(), row, selected);
        }

        return component_to_update;
    }
    
    void DocumentBrowserView::DriveView::backgroundClicked(juce::MouseEvent const&)
    {
        m_document_list.deselectAllRows();
    }
    
    void DocumentBrowserView::DriveView::returnKeyPressed(int last_row_selected)
    {
        if(m_document_list.getNumSelectedRows() > 0)
        {
            auto* c = m_document_list.getComponentForRowNumber(last_row_selected);
            if(c)
            {
                static_cast<DocumentSessionView*>(c)->showEditor();
            }
        }
    }
    
    bool DocumentBrowserView::DriveView::operator==(DocumentBrowser::Drive const& other_drive) const
    {
        return (m_drive == other_drive);
    }
}
