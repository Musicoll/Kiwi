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

#include "KiwiViewManager.hpp"

namespace kiwi
{
    namespace gui
    {
        void ViewManager::addView(View& view)
        {
            view.setManager(this);
            m_views.push_back(&view);
        }
        
        void ViewManager::removeView(View& view)
        {
            const auto it = std::find(m_views.begin(), m_views.end(), &view);
            if(it != m_views.cend())
            {
                view.setManager(nullptr);
                m_views.erase(it);
            }
        }
        
        View* ViewManager::getFirstView()
        {
            if(!m_views.empty())
            {
                return m_views[0];
            }
            
            return nullptr;
        }
        
        void ViewManager::redraw()
        {
            for(auto& view : m_views)
            {
                view->redraw();
            }
        }
        
        void ViewManager::redraw(View& view)
        {
            view.redraw();
        }
    }
}