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
        class Patcher : public flip::DocumentObserver<model::Patcher>
        {
        public:
            
            //! Destructor.
            ~Patcher();
            
            static std::unique_ptr<Patcher> create(Instance& instance);
            
            model::Patcher& getModel()
            {
                return m_document->root<model::Patcher>();
            }
            
            //! Get the objects.
            /** The function retrieves the objects from the patcher.
             @return A vector with the objects.
             */
            inline std::vector<std::unique_ptr<Object>>::size_type getNumberOfObjects() const
            {
                return m_objects.size();
            }
            
            //! Get the objects.
            /** The function retrieves the objects from the patcher.
             @return A vector with the objects.
             */
            inline std::vector<std::unique_ptr<Object>> const& getObjects() noexcept
            {
                return m_objects;
            }
            
            //! Get an object with the id.
            /** The function retrieves an object with an id.
             @param id   The id of the object.
             */
            inline Object* getObjectByModel(const model::Object& object_model) const noexcept
            {
                for(auto& obj : m_objects)
                {
                    if(&obj.get()->m_model == &object_model)
                        return obj.get();
                }
                
                return nullptr;
            }
            
            //! @brief Get an object from its model.
            //! @param id   The id of the object.
            inline Object* getObjectWithId(const uint64_t /*ID*/) const noexcept
            {
                return nullptr;
            }
            
            //! @brief Creates and add an object to the Patcher.
            controller::Object* addObject(std::string const& name, std::string const& text = "");
            
            //! @brief Constructs and add a Link to the Patcher.
            //! @details Constructs a Link with given origin and destination Object pointers
            //! and IO indexes then adds it in the Patcher.
            //! @param from     The origin Object pointer.
            //! @param outlet   The origin outlet index.
            //! @param to       The destination Object pointer.
            //! @param inlet    The destination inlet index.
            controller::Link* addLink(controller::Object&   from,
                                      const uint32_t        outlet,
                                      controller::Object&   to,
                                      const uint32_t        inlet);
            
            //! Free a object.
            /** The function removes a object from the patcher.
             @param object        The pointer to the object.
             */
            void remove(Object* object);
            
            //! Begin a new transaction
            /** Each call to this function must be followed by a call to endTransaction.
             @param transaction_name The name of the current transaction
             @see endTransaction
             */
            void beginTransaction(std::string transaction_name)
            {
                m_document->set_label(transaction_name);
                std::cout << "* " << transaction_name << '\n';
            }
            
            //! Ends a transaction
            /** Each call to this function must be preceded by a call to beginTransaction.
             @see beginTransaction
             */
            void endTransaction()
            {
                auto tx = m_document->commit();
                m_history->add_undo_step(tx);
            }
            
            //! Undo the last transaction and optionally commit
            void undo(const bool commit = false);
            
            //! Redo the next transaction and optionally commit
            void redo(const bool commit = false);
            
        private:
            
            template <class TControllerClass, class TModelClass = model::Object>
            controller::Object* addObjectController(model::Object& obj_model,
                                                    std::vector<Atom> const& args)
            {
                auto ctrl = m_objects.emplace(m_objects.cend(), std::unique_ptr<TControllerClass>(new TControllerClass(static_cast<TModelClass&>(obj_model), args)));
                
                return ctrl->get();
            }
            
            void debug_document(model::Patcher& patcher);
            
            Instance&           m_instance;
            
            std::unique_ptr<flip::Document>         m_document;
            
            std::unique_ptr<flip::History<flip::HistoryStoreMemory>>
                                                    m_history;
            
            std::vector<std::unique_ptr<controller::Object>>    m_objects;
            std::vector<std::unique_ptr<controller::Link>>      m_links;
            
            void document_changed(model::Patcher& patcher) override;
            
            //! Constructor.
            Patcher(Instance& instance) noexcept;
        };
    }
}


#endif // KIWI_CONTROLLER_PATCHER_HPP_INCLUDED
