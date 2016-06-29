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

#ifndef KIWI_MODEL_PATCHER_USER_HPP_INCLUDED
#define KIWI_MODEL_PATCHER_USER_HPP_INCLUDED

#include "KiwiModelPatcherView.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                 PATCHER USER                                   //
        // ================================================================================ //
        
        class Patcher::User : public flip::Object
        {
        public:
            
            //! @brief flip default Constructor.
            User(flip::Default&) {}
            
            //! @brief Constructor.
            User(uint32_t user_id);
            
            //! @brief Destructor.
            ~User() = default;
            
            //! @brief Add a new View.
            View& addView();
            
            //! @brief Remove a View.
            flip::Collection<Patcher::View>::iterator removeView(View const& view);
            
            //! @brief Get views.
            flip::Collection<Patcher::View> const& getViews() const noexcept;
            
            //! @brief Get the User id
            uint32_t getId() const;
            
            //! @brief flip declare method
            static void declare();
            
        private:
            
            flip::Int                       m_user_id;
            flip::Collection<Patcher::View> m_views;
            
            friend Patcher;
        };
    }
}


#endif // KIWI_MODEL_PATCHER_USER_HPP_INCLUDED
