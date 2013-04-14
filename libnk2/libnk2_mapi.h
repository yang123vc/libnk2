/*
 * Message API (MAPI) definitions
 *
 * Copyright (c) 2009-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBNK2_INTERNAL_MAPI_H )
#define _LIBNK2NK2_INTERNAL_H

#include <common.h>
#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBNK2 )

#include <libnk2/mapi.h>

/* Define HAVE_LOCAL_LIBNK2 for local use of libnk2
 * The definitions in <libnk2/mapi.h> are copied here
 * for local use of libnk2
 */
#else

/* The attachment methods
 */
enum LIBNK2_ATTACHMENT_METHODS
{
	LIBNK2_ATTACHMENT_METHOD_NONE						= 0,
	LIBNK2_ATTACHMENT_METHOD_BY_VALUE					= 1,
	LIBNK2_ATTACHMENT_METHOD_BY_REFERENCE					= 2,
	LIBNK2_ATTACHMENT_METHOD_BY_REFERENCE_RESOLVE				= 3,
	LIBNK2_ATTACHMENT_METHOD_BY_REFERENCE_ONLY				= 4,
	LIBNK2_ATTACHMENT_METHOD_EMBEDDED_MESSAGE				= 5,
	LIBNK2_ATTACHMENT_METHOD_OLE						= 6
};

/* The message flags
 */
enum LIBNK2_MESSAGE_FLAGS
{
	LIBNK2_MESSAGE_FLAG_READ						= 0x00000001,
	LIBNK2_MESSAGE_FLAG_UNMODIFIED						= 0x00000002,
	LIBNK2_MESSAGE_FLAG_SUBMIT						= 0x00000004,
	LIBNK2_MESSAGE_FLAG_UNSENT						= 0x00000008,
	LIBNK2_MESSAGE_FLAG_HAS_ATTACHMENTS					= 0x00000010,
	LIBNK2_MESSAGE_FLAG_FROM_ME						= 0x00000020,
	LIBNK2_MESSAGE_FLAG_ASSOCIATED						= 0x00000040,
	LIBNK2_MESSAGE_FLAG_RESEND						= 0x00000080,
	LIBNK2_MESSAGE_FLAG_RN_PENDING						= 0x00000100,
	LIBNK2_MESSAGE_FLAG_NRN_PENDING						= 0x00000200
};

/* The message importance types
 */
enum LIBNK2_MESSAGE_IMPORTANCE_TYPES
{
	LIBNK2_MESSAGE_IMPORTANCE_TYPE_LOW					= 0,
	LIBNK2_MESSAGE_IMPORTANCE_TYPE_NORMAL					= 1,
	LIBNK2_MESSAGE_IMPORTANCE_TYPE_HIGH					= 2
};

/* The message priority types
 */
enum LIBNK2_MESSAGE_PRIORITY_TYPES
{
	LIBNK2_MESSAGE_PRIORITY_TYPE_NON_URGENT					= -1,
	LIBNK2_MESSAGE_PRIORITY_TYPE_NORMAL					= 0,
	LIBNK2_MESSAGE_PRIORITY_TYPE_URGENT					= 1
};

/* The message sensitivity types
 */
enum LIBNK2_MESSAGE_SENSITIVITY_TYPES
{
	LIBNK2_MESSAGE_SENSITIVITY_TYPE_NONE					= 0,
	LIBNK2_MESSAGE_SENSITIVITY_TYPE_PERSONAL				= 1,
	LIBNK2_MESSAGE_SENSITIVITY_TYPE_PRIVATE					= 2,
	LIBNK2_MESSAGE_SENSITIVITY_TYPE_CONFIDENTIAL				= 3
};

/* The message store valid folder masks
 */
enum LIBNK2_MESSAGE_STORE_VALID_FOLDER_MASKS
{
	LIBNK2_VALID_FOLDER_MASK_SUBTREE					= 0x00000001,
	LIBNK2_VALID_FOLDER_MASK_INBOX						= 0x00000002,
	LIBNK2_VALID_FOLDER_MASK_OUTBOX						= 0x00000004,
	LIBNK2_VALID_FOLDER_MASK_WASTEBOX					= 0x00000008,
	LIBNK2_VALID_FOLDER_MASK_SENTMAIL					= 0x00000010,
	LIBNK2_VALID_FOLDER_MASK_VIEWS						= 0x00000020,
	LIBNK2_VALID_FOLDER_MASK_COMMON_VIEWS					= 0x00000040,
	LIBNK2_VALID_FOLDER_MASK_FINDER						= 0x00000080
};

