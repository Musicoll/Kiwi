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

#ifndef KIWI_MODEL_HPP_INCLUDED
#define KIWI_MODEL_HPP_INCLUDED

#include "KiwiFactory.hpp"

namespace kiwi
{
    namespace model
    {
        //! @brief The Patcher Model class
        class Model : public flip::DataModel<Model>
        {
        public:
            
            //! @brief Initializes the model.
            //! @details Declares all flip classes.
            //! @param version The model version.
            static void init(std::string const& version)
            {
                Model::version(version);
                
                // basic types declaration :
                FlipRGBA::declare<Model>();
                FlipPoint::declare<Model>();
                
                // attributes declaration :
                Attribute::declare<Model>();
                Attribute::declare<Model>();
                
                // Attributes types
                Attribute::Int::declare<Model>();
                Attribute::Float::declare<Model>();
                Attribute::String::declare<Model>();
                Attribute::RGBA::declare<Model>();
                Attribute::Enum::declare<Model>();
                
                // patcher elements declaration :
                model::Object::declare<Model>();
                
                // Links
                Link::declare<Model>();
                Link::Control::declare<Model>();
                Link::Dsp::declare<Model>();
                
                // Patcher
                Patcher::declare<Model>();
            }
        };
    }
    
    using Model = kiwi::model::Model;
    using FlipRGBA = kiwi::model::FlipRGBA;
    using FlipPoint = kiwi::model::FlipPoint;
    using Attribute = kiwi::model::Attribute;
    using Object = kiwi::model::Object;
    using Link = kiwi::model::Link;
    using Patcher = kiwi::model::Patcher;

}

#endif // KIWI_MODEL_HPP_INCLUDED
