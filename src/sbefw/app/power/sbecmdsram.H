/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdsram.H $                            */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2018                        */
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
/*
 * @file: ppe/sbe/sbefw/sbecmdsram.H
 *
 * @brief This file contains the Interfaces for the Sram Access chip-ops
 *
 */

#ifndef __SBEFW_SBECMDSRAM_H
#define __SBEFW_SBECMDSRAM_H

#include <stdint.h>

static const uint8_t SBE_32BIT_ALIGN_FACTOR = 4;
static const uint8_t SBE_64BIT_ALIGN_FACTOR = 8;

static const uint64_t SBE_FWCTRLFLG3_FSP_ATTACHED = 0X1000000000000000;

/*
 * @brief Granule size in Bytes for 'Get from Hwp'/'Put to Hwp'
 */
static const uint32_t SBE_OCC_SRAM_GRANULE = 128;

/**
  * @brief sbeGetOccSram : Implements SBE Get Occ Sram ChipOp
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbeGetOccSram (uint8_t *i_pArg);

/**
  * @brief sbePutOccSram : Implements SBE Put Occ Sram ChipOp
  *
  * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
  *
  * @return    Rc from the FIFO access utility
  */
uint32_t sbePutOccSram  (uint8_t *i_pArg);

#endif /* __SBEFW_SBECMDSRAM_H */
