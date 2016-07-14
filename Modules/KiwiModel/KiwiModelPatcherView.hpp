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

#ifndef KIWI_MODEL_PATCHER_VIEW_HPP_INCLUDED
#define KIWI_MODEL_PATCHER_VIEW_HPP_INCLUDED

#include "KiwiModelPatcherUser.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                   PATCHER VIEW                                   //
        // ================================================================================ //
        
        //! @brief The Patcher::View class holds the informations about a view of a Patcher.
        class Patcher::View : public flip::Object
        {
        public: // methods
            
            //! @brief Constructor.
            View();
            
            //! @brief Destructor.
            ~View();
            
            //! @brief Return the parent Patcher object
            Patcher& getPatcher();
            
            //! @brief Set the lock status.
            void setLock(bool locked);
            
            //! @brief Returns true if the view is locked.
            bool getLock() const noexcept;
            
            //! @brief Returns true if the lock status changed.
            bool lockChanged() const noexcept;
            
            //! @brief Set zoom factor.
            void setZoomFactor(double zoom_factor);
            
            //! @brief Returns the current zoom factor.
            double getZoomFactor() const noexcept;
            
            //! @brief Returns true if the zoom factor changed.
            bool zoomFactorChanged() const noexcept;
            
            // ================================================================================ //
            //                                   SELECTION                                      //
            // ================================================================================ //
            
            //! @brief Return the selected Objects.
            std::vector<model::Object*> getSelectedObjects();
            
            //! @brief Return the selected Links.
            std::vector<model::Link*> getSelectedLinks();
            
            //! @brief Return true if the given Object is selected in this view.
            bool isSelected(model::Object const& object) const;
            
            //! @brief Return true if the given Link is selected in this view.
            bool isSelected(model::Link const& link) const;
            
            //! @brief Returns true if selection has changed.
            bool selectionChanged() const;
            
            //! @brief Select an Object.
            void selectObject(model::Object& object);
            
            //! @brief Select a Link.
            void selectLink(model::Link& object);
            
            //! @brief Unselect an Object.
            void unselectObject(model::Object& object);
            
            //! @brief Unselect a Link.
            void unselectLink(model::Link& object);
            
            //! @brief Unselect all objects and links
            void unselectAll();
            
            //! @brief Select all objects and links
            void selectAll();
            
        public: // internal methods
            
            //! @internal flip Default constructor.
            View(flip::Default&) {};
            
            //! @internal flip declare method
            static void declare();
            
        private: // nested classes
            
            // ================================================================================ //
            //                                PATCHER VIEW OBJECT                               //
            // ================================================================================ //
 
            //! @internal A model::Object reference wrapper.
            struct Object : public flip::Object
            {
            public: // methods
                
                Object() = default;
                ~Object() = default;
                Object(model::Object& object);
                model::Object* get() const;
                
            public: // internal methods
                
                //! @internal flip declare method
                static void declare();
                
            private: // members
                
                flip::ObjectRef<model::Object> m_ref;
            };
            
            // ================================================================================ //
            //                                 PATCHER VIEW LINK                                //
            // ================================================================================ //
            
            //! @internal A model::Link reference wrapper.
            struct Link : public flip::Object
            {
            public: // methods
                
                Link() = default;
                ~Link() = default;
                Link(model::Link& link);
                model::Link* get() const;
                
            public: // internal methods
                
                //! @internal flip declare method
                static void declare();
                
            private: // members
                
                flip::ObjectRef<model::Link> m_ref;
            };

        private: // members
            
            flip::Collection<View::Object>  m_selected_objects;
            flip::Collection<View::Link>    m_selected_links;
            
            flip::Bool                      m_is_locked;
            flip::Float                     m_zoom_factor;
        };
    }
}


#endif // KIWI_MODEL_PATCHER_VIEW_HPP_INCLUDED
