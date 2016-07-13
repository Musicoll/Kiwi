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

#ifndef KIWI_APP_PATCHERVIEW_HPP_INCLUDED
#define KIWI_APP_PATCHERVIEW_HPP_INCLUDED

#include <KiwiModel/KiwiModelPatcher.hpp>

#include "flip/DocumentObserver.h"

#include "../JuceLibraryCode/JuceHeader.h"

namespace kiwi
{
    class PatcherManager;
    class ObjectView;
    class ClassicBox;
    class LinkView;
    class LinkViewCreator;
    class Instance;
    class HitTester;
    class PatcherViewport;
    class IoletHighlighter;
    class Lasso;
    
    // ================================================================================ //
    //                                    PATCHER VIEW                                  //
    // ================================================================================ //
    
    //! @brief The juce Patcher Component.
    class PatcherView : public juce::Component, public juce::ApplicationCommandTarget
    {
    public:
        
        //! @brief Constructor
        PatcherView(PatcherManager& manager,
                    Instance& instance,
                    model::Patcher& patcher,
                    model::Patcher::View& view);
        
        //! @brief Destructor
        ~PatcherView();
        
        using ObjectViews = std::vector<std::unique_ptr<ObjectView>>;
        using LinkViews = std::vector<std::unique_ptr<LinkView>>;
        
        //! @internal flip::DocumentObserver<model::Patcher>::document_changed
        void patcherChanged(model::Patcher& patcher, model::Patcher::View& view);
        
        //! @brief Returns the patcher view model.
        model::Patcher::View& getPatcherViewModel();
        
        //! @brief Returns the ObjectViews.
        ObjectViews const& getObjects() const;
        
        //! @brief Returns the LinkViews.
        LinkViews const& getLinks() const;
        
        //! @brief Returns the ObjectView corresponding to a given Object model.
        ObjectView* getObject(model::Object const& object);
        
        //! @brief Returns the LinkView corresponding to a given Link model.
        LinkView* getLink(model::Link const& link);
        
        //! @brief Set the lock status of the patcher view.
        void setLock(bool locked);
        
        //! @brief Get the lock status of the patcher view.
        bool isLocked() const;
        
        //! @brief Returns true if the object is selected.
        bool isSelected(ObjectView const& object) const;
        
        //! @brief Returns true if the link is selected.
        bool isSelected(LinkView const& link) const;
        
        //! @brief Returns the Viewport that contains this patcher view.
        //! @details You must use this method if you want to add this component into an other one.
        PatcherViewport& getViewport() { return *m_viewport.get(); }
        
        //! @brief Returns the position of the patcher origin relative to the component position.
        juce::Point<int> getOriginPosition() const;
        
        //! @brief Create an object model. 
        model::Object& createObjectModel(std::string const& text);
        
        //! @brief called by ClassicBox when hmmm.. the text has been edited.
        void boxHasBeenEdited(ClassicBox& box, std::string new_text);
        
        //! @internal Update the patcher window title.
        void updateWindowTitle() const;
        
        // ================================================================================ //
        //                                    COMPONENT                                     //
        // ================================================================================ //
        
        void paint(juce::Graphics& g) override;
        void mouseDown(juce::MouseEvent const& event) override;
        void mouseDrag(juce::MouseEvent const& e) override;
        void mouseUp(juce::MouseEvent const& e) override;
        void mouseMove(juce::MouseEvent const& event) override;
        void mouseDoubleClick(const juce::MouseEvent& event) override;
        bool keyPressed(const juce::KeyPress& key) override;
        
        // ================================================================================ //
        //                              APPLICATION COMMAND TARGET                          //
        // ================================================================================ //
        
        juce::ApplicationCommandTarget* getNextCommandTarget() override;
        void getAllCommands(juce::Array<juce::CommandID>& commands) override;
        void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
        bool perform(const InvocationInfo& info) override;
        
    private: // methods
        
        //! @brief Called internally when the origin of the patcher view changed.
        void originPositionChanged();
        
        // ================================================================================ //
        //                                  MODEL OBSERVER                                  //
        // ================================================================================ //
        
        //! @brief Notify PatcherView that selection has changed.
        void selectionChanged();
        
        //! @brief Load object and links.
        void loadPatcher();
        
        //! @brief Check patcher view information changes (lock_status...).
        void checkViewInfos(model::Patcher::View& view);
        
