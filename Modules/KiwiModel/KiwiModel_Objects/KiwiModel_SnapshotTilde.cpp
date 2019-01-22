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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_SnapshotTilde.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                   SNAPSHOT~                                      //
    // ================================================================================ //
    
    void SnapshotTilde::declare()
    {
        std::unique_ptr<ObjectClass> snapshottilde_class(new ObjectClass("snapshot~", &SnapshotTilde::create));
        
        flip::Class<SnapshotTilde> & snapshottilde_model = DataModel::declare<SnapshotTilde>()
                                                           .name(snapshottilde_class->getModelName().c_str())
                                                           .inherit<Object>();
        
        Factory::add<SnapshotTilde>(std::move(snapshottilde_class), snapshottilde_model);
    }
    
    std::unique_ptr<Object> SnapshotTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<SnapshotTilde>(args);
    }
    
    SnapshotTilde::SnapshotTilde(std::vector<tool::Atom> const& args)
    {
        if (args.size() > 0)
        {
            throw Error("snapshot~ too many arguments");
        }
        pushInlet({PinType::IType::Signal, PinType::IType::Control});
        pushOutlet(PinType::IType::Control);
    }
    
    std::string SnapshotTilde::getIODescription(bool is_inlet, size_t index) const
    {
        return (is_inlet
                ? "(signal) Input, (bang) Reports signal value"
                : "(signal) Values");
    }
    
}}

