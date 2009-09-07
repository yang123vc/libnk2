/*
 * Extracts items from a Nickfile (NK2)
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include <stdio.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H )
#include <stdlib.h>
#endif

/* If libtool DLL support is enabled set LIBNK2_DLL_IMPORT
 * before including libnk2.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBNK2_DLL_IMPORT
#endif

#include <libnk2.h>

#include <libsystem.h>

#include "export_handle.h"
#include "filetime.h"
#include "nk2common.h"
#include "nk2input.h"
#include "nk2output.h"

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use nk2export to export items stored in a Nickfile (NK2).\n\n" );

	fprintf( stream, "Usage: nk2export [ -c codepage ] [ -l logfile ] [ -t target ] [ -hvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file\n\n" );

	fprintf( stream, "\t-c:     codepage of ASCII strings, options: ascii, windows-1250 (default),\n"
	                 "\t        windows-1251, windows-1252, windows-1253, windows-1254,\n"
	                 "\t        windows-1255, windows-1256, windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-l:     logs information about the exported items\n" );
	fprintf( stream, "\t-t:     specify the target directory to export to\n"
	                 "\t        (default is the source filename followed by .export)\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* The main program
 */
#if defined( LIBSYSTEM_HAVE_WIDE_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	export_handle_t *export_handle            = NULL;
	liberror_error_t *error                   = NULL;
	libsystem_character_t *log_filename       = NULL;
	libsystem_character_t *option_target_path = NULL;
	libsystem_character_t *path_separator     = NULL;
	libsystem_character_t *source             = NULL;
	libsystem_character_t *target_path        = NULL;
	FILE *log_file_stream                     = NULL;
	char *program                             = "nk2export";
	size_t source_length                      = 0;
	size_t target_path_length                 = 0;
	libsystem_integer_t option                = 0;
	int ascii_codepage                        = LIBNK2_CODEPAGE_WINDOWS_1250;
	int result                                = 0;
	int verbose                               = 0;

	libsystem_notify_set_stream(
	 stderr,
	 NULL );
	libsystem_notify_set_verbose(
	 1 );

        if( libsystem_initialize(
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	nk2output_version_fprint(
	 stdout,
	 program );

	while( ( option = libsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBSYSTEM_CHARACTER_T_STRING( "c:hl:t:vV" ) ) ) != (libsystem_integer_t) -1 )
	{
		switch( option )
		{
			case (libsystem_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %s\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libsystem_integer_t) 'c':
				if( nk2input_determine_ascii_codepage(
				     optarg,
				     &ascii_codepage,
				     &error ) != 1 )
				{
					libsystem_notify_print_error_backtrace(
					 error );
					liberror_error_free(
					 &error );

					ascii_codepage = LIBNK2_CODEPAGE_WINDOWS_1250;

					fprintf(
					 stderr,
					 "Unsupported ASCII codepage defaulting to: windows-1250.\n" );
				}
				break;

			case (libsystem_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libsystem_integer_t) 'l':
				log_filename = optarg;

				break;

			case (libsystem_integer_t) 't':
				option_target_path = optarg;

				break;

			case (libsystem_integer_t) 'v':
				verbose = 1;

				break;

			case (libsystem_integer_t) 'V':
				nk2output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	if( option_target_path != NULL )
	{
		target_path_length = libsystem_string_length(
		                      option_target_path );

		if( target_path_length > 0 )
		{
			target_path = (libsystem_character_t *) memory_allocate(
			                                         sizeof( libsystem_character_t ) * ( target_path_length + 1 ) );

			if( target_path == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to create target path.\n" );

				return( EXIT_FAILURE );
			}
			else if( libsystem_string_copy(
			          target_path,
			          option_target_path,
			          target_path_length ) == NULL )
			{
				fprintf(
				 stderr,
				 "Unable to set target path.\n" );

				memory_free(
				 target_path );

				return( EXIT_FAILURE );
			}
			target_path[ target_path_length ] = 0;
		}
	}
	else
	{
		source_length = libsystem_string_length(
		                 source );

		path_separator = libsystem_string_search_reverse(
		                  source,
		                  (libsystem_character_t) NK2COMMON_PATH_SEPARATOR,
		                  source_length );

		if( path_separator == NULL )
		{
			path_separator = source;
		}
		else
		{
			path_separator++;
		}
		target_path_length = 8 + libsystem_string_length(
		                          path_separator );

		target_path = (libsystem_character_t *) memory_allocate(
		                                         sizeof( libsystem_character_t ) * target_path_length );

		if( target_path == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to create target path.\n" );

			return( EXIT_FAILURE );
		}
		if( libsystem_string_snprintf(
		     target_path,
		     target_path_length,
		     "%" PRIs_LIBSYSTEM ".export",
		     path_separator ) == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set target path.\n" );

			memory_free(
			 target_path );

			return( EXIT_FAILURE );
		}
	}
	result = libsystem_file_exists(
	          target_path,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to determine if %" PRIs_LIBSYSTEM " exists.\n",
		 target_path );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
	else if( result == 1 )
	{
		fprintf(
		 stderr,
		 "%" PRIs_LIBSYSTEM " already exists.\n",
		 target_path );
	}
	if( result != 0 )
	{
		memory_free(
		 target_path );

		return( EXIT_FAILURE );
	}
	libsystem_notify_set_verbose(
	 verbose );
	libnk2_notify_set_stream(
	 stderr,
	 NULL );
	libnk2_notify_set_verbose(
	 verbose );

	if( export_handle_initialize(
	     &export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize export handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		memory_free(
		 target_path );

		return( EXIT_FAILURE );
	}
	if( log_filename != NULL )
	{
		log_file_stream = libsystem_file_stream_open(
		                   log_filename,
		                   _LIBSYSTEM_CHARACTER_T_STRING( "a" ) );

		if( log_file_stream == NULL )
		{
			fprintf(
			 stderr,
			 "Unable to open log file: %s.\n",
			 log_filename );
		}
	}
	fprintf(
	 stdout,
	 "Opening file.\n" );

	if( export_handle_open_input(
	     export_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Error opening file: %" PRIs_LIBSYSTEM ".\n",
		 argv[ optind ] );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		export_handle_free(
		 &export_handle,
		 NULL );
		memory_free(
		 target_path );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Exporting items.\n" );

	result = export_handle_export_items(
	          export_handle,
	          target_path,
	          log_file_stream,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to export items.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		export_handle_free(
		 &export_handle,
		 NULL );
		memory_free(
		 target_path );

		return( EXIT_FAILURE );
	}
	if( export_handle_close(
	     export_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Error closing file: %" PRIs_LIBSYSTEM ".\n",
		 argv[ optind ] );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		export_handle_free(
		 &export_handle,
		 NULL );
		memory_free(
		 target_path );

		return( EXIT_FAILURE );
	}
	if( export_handle_free(
	     &export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free export handle.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		memory_free(
		 target_path );

		return( EXIT_FAILURE );
	}
	memory_free(
	 target_path );

	if( log_file_stream != NULL )
	{
		if( libsystem_file_stream_close(
		     log_file_stream ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to close log file: %s.\n",
			 log_filename );
		}
	}
	if( result == 0 )
	{
		fprintf(
		 stdout,
		 "No items to export.\n" );
	}
	else
	{
		fprintf(
		 stdout,
		 "Export completed.\n" );
	}
	return( EXIT_SUCCESS );
}