        //! @brief Check the patcher objects selection changes.
        void checkObjectsSelectionChanges(model::Patcher& patcher);
        
        //! @brief Check the patcher objects selection changes.
        void checkLinksSelectionChanges(model::Patcher& patcher);
        
        //! @internal Object model has just been added to the document.
        void addObjectView(model::Object& object, int zorder = -1);
        
        //! @internal Object model is resident and internal value changed.
        void objectChanged(model::Patcher::View& view, model::Object& object);
        
        //! @internal Object model will be removed from the document.
        void removeObjectView(model::Object& object);
        
        //! @internal Link model has just been added to the document.
        void addLinkView(model::Link& link);
        
        //! @internal Link model is resident and internal value changed.
        void linkChanged(model::Link& link);
        
        //! @internal Link model will be removed from the document.
        void removeLinkView(model::Link& link);
        
        //! @brief Add a new Object to the model at a given position.
        void createObjectModel(std::string const& text, double pos_x, double pos_y);
        
        //! @brief Add a newbox Object at current mouse position and give it focus.
        void createNewBoxModel(bool give_focus);
        
        // ================================================================================ //
        //                                     UNDO/REDO                                    //
        // ================================================================================ //
        
        //! @internal Try to undo last action.
        void undo();
        
        //! @internal Returns true if there is an action to undo.
        bool canUndo();
        
        //! @internal Returns the undo label if there is an action to undo or an empty string.
        std::string getUndoLabel();
        
        //! @internal Try to redo next action.
        void redo();
        
        //! @internal Returns true if there is an action to undo.
        bool canRedo();
        
        //! @internal Returns the redo label if there is an action to redo or an empty string.
        std::string getRedoLabel();
        
        // ================================================================================ //
        //                                     SELECTION                                    //
        // ================================================================================ //
        
        //! @brief Returns the selected objects.
        std::set<flip::Ref> const& getSelectedObjects() const;
        
        //! @brief Returns the selected links.
        std::set<flip::Ref> const& getSelectedLinks() const;
        
        void addToSelectionBasedOnModifiers(ObjectView& object, bool select_only);
        
        void addToSelectionBasedOnModifiers(LinkView& link, bool select_only);
        
        bool selectOnMouseDown(ObjectView& object, bool select_only);
        
        bool selectOnMouseDown(LinkView& link, bool select_only);
        
        void selectOnMouseUp(ObjectView& box, bool select_only,
                             const bool box_was_dragged,
                             const bool result_of_mouse_down_select_method);
        
        void selectOnMouseUp(LinkView& link, bool select_only,
                             const bool box_was_dragged,
                             const bool result_of_mouse_down_select_method);
        
        //! @brief Returns true if any object or link is selected
        bool isAnythingSelected();
        
        //! @brief Returns true if there is at least one object selected
        bool isAnyObjectSelected();
        
        //! @brief Returns true if there is at least one link selected
        bool isAnyLinksSelected();
        
        //! @brief Select an Object.
        void selectObject(ObjectView& object);
        
        //! @brief Select multiple objects
        void selectObjects(std::vector<ObjectView*> const& objects);
        
        //! @brief Unselect all and select an object.
        void selectObjectOnly(ObjectView& object);
        
        //! @brief Select a Link.
        void selectLink(LinkView& link);
        
        //! @brief Select multiple links
        void selectLinks(std::vector<LinkView*> const& links);
        
        //! @brief Unselect all and select a link.
        void selectLinkOnly(LinkView& link);
        
        //! @brief Unselect an Object.
        void unselectObject(ObjectView& object);
        
        //! @brief Unselect a Link.
        void unselectLink(LinkView& link);
        
        //! @brief Add all objects to the patcher selection.
        void selectAllObjects();
        
        //! @brief Unselect all objects.
        void unselectAll();
        
        //! @brief Unselect all objects.
        void deleteSelection();
        
        //! @brief Begins a move or resize gesture.
        void startMoveOrResizeObjects();
        
        //! @brief Ends a move or resize gesture.
        void endMoveOrResizeObjects();
        
        //! @brief Resize selected objects by a given amount of pixels.
        //! @param delta        The given amount of pixel.
        //! @param border_flag  The border flag (see HitTester::Border enum)
        //! @param preserve_ratio Should preserve box ratio
        void resizeSelectedObjects(juce::Point<int> const& delta,
                                   const long border_flag, const bool preserve_ratio);
        
