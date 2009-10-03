/*
 * The libfmapi header wrapper
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBNK2_LIBFMAPI_H )
#define _LIBNK2_LIBFMAPI_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFMAPI for local use of libfmapi
 */
#if defined( HAVE_LOCAL_LIBFMAPI )

#include <libfmapi_class_identifier.h>
#include <libfmapi_crc.h>
#include <libfmapi_definitions.h>
#include <libfmapi_entry_identifier.h>
#include <libfmapi_guid.h>
#include <libfmapi_lzfu.h>
#include <libfmapi_property_type.h>
#include <libfmapi_security_identifier.h>
#include <libfmapi_service_provider_identifier.h>
#include <libfmapi_types.h>
#include <libfmapi_value_type.h>

#if defined( HAVE_DEBUG_OUTPUT )
#include <libfmapi_debug.h>
#endif

#elif defined( HAVE_LIBFMAPI_H )

/* If libtool DLL support is enabled set LIBFMAPI_DLL_IMPORT
 * before including libfmapi.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFMAPI_DLL_IMPORT
#endif

#include <libfmapi.h>

#else
#error Missing libfmapi.h
#endif

#endif
