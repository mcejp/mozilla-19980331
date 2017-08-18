/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */

#include "hotlist.h"
#include "net.h"
#include "xp_mcom.h"
#include "client.h"
#include "xpgetstr.h"

extern int XP_HOTLIST_DEF_NAME;
extern int XP_HOTLIST_AUTOGENERATED_FILE;

#define DEF_NAME XP_GetString(XP_HOTLIST_DEF_NAME)

/*
	Private Prototypes
*/

#ifdef NOPE
PRIVATE HotlistStruct *
hot_FindItemsParent(HotlistStruct * parent_node, HotlistStruct * item);
#endif

HotlistStruct *
hot_GetIndexOf(HotlistStruct *parent, int *index);

/* the main parent node
 */
PRIVATE HotlistStruct * hot_list=0;
PRIVATE XP_Bool hot_list_modified=FALSE;

/* return a pointer to the main hotlist list
 *
 * returns NULL if nothing has ever been
 * added to the hotlist
 */
PUBLIC XP_List *
HOT_GetHotlistList(void)
{
  if (! hot_list) return 0;
  return (hot_list->children);
}

/* tell the hotlist code that the hotlist has been modified
 * so that it gets saved the next time SaveHotlist is called
 */
PUBLIC void
HOT_SetModified(void)
{
    hot_list_modified=TRUE;
}

PRIVATE HotlistStruct *
hot_FindItemStub(HotlistStruct *parent, char * url_address)
{
    XP_List * list_ptr;
    HotlistStruct * object;

    if(parent == NULL) /* Eric made me do it */
		return(0);

    list_ptr = parent->children;

    while((object = (HotlistStruct *)XP_ListNextObject(list_ptr))!=0)
      {

        if(object->address && !XP_STRCMP(object->address, url_address))
		  {
            return(object);
		  }

        if(object->type == HOT_HeaderType)
          {
    		HotlistStruct * rv;
            rv = hot_FindItemStub(object, url_address);

            if(rv)
                return(rv);
          }
      }

    return(0);
}

/* checks the hotlist for a url and updates the last accessed
 * time
 */
PUBLIC void
HOT_UpdateHotlistTime(URL_Struct *URL_s, time_t cur_time)
{
	HotlistStruct * hs = hot_FindItemStub(hot_list, URL_s->address);

	if(hs)
	  {
		TRACEMSG(("Updating hotlist item access time: %s", hs->address));
		hs->last_visit = cur_time;
		hot_list_modified = TRUE;
	  }
}

/* create a hotlist entry struct and fill it in with
 * the passed in data
 *
 * returns NULL on out of memory error.
 */
PUBLIC HotlistStruct *
HOT_CreateEntry(HOT_Type type,
				const char *name, 
				const char *address,
				const char *content_type,
				time_t      last_visit)
{
    HotlistStruct * new_entry = XP_NEW(HotlistStruct);

	if(!new_entry)
	    return(NULL);

	memset(new_entry, 0, sizeof(HotlistStruct));

    new_entry->type = type;

	if(type == HOT_HeaderType)
		new_entry->children = XP_ListNew();
	else
	    StrAllocCopy(new_entry->address, address); /* headers can't have addresses */

	StrAllocCopy(new_entry->name, name);
	StrAllocCopy(new_entry->content_type, content_type);
	new_entry->last_visit = last_visit;
	new_entry->addition_date = time(NULL);
    new_entry->parent = NULL;
    new_entry->lParent = NULL;

	return(new_entry);
}

/* free's a hotlist entry
 */
PUBLIC void
HOT_FreeEntry(HotlistStruct * entry)
{
	if(entry)
	  {
    	if(entry->type == HOT_HeaderType)
          {
            HotlistStruct * subItem;

            while((subItem = (HotlistStruct *) XP_ListRemoveTopObject(entry->children))!=0)
              {
                HOT_FreeEntry(subItem);
              }
	
			XP_ListDestroy(entry->children);
          }

		if(entry->name)
			XP_FREE(entry->name);
		if(entry->address)
			XP_FREE(entry->address);
		if(entry->content_type)
			XP_FREE(entry->content_type);
		XP_FREE(entry);
	  }
}

/* changes a entry to a header type from a non header type
 * and vice versa.  If the object was a header and
 * has children, the children will be blown away. (very bad)
 */
PUBLIC void
HOT_ChangeEntryType(HotlistStruct * entry, HOT_Type type)
{
	if(entry->type == type)
		return;

	entry->type = type;
	if(entry->type == HOT_HeaderType)
	  {
		entry->children = XP_ListNew();
	  }
	else
	  {
		XP_ListDestroy(entry->children);
		entry->children = NULL;
	  }

    hot_list_modified = TRUE;
}


