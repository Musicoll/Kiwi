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

#ifndef KIWI_MODEL_DATA_MODEL_HPP_INCLUDED
#define KIWI_MODEL_DATA_MODEL_HPP_INCLUDED

#include "flip/DataModel.h"

namespace kiwi
{
    namespace model
    {
        //! @brief The Patcher data model.
        class DataModel : public flip::DataModel<DataModel>
        {
        public: // methods
            
            //! @brief Initializes the model.
            //! @details Declares all flip classes.
            static void init();
            
        private: // methods
            
            //! @brief Declare all kiwi objects.
            static void declareObjects();
            
        public: // members
            
            static bool initialised;
        };
    }
}

#endif // KIWI_MODEL_DATA_MODEL_HPP_INCLUDED
