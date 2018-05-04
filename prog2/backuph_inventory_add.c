
h_object *h_inventory_add(h_inventory inventory, h_object *object)
{
	// ToDo: add functionality "object-?id must be unique within the inventory"

	// for returning the current object to where it was before this function is called
	h_object * rtrnCurr = (h_object *) list_curr(inventory->invList);

	if (!inventory || !object)
		return NULL;

	// find objects of the desired class in the list
	h_object * data = (h_object *) list_first(inventory->invList);
	if (!data)
	{
		// list is empty
		if (!list_insert(inventory->invList, (list_data) object))
			return NULL;

		else
			return object;
	}

	if (data->class == object->class)
	{
		// loop until we find the first object whose class does NOT match object's
		while ( data->class == object->class ) 
			{
				data = (h_object *)list_next(inventory->invList);
				if (!data)
				{	// if this happens, desired class was the last class
					// in the list, therefore at this point current == NULL
					// which won't work for list_insert_before(); instead,
					// append to end of list
					if (!list_append(inventory->invList, (list_data) object))
					{	// failed to append

						// return current to position prior to call to 
						// h_inventory_add()
						h_inventory_find_id(inventory, rtrnCurr->id);

						return NULL;
					}


					// return current to position prior to call to 
					// h_inventory_add()
					if (!h_inventory_find_id(inventory, rtrnCurr->id))
						return NULL;

					return object;
				}
			} 

		// now, current positioned where we want for a call to list_insert_before()
		if (!list_insert_before(inventory->invList, (list_data) object))
		{	// failed to insert
			
			// return current to former position prior to call to 
			// h_inventory_add()
			h_inventory_find_id(inventory, rtrnCurr->id);
			return NULL;
		}
		
	} // end if (((h_object *)data)->class == object->class)

	else // first node in list did not have same class as object
	{
		while ((data = (h_object *) list_next(inventory->invList)) != NULL) 
			// while nodes still remain in list
		{
			if (data->class == object->class)
			{	// Found a match for class

				// loop until we find the first object whose class does 
				// NOT match object's
				while ( data->class == object->class ) 
				{
					data = (h_object *)list_next(inventory->invList);
					if (!data)
					{	// if this happens, desired class was the last class
						// in the list, therefore at this point current == NULL
						// which won't work for list_insert_before(); instead,
						// append to end of list
						if (!list_append(inventory->invList, (list_data) object))
						{	// failed to append

							// return current to position prior to call to 
							// h_inventory_add()
							h_inventory_find_id(inventory, rtrnCurr->id);

							return NULL;
						}

						// successfully appended
						
						// return current to position prior to call to 
						// h_inventory_add()
						if (!h_inventory_find_id(inventory, rtrnCurr->id))
							return NULL;

						return object;
					}
				} 

				// now, current positioned where we want for a call to 
				// list_insert_before()
				if (!list_insert_before(inventory->invList, (list_data) object))
				{	// failed to insert (that's what she said)

					// return current to position prior to call to 
					// h_inventory_add()
					h_inventory_find_id(inventory, rtrnCurr->id);
					return NULL;
				}

				else if (data->class > object->class)
				{	// first object with a class greater that object's class;
					// desired class not found; this would be a good place to
					// insert_before
				
					// now, current positioned where we want for a call to
					// list_insert_before()
					if (!list_insert_before(inventory->invList, 
						(list_data) object))
					{	// failed to insert
						
						// return current to position prior to call to 
						// h_inventory_add()
						h_inventory_find_id(inventory, rtrnCurr->id);
						return NULL;
					}
				} // end else if (((h_object *)data)->class >  object->class)
			
			} // end if (((h_object *)data)->class == object->class)
		
		} // end while ((data = list_next(inventory->invList)) != NULL) 

		// if made it to this point, an object with desired class (or one with a class
		// that is greater than the one we want) was not found in list;
		// append to end of list
		if (!list_append(inventory->invList, (list_data) object))
		{ // failled to append
			
			// return current to position prior to call to 
			// h_inventory_add()
			h_inventory_find_id(inventory, rtrnCurr->id);
			return NULL;
		}	

	} // end else (((h_object *)data)->class == object->class)


	// return current to position prior to call to 
	// h_inventory_add()
	if (!h_inventory_find_id(inventory, rtrnCurr->id))
		return NULL;

	return object;
}