/* create a completely new copy of the entry passed in
 */
PUBLIC HotlistStruct *
HOT_CopyEntry(HotlistStruct * entry)
{
	if(!entry)
		return(NULL);

	return(HOT_CreateEntry(entry->type,
						   entry->name,
						   entry->address,
						   entry->content_type,
						   entry->last_visit));
}

/* Fold or unfold a hotlist header
 * 
 * set the Boolean to True to fold the list and
 * False to unfold
 */
PUBLIC void
HOT_FoldHeader(HotlistStruct * item, Bool fold)
{
    hot_list_modified = TRUE;

	if(item)
		item->is_folded = fold;
}

#ifdef NOPE
/* recursive routine to find a HotlistStruct parent structure containing 
 * the list containing the specified hotlist item
 */
PRIVATE HotlistStruct *
hot_FindItemsParent(HotlistStruct * parent_node, HotlistStruct * item)
{
	XP_List *cur_list = parent_node->children->next;

	if(!cur_list)
		return(NULL);

	while(cur_list)
	  {
		HotlistStruct *rv = NULL;
		HotlistStruct *obj = (HotlistStruct *)cur_list->object;

		if(obj == item)
		  {
			return(parent_node);
		  }
		else if(obj->type == HOT_HeaderType)
		  {
			rv = hot_FindItemsParent(obj, item);
			if(rv)
				return(rv);
		  }
		cur_list = cur_list->next;
	  }

	return(NULL);   /* not found */
}


#endif

/* insert an item before another item in the hotlist
 *
 * if the insert_before item is NULL or not found the item
 * will be inserted at the begining of the list
 */
PUBLIC void
HOT_InsertItemBefore(HotlistStruct * insert_before, HotlistStruct * insertee)
{
	HotlistStruct * hls=0;

	if(!insertee)
		return;

    hot_list_modified = TRUE;

    if(!hot_list)
	  {
		hot_list = HOT_CreateEntry (HOT_HeaderType, XP_GetString(XP_HOTLIST_DEF_NAME), NULL, NULL, 0);
		if(!hot_list)
			return;
	    hot_list->children = XP_ListNew();
	    if (!hot_list->children) 
	   	  {
	    	HOT_FreeEntry(hot_list);
	    	hot_list = NULL;
			return;
	      }
	  }

	/* find the list that contains the insert_before object 
	 */
    if(insert_before) {
        if(insert_before->parent) {
            hls = insert_before->parent;
            XP_ListInsertObject(hls->children, insert_before, insertee);
            insertee->parent = hls;
            insertee->lParent = XP_ListFindObject(hls->children, insertee);
        } else {
            XP_ListInsertObject(hot_list->children, insert_before, insertee);
            insertee->parent = NULL;
            insertee->lParent = XP_ListFindObject(hot_list->children, insertee);
        }
    } else {
        XP_ListAddObjectToEnd(hot_list->children, insertee);
        insertee->parent = NULL;
        insertee->lParent = XP_ListFindObject(hot_list->children, insertee);
    }

	return;
}

/* insert an item after another item in the hotlist
 *
 * if the insert_after item is NULL or not found the item
 * will be inserted at the end of the list
 */
PUBLIC void
HOT_InsertItemAfter(HotlistStruct * insert_after, HotlistStruct * insertee)
{
    HotlistStruct * hls=0;

    if(!insertee)
        return;

    hot_list_modified = TRUE;

    if(!hot_list)
      {
        hot_list = HOT_CreateEntry(HOT_HeaderType, DEF_NAME, NULL, NULL, 0);
        if(!hot_list)
            return;
        hot_list->children = XP_ListNew();
      }

    /* find the list that contains the insert_after object
     */
    if(insert_after) {
        if(insert_after->parent) {
            hls = insert_after->parent;
            XP_ListInsertObjectAfter(hls->children, insert_after, insertee);
            insertee->parent = hls;
            insertee->lParent = XP_ListFindObject(hls->children, insertee);
        } else {
            XP_ListInsertObjectAfter(hot_list->children, insert_after, insertee);
            insertee->parent = NULL;
            insertee->lParent = XP_ListFindObject(hot_list->children, insertee);
        }
    } else {
        XP_ListAddObjectToEnd(hot_list->children, insertee);
        insertee->parent = NULL;
        insertee->lParent = XP_ListFindObject(hot_list->children, insertee);
    }

    return;
}

/* insert an item in a header if "insert_after" is a
 * Header type, or after the item if "insert after" is
 * not a header type.
 *
 * if the insert_after item is NULL or not found the item
 * will be inserted at the end of the hotlist
 */
