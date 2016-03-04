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

#include <KiwiGraphics/KiwiPath.hpp>

namespace kiwi
{
    namespace graphics
    {
        void Path::transform(AffineMatrix const& matrix) noexcept
        {
            for(auto&& node : m_nodes)
            {
                node.transform(matrix);
            }
            //m_bounds update
        }
        
        Path Path::transformed(AffineMatrix const& matrix) const noexcept
        {
            Path p = *this;
            p.transform(matrix);
            return p;
        }
    }
}