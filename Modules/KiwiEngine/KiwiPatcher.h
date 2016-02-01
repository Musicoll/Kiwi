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

#ifndef __DEF_KIWI_PATCHER__
#define __DEF_KIWI_PATCHER__

#include "KiwiLink.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  PATCHER DOCUMENT                                //
    // ================================================================================ //
    
    class PatcherDocument : public flip::Document
    {
    public:
        PatcherDocument(const flip::DataModelBase& data_model,
                        flip::DocumentObserverBase& observer,
                        flip::DocumentValidatorBase& validator,
                        uint64_t user_id)
        : flip::Document(data_model, observer, validator, user_id, 'cicm', 'kpat') {}
    };
    
    // ================================================================================ //
    //                                      PATCHER                                     //
    // ================================================================================ //
    
    //! The patcher manages objects and links.
    /**
     The patcher is... ??
     */
    class Patcher : public flip::DocumentObserver<PatcherModel>
    {
    private:
        Instance*                               m_instance = nullptr;
        PatcherModel*                           m_model = nullptr;
        
        shared_ptr<flip::Document>              m_document;     // unique_ptr
        shared_ptr<flip::History<flip::HistoryStoreMemory>>
                                                m_history;
        
        vector<sObject>                         m_objects;
        vector<sLink>                           m_links;
        
        mutable mutex                           m_mutex;
        
        void createObject(Dico& dico);
        
        //! flip::DocumentObserver<PatcherModel>
        void document_changed(PatcherModel& patcher) override;
        
        //! Constructor.
        Patcher(Instance* instance) noexcept;
        
    public:
        //! Destructor.
        ~Patcher();
        
        static unique_ptr<Patcher> create(Instance* instance);

        //! Get the objects.
        /** The function retrieves the objects from the patcher.
         @return A vector with the objects.
         */
        inline ulong getNumberOfObjects() const
        {
            lock_guard<mutex> guard(m_mutex);
            return m_objects.size();
        }
        
        //! Get the objects.
        /** The function retrieves the objects from the patcher.
         @return A vector with the objects.
         */
        inline vector<sObject>& getObjects() noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return m_objects;
        }
        
        //! Get an object with the id.
        /** The function retrieves an object with an id.
         @param id   The id of the object.
         */
        inline sObject getObjectWithId(const ulong ID) const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return nullptr;
        }
        
        //! Append a dico.
        /** The function reads a dico and add the objects and links to the patcher.
         @param dico The dico.
         */
        void add(Dico const& dico);
        
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
        void beginTransaction(string transaction_name)
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
        
        //! Set a patcher attribute value.
        /** The function sets a patcher attribute value.
         @param name the name of the attribute.
         @param value The new attribute value.
         */
        inline void setAttributeValue(const sTag name, Atom const& value, const bool commit = false) noexcept
        {
            if(commit)
            {
                beginTransaction("Patcher's " + name->getName() + " attribute changed");
            }
            
            m_model->setAttributeValue(name, value);
            
            if(commit)
            {
                endTransaction();
            }
        }
        
        //! Retrieve a patcher attribute value.
        /** The function retrieves a patcher attribute value.
         @param name the name of the attribute.
         @return The value of the attribute as an Atom.
         */
        inline Atom getAttributeValue(const sTag name) const noexcept
        {
            return m_model->getAttributeValue(name);
        }
        
        //! Retrieve the "gridsize" attribute value of the patcher.
        /** The function retrieves the "gridsize" attribute value of the patcher.
         @return The "gridsize" attribute value of the patcher.
         */
        inline Atom getGridSize() const noexcept
        {
            return m_model->getAttributeValue(Tags::gridsize);
        }
        
        //! Set the "gridsize" attribute value of the patcher.
        /** The function sets the "gridsize" attribute value of the patcher.
         @param value The "gridsize" attribute value of the patcher.
         */
        inline void setGridSize(Atom const& value, const bool commit = false) noexcept
        {
            setAttributeValue(Tags::gridsize, value, commit);
        }
    };
}


#endif