PUBLIC void
HOT_InsertItemInHeaderOrAfterItem(HotlistStruct * insert_after, 
							      HotlistStruct * insertee)
{
    if(!insertee)
        return;

	if(insert_after && insert_after->type == HOT_HeaderType)
	  {
		XP_ListAddObject(insert_after->children, insertee);
        if(insert_after != hot_list)
            insertee->parent = insert_after;
        else
            insertee->parent = NULL;
        insertee->lParent = XP_ListFindObject(insert_after->children, insertee);
		return;
	  }

	/* else */
    HOT_InsertItemAfter(insert_after, insertee);
}


/* remove an item from the hotlist and free's it
 *
 * returns TRUE on success, FALSE if not found
 */
PUBLIC Bool
HOT_RemoveItem(HotlistStruct * old_item)
{
	HotlistStruct * hls = old_item->parent;
    if(!hls)
        hls = hot_list;

    hot_list_modified = TRUE;

	if(hls)
	  {
        XP_ListRemoveObject(hls->children, old_item);
		HOT_FreeEntry(old_item);
		return(TRUE);
	  }

	/* else */
	return(FALSE);
}

/* remove an item from the hotlist and doesn't free it
 *
 * returns TRUE on success, FALSE if not found
 */
PUBLIC Bool
HOT_RemoveItemFromList(HotlistStruct * old_item)
{
	HotlistStruct * hls = old_item->parent;
    if(!hls)
        hls = hot_list;

    hot_list_modified = TRUE;

    if(hls)
      {
        XP_ListRemoveObject(hls->children, old_item);
        return(TRUE);
      }

    /* else */
    return(FALSE);
}

PRIVATE int
hot_GetIndexNum(HotlistStruct *parent, HotlistStruct * item, int *cur_count)
{
	XP_List * list_ptr = parent->children;
	HotlistStruct * object;
	int rv=0;

	while((object = (HotlistStruct *)XP_ListNextObject(list_ptr))!=0)
	  {

		if(object == item)
			return(*cur_count);

		*(cur_count) += 1;

		if(object->type == HOT_HeaderType && !object->is_folded)
		  {
		     rv = hot_GetIndexNum(object, item, cur_count);

		    if(rv)
			    return(rv);
		  }
	  }

	return(0);
}

/* returns an integer index of the item in the list
 */
PUBLIC int
HOT_GetIndex(HotlistStruct * item)
{
	int count=1;

	if(hot_list)
        return(hot_GetIndexNum(hot_list, item, &count));
	else
		return(0);
}

PRIVATE int
hot_GetUnfoldedIndexNum(HotlistStruct *parent, HotlistStruct * item, int *cur_count)
{
    XP_List * list_ptr = parent->children;
    HotlistStruct * object;
    int rv=0;

    while((object = (HotlistStruct *)XP_ListNextObject(list_ptr))!=0)
      {

        if(object == item)
            return(*cur_count);

        *(cur_count) += 1;

        if(object->type == HOT_HeaderType)
          {
             rv = hot_GetUnfoldedIndexNum(object, item, cur_count);

            if(rv)
                return(rv);
          }
      }

    return(0);
}

/* returns an integer index of the item in the list
 * and does not pay attention to the is_folded value
 */
PUBLIC int
HOT_GetUnfoldedIndex(HotlistStruct * item)
{
    int count=1;

    if(hot_list)
        return(hot_GetUnfoldedIndexNum(hot_list, item, &count));
    else
        return(0);
}

/* returns TRUE if the second argument is a direct
 * descendent of the first argument.
 *
 * returns FALSE otherwise
 */
PUBLIC Bool
HOT_IsDescendent(HotlistStruct *parent, HotlistStruct *possible_child)
{
    int count = 1;

    if(parent
        && parent->type == HOT_HeaderType
          && hot_GetUnfoldedIndexNum(parent, possible_child, &count))
        return(TRUE);

    return(FALSE);
}

PRIVATE HotlistStruct *
hot_SearchHotlistStub(HotlistStruct *parent, 
					char * search_string, 
					HotlistStruct ** start_obj, 
					int *cur_count,
					Bool *redisplay_all)
{
    XP_List * list_ptr = parent->children;
    HotlistStruct * object;
    HotlistStruct * rv;

    while((object = (HotlistStruct *)XP_ListNextObject(list_ptr))!=0)
      {

		if(object == *start_obj)
			*start_obj = NULL;
		else if(!*start_obj &&
			  ((object->name && strcasestr(object->name, search_string)) ||
			  	  (object->address && strcasestr(object->address, search_string))))
            return(object);

        *(cur_count) += 1;

        if(object->type == HOT_HeaderType)
          {
             rv = hot_SearchHotlistStub(object, 
										search_string, 
										start_obj, 
										cur_count,
										redisplay_all);
            if(rv)
			  {
			   if(object->is_folded)
			     {
				   *redisplay_all = TRUE;
				   object->is_folded = FALSE;
			     }

                return(rv);
			  }
          }
      }

    return(NULL);
}

