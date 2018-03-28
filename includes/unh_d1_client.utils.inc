
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