<?php

/**
 * @file
 *
 * @todo: Update this docblock.
 *
 * Implements DestinyOne v5.4/5.5 API requests, specifically using REST/JSON.
 */

/******************************************************************************
 *
 * API service call functions (and additional functionality) - v5.4
 *
 ******************************************************************************/

/**
 * Implements a webservice request.
 *
 * @todo: finish this function!
 */
function unh_d1_client_request($op = '', $baseEntity = '', $options = array(), $criteria = array(), $method = 'GET') {
  $ret = array(
    '500',
    'Unable to make webservice request - $op = ' . $op . (!empty($baseEntity) ? '$baseEntity = ' . $baseEntity : ''),
    NULL,
  );

  // Set url.
  $url = variable_get('unh_d1_client_webservice_path') . (!empty($op) ? '/' . $op : '') . (!empty($baseEntity) ? '/' . $baseEntity : '');
  $url .= (!empty($options) ? '?' . drupal_http_build_query($options) : '');
  ddl($url, '$url');
  // Set options.
  $opts['method'] = $method;

  if ((strtoupper($method) == 'POST') && (!empty($criteria))) {
    // @todo: process and add $criteria to request.
    $opts['data'] = $criteria;
  }
  // $options['headers'] = array('Authorization' => 'Bearer ' . variable_get('api_key'));

  // Determine cache status.
  $d1_cache = &drupal_static('d1_requests');
  $key = _unh_d1_client_cache_key($url . $opts);

  if (isset($d1_cache[$key])) {
    $ret = $d1_cache[$key];
    ddl('GETTING CACHED VALUE');
  }
  else {
    $result = drupal_http_request($url, $opts);
    ddl($result, '$result abc');
    ddl($url, '$url');
    ddl($opts, '$opts');

    if ($result->code == '200') {
      $decoded_result = json_decode($result->data, TRUE);
      $ret = array($result->code, NULL, $decoded_result);
      $ds_cache[$key] = $ret;
      ddl('STORING CACHED VALUE');
    }
    else {
      $ret = array($result->code, 'error is: ' . $result->data, NULL);
    }
  }
  ddl($ret, '$ret direct');
  return $ret;
}

/*
function unh_d1_client_webservice($request = '', $criteria = array(), $options = array()) {

  $ret = array('500', 'Unable to make webservice request.', null);

  if (!empty($request)) {
    $url = variable_get('unh_d1_client_webservice_path') . "/" . $request;
    $url .= (!empty($options) ? '?' . drupal_http_build_query($options) : '');

    $headers = array('Content-Type' => 'application/json');
    $method = "POST";
    $body = $criteria;

    $options = array(
       'headers' => array('Content-Type' => 'application/json', 'Accept' => 'application/json'),
       'method' => 'POST',
       'data' => $body
       );

    $result = drupal_http_request($url, $options);

    // Check for service unavailable and forward to appropriate page. Otherwise, just
    // return $result

    $error =  (int)$result->code;
    if (($error >= 500) && ($error <=599)) {
      drupal_goto('/service-unavailable');
    }

    if ($result->code == '200') {
      $decoded_result = json_decode($result->data, true);
      return array($result->code, null, $decoded_result);
    } else {
      return array($result->code, d1client_error($result->data), null);
    }
  }

  return $ret;
}
*/

/******************************************************************************
 *
 * API option-building functions - v5.4
 *
 * Creates/adds search options to the options structure which can then be
 * converted to json for the actual call.
 *
 * Build the search criteria structure for a v54 request based on the request
 * structure as defined in the D1CLIENT_API_REQUESTS definition.

 * If an unknown request is submitted, it is ignored.
 * If an unknown arg/param exists, it is ignored.
 *
 * $requests - matches the api request string
 * $args - an key/value pair where value may be:
 *    a 'scalar' value
 *    an array of key/'scalar values'
 *    a data structure (ex: paginationConstruct)
 *    an array of key/data structures (ex: userDefinedFields)
 *
 * key must match the key of parameter of the specific API request.
 * value must fit the requirements of its associated key.  If value is a
 *    data structure or an array of data structures, the structure must match
 *    the corresponding structure in the API request.
 *
 * The idea is to simplify the building of the request structures, not to
 * have to recode the request structure for every request.
 * (Was not done well in 5.3)
 *
 * However, we are counting on parameters that are structures to be built and
 * passed in correctly, for ex. 'programAreas' is an array['programAreaCode'].
 * (which makes no sense, but it doesn't work otherwise - REPORT BUG.)
 *
 ******************************************************************************/

/**
 * Builds the request body for POST requests.
 *
 * Given an array of key=>value, builds a json-encoded argument
 * structure for a webservice request.
 *
 * @todo: finish this function!
 */
function d1_unh_client_build_search_criteria($request, $args) {

  $ret = json_encode(array());

  if (!empty($args)) {
    $opts = array();

    // Test to see if we have a valid request.
    if (array_key_exists($request, UNH_D1_CLIENT_API_REQUESTS)) {
      // Get the definition of the request.
      $def = UNH_D1_CLIENT_API_REQUESTS[$request];

      // Build an empty request structure.
      $opts = [
        $def['requestDetail']['label'] => [
          $def['searchCriteria']['label'] => [
            $def['advancedCriteria']['label'] =>[]
          ],
        ],
      ];

      // Build the opts for the request.  These will go into the body of the request.
      foreach($args as $key=>$arg) {
        if (in_array($key, $def['base']['params'])) {
          $opts['$key'] = $arg;
        } elseif (in_array($key, $def['requestDetail']['params'])) {
          $opts [$def['requestDetail']['label']] [$key] = $arg;
        } elseif (in_array($key, $def['searchCriteria']['params'])) {
          $opts [$def['requestDetail']['label']] [$def['searchCriteria']['label']] [$key] = $arg;
        } elseif (in_array($key, $def['advancedCriteria']['params'])) {
          $opts [$def['requestDetail']['label']] [$def['searchCriteria']['label']] [$def['advancedCriteria']['label']] [$key] = $arg;
        } // else { warn of invalid option }
      }

      // Check structure for correctness.  I.E., if you have a page construct,
      // make sure you have both a pageNumber and pageSize (or defaults.)

      // Cleanup the request structure.  If there are parts with no parameters,
      // delete that part.  If it turns out we have no paramaters at all,
      // return an empty $opts array()
    }
    $ret = json_encode($opts);
  }

  return $ret;
}

/**
 * Implements key calculation for the drupal static cache.
 *
 * Given a fully-qualified url, (http://... or https://... returns a key for
 * the Drupal static has array.
 *
 * The purpose of this is to allow us to cache requests in Drupal static
 * so we don't have to make the same request twice.
 */
function _unh_d1_client_cache_key($str) {
  $ret = '';

  $ret = hash('crc32', $str, FALSE);

  return $ret;
}


