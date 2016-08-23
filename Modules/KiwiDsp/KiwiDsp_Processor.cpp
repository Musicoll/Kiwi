/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include "KiwiDsp_Processor.hpp"

namespace kiwi
{
    namespace dsp
    {
        bool Processor::isInletInplace(const size_t inlet_index, size_t * const outlet_index) const
        {
            bool isInplace = false;

            if (inlet_index < m_noutputs)
            {
                isInplace = true;
                
                if (outlet_index)
                    *outlet_index = inlet_index;
            }
            
            return isInplace;
        }
        
        bool Processor::isOutletInplace(const size_t outlet_index, size_t * const inlet_index) const
        {
            bool isInplace = false;
            
            if (outlet_index < m_ninputs)
            {
                isInplace = true;
                
                if (inlet_index)
                    *inlet_index = outlet_index;
            }
            
            return isInplace;
        }
    }
}
