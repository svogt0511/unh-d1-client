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
 * Returns one course if there is one with the given course code.  Otherwise returns NULL.
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
    ddl($result, '$result - EEEEEE');
    ddl ($code, '$code - EEEEE');
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
 *
 * @todo - make a loop to get paged results.
 */
function unh_d1_client_searchCourseSections($course_code = '', $programAreas = array(), $information_level = 'short') {
  $ret = array();
  
  if (!empty($course_code)) {
    $options = array('informationLevel'  =>  $information_level);
    $criteria = array('courseCode' => $course_code, 'programAreas' => $programAreas);

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
 * Gets the section belonging to a course with given section number.
 *
 * Returns an array of courses.
 */
function unh_d1_client_searchCourseSection($course_code = '', $custom_section_number = '', $programAreas = array(), $information_level = 'short') {
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
    // There should only be 1 section.
    if ($code == '200') {
      $result = $result['SearchCourseSectionProfileResult']['courseSectionProfiles']['courseSectionProfile'];
      $sections = _unh_d1_client_idxed_array($result);
      $ret = $sections[0];
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
 * Section is available if:
 *   - section status is "available" || "waitlist_available".
 * 
 */
function unh_d1_client_courseIsAvailable($course = NULL) {
  $ret = FALSE;
  
  if (!empty($course) && array_key_exists('courseNumber', $course)) {
    $sections = unh_d1_client_searchCourseSections($course['courseNumber']);
    
    if (!empty($sections)) {
      foreach($sections as $section) {
        if (unh_d1_client_sectionIsAvailable($section) ||
          unh_d1_client_sectionIsWaitlisted($section)
        ) {
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
 * @todo: set to return true/false depending on if API version is 5.4 or 5.5
 */
function unh_d1_client_sectionIsAvailable($section = NULL) {
  //return TRUE;

  $ret = FALSE;
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_AVAILABLE) {
      $ret = TRUE;
    }
  }
  
  return $ret;
}

/**
 * Given a section, returns the section is no longer available (TRUE OR FALSE).
 *
 */
function unh_d1_client_sectionIsNoLongerAvailable($section = NULL) {
  $ret = FALSE;
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_NO_LONGER_AVAILABLE) {
      $ret = TRUE;
    }
  }
  
  return $ret;
}

/**
 * Given a section, returns the section is waitlisted (TRUE OR FALSE).
 *
 */
function unh_d1_client_sectionIsWaitlisted($section = NULL) {
  $ret = FALSE;
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_WAITLIST_AVAILABLE) {
      $ret = TRUE;
    }
  }
  ddl($ret, '$ret - waitlist status');
  return $ret;
}

/**
 * Given a section, returns the section is a future offering (TRUE OR FALSE).
 *
 */
function unh_d1_client_sectionIsFutureOffering($section = NULL) {
  $ret = FALSE;
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_FUTURE_OFFERING) {
      $ret = TRUE;
    }
  }
  
  return $ret;
}

/**
 * Given a section, returns the section is full (TRUE OR FALSE).
 *
 */
function unh_d1_client_sectionIsFull($section = NULL) {
  $ret = FALSE;
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_FULL) {
      $ret = TRUE;
    }
  }
  
  return $ret;
}

/**
 * Given a section, returns the section is waitlisted (TRUE OR FALSE).
 * 
 * @todo: check dates, as well as status.
 */