        //! @brief Move selected objects by a given amount of pixels.
        //! @param delta The given amount of pixel.
        //! @param commit Pass false if you don't want to commit.
        //! @param gesture Pass true to commit a gesture (commit must be true).
        void moveSelectedObjects(juce::Point<int> const& delta,
                                 bool commit = true, bool gesture = false);
        
        //! @brief Add selected objects and links to clipboard.
        void copySelectionToClipboard();
        
        //! @brief Try to paste clipboard content.
        void pasteFromClipboard(juce::Point<int> const& delta);
        
        //! @brief Duplicate selected objects.
        void duplicateSelection();
        
        //! @brief Copy selected objects then delete selection.
        void cut();
        
        //! @brief Try to replace selected object by the object copied into clipboard.
        void pasteReplace();
        
        model::Object& replaceObjectWith(model::Object& object_to_replace, flip::Mold const& mold);
        
        // ================================================================================ //
        //                                      MISC                                        //
        // ================================================================================ //
        
        //! @internal Find a ObjectView with a given Object model.
        ObjectViews::iterator findObject(model::Object const& object);
        
        //! @internal Find a LinkView with a given Link model.
        LinkViews::iterator findLink(model::Link const& link);
        
        //! @internal Returns true if a link can be created between two objects.
        bool canConnect(model::Object const& from, const size_t outlet,
                        model::Object const& to, const size_t inlet) const;
        
        //! @internal Returns the current objects area.
        juce::Rectangle<int> getCurrentObjectsArea();
        
        //! @internal Returns the current selection bounds.
        juce::Rectangle<int> getSelectionBounds();
        
        //! @internal get the nearest valid iolet of the link creator.
        std::pair<ObjectView*, size_t> getLinkCreatorNearestEndingIolet();
        
        //! @brief Zoom in Patcher View.
        void zoomIn();
        
        //! @brief Zoom out Patcher View.
        void zoomOut();
        
        //! @brief Reset Patcher View zoom to 100%.
        void zoomNormal();
        
        //! @internal Show Patcher contextual Popup menu
        void showPatcherPopupMenu(juce::Point<int> const& position);
        
        //! @internal Show Object contextual Popup menu
        void showObjectPopupMenu(ObjectView const& object_view, juce::Point<int> const& position);
        
        //! @internal Show Link contextual Popup menu
        void showLinkPopupMenu(LinkView const& link_view, juce::Point<int> const& position);
        
        //! @brief Bring all link components in front of object ones.
        void bringsLinksToFront();
        
        //! @brief Bring all object components in front of link ones.
        void bringsObjectsToFront();
        
        //! Get the appropriate mouse cursor for a given border flag.
        juce::MouseCursor::StandardCursorType getMouseCursorForBorder(int border_flag) const;

    private: // members
        
        PatcherManager&                             m_manager;
        Instance&                                   m_instance;
        model::Patcher&                             m_patcher_model;
        model::Patcher::View&                       m_view_model;
        
        ObjectViews                                 m_objects;
        LinkViews                                   m_links;
        
        std::set<flip::Ref>                         m_local_objects_selection;
        std::set<flip::Ref>                         m_local_links_selection;
        
        std::map<flip::Ref, std::set<uint64_t>>     m_distant_objects_selection;
        std::map<flip::Ref, std::set<uint64_t>>     m_distant_links_selection;
        
        std::unique_ptr<PatcherViewport>            m_viewport;
        std::unique_ptr<HitTester>                  m_hittester;
        std::unique_ptr<LinkViewCreator>            m_link_creator;
        std::unique_ptr<IoletHighlighter>           m_io_highlighter;
        std::unique_ptr<Lasso>                      m_lasso;
        
        bool    m_is_locked;
        int     m_grid_size;
                
        // mouse interactions flags
        juce::Point<int> m_last_drag;
        bool m_object_received_down_event = false;
        bool m_copy_on_drag = false;
        bool m_is_dragging = false;
        bool m_is_dragging_links = false;
        bool m_mouse_has_just_been_clicked = false;
        bool m_select_on_mouse_down_status = false;
        bool m_link_downstatus = false;
        bool m_is_in_move_or_resize_gesture = false;
        long m_object_border_down_status;
        
        // here to initialise PatcherView commands only one time.
        static bool m_command_manager_binded;
        
        friend PatcherViewport;
        friend Lasso;
    };
}

#endif // KIWI_APP_PATCHERVIEW_HPP_INCLUDED
