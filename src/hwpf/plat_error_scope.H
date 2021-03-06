/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_error_scope.H $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2017                        */
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
/**
 * @file plat_error_scope.H
 * @brief platform definitions which create a scope for automatic error handling
 */

#ifndef __FAPI2_PLAT_ERROR_SCOPE__
#define __FAPI2_PLAT_ERROR_SCOPE__

/// @cond
#define PLAT_FAPI_TRY_NO_TRACE( __operation__ )                         \
    if(std::is_same<decltype((__operation__)), fapi2::AttributeRC>::value)                     \
    { \
        (void)((__operation__)); \
    } \
    else \
    { \
    if ((fapi2::current_err = (__operation__)) != fapi2::FAPI2_RC_SUCCESS) \
    {                                                                   \
        goto fapi_try_exit;                                             \
    } \
    }


#define PLAT_FAPI_TRY_TRACE( __operation__, ... )                       \
    if ((fapi2::current_err = (__operation__)) != fapi2::FAPI2_RC_SUCCESS) \
    {   /* TODO via RTC 172311 */                                             \
        /* Enable these traces back again */                                  \
        /* FAPI_ERR(__VA_ARGS__); */                                         \
        goto fapi_try_exit;                                             \
    }

///
/// @brief Assert a conditional is true.
/// If it is not, the FFDC gathering function is called and the
/// trace is output as a FAPI error trace.
/// @param[in] __conditional__ the condition to assert
/// @param[in] __ffdc__ the FFDC gathering function
/// @param[in] ... varargs, as input to FAPI_ERR
///
#define PLAT_FAPI_ASSERT( __conditional__, __ffdc__, ... )      \
    if (! (__conditional__))                                    \
    {                                                           \
        (__ffdc__).execute();                                   \
        FAPI_ERR(__VA_ARGS__);                                  \
        goto fapi_try_exit;                                     \
    }

///
/// @brief Create an error log based on __conditional__,
/// the FFDC gathering function is called and the
/// trace is output as a FAPI error trace. An error log
/// is created and returned to the caller via the platform
/// log pointer of the fapi2::current_err return code object
/// The caller is responsible for handling the error object.
///
/// @param[in] __conditional__ the condition to assert
/// @param[in] __ffdc__ the FFDC gathering function
/// @param[in] ... varargs, as input to FAPI_ERR
///
/// @note: Plat error, if supported, is stored in RC object. Failing
///        the __conditional__ test does not cause a branch to fapi_try_exit
///
///
#define PLAT_FAPI_ASSERT_NOEXIT( __conditional__, __ffdc__, ... )   \
    if (! (__conditional__))                                        \
    {                                                               \
        (__ffdc__).execute();                                       \
        FAPI_ERR(__VA_ARGS__);                                      \
    }

///
/// @brief Temporary macro for error label until all are removed.
/// @todo REMOVE this in time.
#define FAPI_CLEANUP()   \
fapi_try_exit:
/// @endcond

#endif
