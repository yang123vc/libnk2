/*
 * libnk2 file
 *
 * Copyright (c) 2008-2010, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#include <common.h>
#include <narrow_string.h>
#include <memory.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>
#include <libnotify.h>

#include "libnk2_array_type.h"
#include "libnk2_codepage.h"
#include "libnk2_debug.h"
#include "libnk2_definitions.h"
#include "libnk2_io_handle.h"
#include "libnk2_item.h"
#include "libnk2_file.h"
#include "libnk2_libbfio.h"

/* Initialize a file
 * Make sure the value file is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libnk2_file_initialize(
     libnk2_file_t **file,
     liberror_error_t **error )
{
	libnk2_internal_file_t *internal_file = NULL;
	static char *function                 = "libnk2_file_initialize";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file == NULL )
	{
		internal_file = (libnk2_internal_file_t *) memory_allocate(
		                                            sizeof( libnk2_internal_file_t ) );

		if( internal_file == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create file.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     internal_file,
		     0,
		     sizeof( libnk2_internal_file_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear file.",
			 function );

			memory_free(
			 internal_file );

			return( -1 );
		}
		if( libnk2_array_initialize(
		     &( internal_file->item_array ),
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create item array.",
			 function );

			memory_free(
			 internal_file );

			return( -1 );
		}
		if( libnk2_io_handle_initialize(
		     &( internal_file->io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create io handle.",
			 function );

			libnk2_array_free(
			 &( internal_file->item_array ),
			 NULL,
			 NULL );
			memory_free(
			 internal_file );

			return( -1 );
		}
		internal_file->ascii_codepage = LIBNK2_CODEPAGE_WINDOWS_1252;

		*file = (libnk2_file_t *) internal_file;
	}
	return( 1 );
}

/* Frees an exisisting file
 * Returns 1 if successful or -1 on error
 */
int libnk2_file_free(
     libnk2_file_t **file,
     liberror_error_t **error )
{
	libnk2_internal_file_t *internal_file = NULL;
	static char *function                 = "libnk2_file_free";
	int result                            = 1;

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		internal_file = (libnk2_internal_file_t *) *file;
		*file         = NULL;

		if( libnk2_array_free(
		     &( internal_file->item_array ),
		     &libnk2_item_values_free_as_referenced_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free item array.",
			 function );

			result = -1;
		}
		if( internal_file->io_handle != NULL )
		{
			if( libnk2_io_handle_free(
			     &( internal_file->io_handle ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free io handle.",
				 function );

				result = -1;
			}
		}
		if( internal_file->file_io_handle_created_in_library != 0 )
		{
			if( internal_file->file_io_handle != NULL )
			{
				if( libbfio_handle_free(
				     &( internal_file->file_io_handle ),
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free file io handle.",
					 function );

					result = -1;
				}
			}
		}
		memory_free(
		 internal_file );
	}
	return( result );
}

/* Signals the libnk2 file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libnk2_file_signal_abort(
     libnk2_file_t *file,
     liberror_error_t **error )
{
	static char *function = "libnk2_file_signal_abort";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	( (libnk2_internal_file_t *) file )->abort = 1;

	return( 1 );
}

/* Opens a Nickfile
 * Returns 1 if successful or -1 on error
 */
int libnk2_file_open(
     libnk2_file_t *file,
     const char *filename,
     int flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle      = NULL;
	libnk2_internal_file_t *internal_file = NULL;
	static char *function                 = "libnk2_file_open";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBNK2_FLAG_READ ) != LIBNK2_FLAG_READ )
	 && ( ( flags & LIBNK2_FLAG_WRITE ) != LIBNK2_FLAG_WRITE ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBNK2_FLAG_WRITE ) == LIBNK2_FLAG_WRITE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access to Nickfiles currently not supported.",
		 function );

		return( -1 );
	}
	internal_file = (libnk2_internal_file_t *) file;

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file io handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file io handle.",
                 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

                return( -1 );
	}
#endif
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     narrow_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file io handle.",
                 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

                return( -1 );
	}
	if( libnk2_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %s.",
		 function,
		 filename );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	internal_file->file_io_handle_created_in_library = 1;

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a Nickfile
 * Returns 1 if successful or -1 on error
 */
int libnk2_file_open_wide(
     libnk2_file_t *file,
     const wchar_t *filename,
     int flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle      = NULL;
	libnk2_internal_file_t *internal_file = NULL;
	static char *function                 = "libnk2_file_open_wide";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBNK2_FLAG_READ ) != LIBNK2_FLAG_READ )
	 && ( ( flags & LIBNK2_FLAG_WRITE ) != LIBNK2_FLAG_WRITE ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBNK2_FLAG_WRITE ) == LIBNK2_FLAG_WRITE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access to Nickfiles files currently not supported.",
		 function );

		return( -1 );
	}
	internal_file = (libnk2_internal_file_t *) file;

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file io handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file io handle.",
                 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

                return( -1 );
	}
#endif
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     wide_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file io handle.",
                 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

                return( -1 );
	}
	if( libnk2_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %ls.",
		 function,
		 filename );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	internal_file->file_io_handle_created_in_library = 1;

	return( 1 );
}

#endif

