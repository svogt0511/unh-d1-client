<?php

/**
 * @file
 *
 * @todo: Update this docblock.
 * @todo: make sure we get full output on other than 'short' informationLevel
 *
 * Functions/'methods' get data from D1 using the web services.
 * For ex, get lists of things like: course lists, section lists, etc.
 *
 * Also functions/'methods' for getting information from the data structures
 * returned from D1, like determining course status, section status, etc.
 */

/******************************************************************************
 *
 * Calls to extract data of interest from the API.
 *
 ******************************************************************************/

/**
 * Implements a course search with the given keyword/text.
 *
 * Returns an array of courses that match the specifications.  If no courses
 * match specifications, returns an empty array.
 *
 * Searches all courses in the configured program area.
 */
function unh_d1_client_searchCourse_keyword($keyword = '', $programAreas = array(), $information_level = 'short') {
  $ret = array();
  
  if (!empty($keyword)) {
    list($code, $error, $result) = unh_d1_client_request(
      UNH_D1_CLIENT_SEARCHCOURSE, 
      '', 
      array('informationLevel'  =>  $information_level), 
      array('keyword' => $keyword, 'programAreas' => $programAreas), 
      'POST'
    );
    
    // There could be some checking here, but mostly, best effort.
    if ($code == '200') {
      if (!empty($result['searchCourseResult']['courseProfiles'])) {
        $courses = $result['searchCourseResult']['courseProfiles']['courseProfile'];
        $ret = _unh_d1_client_idxed_array($courses);
      }
    }
  }

  return $ret;
}


/**
 * Implements a course search with the given course code.
 *
 * Returns a course if there is one with the given course code.  Otherwise returns NULL.
 */
function unh_d1_client_searchCourse_courseCode($course_code = '', $programAreas = array(), $information_level = 'short') {
  $ret = NULL;
  
  if (!empty($course_code)) {
    list($code, $error, $result) = unh_d1_client_request(
      UNH_D1_CLIENT_SEARCHCOURSE, 
      '', 
      array('informationLevel'  =>  $information_level), 
      array('courseCode' => $course_code, 'programAreas' => $programAreas), 
      'POST'
    );
    
    // There could be some checking here, but mostly, best effort.
    if ($code == '200') {
      $course = $result['searchCourseResult']['courseProfiles']['courseProfile'];
      // Returning at most 1 course. (Don't use _unh_d1_client_idxed_array.)
      $ret = $course;
    }
  }

  return $ret;
}

/**
 * Gets specified groups of courses in the given program areas. 
 *
 * Returns a list of all courses within the configured program area.
 * Default - returns all courses in all program areas.
 *
 * If no criteria are specified - no courses are returned.  However,
 * if given a list of all program areas, will return all courses.
 */
function unh_d1_client_searchCourse_programAreas($programAreas = array(), $information_level = 'short') {
  $ret = array();
  
  $options = array('informationLevel'  =>  $information_level);
  $criteria = array('programAreas' => $programAreas);
  list($code, $error, $result) = unh_d1_client_request(
    UNH_D1_CLIENT_SEARCHCOURSE, 
    '', 
    $options, 
    $criteria, 
    'POST'
  );
  // There could be some checking here, but mostly, best effort.
  if ($code == '200') {
    $courses = $result['searchCourseResult']['courseProfiles']['courseProfile'];
    $ret = _unh_d1_client_idxed_array($courses);
  }

  return $ret;
}

/**
 * Gets the sections belonging to a course
 *
 * Returns an array of courses.
 */
function unh_d1_client_searchCourseSections($course_code = '', $custom_section_number = '', $programAreas = array(), $information_level = 'short') {
  $ret = array();
  
  if (!empty($course_code)) {
    $options = array('informationLevel'  =>  $information_level);
    $criteria = array('courseCode' => $course_code, 'programAreas' => $programAreas);
    if (!empty($custom_section_number)) {
      $criteria['customSectionNumber'] = $custom_section_number;
    }
    list($code, $error, $result) = unh_d1_client_request(
      UNH_D1_CLIENT_SEARCHCOURSESECTION, 
      '', 
      $options, 
      $criteria, 
      'POST'
    );

    // There could be some checking here, but mostly, best effort.
    if ($code == '200') {
      $sections = $result['SearchCourseSectionProfileResult']['courseSectionProfiles']['courseSectionProfile'];
      $ret = _unh_d1_client_idxed_array($sections);
    }
  }

  return $ret;
}


/**
 * Returns a list of programAreas.
 *
 * Gets the list through the programAreas call.
 */
