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


/*
 * UTIL:  converts expected sectionSchedule times to workable format time string.
 * 
 * sectionSchedule gives us a time string formatted like 'am830'.  
 * PHP DateTime doesn't like that so we reformat into a time format that 
 * DateTime can work with.
 *
 * Can probably be done better.
 */
function _unh_d1_client_cv_time_string_sectionSchedule($time = '')
{
  $ret = '';

  if (!empty($time)) {
    $parts = preg_split("/^am|^pm/", $time, -1, PREG_SPLIT_NO_EMPTY);
    $ignore = preg_match("/^am|^pm/", $time, $matches);

    if (!empty($matches)) {
      $time_str = $parts[0];
      if (strlen($time_str) == 3) {
          $time_str = '0' . $time_str;
      }
      $time_str = $time_str . $matches[0];
      
      $ret = $time_str;
    }
  }

  return $ret;
}
