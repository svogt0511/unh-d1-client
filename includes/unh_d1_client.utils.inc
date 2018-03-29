<?php

/**
 * @file
 *
 * @todo: Update this docblock.
 *
 * Utility functions.
 */

/*
 * UTIL: Checks to see if an array is associative or indexed.  Useful when we can get
 * results that are a data structure if only one item is being returned -OR- an array if
 * more than one item is being returned.
 */
function _unh_d1_client_isAssoc($arr)
{
  if (array() === $arr) return FALSE;
  return array_keys($arr) !== range(0, count($arr) - 1);
}


/*
 * UTIL:  returns an indexed array.
 * 
 * This helps to solve the problem of getting back a keyed array if there is 
 * only one item in results from an API call.  We always want to return an 
 * indexed array.  It makes looping on results easier.
 */
function _unh_d1_client_idxed_array($arr)
{
  $ret = array();
  
  if (is_array($arr)) {
    if (_unh_d1_client_isAssoc($arr)) {
      $ret = (array($arr));
    } else {
      $ret = $arr;
    }
  }
  
  return $ret;
}

