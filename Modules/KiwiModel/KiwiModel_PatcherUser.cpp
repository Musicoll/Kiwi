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

#include "KiwiModel_DataModel.h"
#include "KiwiModel_PatcherUser.h"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                  PATCHER::declare                                //
        // ================================================================================ //

        void Patcher::User::declare()
        {
            assert(! DataModel::has<Patcher::User>());
            
            DataModel::declare<Patcher::User>()
            .name("cicm.kiwi.Patcher.User")
            .member<flip::Collection<Patcher::View>, &Patcher::User::m_views>("views");
        }
        
        // ================================================================================ //
        //                                   PATCHER USER                                   //
        // ================================================================================ //
        
        Patcher::User::User()
        {
            
        }
        
        Patcher::View& Patcher::User::addView()
        {
            return *m_views.emplace();
        }
        
        flip::Collection<Patcher::View>::iterator Patcher::User::removeView(View const& view)
        {
            auto it = m_views.find_if([&view](View const& patcher_view) {
                return (&view == &patcher_view);
            });
            
            if(it != m_views.end())
            {
                return m_views.erase(it);
            }
            
            return m_views.end();
        }
        
        uint64_t Patcher::User::getId() const
        {
            return ref().user();
        }
        
        flip::Collection<Patcher::View> const& Patcher::User::getViews() const noexcept
        {
            return m_views;
        }
        
        flip::Collection<Patcher::View>& Patcher::User::getViews() noexcept
        {
            return m_views;
        }
        
        size_t Patcher::User::getNumberOfViews() const noexcept
        {
            return std::count_if(m_views.begin(), m_views.end(), [](auto){return true;});
        }
    }
}
