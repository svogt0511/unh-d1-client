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

// List of options for the config menu.
const UNH_D1_API_VERSION_OPTIONS = array(
  // UNH_D1_CLIENT_API_VERSION_53 => 'V5.3',
  // UNH_D1_CLIENT_API_VERSION_54_LEGACY => 'v5.4 Legacy',
  UNH_D1_CLIENT_API_VERSION_54 => 'v5.4',
  UNH_D1_CLIENT_API_VERSION_55 => 'v5.5'
);

/**
 * Default configuration values.
 */
define("UNH_D1_CLIENT_API_RELEASE", UNH_D1_CLIENT_API_VERSION_55);
define("UNH_D1_CLIENT_CACHE_ERROR_TIME", 300);
define("UNH_D1_CLIENT_CACHE_TIME", 3600*3);
define("UNH_D1_CLIENT_DOMAIN_BE", "unhsv.destinysolutions.com");
define("UNH_D1_CLIENT_DOMAIN_FE", "unhpv.destinysolutions.com");
define("UNH_D1_CLIENT_DOMAIN_PUBLIC", "learnforlife.unh.edu");
define("UNH_D1_CLIENT_WEBSERVICE_PATH", "http://unhws.destinysolutions.com/webservice/PublicViewREST");

/**
 * NEW: API service point (request) - v5.4/5.5
 *
 * Hopefully this can be easily expanded to handle more requests.
 */
define("UNH_D1_CLIENT_API_RQST_SCS", "searchCourseSection");
define("UNH_D1_CLIENT_API_RQST_PA", "programAreas");

const UNH_D1_CLIENT_API_REQUESTS = [
  UNH_D1_CLIENT_API_RQST_SCS => [
    'request' => UNH_D1_CLIENT_API_RQST_SCS,
    'base' => [
      'label' => '',
      'params' => array('informationLevel', 'locale', 'overridemaxresults'),
    ],
    'requestDetail' => [
      'label' => 'searchCourseSectionProfileRequestDetail',
      'params' => array('paginationConstruct'),
    ],
    'searchCriteria' => [
      'label'  => 'courseSectionSearchCriteria',
      'params' => array('keyword', 'courseCode', 'searchOnlyScheduledCoursesFlag', 'returnOnlyAvailableSectionsFlag'),
    ],
    'advancedCriteria' => [
      'label' => 'advancedCriteria',
      'params' => array('programAreas', 'customSectionNumber'),
    ],
  ]
];




