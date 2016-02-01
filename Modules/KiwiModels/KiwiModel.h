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

#ifndef __DEF_KIWI_MODEL__
#define __DEF_KIWI_MODEL__

#include "../KiwiCore/KiwiCore.h"
#include "../KiwiGraphics/KiwiGraphics.h"

// ---- Flip headers ---- //
#include "flip/DataModel.h"
#include "flip/Array.h"
#include "flip/Collection.h"
#include "flip/Bool.h"
#include "flip/Int.h"
#include "flip/Float.h"
#include "flip/Blob.h"
#include "flip/String.h"
#include "flip/Object.h"
#include "flip/ObjectRef.h"
#include "flip/Variant.h"
#include "flip/Optional.h"

#include "flip/Document.h"
#include "flip/DocumentObserver.h"

#include "flip/History.h"
#include "flip/HistoryStoreMemory.h"
#include "flip/DataConsumerMemory.h"
#include "flip/BackEndBinary.h"
#include "flip/BackEndMl.h"

namespace Kiwi
{
    //! The Model class
    class Model : public flip::DataModel<Model>
    {
    };
    
    class ObjectModel;
    typedef shared_ptr<ObjectModel>          sObjectModel;
    typedef weak_ptr<ObjectModel>            wObjectModel;
    typedef shared_ptr<const ObjectModel>    scObjectModel;
    typedef weak_ptr<const ObjectModel>      wcObjectModel;
    
    class LinkModel;
    typedef shared_ptr<LinkModel>            sLinkModel;
    typedef shared_ptr<const LinkModel>      scLinkModel;
    typedef weak_ptr<LinkModel>              wLinkModel;
    typedef weak_ptr<const LinkModel>        wcLinkModel;
    
    class PatcherModel;
    typedef shared_ptr<PatcherModel>        sPatcherModel;
    typedef weak_ptr<PatcherModel>          wPatcherModel;
    typedef shared_ptr<const PatcherModel>  scPatcherModel;
    typedef weak_ptr<const PatcherModel>    wcPatcherModel;
    
    class InstanceModel;
    typedef shared_ptr<InstanceModel>        sInstanceModel;
    typedef weak_ptr<InstanceModel>          wInstanceModel;
    typedef shared_ptr<const InstanceModel>  scInstanceModel;
    typedef weak_ptr<const InstanceModel>    wcInstanceModel;
}

#endif