/* The object types
 * TODO
enum LIBNK2_OBJECT_TYPES
{
	MAPI_STORE	= 0x00000001,
	MAPI_ADDRBOOK	= 0x00000002,
	MAPI_FOLDER	= 0x00000003,
	MAPI_ABCONT	= 0x00000004,
	MAPI_MESSAGE	= 0x00000005,
	MAPI_MAILUSER	= 0x00000006,
	MAPI_ATTACH	= 0x00000007,
	MAPI_DISTLIST	= 0x00000008,
	MAPI_PROFSECT	= 0x00000009,
	MAPI_STATUS	= 0x0000000a,
	MAPI_SESSION	= 0x0000000b,
	MAPI_FORMINFO	= 0x0000000c
};
 */

/* The recipient type
 */
enum LIBNK2_RECIPIENT_TYPES
{
	LIBNK2_RECIPIENT_TYPE_ORIGINATOR					= 0,
	LIBNK2_RECIPIENT_TYPE_TO						= 1,
	LIBNK2_RECIPIENT_TYPE_CC						= 2,
	LIBNK2_RECIPIENT_TYPE_BCC						= 3
};

/* The value types
 */
enum LIBNK2_VALUE_TYPES
{
	LIBNK2_VALUE_TYPE_UNSPECIFIED						= 0x0000,
	LIBNK2_VALUE_TYPE_NULL							= 0x0001,
	LIBNK2_VALUE_TYPE_INTEGER_16BIT_SIGNED					= 0x0002,
	LIBNK2_VALUE_TYPE_INTEGER_32BIT_SIGNED					= 0x0003,
	LIBNK2_VALUE_TYPE_FLOAT_32BIT						= 0x0004,
	LIBNK2_VALUE_TYPE_DOUBLE_64BIT						= 0x0005,
	LIBNK2_VALUE_TYPE_CURRENCY						= 0x0006,
	LIBNK2_VALUE_TYPE_FLOATINGTIME						= 0x0007,
	LIBNK2_VALUE_TYPE_ERROR							= 0x000a,
	LIBNK2_VALUE_TYPE_BOOLEAN						= 0x000b,
	LIBNK2_VALUE_TYPE_OBJECT						= 0x000d,
	LIBNK2_VALUE_TYPE_INTEGER_64BIT_SIGNED					= 0x0014,
	LIBNK2_VALUE_TYPE_STRING_ASCII						= 0x001e,
	LIBNK2_VALUE_TYPE_STRING_UNICODE					= 0x001f,
	LIBNK2_VALUE_TYPE_FILETIME						= 0x0040,
	LIBNK2_VALUE_TYPE_GUID							= 0x0048,
	LIBNK2_VALUE_TYPE_SERVER_IDENTIFIER					= 0x00fb,
	LIBNK2_VALUE_TYPE_RESTRICTION						= 0x00fd,
	LIBNK2_VALUE_TYPE_RULE_ACTION						= 0x00fe,
	LIBNK2_VALUE_TYPE_BINARY_DATA						= 0x0102,
	LIBNK2_VALUE_TYPE_MULTI_VALUE_INTEGER_16BIT_SIGNED			= 0x1002,
	LIBNK2_VALUE_TYPE_MULTI_VALUE_INTEGER_32BIT_SIGNED			= 0x1003,
	LIBNK2_VALUE_TYPE_MULTI_VALUE_FLOAT_32BIT				= 0x1004,
	LIBNK2_VALUE_TYPE_MULTI_VALUE_DOUBLE_64BIT				= 0x1005,
	LIBNK2_VALUE_TYPE_MULTI_VALUE_CURRENCY					= 0x1006,
	LIBNK2_VALUE_TYPE_MULTI_VALUE_APPLICATION_TIME				= 0x1007,
	LIBNK2_VALUE_TYPE_MULTI_VALUE_INTEGER_64BIT_SIGNED			= 0x1014,
	LIBNK2_VALUE_TYPE_MULTI_VALUE_STRING_ASCII				= 0x101e,
	LIBNK2_VALUE_TYPE_MULTI_VALUE_STRING_UNICODE				= 0x101f,
	LIBNK2_VALUE_TYPE_MULTI_VALUE_FILETIME					= 0x1040,
	LIBNK2_VALUE_TYPE_MULTI_VALUE_GUID					= 0x1048,
	LIBNK2_VALUE_TYPE_MULTI_VALUE_BINARY_DATA				= 0x1102
};

