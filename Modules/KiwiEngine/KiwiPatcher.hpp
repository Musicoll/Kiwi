/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#ifndef KIWI_ENGINE_PATCHER_HPP_INCLUDED
#define KIWI_ENGINE_PATCHER_HPP_INCLUDED

#include "KiwiLink.hpp"

#include "flip/Document.h"
#include "flip/DocumentObserver.h"
#include "flip/History.h"
#include "flip/HistoryStoreMemory.h"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      PATCHER                                     //
        // ================================================================================ //
        
        //! @brief The Patcher manages object and link controllers.
        //! @details The patcher engine observes the Patcher mode for changes.
        class Patcher : public flip::DocumentObserver<model::Patcher>
        {
        public:
            
            //! @brief Constructor.
            //! @details use the create method instead.
            Patcher(Instance& instance) noexcept;
            
            //! @brief Destructor.
            ~Patcher();
            
            //! @brief Creates and adds a "plus" object to the Patcher.
            void addPlus();
            
            //! @brief Creates and adds a "print" object to the Patcher.
            void addPrint();
            
            //! @brief Constructs and add a Link to the Patcher.
            //! @details Constructs a Link with given origin and destination Object pointers
            //! and IO indexes then adds it in the Patcher.
            //! @param from     The origin Object pointer.
            //! @param outlet   The origin outlet index.
            //! @param to       The destination Object pointer.
            //! @param inlet    The destination inlet index.
            void addLink(Object const& from, const uint32_t outlet, Object const& to, const uint32_t inlet);
            
            //! @brief Removes an Object from the Patcher.
            //! @param id The ID of the object to be removed.
            void removeObject(Object const& object);
            
            //! @brief Removes an Object from the Patcher.
            //! @param object The pointer to the object to be removed.
            void removeLink(Link const& link);
            
            //! @brief Returns the objects.
            std::vector<engine::Object const*> getObjects() const;
            
            //! @brief Returns the objects.
            std::vector<engine::Link const*> getLinks() const;
            
            //! @brief Begins a new transaction
            //! @details Each call to this function must be followed by a call to endTransaction.
            //! @param label The label of the current transaction.
            //! @see endTransaction
            void beginTransaction(std::string const& label);
            
            //! @brief Ends a transaction
            //! @details Each call to this function must be preceded by a call to beginTransaction.
            //! @see beginTransaction
            void endTransaction();
            
            //! @brief Undo the last transaction and optionally commit
            void undo(const bool commit = false);
            
            //! @brief Redo the next transaction and optionally commit
            void redo(const bool commit = false);
            
            // Rebouger et refaire
            //! @brief Send a message to an object
            void sendToObject(Object& object, uint32_t inlet, std::vector<Atom> args);
            
        private:

            using uobject_t = std::unique_ptr<engine::Object>;
            using ulink_t   = std::unique_ptr<engine::Link>;
            
            //! @brief Get the Patcher model
            inline model::Patcher& getModel() { return m_document.root<model::Patcher>(); }
            
            //! @brief Get the Patcher model
            inline model::Patcher const& getModel() const { return m_document.root<model::Patcher>(); }
            
            //! @brief Find an object engine with an Object model.
            std::vector<uobject_t>::const_iterator findObject(model::Object const& object) const;
            
            //! @brief Find a Link engine with a Link model.
            std::vector<ulink_t>::const_iterator findLink(model::Link const& link) const;
            
            //! @brief Find an object engine.
            std::vector<uobject_t>::const_iterator findObject(Object const& object) const;
            
            //! @brief Find a link engine.
            std::vector<ulink_t>::const_iterator findLink(Link const& link) const;
            
            //! @internal flip::DocumentObserver<model::Patcher>::document_changed
            void document_changed(model::Patcher& patcher) final;
            
            //! @internal for debugging purpose.
            void debug_document(model::Patcher& patcher);
            
            //! @internal Object has just been added to the document.
            void objectHasBeenAdded(model::Object& object);
            
            //! @internal Object is resident and internal value changed.
            void objectChanged(model::Object& object);
            
            //! @internal Object will be removed from the document.
            void objectWillBeRemoved(model::Object& object);
            
            //! @internal Link has just been added to the document.
            void linkHasBeenAdded(model::Link& link);
            
            //! @internal Link is resident and internal value changed.
            void linkChanged(model::Link& link);
            
            //! @internal Link will be removed from the document.
            void linkWillBeRemoved(model::Link& link);
            
            // -----------------------------
            
            Instance&       m_instance;
            
            flip::Document  m_document;
            flip::History
            <flip::HistoryStoreMemory>
                            m_history;
            
            std::vector<uobject_t>  m_objects;
            std::vector<ulink_t>    m_links;
        };
    }
}


#endif // KIWI_ENGINE_PATCHER_HPP_INCLUDED
