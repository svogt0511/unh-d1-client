<?php

/**
 * @file
 *
 * Constants.
 *
 * @todo: allow overridemaxresults for searchCourseSection
 */
 
/**
 * Version Info.
 */
define("UNH_D1_CLIENT_VERSION_7X00_alpha0", "7.x.00.0.alpha.0");

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

/**
 * Webservice constants.
 */
define("UNH_D1_CLIENT_WEBSVC_RSP_FMT", "json");
define("UNH_D1_CLIENT_WEBSVC_RQST_FMT", "application/json");
define("UNH_D1_CLIENT_MAX_PAGE_SIZE", 50);

// methods go here ;'get', 'post'


/**
 * Webservices requests - v5.4/5.5
 *
 * Hopefully this can be easily expanded to handle more requests.
 */
define("UNH_D1_CLIENT_BASEENTITIES", "baseEntities");
define("UNH_D1_CLIENT_CERTIFICATES", "certificates");
define("UNH_D1_CLIENT_PROGRAMAREAS", "programAreas");
define("UNH_D1_CLIENT_SEARCHCOURSE", "searchCourse");
define("UNH_D1_CLIENT_SEARCHCOURSESECTION", "searchCourseSection");
define("UNH_D1_CLIENT_SEMESTERS", "semesters");


/**
 * Webservices parameters - v5.4/5.5
 *
 * Hopefully this can be easily expanded to handle more requests.
 *
 * @todo: add programAreas to the d1client settings and set the default appropriately (below)
 * @todo: report a bug for unh_D1_CLIENT_SEARCHCOURSESECTION broken with overrideMaxResults
 *    When this is fixed, uncomment it below.
 *
 * Note:
 *   1) informationLevel of anything other than short will cause an error if
 *      overrideMaxResults = true.
 *   2) documentation is wrong: informationLevel and local should go in the url
 *      not the body of the request.
 */

const UNH_D1_CLIENT_API_REQUESTS = [
  UNH_D1_CLIENT_BASEENTITIES => [
    'opts' => [
      'params' => array('informationLevel', 'locale', '_type'),
      'defaults' => array('informationLevel' => 'short','_type' => 'json'),
    ],
  ],
  UNH_D1_CLIENT_CERTIFICATES => [
    'opts' => [
      'params' => array('informationLevel', 'locale', 'programOfficeCode', '_type'),
      'defaults' => array('informationLevel' => 'short','_type' => 'json'),
    ],
  ],
  UNH_D1_CLIENT_PROGRAMAREAS => [
    'opts' => [
      'params' => array('informationLevel', 'locale', '_type'),
      'defaults' => array('informationLevel' => 'short','_type' => 'json'),
    ],
  ],
  UNH_D1_CLIENT_SEARCHCOURSE => [
    'opts' => [
      'params' => array('informationLevel', 'locale', '_type'),
      'defaults' => array('informationLevel' => 'short','_type' => 'json'),
     ],
    'base' => [
      'label' => '',
      'params' => array(),
      'defaults' => array(),
      'builder' => array(),
    ],
    'requestDetail' => [
      'label' => 'searchCourseProfileRequestDetail',
      'params' => array('overrideMaxResults', 'pageNumber', 'pageSize'),
      'defaults' => array('overrideMaxResults' => true),
      'builder' => array(),
    ],
    'requestCriteria' => [
      'label'  => 'courseSectionSearchCriteria',
      'params' => array('keyword', 'courseCode', 'searchOnlyScheduledCoursesFlag', 'returnOnlyAvailableSectionsFlag'),
      'defaults' => array('searchOnlyScheduledCoursesFlag' => FALSE, 'returnOnlyAvailableSectionsFlag' => FALSE),
      'builder' => array(),
    ],
    'advancedCriteria' => [
      'label' => 'advancedCriteria',
      'params' => array('programAreas'),
      'defaults' => array(),
      'builder' => array('programAreas' => '_unh_d1_client_build_programAreas'),
    ],  
  ],
  UNH_D1_CLIENT_SEARCHCOURSESECTION => [
    'opts' => [
      'params' => array('informationLevel', 'locale','_type'),
      'defaults' => array('informationLevel' => 'full','_type' => 'json'),
     ],
    'base' => [
      'label' => '',
      'params' => array(),
      'defaults' => array(),
      'builder' => array(),
    ],
    'requestDetail' => [
      'label' => 'searchCourseSectionProfileRequestDetail',
      // 'params' => array(/*'overrideMaxResults',*/ 'pageNumber', 'pageSize'),
      'params' => array(/*'overrideMaxResults',*/ 'paginationConstruct'),
      'defaults' => array(/*'overrideMaxResults' => true*/),
      'builder' => array(),
    ],
    'requestCriteria' => [
      'label'  => 'courseSectionSearchCriteria',
      'params' => array('keyword', 'courseCode', 'searchOnlyScheduledCoursesFlag', 'returnOnlyAvailableSectionsFlag'),
      'defaults' => array('searchOnlyScheduledCoursesFlag' => FALSE, 'returnOnlyAvailableSectionsFlag' => FALSE),
      'builder' => array(),
    ],
    'advancedCriteria' => [
      'label' => 'advancedCriteria',
      'params' => array('programAreas', 'customSectionNumber'),
      'defaults' => array(),
      'builder' => array('programAreas' => '_unh_d1_client_build_programAreas'),
    ],
  ],
  UNH_D1_CLIENT_SEMESTERS => [
    'opts' => [
      'params' => array('informationLevel', 'locale', '_type'),
      'defaults' => array('informationLevel' => 'short','_type' => 'json'),
    ],
  ]
];

/**
 * SECTIONS: valid 'sectionAvailabilityCode' values.
 */
 
define("UNH_D1_CLIENT_SECTION_AVAILABLE", "available");
define("UNH_D1_CLIENT_SECTION_NO_LONGER_AVAILABLE", "no_longer_available");
define("UNH_D1_CLIENT_SECTION_WAITLIST_AVAILABLE", "waitlist_available");
define("UNH_D1_CLIENT_SECTION_FULL", "full");
define("UNH_D1_CLIENT_SECTION_FUTURE_OFFERING", "future_offering");
define("UNH_D1_CLIENT_SECTION_ENROLLMENT_CLOSED", "enrollment_closed");

/**
 * SECTIONS: valid 'action' values for sections.
 */

define("UNH_D1_CLIENT_SECTION_ACTION_REQUEST_INFO", "request_information");



/**
 * TIME CONSTANTS.
 */
define('UNH_D1_CLIENT_TZ', 'America/New_York');
define("UNH_D1_DRUPAL_SECTION_SCHEDULE_TIME_FORMAT", "hia");

/** 
 * SORT CONSTANTS 
 */
define("UNH_D1_CLIENT_SORT_ASCENDING", 0);
define("UNH_D1_CLIENT_SORT_DESCENDING", 1);