function unh_d1_client_getprogramAreas($information_level = 'short') {
  $ret = array();
  
  $options = array('informationLevel'  =>  $information_level);
  $criteria = array();
  list($code, $error, $result) = unh_d1_client_request(
    UNH_D1_CLIENT_PROGRAMAREAS, 
    '', 
    $options, 
    $criteria, 
    'GET'
  );
  
  // There could be some checking here, but mostly, best effort.
  if ($code == '200') {
    $programAreas = $result['programArea'];
    $ret = _unh_d1_client_idxed_array($programAreas);
  }

  return $ret;
}


/**
 * Returns a list of programAreas (v1). 
 *
 * Gets the list through the baseEntities call.
 */
function unh_d1_client_getprogramAreasV1($information_level = 'short') {
  $ret = array();
  
  $options = array('informationLevel'  =>  $information_level);
  $criteria = array();
  list($code, $error, $result) = unh_d1_client_request(
    UNH_D1_CLIENT_BASEENTITIES, 
    'programAreas', 
    $options, 
    $criteria, 
    'GET'
  );
  
  // There could be some checking here, but mostly, best effort.
  if ($code == '200') {
    $programAreas = $result['baseEntity'];
    $ret = _unh_d1_client_idxed_array($programAreas);
  }

  return $ret;
}


/******************************************************************************
 *
 * Calls to determine STATUSES of various data objects returned by the API.
 *
 ******************************************************************************/


/**
 * Given a course, returns the course availability (TRUE OR FALSE).
 *
 * Courses are available if:
 *
 *   - there is 1 or more scheduled sections that:
 *       - Are publically available ('returnOnlyAvailableSectionsFlag = true). Default.
 *       - Have scheduled sections('searchOnlyScheduledCoursesFlag = 'true'). Default.
 *       - At least one of the scheduled sections is 'available'.
 * 
 */
function unh_d1_client_courseIsAvailable($course = NULL) {
  $ret = FALSE;
  
  if (!empty($course) && array_key_exists('courseNumber', $course)) {
    $sections = unh_d1_client_searchCourseSections($course['courseNumber']);
    
    if (!empty($sections)) {
      foreach($sections as $section) {
        if (unh_d1_client_sectionIsAvailable($section)) {
          $ret = TRUE;
          break;
        }
      }
    }
  }
  
  return $ret;
}

/**
 * Given a section, returns the section availability (TRUE OR FALSE).
 *
 * Sections are 'available' if 'sectionAvailabilityCode' equals:
 *
 *   - available
 * 
 * Possible values for 'sectionAvailabilityCode' are:
 *
 *   - no_longer_available
 *   - available
 * 
 */
function unh_d1_client_sectionIsAvailable($section = NULL) {
  $ret = FALSE;
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_AVAILABLE) {
      $ret = TRUE;
    }
  }
  
  return $ret;
}

/******************************************************************************
 *
 * Calls to extract data from COURSE objects obtained from the API.
 *
 ******************************************************************************/
 
/**
 *
 * Given a course, returns the 'name'.
 * 
 */
function unh_d1_client_getcourseName($course = NULL) {
  $ret = '';
  
  if (!empty($course) && array_key_exists('name', $course)) {
    $ret = $course['name'];
  }
  
  return $ret;
}
 
/**
 *
 * Given a course, returns the 'courseNumber'.
 * 
 */
function unh_d1_client_getcourseNumber($course = NULL) {
  $ret = '';
  
  if (!empty($course) && array_key_exists('courseNumber', $course)) {
    $ret = $course['courseNumber'];
  }
  
  return $ret;
}

/******************************************************************************
 *
 * Calls to extract data from SECTION objects obtained from the API.
 *
 ******************************************************************************/

/**
 *
 * Given a section, returns the 'sectionTitle'.
 * 
 */
function unh_d1_client_getsectionTitle($section = NULL) {
  $ret = '';
  
  if (!empty($section) && array_key_exists('sectionTitle', $section)) {
    $ret = $section['sectionTitle'];
  }
  
  return $ret;
}

/**
 *
 * Given a section, returns the 'customSectionNumber'.
 * 
 */
function unh_d1_client_getcustomSectionNumber($section = NULL) {
  $ret = '';
  
  if (!empty($section) && array_key_exists('customSectionNumber', $section)) {
    $ret = $section['customSectionNumber'];
  }
  
  return $ret;
}

/**
 *
 * Given a section, returns the 'sectionAvailabilityCode'.
 * 
 */
function unh_d1_client_getsectionAvailabilityCode($section = NULL) {
  $ret = '';
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    $ret = $section['sectionAvailabilityCode'];
  }
  
  return $ret;
}






