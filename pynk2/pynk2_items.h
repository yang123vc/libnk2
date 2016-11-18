/*
 * Python object definition of the sequence and iterator object of items
 *
 * Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYNK2_ITEMS_H )
#define _PYNK2_ITEMS_H

#include <common.h>
#include <types.h>

#include "pynk2_libnk2.h"
#include "pynk2_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pynk2_items pynk2_items_t;

struct pynk2_items
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The parent object
	 */
	PyObject *parent_object;

	/* The get item by index callback function
	 */
	PyObject* (*get_item_by_index)(
	             PyObject *parent_object,
	             int item_index );

	/* The (current) item index
	 */
	int item_index;

	/* The number of items
	 */
	int number_of_items;
};

extern PyTypeObject pynk2_items_type_object;

PyObject *pynk2_items_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int item_index ),
           int number_of_items );

int pynk2_items_init(
     pynk2_items_t *pynk2_items );

void pynk2_items_free(
      pynk2_items_t *pynk2_items );

Py_ssize_t pynk2_items_len(
            pynk2_items_t *pynk2_items );

PyObject *pynk2_items_getitem(
           pynk2_items_t *pynk2_items,
           Py_ssize_t item_index );

PyObject *pynk2_items_iter(
           pynk2_items_t *pynk2_items );

PyObject *pynk2_items_iternext(
           pynk2_items_t *pynk2_items );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYNK2_ITEMS_H ) */