/* Performs regular expression match on hotlist name and address
 * fields.  Returns the found object, or NULL if not
 * found.
 *
 * start_obj specifies the object to start searching
 * on.  The start_num object WILL NOT be searched but all those
 * after it will be.
 * To search the whole list give NULL as start_obj.
 *
 * If headers became unfolded because of the search then redisplay_all
 * will be set to TRUE
 */
PUBLIC HotlistStruct *
HOT_SearchHotlist(char * search_string, HotlistStruct * start_obj, Bool * redisplay_all)
{
    int count=1;

	*redisplay_all = FALSE;

    if(hot_list)
        return(hot_SearchHotlistStub(hot_list, 
									search_string, 
									&start_obj, 
									&count,
									redisplay_all));
    else
        return(NULL);
}


PRIVATE int
hot_GetObjDepth(HotlistStruct *parent, HotlistStruct * item, int cur_depth)
{
    XP_List * list_ptr = parent->children;
    HotlistStruct * object;
    int rv=0;

	cur_depth++;

    while((object = (HotlistStruct *)XP_ListNextObject(list_ptr))!=0)
      {

        if(object == item)
            return(cur_depth);

        if(object->type == HOT_HeaderType)
          {
            rv = hot_GetObjDepth(object, item, cur_depth);

            if(rv)
                return(rv);
          }
      }

    return(0);
}

/* returns an integer depth of the item in the list starting at zero
 */
PUBLIC int
HOT_GetDepth(HotlistStruct * item)
{

    if(hot_list)
        return(hot_GetObjDepth(hot_list, item, -1));
    else
        return(0);
}


PUBLIC HotlistStruct *
hot_GetIndexOf(HotlistStruct *parent, int *index)
{
	HotlistStruct * rv=0;
	HotlistStruct * object;
    XP_List * list_ptr = parent->children;

	while((object = (HotlistStruct *)XP_ListNextObject(list_ptr))!=0)
      {
		*(index) -= 1;

        if(*index <= 0)
            return(object);

        if(object->type == HOT_HeaderType && !object->is_folded)
          {
             rv = hot_GetIndexOf(object, index);

            if(rv)
                return(rv);
          }
      }

	return(NULL);
}

/* returns the object associated with the index returned by 
 * HOT_GetIndex()
 */
PUBLIC HotlistStruct *
HOT_IndexOf(int index)
{
	TRACEMSG(("HOT_IndexOf: Getting index: %d",index));

    if(hot_list && index > 0)
		return(hot_GetIndexOf(hot_list, &index));
	else
		return(NULL);
}

PUBLIC HotlistStruct *
hot_GetUnfoldedIndexOf(HotlistStruct *parent, int *index)
{
    HotlistStruct * rv=0;
    HotlistStruct * object;
    XP_List * list_ptr = parent->children;

    while((object = (HotlistStruct *)XP_ListNextObject(list_ptr))!=0)
      {
        *(index) -= 1;

        if(*index <= 0)
            return(object);

        if(object->type == HOT_HeaderType)
          {
             rv = hot_GetUnfoldedIndexOf(object, index);

            if(rv)
                return(rv);
          }
      }

    return(NULL);
}

/* returns the object associated with the index returned by
 * HOT_GetUnfoldedIndex()
 */
PUBLIC HotlistStruct *
HOT_UnfoldedIndexOf(int index)
{
    TRACEMSG(("HOT_IndexOf: Getting index: %d",index));

    if(hot_list && index > 0)
        return(hot_GetUnfoldedIndexOf(hot_list, &index));
    else
        return(NULL);
}


PRIVATE HotlistStruct *
hot_FindLastItem(HotlistStruct * item)
{
	if(item->type == HOT_HeaderType)
	  {
		HotlistStruct * rv = (HotlistStruct *)XP_ListGetEndObject(item->children);
		if(rv)
		   return(hot_FindLastItem(rv));
	  }

	return(item);
}

/* move an item up in the list
 */