#define LIBNK2_VALUE_TYPE_MULTI_VALUE_FLAG					0x1000

#define LIBNK2_VALUE_TYPE_APPLICATION_TIME LIBNK2_VALUE_TYPE_FLOATINGTIME

/* The entry types
 */
enum LIBNK2_ENTRY_TYPES
{
	LIBNK2_ENTRY_TYPE_MESSAGE_IMPORTANCE					= 0x0017,

	LIBNK2_ENTRY_TYPE_MESSAGE_CLASS						= 0x001a,

	LIBNK2_ENTRY_TYPE_MESSAGE_PRIORITY					= 0x0026,

	LIBNK2_ENTRY_TYPE_MESSAGE_SENSITIVITY					= 0x0036,
	LIBNK2_ENTRY_TYPE_MESSAGE_SUBJECT					= 0x0037,

	LIBNK2_ENTRY_TYPE_MESSAGE_CLIENT_SUBMIT_TIME				= 0x0039,

	LIBNK2_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_SEARCH_KEY			= 0x003b,

	LIBNK2_ENTRY_TYPE_MESSAGE_RECEIVED_BY_ENTRY_IDENTIFIER			= 0x003f,
	LIBNK2_ENTRY_TYPE_MESSAGE_RECEIVED_BY_NAME				= 0x0040,
	LIBNK2_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_ENTRY_IDENTIFIER		= 0x0041,
	LIBNK2_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_NAME			= 0x0042,
	LIBNK2_ENTRY_TYPE_MESSAGE_RECEIVED_REPRESENTING_ENTRY_IDENTIFIER	= 0x0043,
	LIBNK2_ENTRY_TYPE_MESSAGE_RECEIVED_REPRESENTING_NAME			= 0x0044,

	LIBNK2_ENTRY_TYPE_MESSAGE_REPLY_RECIPIENT_ENTRIES			= 0x004f,
	LIBNK2_ENTRY_TYPE_MESSAGE_REPLY_RECIPIENT_NAMES				= 0x0050,
	LIBNK2_ENTRY_TYPE_MESSAGE_RECEIVED_BY_SEARCH_KEY			= 0x0051,
	LIBNK2_ENTRY_TYPE_MESSAGE_RECEIVED_REPRESENTING_SEARCH_KEY		= 0x0052,

	LIBNK2_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_ADDRESS_TYPE		= 0x0064,
	LIBNK2_ENTRY_TYPE_MESSAGE_SENT_REPRESENTING_EMAIL_ADDRESS		= 0x0065,

	LIBNK2_ENTRY_TYPE_MESSAGE_CONVERSATION_TOPIC				= 0x0070,
	LIBNK2_ENTRY_TYPE_MESSAGE_CONVERSATION_INDEX				= 0x0071,

	LIBNK2_ENTRY_TYPE_MESSAGE_RECEIVED_BY_ADDRESS_TYPE			= 0x0075,
	LIBNK2_ENTRY_TYPE_MESSAGE_RECEIVED_BY_EMAIL_ADDRESS			= 0x0076,
	LIBNK2_ENTRY_TYPE_MESSAGE_RECEIVED_REPRESENTING_ADDRESS_TYPE		= 0x0077,
	LIBNK2_ENTRY_TYPE_MESSAGE_RECEIVED_REPRESENTING_EMAIL_ADDRESS		= 0x0078,

	LIBNK2_ENTRY_TYPE_MESSAGE_TRANSPORT_HEADERS				= 0x007d,

	LIBNK2_ENTRY_TYPE_RECIPIENT_TYPE					= 0x0c15,

	LIBNK2_ENTRY_TYPE_MESSAGE_SENDER_ENTRY_IDENTIFIER			= 0x0c19,
	LIBNK2_ENTRY_TYPE_MESSAGE_SENDER_NAME					= 0x0c1a,

	LIBNK2_ENTRY_TYPE_MESSAGE_SENDER_SEARCH_KEY				= 0x0c1d,
	LIBNK2_ENTRY_TYPE_MESSAGE_SENDER_ADDRESS_TYPE				= 0x0c1e,
	LIBNK2_ENTRY_TYPE_MESSAGE_SENDER_EMAIL_ADDRESS				= 0x0c1f,

	LIBNK2_ENTRY_TYPE_MESSAGE_DISPLAY_TO					= 0x0e04,