function unh_d1_client_sectionIsEnrollmentClosed($section = NULL) {
  $ret = FALSE;
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    if ($section['sectionAvailabilityCode'] == UNH_D1_CLIENT_SECTION_ENROLLMENT_CLOSED) {
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
function unh_d1_client_getsectionTitle($section = '') {
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
function unh_d1_client_getsectionCustomSectionNumber($section = '') {
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
function unh_d1_client_getsectionAvailabilityCode($section = '') {
  $ret = '';
  
  if (!empty($section) && array_key_exists('sectionAvailabilityCode', $section)) {
    $ret = $section['sectionAvailabilityCode'];
  }
  
  return $ret;
}

/**
 *
 * Given a section, returns the 'associatedTerm' name.
 * 
 */
function unh_d1_client_getsectionSemester($section = '') {
  $ret = '';
  
  if (!empty($section) && array_key_exists('associatedTerm', $section)) {
    $ret = $section['associatedTerm']['name'];
  }
  
  return $ret;
}

/**
 *
 * Given a section, returns the 'objectId'.
 * 
 */
function unh_d1_client_getsectionObjectId($section = NULL) {
  $ret = null;
  
  if (!empty($section) && array_key_exists('objectId', $section)) {
    $ret = $section['objectId'];
  }
  return $ret;
}

/**
 *
 * Given a section, returns the 'pvEnrollmentBeginDate' name.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionPvEnrollmentBeginDate($section = NULL) {
  $ret = NULL;
  if (!empty($section) && array_key_exists('pvEnrollmentBeginDate', $section)) {
    $ret = new DateTime($section['pvEnrollmentBeginDate']);
  }
  return $ret;
}

/**
 *
 * Given a section, returns the 'pvEnrollmentEndDate' name.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionPvEnrollmentEndDate($section = NULL) {
  $ret = NULL;
  if (!empty($section) && array_key_exists('pvEnrollmentEndDate', $section)) {
    $ret = new DateTime($section['pvEnrollmentEndDate']);
  }
  return $ret;
}

/**
 *
 * Given a section, returns the 'pvAvailabilityBeginDate' name.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionPvAvailaibilityBeginDate($section = NULL) {
  $ret = NULL;
  if (!empty($section) && array_key_exists('pvAvailabilityBeginDate', $section)) {
    $ret = new DateTime($section['pvAvailabilityBeginDate']);
  }
  return $ret;
}

/**
 *
 * Given a section, returns the 'pvAvailabilityEndDate' name.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionPvAvailabilityEndDate($section = NULL) {
  $ret = NULL;
  if (!empty($section) && array_key_exists('pvAvailabilityEndDate', $section)) {
    $ret = new DateTime($section['pvAvailabilityEndDate']);
  }
  return $ret;
}

/**
 *
 * Given a section, returns an array of 'sectionSchedules'.
 *
 */
function unh_d1_client_getSectionSchedules($section = NULL) {
  $ret = NULL;
  if (!empty($section) && array_key_exists('sectionSchedules', $section)) {
    $ret = _unh_d1_client_idxed_array($section['sectionSchedules']['sectionSchedule']);
  }
  return $ret;
}

/******************************************************************************
 *
 * Calls to extract data from SECTION SCHEDULE objects obtained from the API.
 *
 ******************************************************************************/

/**
 *
 * Given a sectionSchedule, returns a the start date.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionScheduleStartDate($sectionSchedule = NULL) {
  $ret = NULL;
  if (!empty($sectionSchedule) && array_key_exists('startDate', $sectionSchedule) && !empty($sectionSchedule['startDate'])) {
    $ret = new DateTime($sectionSchedule['startDate']);
  }
  return $ret;
}

/**
 *
 * Given a sectionSchedule, returns the end date.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionScheduleEndDate($sectionSchedule = NULL) {
  $ret = NULL;
  if (!empty($sectionSchedule) && array_key_exists('endDate', $sectionSchedule) && !empty($sectionSchedule['endDate'])) {
    $ret = new DateTime($sectionSchedule['endDate']);
  }
  return $ret;
}

/**
 *
 * Given a sectionSchedule, returns a the start time.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionScheduleStartTime($sectionSchedule = NULL) {
  $ret = NULL;
  if (!empty($sectionSchedule) && array_key_exists('startTimeString', $sectionSchedule) && !empty($sectionSchedule['startTimeString'])) {
    $ret = new DateTime($sectionSchedule['startTimeString']);
  }
  return $ret;
}

/**
 *
 * Given a sectionSchedule, returns the end time.
 *
 * Returns a DateTime object.
 *
 */
function unh_d1_client_getSectionScheduleEndTime($sectionSchedule = NULL) {
  $ret = NULL;
  if (!empty($sectionSchedule) && array_key_exists('endTimeString', $sectionSchedule) && !empty($sectionSchedule['endTimeString'])) {
    $ret = new DateTime($sectionSchedule['endTimeString']);
  }
  return $ret;
}

/******************************************************************************
 *
 * Calls to build various urls for action buttons.
 *
 ******************************************************************************/
 
 //UTIL: Returns the enrollment url.  The actual url should be a module parameter.
function unh_d1_client_get_enrollment_url($section) {
  //return 'https://learnforlife.unh.edu/search/publicCourseSearchDetails.do?method=addToCart&cspIndex=' . $sectionID;
  return 'https://' . unh_d1_client_settings_public_domain() . '/search/publicCourseSearchDetails.do?method=addToCart&cspIndex=' . unh_d1_client_getsectionObjectId($section);
}

//UTIL: Returns the wait list url.  The actual url should be a module parameter.
function unh_d1_client_get_wait_list_url($section) {
  //return 'https://learnforlife.unh.edu/search/publicCourseSearchDetails.do?method=addToCart&cspIndex=' . $sectionID;
  return 'https://' . unh_d1_client_settings_public_domain() . '/search/publicCourseSearchDetails.do?method=addToCart&cspIndex=' . unh_d1_client_getsectionObjectId($section);
}

/*
//UTIL: Returns the notify me url.  The actual url should be a module parameter.
function unh_d1_client_get_notify_me_url($courseObjID, $sectionNumber) {
  //https://unhtestpv.destinysolutions.com/public/student/studentRequestInformation.do
  //  ?method=edit&type=0&courseNumber=798628&sectionNumber=002&notify=course
  return 'https://' . variable_get('d1client_domain_public') . '/public/student/studentRequestInformation.do?method=edit&type=0&notify=course' .
    '&courseNumber=' . $courseObjID .
    '&sectionNumber=' . $sectionNumber;
}
*/
 
 
// card_header - section status to display in card header: waitlist, ???
function unh_d1_client_getsectionDisplayStatus($courseCode, $section, $node) {
  $ret = '';

  if (_d1pdt_section_is_waitlisted($courseCode, $section, $node)) {
    $ret = '<div class="section-wait-list-status">Wait List</div>';
  } elseif (_d1pdt_section_will_be_available($courseCode, $section, $node)) {
    $date = _d1pdt_section_availability_date($courseCode, $section, $node);
    $msg = "Enrollment opens:&nbsp;&nbsp;" . $date;
    if (!empty($msg)) {
      $ret = '<div class="section-wait-list-status"  style="margin: 5px 0 0 0">' . $msg . '</div>';
    }
  }

  return $ret;
}