PUBLIC void
HOT_MoveObjectUp(HotlistStruct * item)   
{
	HotlistStruct * hls = item->parent;
    if(!hls)
        hls = hot_list;

    hot_list_modified = TRUE;

	if(hls)
	  {
		 /* find the previous object in the list
		  */
      	XP_List *prev=0;
		XP_List *list_ptr = hls->children;
		HotlistStruct * prev_item;

      	while (list_ptr->next)
          {
            prev = list_ptr;
            list_ptr = list_ptr->next;
            if (item == (HotlistStruct *)list_ptr->object)
              {
				prev_item = (HotlistStruct *)prev->object;

				if(prev_item)
				  {
					/* remove the item from the list
					 */
					XP_ListRemoveObject(hls->children, item);

					if(prev_item->type != HOT_HeaderType || prev_item->is_folded)
					  {
						HOT_InsertItemBefore(prev_item, item);
						TRACEMSG(("Normal jump move"));
					  }
					else
					  {
					    /* add to end of children list
					     */
                        XP_ListAddObjectToEnd(prev_item->children, item);
                        item->lParent = XP_ListFindObject(prev_item->children, item);
                        item->parent = prev_item;
						TRACEMSG(("Addition to list above"));
					  }
					return;
				  }
				else
				  {
					/* oh nasty, now we have to move object to a higher list
					 */
					if(hls == hot_list)
					    return; /* cannot move it up, already at the top */

					/* remove the item from the other list
					 */
					XP_ListRemoveObject(hls->children, item);

					HOT_InsertItemBefore(hls, item);

					return;
				  }
              }
		  }
	  }
}

/* returns True if the object can be moved Up
 * False if the object cannot be moved Up or if
 * it cannot be found in the list
 */
PUBLIC Bool
HOT_ObjectCanGoUp(HotlistStruct * item)
{
    /* this is sortof a hack.
     * find the index of item.
	 * if greater than one then it can go up
     */
    int item_count = HOT_GetIndex(item);

    if(item_count > 1)
        return(TRUE);

    return(FALSE);

}





/* move an item down in the list
 */
PUBLIC void
HOT_MoveObjectDown(HotlistStruct * item)
{
	HotlistStruct * hls = item->parent;
    if(!hls)
        hls = hot_list;

    hot_list_modified = TRUE;

    if(hls)
      {
         /* find the object in the list
          */
        XP_List *list_ptr = hls->children;
		XP_List *prev;
		XP_List *next_ptr;
		HotlistStruct * next_item;

        while (list_ptr->next)
          {
			prev = list_ptr;
            list_ptr = list_ptr->next;
            if (item == (HotlistStruct *)list_ptr->object)
              {
				next_ptr = list_ptr->next;

				if(list_ptr->next)
				  {
					next_item = (HotlistStruct *)next_ptr->object;

					if(next_item->type == HOT_HeaderType)
					  {
					    /* remove the item from list
					     */
					    XP_ListRemoveObject(hls->children, item);

						if(next_item->is_folded)
						  {
							/* add right after folded lists
							 */
							HOT_InsertItemAfter(next_item, item); 
TRACEMSG(("Moving item after folded list"));
						  }
						else
						  {
						    /* add to top of children list
						     */
						    XP_ListAddObject(next_item->children, item);
                            item->lParent = XP_ListFindObject(next_item->children, item);
                            item->parent = next_item;
TRACEMSG(("Moving item into list"));
						  }

					    return;
					  }
					else
					  {
						/* move it after the next one */
						/* remove the item from list
					 	 */
						XP_ListRemoveObject(hls->children, item);

						XP_ListInsertObjectAfter(hls->children, 
										         next_ptr->object, 
											     item);
                        item->lParent = XP_ListFindObject(hls->children, item);
TRACEMSG(("Moving item after normal item"));
						return;
					  }
				  }
				else
				  {
                    if(hls == hot_list)
                        return; /* cannot move it down, already at the bottom */

					/* remove the item from list
					 */
					XP_ListRemoveObject(hls->children, item);
TRACEMSG(("Moving item down a hicharchy"));

					HOT_InsertItemAfter(hls, item); 
					return;
                  }
		  	  }
		  }
	  }
}

/* returns True if the object can be moved down
 * False if the object cannot be moved down or if
 * it cannot be found in the list
 */
PUBLIC Bool
HOT_ObjectCanGoDown(HotlistStruct * item)
{
	/* this is sortof a hack.
	 * find the index of item.
	 * if found then try and find
	 * an object with an index one higher
	 * than item.  If it exists then the
	 * object can move down :)
	 */
	int item_count = HOT_GetIndex(item);

	if(item_count)
	  {
		item_count++;

		/* if there are other object below it we can obviously
		 * move down
		 */
        if(HOT_IndexOf(item_count))
			return(TRUE);

		/* if the object is at a depth other than the first
		 * then we can move down as well
		 */
		if(HOT_GetDepth(item) > 0)
			return(TRUE);
	  }

	return(FALSE);
}

#define HOTLIST_COOKIE "<!DOCTYPE NETSCAPE-Bookmark-file-1>"

#define READ_BUFFER_SIZE 2048