	LIBNK2_ENTRY_TYPE_MESSAGE_DELIVERY_TIME					= 0x0e06,
	LIBNK2_ENTRY_TYPE_MESSAGE_FLAGS						= 0x0e07,
	LIBNK2_ENTRY_TYPE_MESSAGE_SIZE						= 0x0e08,

	LIBNK2_ENTRY_TYPE_MESSAGE_STATUS					= 0x0e17,

	LIBNK2_ENTRY_TYPE_ATTACHMENT_SIZE					= 0x0e20,

	LIBNK2_ENTRY_TYPE_MESSAGE_INTERNET_ARTICLE_NUMBER			= 0x0e23,

	LIBNK2_ENTRY_TYPE_MESSAGE_PERMISSION					= 0x0e27,

	LIBNK2_ENTRY_TYPE_MESSAGE_URL_COMPUTER_NAME_SET				= 0x0e62,

	LIBNK2_ENTRY_TYPE_MESSAGE_TRUST_SENDER					= 0x0e79,

	LIBNK2_ENTRY_TYPE_MESSAGE_BODY_PLAIN_TEXT				= 0x1000,

	LIBNK2_ENTRY_TYPE_MESSAGE_BODY_COMPRESSED_RTF				= 0x1009,

	LIBNK2_ENTRY_TYPE_MESSAGE_BODY_HTML					= 0x1013,

	LIBNK2_ENTRY_TYPE_EMAIL_EML_FILENAME					= 0x10f3,

	LIBNK2_ENTRY_TYPE_DISPLAY_NAME						= 0x3001,
	LIBNK2_ENTRY_TYPE_ADDRESS_TYPE						= 0x3002,
	LIBNK2_ENTRY_TYPE_EMAIL_ADDRESS						= 0x3003,

	LIBNK2_ENTRY_TYPE_MESSAGE_CREATION_TIME					= 0x3007,
	LIBNK2_ENTRY_TYPE_MESSAGE_MODIFICATION_TIME				= 0x3008,

	LIBNK2_ENTRY_TYPE_MESSAGE_STORE_VALID_FOLDER_MASK			= 0x35df,

	LIBNK2_ENTRY_TYPE_FOLDER_TYPE						= 0x3601,
	LIBNK2_ENTRY_TYPE_NUMBER_OF_CONTENT_ITEMS				= 0x3602,
	LIBNK2_ENTRY_TYPE_NUMBER_OF_UNREAD_CONTENT_ITEMS			= 0x3603,

	LIBNK2_ENTRY_TYPE_HAS_SUB_FOLDERS					= 0x360a,

	LIBNK2_ENTRY_TYPE_CONTAINER_CLASS					= 0x3613,

	LIBNK2_ENTRY_TYPE_NUMBER_OF_ASSOCIATED_CONTENT				= 0x3617,

	LIBNK2_ENTRY_TYPE_ATTACHMENT_DATA_OBJECT				= 0x3701,

	LIBNK2_ENTRY_TYPE_ATTACHMENT_FILENAME_SHORT				= 0x3704,
	LIBNK2_ENTRY_TYPE_ATTACHMENT_METHOD					= 0x3705,

	LIBNK2_ENTRY_TYPE_ATTACHMENT_FILENAME_LONG				= 0x3707,

	LIBNK2_ENTRY_TYPE_ATTACHMENT_RENDERING_POSITION				= 0x370b,

	LIBNK2_ENTRY_TYPE_CONTACT_CALLBACK_PHONE_NUMBER				= 0x3a02,

	LIBNK2_ENTRY_TYPE_CONTACT_GENERATIONAL_ABBREVIATION			= 0x3a05,
	LIBNK2_ENTRY_TYPE_CONTACT_GIVEN_NAME					= 0x3a06,

	LIBNK2_ENTRY_TYPE_CONTACT_BUSINESS_PHONE_NUMBER_1			= 0x3a08,
	LIBNK2_ENTRY_TYPE_CONTACT_HOME_PHONE_NUMBER				= 0x3a09,
	LIBNK2_ENTRY_TYPE_CONTACT_INITIALS					= 0x3a0a,

	LIBNK2_ENTRY_TYPE_CONTACT_SURNAME					= 0x3a11,

