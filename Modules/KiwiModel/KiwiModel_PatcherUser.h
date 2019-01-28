/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#pragma once

#include "KiwiModel_Patcher.h"
#include "KiwiModel_PatcherView.h"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                 PATCHER USER                                   //
        // ================================================================================ //
        
        //! @brief Represents and stores informations about a user of a patcher document.
        class Patcher::User : public flip::Object
        {
        public: // methods
            
            //! @brief Constructor.
            User();
            
            //! @brief Destructor.
            ~User() = default;
            
            //! @brief Add a new View.
            View& addView();
            
            //! @brief Remove a View.
            flip::Collection<Patcher::View>::iterator removeView(View const& view);
            
            //! @brief Get views.
            flip::Collection<Patcher::View> const& getViews() const noexcept;
            
            //! @brief Get views.
            flip::Collection<Patcher::View>& getViews() noexcept;
            
            //! @brief Get the number of active views.
            size_t getNumberOfViews() const noexcept;
            
            //! @brief Get the User id
            uint64_t getId() const;
            
        public: // internal methods
            
            //! @brief flip declare method
            static void declare();
            
        private: // members
            
            flip::Collection<Patcher::View> m_views;
            
            friend Patcher;
        };
    }
}