PRIVATE void
hot_ReadSaveList(XP_File fp, HotlistStruct * item, char *buffer, char * relative_url)
{
	char *ptr, *end, *url;
	HotlistStruct * new_item=0;
	Bool item_saved=FALSE;
	char *buffer_ptr;

    /* read loop
     */
	while(XP_FileReadLine(buffer, READ_BUFFER_SIZE, fp))
	  {
		buffer_ptr = XP_StripLine(buffer);

		if((ptr = strcasestr(buffer_ptr, "HREF=\""))!=0)
		  {
			char *quote, *gtr_than;
			/* find next quote
			 */
			quote = XP_STRCHR(ptr+6, '"');
	
			if(quote)
			  {
			    *quote = '\0';

				url = NET_MakeAbsoluteURL(relative_url, ptr+6);

                new_item = HOT_CreateEntry(HOT_URLType, NULL, url, NULL, 0);
				item_saved = FALSE;

				XP_FREE(url);
                if(!new_item)
                    return;

			    /* find '>' and the name will be right after it
			     */
			    gtr_than = XP_STRCHR(quote+1, '>');

				if(gtr_than)
				  {
					/* find the end of the name
					 */
					end = strcasestr(gtr_than, "</A>");

					if(end)
					  {
					    *end = '\0';
						StrAllocCopy(new_item->name, XP_StripLine(gtr_than+1));

						/* terminate at beginning of name since there
					 	 * is nothing interesting after that
					 	 */
						*gtr_than = '\0';
					  }
					else
					  {
						StrAllocCopy(new_item->name, XP_StripLine(gtr_than+1));

						XP_FileReadLine(buffer_ptr, READ_BUFFER_SIZE, fp);

						end = strcasestr(buffer_ptr, "</A>");
							
                        if(end)
                            *end = '\0';

						StrAllocCat(new_item->name, XP_StripLine(buffer_ptr));
					  }
				  }

				*quote = '"';
			  }
		  }
		else if(((ptr = strcasestr(buffer_ptr, "<H"))!=0) && isdigit(*(ptr+2)))
		  {
			/* find the beginning of the name
			 */	
			char * gtr_than = XP_STRCHR(ptr+3, '>');

			/* find the end of the name
			 */
			if(gtr_than)
			    end = strcasestr(gtr_than, "</H");
		
			if(gtr_than && end)
			  {
				*end = '\0';

            	new_item = HOT_CreateEntry(HOT_HeaderType, gtr_than+1, NULL, NULL, 0);
				item_saved = FALSE;

            	if(!new_item)
                	return;

	    		if(strcasestr(buffer_ptr, "FOLDED"))
			        new_item->is_folded = TRUE;
				else
			        new_item->is_folded = FALSE;

				*end = '<';
			  }
		  }
		else if((ptr = strcasestr(buffer_ptr, "<HR>"))!=0)
		  {
            new_item = HOT_CreateEntry(HOT_SeparatorType, NULL, NULL, NULL, 0);
			item_saved = FALSE;
		  }
		else if(strcasestr(buffer_ptr, "</UL>") ||
					strcasestr(buffer_ptr, "</MENU>") ||
					    strcasestr(buffer_ptr, "</DL>"))
		  {
			/* end of a header */
			if(item != hot_list)
			    return;
		  }
		else
		  {
			/* assume the rest is descriptions
			 * only add if new_item is a URL
			 */
			if(new_item && new_item->type == HOT_URLType)
		      {
				/* skip <DL> if present */
				if(*buffer_ptr == '<')
					buffer_ptr += 4;

				end = buffer_ptr+XP_STRLEN(buffer_ptr)-1;

				/* check for <BR> on the end and remove it
				 *
				 * also add a return
				 */
				if(*end == '>')
				  {
					end -= 3;
					XP_STRCPY(end, LINEBREAK);
					end += XP_STRLEN(LINEBREAK);
					*end = '\0';
				  }
				else
				  {
					end++;
					XP_STRCPY(end, LINEBREAK);
					end += XP_STRLEN(LINEBREAK);
					*end = '\0';
				  }

				/* go through and turn &lt; into '<'
				 */
				for(ptr=buffer_ptr, end=buffer_ptr; *end != '\0'; end++)
				  {
					if(!strncasecomp(end, "&lt;", 4))
					  {
						end += 3;
						*ptr++ = '<';
					  }
					else
				  	  {
						*ptr++ = *end;
					  }
				  }
				*ptr = '\0'; /* terminate */
					
				StrAllocCat(new_item->description, buffer_ptr);
			  }
		  }

		if(new_item)
		  {
		    /* search for other general attributes
		     */
		    if((ptr = strcasestr(buffer_ptr, "ADD_DATE=\""))!=0)
	          {
			    end = XP_STRCHR(ptr+10, '"');

			    if(end)
			      {
				    *end = '\0';
			        if(new_item)
			            new_item->addition_date = (time_t) atol(ptr+10);
				    *end = '"';
			      }
		      }

		    if((ptr = strcasestr(buffer_ptr, "LAST_VISIT=\""))!=0)
	          {
                end = XP_STRCHR(ptr+12, '"');

                if(end)
                  {
                    *end = '\0';
                    if(new_item)
			            new_item->last_visit = (time_t) atol(ptr+12);
                    *end = '"';
                  }
		      }

			if(!item_saved)
			  {
			    if(!item)
		 	      {
				    if(!hot_list)
				      {
				        if(new_item->type == HOT_HeaderType)
				          {
					        hot_list = new_item;
				          }
				        else
				          {
        			        hot_list = HOT_CreateEntry(HOT_HeaderType, 
														DEF_NAME, NULL, NULL, 0);
        			        if(!hot_list)
            			        return;

			    		    XP_ListAddObjectToEnd(hot_list->children, new_item);
                            new_item->parent = NULL;
                            new_item->lParent = XP_ListFindObject(hot_list->children, new_item);
				          }
				      }
    
				    item = hot_list;
		 	      }
			    else
			      {
			        XP_ListAddObjectToEnd(item->children, new_item);
                    if(item == hot_list)
                        new_item->parent = NULL;
                    else
                        new_item->parent = item;
                    new_item->lParent = XP_ListFindObject(item->children, new_item);
                    
			      }

				item_saved = TRUE;
			  }

			if(new_item->type == HOT_HeaderType)
    			hot_ReadSaveList(fp, new_item, buffer, relative_url);
		  }
	  }
}

