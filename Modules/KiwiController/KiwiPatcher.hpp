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
        
        //! The patcher manages objects and links.
        /**
         The patcher is... ??
         */
        class Patcher : public flip::DocumentObserver<model::Patcher>
        {
        private:
            Instance*           m_instance = nullptr;
            model::Patcher*     m_model = nullptr;
            
            std::shared_ptr<flip::Document>              m_document;     // std::unique_ptr
            std::shared_ptr<flip::History<flip::HistoryStoreMemory>>
            m_history;
            
            std::vector<sObject>                         m_objects;
            std::vector<sLink>                           m_links;
                        
            void document_changed(model::Patcher& patcher) override;
            
            //! Constructor.
            Patcher(Instance* instance) noexcept;
            
        public:
            
            //! Destructor.
            ~Patcher();
            
            static std::unique_ptr<Patcher> create(Instance* instance);
            
            //! Get the objects.
            /** The function retrieves the objects from the patcher.
             @return A vector with the objects.
             */
            inline std::vector<sObject>::size_type getNumberOfObjects() const
            {
                return m_objects.size();
            }
            
            //! Get the objects.
            /** The function retrieves the objects from the patcher.
             @return A vector with the objects.
             */
            inline std::vector<sObject> const& getObjects() noexcept
            {
                return m_objects;
            }
            
            //! Get an object with the id.
            /** The function retrieves an object with an id.
             @param id   The id of the object.
             */
            inline sObject getObjectWithId(const uint64_t ID) const noexcept
            {
                return nullptr;
            }
            
            //! @brief Creates and add an object to the Patcher.
            void addObject(std::string const& name, std::string const& text);
            
            //! Free a object.
            /** The function removes a object from the patcher.
             @param object        The pointer to the object.
             */
            void remove(sObject object);
            
            //! Begin a new transaction
            /** Each call to this function must be followed by a call to endTransaction.
             @param transaction_name The name of the current transaction
             @see endTransaction
             */
            void beginTransaction(std::string transaction_name)
            {
                m_document->set_label(transaction_name);
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
            void undo(const bool commit = false)
            {
                m_history->execute_undo();
                
                if(commit)
                {
                    m_document->commit();
                }
            }
            
            //! Redo the next transaction and optionally commit
            void redo(const bool commit = false)
            {
                m_history->execute_redo();
                
                if(commit)
                {
                    m_document->commit();
                }
            }
        };
    }
}


#endif // KIWI_CONTROLLER_PATCHER_HPP_INCLUDED
