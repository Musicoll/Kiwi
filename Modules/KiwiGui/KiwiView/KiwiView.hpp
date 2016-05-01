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

#ifndef KIWI_GUI_VIEW_HPP_INCLUDED
#define KIWI_GUI_VIEW_HPP_INCLUDED

#include <KiwiGui/KiwiEvents/KiwiMouseEvent.hpp>

namespace kiwi
{
    namespace gui
    {
        class ViewManager;
        
        // ================================================================================ //
        //                                      VIEW                                        //
        // ================================================================================ //
        
        class View
        {
        public:
            View() = default;
            virtual ~View() = default;
            
            //! @brief Redraw the view.
            virtual void redraw() = 0;
            
            void receive(gui::MouseEvent const& e);
            
        private:
            void setManager(ViewManager*);
            
            friend ViewManager;
            ViewManager* m_manager;
        };
    }
}

#endif // KIWI_GUI_VIEW_HPP_INCLUDED