/* read hotlist file from disk
 * 
 * pass in a file url
 */
PUBLIC void
HOT_ReadHotlistFromDisk(char * filename, char * relative_url)
{
    XP_File fp;
	char buffer[READ_BUFFER_SIZE];
	
    fp = XP_FileOpen(filename, xpHotlist, XP_FILE_READ);

    if(!fp)
      {
        TRACEMSG(("could not open hotlist file for reading"));
        return;
      }

	TRACEMSG(("Reading MCC hotlist file"));
 
/* DONT REQUIRE THE COOKIE FOR NOW
 *
 * if(XP_STRNCMP(buffer, HOTLIST_COOKIE, strlen(HOTLIST_COOKIE)))
 *   {
 * 	   TRACEMSG(("ERROR! - Hotlist cookie not found in hotlist file"));
 *	   return;
 *   }
 */
	/* hot_list probably doesn't exist yet
	 */
    hot_ReadSaveList(fp, hot_list, buffer, relative_url);
	XP_FileClose(fp);
    hot_list_modified = FALSE;

}


PRIVATE void
hot_PrintList(XP_File fp, HotlistStruct * item, int level)
{
	int i;
	char buffer[16];

	/* indent 
     */
	for(i=0; i<level; i++)
		XP_FileWrite("    ", -1, fp);

	if(item->type == HOT_HeaderType)
	  {
		XP_List * list_ptr = item->children;
		HotlistStruct * obj_ptr;

		if(level != 0)
		  {
			if(item->name)
			  {
			    XP_FileWrite("<DT><H3 ", -1, fp);
				if(item->is_folded)
					XP_FileWrite("FOLDED ", -1, fp);
				XP_FileWrite("ADD_DATE=\"", -1, fp);
				XP_SPRINTF(buffer, "%ld\">", item->addition_date);
			    XP_FileWrite(buffer, XP_STRLEN(buffer), fp);
			    XP_FileWrite(item->name, XP_STRLEN(item->name), fp);
			    XP_FileWrite("</H3>", -1, fp);
                XP_FileWrite(LINEBREAK, XP_STRLEN(LINEBREAK), fp);
			  }
			for(i=0; i<level; i++)
				XP_FileWrite("    ", -1, fp); /* indent */
		  }
		XP_FileWrite("<DL><p>", -1, fp);
        XP_FileWrite(LINEBREAK, XP_STRLEN(LINEBREAK), fp);

		while((obj_ptr = (HotlistStruct *) XP_ListNextObject(list_ptr))!=0)
			hot_PrintList(fp, obj_ptr, level+1);

		for(i=0; i<level; i++)
			XP_FileWrite("    ", -1, fp); /* indent */
		XP_FileWrite("</DL><p>", -1, fp);
        XP_FileWrite(LINEBREAK, XP_STRLEN(LINEBREAK), fp);
	  }
	else if(item->type == HOT_SeparatorType)
	  {
		XP_FileWrite("<HR>", -1, fp);
        XP_FileWrite(LINEBREAK, XP_STRLEN(LINEBREAK), fp);
	  }
	else if(item->address) /* item type */
	  {
	    XP_FileWrite("<DT>", -1, fp);
	    XP_FileWrite("<A HREF=\"", -1, fp);
		XP_FileWrite(item->address, XP_STRLEN(item->address), fp);
	    XP_FileWrite("\" ADD_DATE=\"", -1, fp);

		XP_SPRINTF(buffer, "%ld", item->addition_date);
		XP_FileWrite(buffer, XP_STRLEN(buffer), fp);
			
	    XP_FileWrite("\" LAST_VISIT=\"", -1, fp);

		XP_SPRINTF(buffer, "%ld\">", item->last_visit);
		XP_FileWrite(buffer, XP_STRLEN(buffer), fp);

		if(item->name)
			XP_FileWrite(item->name, XP_STRLEN(item->name), fp);
		else 
			XP_FileWrite(item->address, XP_STRLEN(item->address), fp);

	    XP_FileWrite("</A>", -1, fp);
        XP_FileWrite(LINEBREAK, XP_STRLEN(LINEBREAK), fp);

        /* write description if there is one
         */
        if(item->description)
          {
			char *ptr = XP_StripLine(item->description);

            XP_FileWrite("<DD>", -1, fp);

			for(; *ptr != '\0'; ptr++)
				if(*ptr == '<')
			  	  {
            		XP_FileWrite("&lt;", -1, fp);
			  	  }
				else if(*ptr == '\n')
			  	  {
					XP_FileWrite("<BR>", -1, fp);
        			XP_FileWrite(LINEBREAK, XP_STRLEN(LINEBREAK), fp);
			  	  }
				else
			  	  {
					XP_FileWrite(ptr, 1, fp);
			  	  }

            XP_FileWrite(LINEBREAK, XP_STRLEN(LINEBREAK), fp);
          }
	  }
}

