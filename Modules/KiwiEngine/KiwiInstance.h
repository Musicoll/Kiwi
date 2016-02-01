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

#ifndef __DEF_KIWI_INSTANCE__
#define __DEF_KIWI_INSTANCE__

#include "KiwiPatcher.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                      INSTANCE                                    //
    // ================================================================================ //
    
    class Instance
    {
    public:
        class Listener;
        typedef shared_ptr<Listener>        sListener;
        
    private:
        const uint64_t          m_user_id;
        const sTag              m_name;
        set<sPatcher>           m_patchers;
        mutable mutex           m_patchers_mutex;
        ListenerSet<Listener>   m_listeners;
        
        static bool m_declared_flag;
        
        static void declare();
        
    public:
        
        //! The constructor.
        /** You should never use this method.
         @param guiDevice The gui device manager.
         @param dspDevice The dsp device manager.
         @param name      The instance name.
         */
        Instance(uint64_t user_id, sTag name) noexcept;
        
        //! The destructor.
        /** You should never use this method.
         */
        ~Instance();
        
        //! The instance creation method.
        /** The function allocates an instance and initialize the prototypes of objects.
         @param guiDevice The gui device manager.
         @param dspDevice The dsp device manager.
         @param name      The instance name.
         @return          The instance.
         */
        static unique_ptr<Instance> create(uint64_t user_id, string const& name);
        
        //! Retrieve the name of the instance.
        /** The function retrieves the name of the instance.
         @return The name of the instance.
         */
        inline sTag getName() const noexcept
        {
            return m_name;
        }
        
        /** Returns the user id of the instance.
         @return The user id.
         */
        inline uint64_t getUserId() const noexcept
        {
            return m_user_id;
        }
        
        //! Create a patcher with a dict.
        /** The function creates a patcher with a dict.
         @param dico The dico that defines of the patcher.
         @return The patcher.
         @see removePatcher, getPatchers
         */
        sPatcher createPatcher();
        
        //! Create a patcher with a dict.
        /** The function creates a patcher with a dict.
         @param dico The dico that defines of the patcher.
         @return The patcher.
         @see removePatcher, getPatchers
         */
        sPatcher createPatcher(Dico& dico);
        
        //! Close a patcher.
        /** The function closes patcher.
         @param patcher The patcher.
         @see createPatcher, getPatchers
         */
        void removePatcher(sPatcher patcher);
        
        //! Retreive all the patchers of the instance.
        /** The function retreives all the patchers of the instance.
         @return A vector of patchers.
         @see createPatcher, removePatcher
         */
        vector<sPatcher> getPatchers();
        
        //! Add an instance listener in the binding list of the instance.
        /** The function adds an instance listener in the binding list of the instance. 
		 If the instance listener is already in the binding list, the function doesn't do anything.
         @param listener  The pointer of the instance listener.
         @see unbind
         */
        void addListener(sListener listener);
        
        //! Remove an instance listener from the binding list of the instance.
        /** The function removes an instance listener from the binding list of the instance. 
		 If the instance listener isn't in the binding list, the function doesn't do anything.
         @param listener  The pointer of the instance listener.
         @see bind
         */
        void removeListener(sListener listener);
    };
    
    // ================================================================================ //
    //                                  INSTANCE LISTENER                               //
    // ================================================================================ //
    
    //! The instance listener is a virtual class that can bind itself to an instance and be notified of several changes.
    /** The instance listener is a very light class with methods that receive the notifications of the creation and deletion of patchers and from dsp changes. An instance listener must create a shared pointer to be binded to an instance.
     @see Instance
     */
    class Instance::Listener
    {
    public:
        //! The destructor.
        /** The destructor does nothing.
         */
        virtual inline ~Listener() {}
        
        //! Receive the notification that a patcher has been created.
        /** The function is called by the instance when a patcher has been created.
         @param instance    The instance.
         @param patcher        The patcher.
         */
        virtual void patcherCreated(Instance* instance, sPatcher patcher) = 0;
        
        //! Receive the notification that a patcher has been closed.
        /** The function is called by the instance when a patcher has been closed.
         @param instance    The instance.
         @param patcher        The patcher.
         */
        virtual void patcherRemoved(Instance* instance, sPatcher patcher) = 0;
    };
}


#endif


