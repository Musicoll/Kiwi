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

#ifndef KIWI_CONTROLLER_INSTANCE_HPP_INCLUDED
#define KIWI_CONTROLLER_INSTANCE_HPP_INCLUDED

#include "KiwiPatcher.hpp"

namespace kiwi
{
    namespace controller
    {
        // ================================================================================ //
        //                                      INSTANCE                                    //
        // ================================================================================ //
        
        class Instance
        {
        public:
            
            using patcher_container_t = std::vector<std::unique_ptr<Patcher>>;
            
            //! The constructor.
            /** You should never use this method.
             @param guiDevice The gui device manager.
             @param dspDevice The dsp device manager.
             @param name      The instance name.
             */
            Instance(uint64_t user_id, std::string const& name) noexcept;
            
            //! The destructor.
            ~Instance();
            
            //! The instance creation method.
            /** The function allocates an instance and initialize the prototypes of objects.
             @param guiDevice The gui device manager.
             @param dspDevice The dsp device manager.
             @param name      The instance name.
             @return          The instance.
             */
            static std::unique_ptr<Instance> create(uint64_t user_id, std::string const& name);
            
            //! Retrieve the name of the instance.
            /** The function retrieves the name of the instance.
             @return The name of the instance.
             */
            inline std::string getName() const
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
            Patcher& createPatcher();
            
            //! Close a patcher.
            /** The function closes patcher.
             @param patcher The patcher.
             @see createPatcher, getPatchers
             */
            void removePatcher(Patcher* patcher);
            
            //! Retreive all the patchers of the instance.
            /** The function retreives all the patchers of the instance.
             @return A vector of patchers.
             @see createPatcher, removePatcher
             */
            patcher_container_t const& getPatchers();
            
        private:
            const uint64_t          m_user_id;
            const std::string       m_name;
            patcher_container_t     m_patchers;
            
            static bool             m_declared_flag;
            
            static void declare();
        };
    }
}


#endif // KIWI_CONTROLLER_INSTANCE_HPP_INCLUDED