PUBLIC Bool
HOT_Modified (void)
{
  return (hot_list_modified);
}

/* save out the hotlist
 *
 * returns 1 on success and 0 on failure
 * NOTE: the hotlist may not be saved if nothing has
 * changed
 */
PUBLIC int
HOT_SaveHotlist(char * filename)
{
    XP_File fp;
    char buffer[128];
	const char * hot_list_name;

	if(!hot_list_modified)
		return(1);   /* don't rewrite */

#ifdef MOZ_MAIL_NEWS
 	hot_list_name = FE_UsersFullName();
	if(!hot_list_name)
		hot_list_name = FE_UsersMailAddress();
#else
	hot_list_name = NULL;
#endif /* MOZ_MAIL_NEWS */

#if defined(DEBUG_BOOKMARKS) && defined(XP_UNIX)
	fprintf (stderr, "HOT_SaveHotlist: writing %s\n", filename);
#endif

    fp = XP_FileOpen(filename, xpHotlist, XP_FILE_WRITE);

    if(!fp)
      {
        TRACEMSG(("could not open hotlist file for writing"));
        return(0);
      }
    
     /* write cookie
      */
     XP_FileWrite(HOTLIST_COOKIE, strlen(HOTLIST_COOKIE), fp);
     XP_FileWrite(LINEBREAK, XP_STRLEN(LINEBREAK), fp);

	 XP_FileWrite(XP_GetString(XP_HOTLIST_AUTOGENERATED_FILE), -1, fp); 

/* L10N? are we still using this code? */
     XP_SPRINTF(buffer, "\
<TITLE>%s%s Bookmarks</TITLE>\n\
<H1>%s%s Bookmarks</H1>\n", hot_list_name ? hot_list_name : "Personal",
						  hot_list_name ? "'s" : "",
						  hot_list_name ? hot_list_name : "Personal",
                          hot_list_name ? "'s" : "");

    XP_FileWrite(buffer, XP_STRLEN(buffer), fp);
    
	if(hot_list)
        hot_PrintList(fp, hot_list, 0);
	else
        TRACEMSG(("No hotlist to write!"));

	XP_FileClose(fp);

    hot_list_modified = FALSE;

	return(1);
}


/*
 *	Gets the top node of the hotlist
*/
PUBLIC HotlistStruct*
HOT_GetHotlist (void)
{
	if (!hot_list) 
	  {
		hot_list = HOT_CreateEntry (HOT_HeaderType, DEF_NAME, NULL, NULL, 0);
		if (!hot_list)
			return NULL;
	    hot_list->children = XP_ListNew ();
	    if (!hot_list->children) 
	   	  {
	    	HOT_FreeEntry(hot_list);
	    	hot_list = NULL;
	      }
	  }
	return hot_list;
}

/* Free's the entire hotlist
 */
PUBLIC void
HOT_FreeHotlist (void)
{
  if (hot_list)
	HOT_FreeEntry (hot_list);
  hot_list = 0;
}