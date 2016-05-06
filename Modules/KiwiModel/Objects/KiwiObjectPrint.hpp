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

#ifndef KIWI_MODEL_OBJECTS_OBJECTPRINT_HPP_INCLUDED
#define KIWI_MODEL_OBJECTS_OBJECTPRINT_HPP_INCLUDED

#include "../KiwiObject.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
        
        class ObjectPrint : public model::Object
        {
        public:
            
            ObjectPrint(flip::Default& d) : model::Object(d) {}
            
            ObjectPrint() : model::Object("print", 1, 0)
            {
                ;
            }

            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                if(TModel::template has<ObjectPrint>()) return;
                
                TModel::template declare<ObjectPrint>()
                .name("cicm.kiwi.ObjectPrint")
                .template inherit<model::Object>();
            }
        };
    }
}


#endif // KIWI_MODEL_OBJECTS_OBJECTPRINT_HPP_INCLUDED
