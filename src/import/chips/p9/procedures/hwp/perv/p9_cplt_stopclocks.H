/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_cplt_stopclocks.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
//------------------------------------------------------------------------------
/// @file  p9_cplt_stopclocks.H
///
/// @brief stop clocks for Xb,Ob,Pcie
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha Reddy Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : FSP:HB
//------------------------------------------------------------------------------


#ifndef _P9_CPLT_STOPCLOCKS_H_
#define _P9_CPLT_STOPCLOCKS_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_cplt_stopclocks_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&, const bool, const bool,
    const bool, const bool);

/// @brief StopClocks for Xb,Ob,Pcie
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
/// @param[in]     i_stop_xb       if true stop stopping clocks for xbus
/// @param[in]     i_stop_ob       if true stop stopping clocks for ob chiplet
/// @param[in]     i_stop_pcie     if true stop stopping clocks for pcie chiplet
/// @param[in]     i_stop_mc       if true stop stopping clocks for Mc chiplet
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_cplt_stopclocks(const
                                         fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
                                         const bool i_stop_xb = true,
                                         const bool i_stop_ob = true,
                                         const bool i_stop_pcie = true,
                                         const bool i_stop_mc = true);
}

#endif
