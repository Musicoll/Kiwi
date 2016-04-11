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

#ifndef KIWI_CONTROLLER_PATCHER_HPP_INCLUDED
#define KIWI_CONTROLLER_PATCHER_HPP_INCLUDED

#include "KiwiLink.hpp"

#include "flip/Document.h"
#include "flip/DocumentObserver.h"
#include "flip/History.h"
#include "flip/HistoryStoreMemory.h"

namespace kiwi
{
    namespace controller
    {
        // ================================================================================ //
        //                                      PATCHER                                     //
        // ================================================================================ //
        
        //! @brief The Patcher manages object and link controllers.
        //! @details The patcher controller observes the Patcher mode for changes.
        class Patcher : public flip::DocumentObserver<model::Patcher>
        {
        public:
            
            using objects_t = std::vector<std::unique_ptr<controller::Object>>;
            using links_t = std::vector<std::unique_ptr<controller::Link>>;
            
            //! @brief Destructor.
            ~Patcher();
            
            //! @brief Creates and returns a new Patcher.
            static std::unique_ptr<Patcher> create(Instance& instance);
            
            //! @brief Creates and adds a "plus" object to the Patcher.
            ObjectId addPlus();
            
            //! @brief Creates and adds a "print" object to the Patcher.
            ObjectId addPrint();
            
            //! @brief Constructs and add a Link to the Patcher.
            //! @details Constructs a Link with given origin and destination Object pointers
            //! and IO indexes then adds it in the Patcher.
            //! @param from     The origin Object pointer.
            //! @param outlet   The origin outlet index.
            //! @param to       The destination Object pointer.
            //! @param inlet    The destination inlet index.
            LinkId addLink(ObjectId const& from, const uint32_t outlet, ObjectId const& to, const uint32_t inlet);
            
            //! @brief Removes an Object from the Patcher.
            //! @param id The ID of the object to be removed.
            void removeObject(ObjectId const& id);
            
            //! @brief Removes an Object from the Patcher.
            //! @param object The pointer to the object to be removed.
            void removeLink(LinkId const& link);
            
            //! @brief Get the number of objects.
            //! @return The number of objects.
            inline uint64_t getNumberOfObjects() const { return static_cast<uint64_t>(m_objects.size()); };
            
            //! @brief Get the number of links.
            //! @return The number of links.
            inline uint64_t getNumberOfLinks() const { return static_cast<uint64_t>(m_links.size()); };
            
            //! @brief Send a message to an object
            void sendToObject(ObjectId const& object_id, uint32_t inlet, std::vector<Atom> args);
            
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
            
        private:
            
            //! @brief Constructor.
            //! @details use the create method instead.
            Patcher(Instance& instance) noexcept;
            
            //! @brief Get the Patcher model
            inline model::Patcher& getModel() { return m_document.root<model::Patcher>(); }
            
            //! @brief Get the Patcher model
            inline model::Patcher const& getModel() const { return m_document.root<model::Patcher>(); }
            
            //! @brief Get an object model with an id.
            //! @param id The id of the object.
            model::Object* getModel(ObjectId const& id);
            
            //! @brief Get an object controller with an id.
            //! @param id The id of the object.
            Object* getController(ObjectId const& id);
            
            //! @brief Get an object controller with a model.
            //! @param id The id of the object.
            Object* getController(model::Object const& model);
            
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
            
            objects_t       m_objects;
            links_t         m_links;
        };
    }
}


#endif // KIWI_CONTROLLER_PATCHER_HPP_INCLUDED