/* Opens a Nickfile using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libnk2_file_open_file_io_handle(
     libnk2_file_t *file,
     libbfio_handle_t *file_io_handle,
     int flags,
     liberror_error_t **error )
{
	libnk2_internal_file_t *internal_file = NULL;
	static char *function                 = "libnk2_file_open_file_io_handle";
	int file_io_flags                     = 0;
	int file_io_handle_is_open            = 0;

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libnk2_internal_file_t *) file;

	if( internal_file->file_io_handle != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal file - file io handle already set.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBNK2_FLAG_READ ) != LIBNK2_FLAG_READ )
	 && ( ( flags & LIBNK2_FLAG_WRITE ) != LIBNK2_FLAG_WRITE ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBNK2_FLAG_WRITE ) == LIBNK2_FLAG_WRITE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access to OLE Compound File currently not supported.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBNK2_FLAG_READ ) == LIBNK2_FLAG_READ )
	{
		file_io_flags = LIBBFIO_FLAG_READ;
	}
	internal_file->file_io_handle = file_io_handle;

	file_io_handle_is_open = libbfio_handle_is_open(
	                          internal_file->file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		return( -1 );
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     internal_file->file_io_handle,
		     flags,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file io handle.",
			 function );

			return( -1 );
		}
	}
	if( libnk2_file_open_read(
	     internal_file,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes a Nickfile
 * Returns 0 if successful or -1 on error
 */
int libnk2_file_close(
     libnk2_file_t *file,
     liberror_error_t **error )
{
	libnk2_internal_file_t *internal_file = NULL;
	static char *function                 = "libnk2_file_close";
	int result                            = 0;

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libnk2_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( internal_file->file_io_handle_created_in_library != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			if( libnk2_debug_print_read_offsets(
			     internal_file->file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );

				result = -1;
			}
		}
#endif
		if( libbfio_handle_close(
		     internal_file->file_io_handle,
		     error ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file io handle.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Opens a Nickfile for reading
 * Returns 1 if successful or -1 on error
 */
int libnk2_file_open_read(
     libnk2_internal_file_t *internal_file,
     liberror_error_t **error )
{
	static char *function    = "libnk2_file_open_read";
	uint32_t amount_of_items = 0;

	if( internal_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal file.",
		 function );

		return( -1 );
	}
	if( internal_file->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - missing io handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "Reading file header:\n" );
	}
#endif
	if( libnk2_io_handle_read_file_header(
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     &amount_of_items,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "Reading items:\n" );
	}
#endif
	if( libnk2_io_handle_read_items(
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     amount_of_items,
	     internal_file->item_array,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read items.",
		 function );

		return( -1 );
	}
	/* TODO read footer */
	return( 1 );
}

/* Retrieves the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
int libnk2_file_get_ascii_codepage(
     libnk2_file_t *file,
     int *ascii_codepage,
     liberror_error_t **error )
{
	libnk2_internal_file_t *internal_file = NULL;
	static char *function                 = "libnk2_file_get_ascii_codepage";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libnk2_internal_file_t *) file;

	if( ascii_codepage == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ASCII codepage.",
		 function );

		return( -1 );
	}
	*ascii_codepage = internal_file->ascii_codepage;

	return( 1 );
}

/* Sets the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
int libnk2_file_set_ascii_codepage(
     libnk2_file_t *file,
     int ascii_codepage,
     liberror_error_t **error )
{
	libnk2_internal_file_t *internal_file = NULL;
	static char *function                 = "libnk2_file_set_ascii_codepage";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libnk2_internal_file_t *) file;

	if( ( ascii_codepage != LIBNK2_CODEPAGE_ASCII )
	 || ( ascii_codepage != LIBNK2_CODEPAGE_WINDOWS_1250 )
	 || ( ascii_codepage != LIBNK2_CODEPAGE_WINDOWS_1251 )
	 || ( ascii_codepage != LIBNK2_CODEPAGE_WINDOWS_1252 )
	 || ( ascii_codepage != LIBNK2_CODEPAGE_WINDOWS_1253 )
	 || ( ascii_codepage != LIBNK2_CODEPAGE_WINDOWS_1254 )
	 || ( ascii_codepage != LIBNK2_CODEPAGE_WINDOWS_1256 )
	 || ( ascii_codepage != LIBNK2_CODEPAGE_WINDOWS_1257 )
	 || ( ascii_codepage != LIBNK2_CODEPAGE_WINDOWS_1258 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported ASCII codepage.",
		 function );

		return( -1 );
	}
	internal_file->ascii_codepage = ascii_codepage;

	return( 1 );
}

/* Retrieves the amount of items
 * Returns 1 if successful or -1 on error
 */
int libnk2_file_get_amount_of_items(
     libnk2_file_t *file,
     int *amount_of_items,
     liberror_error_t **error )
{
	libnk2_internal_file_t *internal_file = NULL;
	static char *function                 = "libnk2_file_get_amount_of_items";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libnk2_internal_file_t *) file;

	if( internal_file->item_array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - missing item array.",
		 function );

		return( -1 );
	}
	if( amount_of_items == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of items.",
		 function );

		return( -1 );
	}
	*amount_of_items = internal_file->item_array->amount_of_entries;

	return( 1 );
}

/* Retrieves a specific item
 * Returns 1 if successful or -1 on error
 */
int libnk2_file_get_item(
     libnk2_file_t *file,
     int item_index,
     libnk2_item_t **item,
     liberror_error_t **error )
{
	libnk2_internal_file_t *internal_file = NULL;
	libnk2_item_values_t *item_values     = NULL;
	static char *function                 = "libnk2_file_get_item";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libnk2_internal_file_t *) file;

	if( internal_file->item_array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - missing item array.",
		 function );

		return( -1 );
	}
	if( item == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid item.",
		 function );

		return( -1 );
	}
	if( libnk2_item_initialize(
	     item,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create item.",
		 function );

		return( -1 );
	}
	if( libnk2_array_get_entry(
	     internal_file->item_array,
	     item_index,
	     (intptr_t **) &item_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve item values.",
		 function );

		return( -1 );
	}
	if( libnk2_item_attach(
	     (libnk2_internal_item_t *) *item,
	     internal_file->file_io_handle,
	     internal_file,
	     item_values,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to attach item.",
		 function );

		libnk2_item_free(
		 item,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

