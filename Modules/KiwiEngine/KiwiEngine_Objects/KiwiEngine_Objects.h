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

// ================================================================================ //
//                                  BASIC OBJECTS                                   //
// ================================================================================ //

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Basic/KiwiEngine_NewBox.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Basic/KiwiEngine_ErrorBox.h>

// ================================================================================ //
//                                  DSP OBJECTS                                     //
// ================================================================================ //

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Dsp/KiwiEngine_SigTilde.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Dsp/KiwiEngine_PlusTilde.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Dsp/KiwiEngine_TimesTilde.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Dsp/KiwiEngine_OscTilde.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Dsp/KiwiEngine_DelaySimpleTilde.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Dsp/KiwiEngine_AdcTilde.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Dsp/KiwiEngine_DacTilde.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Dsp/KiwiEngine_MeterTilde.h>

// ================================================================================ //
//                                  MATH OBJECTS                                    //
// ================================================================================ //

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Math/KiwiEngine_Plus.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Math/KiwiEngine_Times.h>

// ================================================================================ //
//                                  MESSAGE OBJECTS                                 //
// ================================================================================ //

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Message/KiwiEngine_Receive.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Message/KiwiEngine_Loadmess.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Message/KiwiEngine_Print.h>

// ================================================================================ //
//                                  TIME OBJECTS                                    //
// ================================================================================ //

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Time/KiwiEngine_Pipe.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Time/KiwiEngine_Delay.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Time/KiwiEngine_Metro.h>

// ================================================================================ //
//                               CONTROLLER OBJECTS                                 //
// ================================================================================ //

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Controller/KiwiEngine_Bang.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Controller/KiwiEngine_Toggle.h>
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Controller/KiwiEngine_Slider.h>