	LIBNK2_ENTRY_TYPE_CONTACT_POSTAL_ADDRESS				= 0x3a15,
	LIBNK2_ENTRY_TYPE_CONTACT_COMPANY_NAME					= 0x3a16,
	LIBNK2_ENTRY_TYPE_CONTACT_JOB_TITLE					= 0x3a17,
	LIBNK2_ENTRY_TYPE_CONTACT_DEPARTMENT_NAME				= 0x3a18,
	LIBNK2_ENTRY_TYPE_CONTACT_OFFICE_LOCATION				= 0x3a19,
	LIBNK2_ENTRY_TYPE_CONTACT_PRIMARY_PHONE_NUMBER				= 0x3a1a,
	LIBNK2_ENTRY_TYPE_CONTACT_BUSINESS_PHONE_NUMBER_2			= 0x3a1b,
	LIBNK2_ENTRY_TYPE_CONTACT_MOBILE_PHONE_NUMBER				= 0x3a1c,

	LIBNK2_ENTRY_TYPE_CONTACT_BUSINESS_FAX_NUMBER				= 0x3a24,

	LIBNK2_ENTRY_TYPE_CONTACT_COUNTRY					= 0x3a26,
	LIBNK2_ENTRY_TYPE_CONTACT_LOCALITY					= 0x3a27,

	LIBNK2_ENTRY_TYPE_CONTACT_TITLE						= 0x3a45,

	LIBNK2_ENTRY_TYPE_MESSAGE_BODY_CODEPAGE					= 0x3fde,

	LIBNK2_ENTRY_TYPE_MESSAGE_CODEPAGE					= 0x3ffd,

	LIBNK2_ENTRY_TYPE_RECIPIENT_DISPLAY_NAME				= 0x5ff6,

	LIBNK2_ENTRY_TYPE_FOLDER_CHILD_COUNT					= 0x6638,

	LIBNK2_ENTRY_TYPE_SUB_ITEM_IDENTIFIER					= 0x67f2,

	LIBNK2_ENTRY_TYPE_MESSAGE_STORE_PASSWORD_CHECKSUM			= 0x67ff,

	LIBNK2_ENTRY_TYPE_ADDRESS_FILE_UNDER					= 0x8005,

	LIBNK2_ENTRY_TYPE_TASK_STATUS						= 0x8101,
	LIBNK2_ENTRY_TYPE_TASK_PERCENTAGE_COMPLETE				= 0x8102,

	LIBNK2_ENTRY_TYPE_TASK_START_DATE					= 0x8104,
	LIBNK2_ENTRY_TYPE_TASK_DUE_DATE						= 0x8105,

	LIBNK2_ENTRY_TYPE_TASK_ACTUAL_EFFORT					= 0x8110,
	LIBNK2_ENTRY_TYPE_TASK_TOTAL_EFFORT					= 0x8111,
	LIBNK2_ENTRY_TYPE_TASK_VERSION						= 0x8112,

	LIBNK2_ENTRY_TYPE_TASK_IS_COMPLETE					= 0x811c,

	LIBNK2_ENTRY_TYPE_TASK_IS_RECURRING					= 0x8126,

	LIBNK2_ENTRY_TYPE_APPOINTMENT_BUSY_STATUS				= 0x8205,

	LIBNK2_ENTRY_TYPE_APPOINTMENT_LOCATION					= 0x8208,

	LIBNK2_ENTRY_TYPE_APPOINTMENT_START_TIME				= 0x820d,
	LIBNK2_ENTRY_TYPE_APPOINTMENT_END_TIME					= 0x820e,

	LIBNK2_ENTRY_TYPE_APPOINTMENT_DURATION					= 0x8213,

	LIBNK2_ENTRY_TYPE_APPOINTMENT_IS_RECURRING				= 0x8223,

	LIBNK2_ENTRY_TYPE_APPOINTMENT_RECURRENCE_PATTERN			= 0x8232,

	LIBNK2_ENTRY_TYPE_APPOINTMENT_TIMEZONE_DESCRIPTION			= 0x8234,
	LIBNK2_ENTRY_TYPE_APPOINTMENT_FIRST_EFFECTIVE_TIME			= 0x8235,
	LIBNK2_ENTRY_TYPE_APPOINTMENT_LAST_EFFECTIVE_TIME			= 0x8236,

	LIBNK2_ENTRY_TYPE_MESSAGE_REMINDER_TIME					= 0x8502,
	LIBNK2_ENTRY_TYPE_MESSAGE_IS_REMINDER					= 0x8503,

	LIBNK2_ENTRY_TYPE_MESSAGE_IS_PRIVATE					= 0x8506,

	LIBNK2_ENTRY_TYPE_MESSAGE_REMINDER_SIGNAL_TIME				= 0x8550
};

#endif

#ifdef __cplusplus
}
#endif

#endif

