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
            drive_view_uptr->setBounds(0, last_bottom, bounds.getWidth(), drive_view_uptr->getHeight());
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
    m_document(document),
    m_open_btn("open")
    {
        setRepaintsOnMouseActivity(true);

        m_open_btn.setCommand(std::bind(&DocumentBrowser::Drive::DocumentSession::open, &m_document));
        m_open_btn.setSize(30, 20);
        m_open_btn.setTooltip("open this patcher");
        addAndMakeVisible(m_open_btn);
        
        // label setup
        m_name_label.setText(getName(), juce::NotificationType::dontSendNotification);
        m_name_label.setSize(1, 1);
        
        m_name_label.setEditable(false, true, true);
        m_name_label.addListener(this);
        
        addAndMakeVisible(m_name_label);
        
        resized();

    }
    
    uint16_t DocumentBrowserView::DriveView::DocumentSessionView::getSessionPort() const
    {
        return m_document.useDrive().getSessionPort();
    }
    
    std::string DocumentBrowserView::DriveView::DocumentSessionView::getHost() const
    {
        return m_document.useDrive().getHost();
    }
    
    std::string DocumentBrowserView::DriveView::DocumentSessionView::getName() const
    {
        return m_document.getName();
    }
    
    uint64_t DocumentBrowserView::DriveView::DocumentSessionView::getSessionId() const
    {
        return m_document.getSessionId();
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::documentSessionChanged()
    {
        m_name_label.setText(m_document.getName(), juce::NotificationType::dontSendNotification);
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::paint(juce::Graphics& g)
    {
        const auto bounds = getLocalBounds();
        const bool mouseover = isMouseOver();
        const juce::Colour bg_color = juce::Colour(0xDDFFFFFF);
        
        g.setColour(mouseover ? bg_color.brighter(0.1f) : bg_color);
        g.fillAll();
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::resized()
    {
        const auto bounds = getLocalBounds();
        
        m_open_btn.setTopRightPosition(bounds.getWidth() - 5,
                                       bounds.getHeight() * 0.5 - m_open_btn.getHeight() * 0.5);
        
        m_name_label.setBounds(bounds.reduced(5).withRight(m_open_btn.getX() - 5));
    }
    
    void DocumentBrowserView::DriveView::DocumentSessionView::labelTextChanged(juce::Label* label)
    {
        if(label == &m_name_label)
        {
            m_document.rename(m_name_label.getText().toStdString());
        }
    }
    
    bool DocumentBrowserView::DriveView::DocumentSessionView::operator==(DocumentBrowser::Drive::DocumentSession const& other_document) const
    {
        return m_document == other_document;
    }
    
    // ================================================================================ //
    //                                     DRIVE VIEW                                   //
    // ================================================================================ //
    
    DocumentBrowserView::DriveView::DriveView(DocumentBrowser::Drive& drive) :
    m_drive(drive),
    m_create_document_btn("create"),
    m_refresh_btn("refresh")
    {
        m_create_document_btn.setCommand(std::bind(&DocumentBrowser::Drive::createNewDocument, &m_drive));
        m_create_document_btn.setSize(35, 20);
        m_create_document_btn.setTooltip("Create a new patcher on this drive");
        addAndMakeVisible(m_create_document_btn);
        
        m_refresh_btn.setCommand(std::bind(&DocumentBrowser::Drive::refresh, &m_drive));
        m_refresh_btn.setSize(40, 20);
        m_refresh_btn.setTooltip("Refresh Document list");
        addAndMakeVisible(m_refresh_btn);
        
        for(auto& document : m_drive.getDocuments())
        {
            documentAdded(document);
        }
        
        m_drive.addListener(*this);
        
        updateLayout();
    }
    
    DocumentBrowserView::DriveView::~DriveView()
    {
        m_drive.removeListener(*this);
    }
    
    void DocumentBrowserView::DriveView::resized()
    {
        const auto bounds = getLocalBounds();
        
        const juce::Point<int> padding(5, 5);
        juce::Point<int> last_top_left_pos(padding.x, 30);
        
        for(auto& doc_view : m_documents)
        {
            doc_view->setSize(bounds.getWidth() - padding.x * 2, doc_view->getHeight());
        }
        
        m_refresh_btn.setTopRightPosition(getWidth() - padding.x, 5);
        m_create_document_btn.setTopRightPosition(m_refresh_btn.getX() - 5, 5);
    }
    
    void DocumentBrowserView::DriveView::updateLayout()
    {
        const juce::Point<int> padding(5, 5);
        const int doc_width = getWidth() - padding.x * 2;
        const int doc_height = 30;
        int last_bottom = doc_height;
        
        for(auto& doc_view : m_documents)
        {
            doc_view->setBounds(padding.x,
                                last_bottom + padding.y,
                                doc_width,
                                doc_height);
            
            last_bottom = doc_view->getBottom();
        }
        
        setSize(getWidth(), last_bottom + padding.x);
    }
    
    void DocumentBrowserView::DriveView::paint(juce::Graphics& g)
    {
        const juce::Colour color(0xFF3F3B4E);
        
        g.setColour(color);
        g.fillAll();
        
        g.setColour(color.brighter(0.1));
        g.fillRoundedRectangle(getLocalBounds().reduced(2).toFloat(), 0 );
        
        g.setColour(juce::Colours::whitesmoke);
        g.drawFittedText("• " + m_drive.getName(),
                         getLocalBounds().reduced(8, 6).removeFromTop(22),
                         juce::Justification::centredLeft, 1);
        
        g.setColour(color);
        g.drawLine(0, 30, getWidth(), 30, 3);
    }
    
    void DocumentBrowserView::DriveView::documentAdded(DocumentBrowser::Drive::DocumentSession& doc)
    {
        auto doc_view = std::make_unique<DocumentSessionView>(doc);
        
        doc_view->setSize(getWidth(), 50);
        addAndMakeVisible(doc_view.get());
        
        m_documents.emplace_back(std::move(doc_view));
        
        updateLayout();
    }
    
    void DocumentBrowserView::DriveView::documentRemoved(DocumentBrowser::Drive::DocumentSession& doc)
    {
        const auto doc_view_it = std::find_if(m_documents.begin(), m_documents.end(), [&doc](std::unique_ptr<DocumentSessionView> const& document){
            return (*document.get() == doc);
        });
        
        if(doc_view_it != m_documents.end())
        {
            removeChildComponent((*doc_view_it).get());
            
            m_documents.erase(doc_view_it);
            
            updateLayout();
        }
    }
    
    void DocumentBrowserView::DriveView::documentChanged(DocumentBrowser::Drive::DocumentSession& doc)
    {
        const auto doc_view_it = std::find_if(m_documents.begin(), m_documents.end(),
                                              [&doc](std::unique_ptr<DocumentSessionView> const& document) {
            return (*document.get() == doc);
        });
        
        if(doc_view_it != m_documents.end())
        {
            (*doc_view_it)->documentSessionChanged();
        }
    }
    
    bool DocumentBrowserView::DriveView::operator==(DocumentBrowser::Drive const& other_drive) const
    {
        return (m_drive == other_drive);
    }
    
    // ================================================================================ //
    //                               DOCUMENT EXPLORER WINDOW                           //
    // ================================================================================ //
    
    DocumentBrowserWindow::DocumentBrowserWindow(DocumentBrowser& browser) :
    AppWindow("Document Browser")
    {
        setContentOwned(new DocumentBrowserView(browser), true);
        setResizable(true, false);
        setResizeLimits(300, 100, 32000, 32000);
        
        juce::Rectangle<int> screen_area = getParentMonitorArea();
        
        int width = 300;
        int height = 440;
        
        juce::Rectangle<int> bounds(screen_area.getX() + ((15. / 100.) * screen_area.getWidth() - ((float) width / 2.)),
                                    screen_area.getY() + ((30. / 100.) * screen_area.getHeight()),
                                    width,
                                    height);
        
        initBounds(bounds);
        setVisible(true);
    }
    
    bool DocumentBrowserWindow::isMainWindow() const
    {
        return true;
    }
}
