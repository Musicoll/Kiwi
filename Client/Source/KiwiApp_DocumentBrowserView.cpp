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
            //drive_view_uptr->setBounds(0, last_bottom, bounds.getWidth(), drive_view_uptr->getHeight());
            
            drive_view_uptr->setBounds(bounds);
            last_bottom = drive_view_uptr->getBottom();
        }
    }
    
    void DocumentBrowserView::paint(juce::Graphics& g)
    {
        const juce::Colour color(0xFF3F3B4E);
        
        g.setColour(color);
        g.fillAll();
    }
    
    // ================================================================================ //
    //                                  BROWSER BUTTON                                  //
    // ================================================================================ //
    
    DocumentBrowserView::DriveView::BrowserButton::BrowserButton(juce::String const& button_text) :
    juce::Button(button_text)
    {
        setSize(30, 30);
    }
    
    void DocumentBrowserView::DriveView::BrowserButton::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
    {
        const auto bounds = getLocalBounds();
        
        const juce::Colour bgcolor = isMouseOverButton ? juce::Colour(0xFFE0CA3C) : juce::Colour(0xFF5A54C4);
        g.setColour(bgcolor);
        
        g.fillRoundedRectangle(bounds.reduced(1).toFloat(), 0);
        
        g.setColour(bgcolor.darker(0.8));
        g.drawRoundedRectangle(bounds.reduced(1).toFloat(), 0, 1);
        
        const juce::Colour text_color = isMouseOverButton ? juce::Colour(0xFF000000) : juce::Colour(0xFFFFFFFF);
        g.setColour(text_color);
        g.setFont(12);
        g.drawFittedText(getButtonText(), bounds, juce::Justification::centred, 2);
    }
    
    void DocumentBrowserView::DriveView::BrowserButton::setCommand(std::function<void(void)> fn)
    {
        m_command = fn;
    }
    
    void DocumentBrowserView::DriveView::BrowserButton::clicked(juce::ModifierKeys const& modifiers)
    {
        if(m_command) { m_command(); }
    }
    
    // ================================================================================ //
    //                               DOCUMENT SESSION VIEW                              //
    // ================================================================================ //
    
    DocumentBrowserView::DriveView::DocumentSessionView::DocumentSessionView(DocumentBrowser::Drive::DocumentSession& document) :
    m_document(&document),
    m_open_btn("open")
    {
        m_open_btn.setCommand(std::bind(&DocumentBrowser::Drive::DocumentSession::open, m_document));
        m_open_btn.setSize(30, 20);
        m_open_btn.setTooltip("open this patcher");
        addAndMakeVisible(m_open_btn);
        
        // label setup
        m_name_label.setText(getName(), juce::NotificationType::dontSendNotification);
        m_name_label.setSize(1, 1);
        
        m_name_label.setEditable(false, true, true);
        m_name_label.addListener(this);
        
        m_name_label.setInterceptsMouseClicks(false, false);
        
        addAndMakeVisible(m_name_label);
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
        if(m_selected != now_selected)
        {
            m_selected = now_selected;
            repaint();
        }
        
        if(m_document != doc)
        {
            m_document = doc;
            
            if(m_document != nullptr)
            {
                m_open_btn.setCommand(std::bind(&DocumentBrowser::Drive::DocumentSession::open, m_document));
            }
        }
        
        if(m_document)
        {
            m_name_label.setText(getName(), juce::NotificationType::dontSendNotification);
        }
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::paint(juce::Graphics& g)
    {
        const auto bounds = getLocalBounds();
        const juce::Colour bg_color = juce::Colour(0xDDFFFFFF);
        
        g.setColour(m_mouseover ? bg_color.darker(0.1f) : bg_color);
        g.fillAll();
        
        g.setColour(juce::Colours::grey);
        g.fillRect(0, 0, 5, getHeight());
        
        g.setColour(bg_color.darker(0.5f));
        g.fillRect(0, getBottom() - 1, getWidth(), getBottom());
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::resized()
    {
        const auto bounds = getLocalBounds();
        
        m_open_btn.setTopRightPosition(bounds.getWidth() - 5,
                                       bounds.getHeight() * 0.5 - m_open_btn.getHeight() * 0.5);
        
        m_name_label.setBounds(bounds.reduced(5).withRight(m_open_btn.getX() - 5));
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::mouseEnter(juce::MouseEvent const& event)
    {
        m_mouseover = true;
        repaint();
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::mouseExit(juce::MouseEvent const& event)
    {
        m_mouseover = false;
        repaint();
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::mouseDoubleClick(juce::MouseEvent const& event)
    {
        m_name_label.showEditor();
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::labelTextChanged(juce::Label* label)
    {
        if(m_document && label == &m_name_label)
        {
            m_document->rename(m_name_label.getText().toStdString());
        }
    }
    
    bool DocumentBrowserView::DriveView::DocumentSessionView::operator==(DocumentBrowser::Drive::DocumentSession const& other_document) const
    {
        return *m_document == other_document;
    }
    
    // ================================================================================ //
    //                            DOCUMENT BROWSER PANEL HEADER                         //
    // ================================================================================ //
    
    DocumentBrowserView::DriveView::Header::Header(DocumentBrowser::Drive& drive) :
    m_drive(drive),
    m_refresh_btn(std::make_unique<BrowserButton>("refresh")),
    m_create_document_btn(std::make_unique<BrowserButton>("create"))
    {
        m_create_document_btn->setCommand(std::bind(&DocumentBrowser::Drive::createNewDocument, &m_drive));
        m_create_document_btn->setSize(35, 20);
        m_create_document_btn->setTooltip("Create a new patcher on this drive");
        addAndMakeVisible(*m_create_document_btn);
        
        m_refresh_btn->setCommand(std::bind(&DocumentBrowser::Drive::refresh, &m_drive));
        m_refresh_btn->setSize(40, 20);
        m_refresh_btn->setTooltip("Refresh Document list");
        addAndMakeVisible(*m_refresh_btn);
    }
    
    void DocumentBrowserView::DriveView::Header::resized()
    {
        const auto bounds = getLocalBounds();
        
         const juce::Point<int> padding(5, 5);
         juce::Point<int> last_top_left_pos(padding.x, 30);
         
         m_refresh_btn->setTopRightPosition(getWidth() - padding.x, 5);
         m_create_document_btn->setTopRightPosition(m_refresh_btn->getX() - 5, 5);
    }
    
    void DocumentBrowserView::DriveView::Header::paint(juce::Graphics& g)
    {
        g.fillAll(juce::Colour(0xFF4E4E4E));
        
        g.setColour(juce::Colours::whitesmoke);
        g.drawFittedText("• " + m_drive.getName(),
                         getLocalBounds().reduced(8, 6).removeFromTop(22),
                         juce::Justification::centredLeft, 1);
        
        const juce::Colour color(0xFF3F3B4E);
        g.setColour(color);
        g.drawLine(0, 30, getWidth(), 30, 3);
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
        m_document_list.setRowHeight(30);
        
        auto* header = new Header(m_drive);
        header->setSize(getWidth(), 30);
        m_document_list.setHeaderComponent(header);
        
        addAndMakeVisible(m_document_list);
        
        m_document_list.getViewport()->setScrollBarThickness(10);
        m_document_list.setColour(juce::ListBox::backgroundColourId, juce::Colour(0xFFD4D4D4));
        
        setSize(300, 200);
        
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
        auto const& documents = m_drive.getDocuments();
        if(row < documents.size() && component_to_update == nullptr)
        {
            component_to_update = new DocumentSessionView(*documents[row]);
            std::cout << "component created for row: " << row << std::endl;
        }
        
        if(component_to_update != nullptr)
        {
            static_cast<DocumentSessionView*>(component_to_update)->update(documents[row].get(), row, selected);
        }

        return component_to_update;
    }
    
    bool DocumentBrowserView::DriveView::operator==(DocumentBrowser::Drive const& other_drive) const
    {
        return (m_drive == other_drive);
    }
}
