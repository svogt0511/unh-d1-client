<?php

/**
 * @file
 *
 * Constants.
 */

/**
 * API version.
 */
// define("UNH_D1_CLIENT_API_VERSION_53", "0");
// define("UNH_D1_CLIENT_API_VERSION_54_LEGACY", "1");
define("UNH_D1_CLIENT_API_VERSION_54", "2");
define("UNH_D1_CLIENT_API_VERSION_55", "3");

/**
 * Configuration Form
 */
 
// List of options for the config form.
const UNH_D1_API_VERSION_OPTIONS = array(
  // UNH_D1_CLIENT_API_VERSION_53 => 'V5.3',
  // UNH_D1_CLIENT_API_VERSION_54_LEGACY => 'v5.4 Legacy',
  UNH_D1_CLIENT_API_VERSION_54 => 'v5.4',
  UNH_D1_CLIENT_API_VERSION_55 => 'v5.5'
);

// Default values for the config form. (note the function call works.)
define("UNH_D1_CLIENT_API_RELEASE", UNH_D1_CLIENT_API_VERSION_55);
define("UNH_D1_CLIENT_CACHE_ERROR_TIME", 300);
define("UNH_D1_CLIENT_CACHE_TIME", 3600*3);
define("UNH_D1_CLIENT_DOMAIN_BE", "unhsv.destinysolutions.com");
define("UNH_D1_CLIENT_DOMAIN_FE", "unhpv.destinysolutions.com");
define("UNH_D1_CLIENT_DOMAIN_PUBLIC", "learnforlife.unh.edu");
define("UNH_D1_CLIENT_WEBSERVICE_PATH", "http://unhws.destinysolutions.com/webservice/PublicViewREST");
define("UNH_D1_CLIENT_PROGRAM_AREA", variable_get('d1pdt_program_area')); 

/**
 * Webservice constants.
 */
define("UNH_D1_CLIENT_WEBSVC_RSP_FMT", "json");
define("UNH_D1_CLIENT_WEBSVC_RQST_FMT", "application/json");

// methods go here ;'get', 'post'

/**
 * Webservices requests - v5.4/5.5
 *
 * Hopefully this can be easily expanded to handle more requests.
 */
define("UNH_D1_CLIENT_PROGRAMAREAS", "programAreas");
define("UNH_D1_CLIENT_SEARCHCOURSE", "searchCourse");
define("UNH_D1_CLIENT_SEARCHCOURSESECTION", "searchCourseSection");

/**
 * Webservices parameters - v5.4/5.5
 *
 * Hopefully this can be easily expanded to handle more requests.
 *
 * @todo: add programAreas to the d1client settings and set the default appropriately (below)
 */

const UNH_D1_CLIENT_API_REQUESTS = [
  UNH_D1_CLIENT_PROGRAMAREAS => [
    'opts' => [
      'params' => array('informationLevel', 'locale', '_type'),
      'defaults' => array('_type' => 'json'),
    ],
  ],
  UNH_D1_CLIENT_SEARCHCOURSESECTION => [
    'opts' => [
      'params' => array('informationLevel', 'locale', '_type'),
      'defaults' => array('_type' => 'json'),
     ],
    'base' => [
      'label' => '',
      'params' => array('informationLevel', 'locale'),
      'defaults' => array('informationLevel' => 'short'),
    ],
    'requestDetail' => [
      'label' => 'searchCourseSectionProfileRequestDetail',
      'params' => array('overrideMaxResults', 'pageNumber', 'pageSize'),
      'defaults' => array('overrideMaxResults' => true),
    ],
    'requestCriteria' => [
      'label'  => 'courseSectionSearchCriteria',
      'params' => array('keyword', 'courseCode', 'searchOnlyScheduledCoursesFlag', 'returnOnlyAvailableSectionsFlag'),
      'defaults' => array(),
    ],
    'advancedCriteria' => [
      'label' => 'advancedCriteria',
      'params' => array(/*'programAreas', */'customSectionNumber'),
      // 'defaults' => array('programAreas' => array('programAreaCode' =>  constant(variable_get('d1pdt_program_area')))),
      'defaults' => array('programAreas' => array('programAreaCode' =>  UNH_D1_CLIENT_PROGRAM_AREA)),
    ],
  ],
  UNH_D1_CLIENT_SEARCHCOURSE => [
    'opts' => [
      'params' => array('informationLevel', 'locale', '_type'),
      'defaults' => array('_type' => 'json'),
     ],
    'base' => [
      'label' => '',
      'params' => array('informationLevel', 'locale'),
      'defaults' => array(/*'informationLevel' => 'short'*/),
    ],
    'requestDetail' => [
      'label' => 'searchCourseProfileRequestDetail',
      'params' => array('overrideMaxResults', 'pageNumber', 'pageSize'),
      'defaults' => array('overrideMaxResults' => true),
    ],
    'requestCriteria' => [
      'label'  => 'courseSectionSearchCriteria',
      'params' => array('keyword', 'courseCode', 'searchOnlyScheduledCoursesFlag', 'returnOnlyAvailableSectionsFlag'),
      'defaults' => array(),
    ],
    'advancedCriteria' => [
      'label' => 'advancedCriteria',
      'params' => array(/*'programAreas'*/),
      'defaults' => array('programAreas' => array('programAreaCode' => UNH_D1_CLIENT_PROGRAM_AREA)),
    ],  
  ]
];







