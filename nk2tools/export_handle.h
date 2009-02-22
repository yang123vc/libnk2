/* 
 * Export handle
 *
 * Copyright (C) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _EXPORT_HANDLE_H )
#define _EXPORT_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

/* If libtool DLL support is enabled set LIBNK2_DLL_IMPORT
 * before including libnk2_extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBNK2_DLL_EXPORT
#endif

#include <libnk2.h>

#include <stdio.h>

#include "system_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct export_handle export_handle_t;

struct export_handle
{
	/* The libnk2 input handle
	 */
	libnk2_file_t *input_handle;
};

int export_handle_initialize(
     export_handle_t **export_handle,
     liberror_error_t **error );

int export_handle_free(
     export_handle_t **export_handle,
     liberror_error_t **error );

int export_handle_open_input(
     export_handle_t *export_handle,
     const char *filename,
     liberror_error_t **error );

int export_handle_close(
     export_handle_t *export_handle,
     liberror_error_t **error );

int export_handle_sanitize_filename(
     uint8_t *filename,
     size_t filename_size,
     liberror_error_t **error );

int export_handle_create_fullname(
     system_character_t *export_path,
     uint8_t *filename,
     size_t filename_size,
     system_character_t **fullname,
     liberror_error_t **error );

int export_handle_export_item(
     libnk2_item_t *item,
     int item_index,
     int amount_of_items,
     const system_character_t *export_path,
     FILE *log_file_stream,
     liberror_error_t **error );

int export_handle_export_items(
     export_handle_t *export_handle,
     const system_character_t *export_path,
     FILE *log_file_stream,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif
