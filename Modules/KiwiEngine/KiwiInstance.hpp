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

#ifndef KIWI_ENGINE_INSTANCE_HPP_INCLUDED
#define KIWI_ENGINE_INSTANCE_HPP_INCLUDED

#include "KiwiPatcher.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      INSTANCE                                    //
        // ================================================================================ //
        
        class Instance
        {
        public:
            
            //! @brief Creates and returns an Instance.
            //! @param user_id The user ID.
            //! @param name The instance name.
            //! @return A new instance.
            static std::unique_ptr<Instance> create(uint64_t user_id, std::string const& name);
            
            //! @brief Destructor.
            ~Instance();
            
            //! @brief Get the name of the Instance.
            inline std::string getName() const                  { return m_name; }
            
            //! @brief Get the user ID of the Instance.
            inline uint64_t getUserId() const noexcept          { return m_user_id; }
            
            //! @brief Set debug mode.
            inline void setDebug(bool active_debug) noexcept    { m_debug = active_debug; }
            
            //! @brief Set debug mode.
            inline bool isInDebugMode() const noexcept          { return m_debug; }
            
            //! @brief Creates and adds a new patcher to the Instance.
            //! @return A reference to the created Patcher.
            //! @see removePatcher, getPatchers
            Patcher& createPatcher();
            
            //! @brief Removes a patcher from the Instance.
            //! @param patcher The patcher to remove.
            //! @see createPatcher, getPatchers
            void removePatcher(Patcher* patcher);
            
            //! @brief Get the patchers of the Instance.
            //! @return A vector of patchers.
            //! @see createPatcher, removePatcher
            std::vector<std::unique_ptr<Patcher>> const& getPatchers();
            
        private:
            
            //! @internal Constructor.
            Instance(uint64_t user_id, std::string const& name) noexcept;
            
            const uint64_t          m_user_id;
            const std::string       m_name;
            
            std::vector<std::unique_ptr<Patcher>>
                                    m_patchers;
            
            static bool             m_declared_flag;
            
            bool                    m_debug;
            
            static void declare();
        };
    }
}


#endif // KIWI_ENGINE_INSTANCE_HPP_INCLUDED
