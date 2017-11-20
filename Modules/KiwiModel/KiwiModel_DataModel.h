/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
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

#include <functional>

#include <flip/DataModel.h>

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Objects.h>

namespace kiwi
{
    namespace model
    {
        //! @brief The Patcher data model.
        class DataModel : public flip::DataModel<DataModel>
        {
        public: // methods
            
            //! @brief Declare objects.
            static void declareObjects();
            
            //! @brief Initializes the model.
            //! @details By default will declare all objects.
            //! Can be overriden to declare custom objects or only a few objects.
            static void init(std::function<void(void)> declare_object = &DataModel::declareObjects);
            
        public: // members
            
            static bool initialised;
        };
    }
}
