/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/hwp/nest/p9_sbe_load_bootloader.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
//------------------------------------------------------------------------------------
//
/// @file p9_sbe_load_bootloader.H
/// @brief Shift HB bootloader payload from SEEPROM to L3 cache of master core via PBA
//
// *HWP HWP Owner: Christina Graves clgraves@us.ibm.com
// *HWP HWP Writer: Murey A Luna Torres malunato@us.ibm.com, Joseph McGill jmcgill@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 2
// *HWP Consumed by: SBE
//-----------------------------------------------------------------------------------
// *! ADDITIONAL COMMENTS:
// *!
// *! This hardware procedure is used to load a bootloader image from seeprom into
// *! L3 of master core via PBA unit.
// *!
// *! Successful operation assumes that:
// *! PBA communication is available
// *!
// *! High-level procedure flow:
// *!
//------------------------------------------------------------------------------------

#ifndef _SBE_BOOTLOADER_H_
#define _SBE_BOOTLOADER_H_

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------

#include <fapi2.H>
#include <p9_sbe_hb_structures.H>
//-----------------------------------------------------------------------------------
// Structure definitions
//-----------------------------------------------------------------------------------

//function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode(*p9_sbe_load_bootloader_FP_t) (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
    const fapi2::Target<fapi2::TARGET_TYPE_EX>&,
    const uint64_t,
    uint8_t*);

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------

extern "C" {

    //-----------------------------------------------------------------------------------
    // Function prototype
    //-----------------------------------------------------------------------------------
    //
/// @brief Shift HB bootloader payload from SEEPROM to L3 cache of master core via PBA
/// @param[in] i_master_chip_target Reference to master processor chip target
/// @param[in] i_master_ex_target Reference to master ex unit target
/// @param[in] i_payload_size Size of image payload load to load, in B
/// @param[in] i_payload_data Pointer to image payload data
///
/// @return FAPI_RC_SUCCESS if success, else error code
///
    fapi2::ReturnCode p9_sbe_load_bootloader(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_master_chip_target,
        const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_master_eq_target,
        const uint64_t i_payload_size,
        uint8_t* i_payload_data);

} //extern "C"

#endif //_SBE_BOOTLOADER_H_



